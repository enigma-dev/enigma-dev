/*
 * ALURE  OpenAL utility library
 * Copyright (c) 2009-2010 by Chris Robinson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "config.h"

#include "main.h"

#include <string.h>
#include <assert.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <istream>

#include <fluidsynth.h>


struct fluidStream : public alureStream {
private:
    static const ALubyte MIDI_CHANNEL_MASK = 0x0F;
    static const ALubyte MIDI_EVENT_MASK   = 0xF0;

    static const ALubyte MIDI_NOTEOFF    = 0x80;    // + note + velocity
    static const ALubyte MIDI_NOTEON     = 0x90;    // + note + velocity
    static const ALubyte MIDI_POLYPRESS  = 0xA0;    // + pressure (2 bytes)
    static const ALubyte MIDI_CTRLCHANGE = 0xB0;    // + ctrl + value
    static const ALubyte MIDI_PRGMCHANGE = 0xC0;    // + new patch
    static const ALubyte MIDI_CHANPRESS  = 0xD0;    // + pressure (1 byte)
    static const ALubyte MIDI_PITCHBEND  = 0xE0;    // + pitch bend (2 bytes)
    static const ALubyte MIDI_SPECIAL    = 0xF0;    // Special event

    static const ALubyte MIDI_SYSEX    = 0xF0;      // SysEx begin
    static const ALubyte MIDI_SYSEXEND = 0xF7;      // SysEx end
    static const ALubyte MIDI_SONGPOS  = 0xF2;      // Song position
    static const ALubyte MIDI_SONGSEL  = 0xF3;      // Song select
    static const ALubyte MIDI_META     = 0xFF;      // Meta event begin

    static const ALubyte MIDI_META_EOT   = 0x2F;    // End-of-track
    static const ALubyte MIDI_META_TEMPO = 0x51;    // Tempo change

    struct MidiTrack {
        std::vector<ALubyte> data;
        size_t Offset;
        ALubyte LastEvent;
        ALdouble SamplesLeft;

        MidiTrack() : Offset(0), LastEvent(0), SamplesLeft(0.)
        { }

        void Reset()
        {
            Offset = 0;
            LastEvent = 0;
            SamplesLeft = 0.;
        }

        unsigned long ReadVarLen()
        {
            if(Offset >= data.size())
                return 0;

            unsigned long len = data[Offset]&0x7F;
            while((data[Offset]&0x80))
            {
                if(++Offset >= data.size())
                    return 0;
                len = (len<<7) | (data[Offset]&0x7F);
            }
            Offset++;

            return len;
        }
    };

    ALuint Divisions;
    std::vector<MidiTrack> Tracks;

    ALenum format;
    ALsizei sampleRate;
    ALdouble samplesPerTick;

    fluid_settings_t *fluidSettings;
    fluid_synth_t *fluidSynth;
    int fontID;
    bool doFontLoad;

public:
    static void Init() { }
    static void Deinit() { }

    virtual bool IsValid()
    { return fluidSynth != NULL; }

    virtual bool GetFormat(ALenum *fmt, ALuint *frequency, ALuint *blockalign)
    {
        if(format == AL_NONE)
        {
            format = GetSampleFormat(2, 32, true);
            if(format == AL_NONE)
                format = AL_FORMAT_STEREO16;
        }
        *fmt = format;
        *frequency = sampleRate;
        *blockalign = 2 * ((format==AL_FORMAT_STEREO16) ? sizeof(ALshort) :
                                                          sizeof(ALfloat));
        return true;
    }

    virtual ALuint GetData(ALubyte *data, ALuint bytes)
    {
        ALuint ret;

        if(doFontLoad)
        {
            doFontLoad = false;
            const char *soundfont = getenv("FLUID_SOUNDFONT");
            if(soundfont && soundfont[0])
                fontID = fluid_synth_sfload(fluidSynth, soundfont, true);
        }

        if(format == AL_FORMAT_STEREO16)
        {
            ALshort *ptr = reinterpret_cast<ALshort*>(data);
            ret = FillBuffer(ptr, bytes/2/sizeof(ALshort));
            ret *= 2 * sizeof(ALshort);
        }
        else
        {
            ALfloat *ptr = reinterpret_cast<ALfloat*>(data);
            ret = FillBuffer(ptr, bytes/2/sizeof(ALfloat));
            ret *= 2 * sizeof(ALfloat);
        }

        return ret;
    }

    virtual bool Rewind()
    {
        for(std::vector<MidiTrack>::iterator i = Tracks.begin(), end = Tracks.end();i != end;i++)
        {
            i->Reset();
            unsigned long val = i->ReadVarLen();
            i->SamplesLeft += val * samplesPerTick;
        }
        fluid_synth_program_reset(fluidSynth);
        UpdateTempo(500000);
        return true;
    }

    virtual bool SetPatchset(const char *sfont)
    {
        if(UsingSTDIO)
        {
            int newid = fluid_synth_sfload(fluidSynth, sfont, true);
            if(newid == FLUID_FAILED)
            {
                SetError("Failed to load soundfont");
                return false;
            }

            if(fontID != FLUID_FAILED)
                fluid_synth_sfunload(fluidSynth, fontID, true);
            fontID = newid;
            doFontLoad = false;
            return true;
        }

        /* FluidSynth has no way to load a soundfont using IO callbacks. So we
         * have to copy the specified file using the callbacks to a regular
         * file that FluidSynth can open. */
        int newid = FLUID_FAILED;
        InStream istream(sfont);
        if(istream.fail())
        {
            SetError("Failed to open file");
            return false;
        }

        /* First, get a temp filename */
        const char *str = getenv("TEMP");
        if(!str || !str[0]) str = getenv("TMP");
#ifdef _WIN32
        if(!str || !str[0]) str = ".";
#else
        if(!str || !str[0]) str = "/tmp";
#endif
        std::string fname = str;
        fname += "/alure-sfont-XXXXXX";

        for(size_t i = 0;i < fname.size();i++)
        {
            if(fname[i] == '\\')
                fname[i] = '/';
        }

        std::vector<char> tmpfname(fname.begin(), fname.end());
        tmpfname.push_back(0);

        /* Open a temp file */
        int fd = -1;
        FILE *file;
#ifdef _WIN32
        if(mktemp(&tmpfname[0]) == NULL || (file=fopen(&tmpfname[0], "wb")) == NULL)
#else
        if((fd=mkstemp(&tmpfname[0])) == -1 || (file=fdopen(fd, "wb")) == NULL)
#endif
        {
            if(fd >= 0)
            {
                close(fd);
                remove(&tmpfname[0]);
            }
            SetError("Failed to create temp file");
            return false;
        }

        bool copyok = false;
        char buf[4096];
        size_t got;
        do {
            istream.read(buf, sizeof(buf));
            if((got=istream.gcount()) == 0)
            {
                copyok = true;
                break;
            }
        } while(fwrite(buf, 1, got, file) == got);

        if(copyok)
        {
            fflush(file);
            newid = fluid_synth_sfload(fluidSynth, &tmpfname[0], true);
        }

        fclose(file);
        remove(&tmpfname[0]);

        if(!copyok)
        {
            SetError("Failed to copy file");
            return false;
        }

        if(newid == FLUID_FAILED)
        {
            SetError("Failed to load soundfont");
            return false;
        }

        if(fontID != FLUID_FAILED)
            fluid_synth_sfunload(fluidSynth, fontID, true);
        fontID = newid;
        doFontLoad = false;

        return true;
    }

    fluidStream(std::istream *_fstream)
      : alureStream(_fstream), Divisions(100),
        format(AL_NONE), sampleRate(48000), samplesPerTick(1.),
        fluidSettings(NULL), fluidSynth(NULL), fontID(FLUID_FAILED),
        doFontLoad(true)
    {
        ALCdevice *device = alcGetContextsDevice(alcGetCurrentContext());
        if(device) alcGetIntegerv(device, ALC_FREQUENCY, 1, &sampleRate);

        char hdr[4];
        if(!fstream->read(hdr, 4))
            return;

        if(memcmp(hdr, "MThd", 4) == 0)
        {
            ALuint len = read_be32(fstream);
            if(len != 6)
                return;

            int type = read_be16(fstream);
            if(type != 0 && type != 1)
                return;

            ALuint numtracks = read_be16(fstream);

            Divisions = read_be16(fstream);
            UpdateTempo(500000);

            Tracks.resize(numtracks);
            for(std::vector<MidiTrack>::iterator i = Tracks.begin(), end = Tracks.end();i != end;i++)
            {
                if(!fstream->read(hdr, 4) || memcmp(hdr, "MTrk", 4) != 0)
                    return;

                ALint len = read_be32(fstream);
                i->data.resize(len);
                if(!fstream->read(reinterpret_cast<char*>(&i->data[0]), len) ||
                   fstream->gcount() != len)
                    return;

                unsigned long val = i->ReadVarLen();
                i->SamplesLeft += val * samplesPerTick;
            }
            SetupSynth();
        }
    }

    virtual ~fluidStream()
    {
        if(fontID != FLUID_FAILED)
            fluid_synth_sfunload(fluidSynth, fontID, true);
        fontID = FLUID_FAILED;

        if(fluidSynth != NULL)
            delete_fluid_synth(fluidSynth);
        fluidSynth = NULL;

        if(fluidSettings != NULL)
            delete_fluid_settings(fluidSettings);
        fluidSettings = NULL;
    }

private:
    template<typename T>
    ALuint FillBuffer(T *Buffer, ALuint BufferSamples)
    {
        ALuint SamplesInBuffer = 0;
        while(SamplesInBuffer < BufferSamples)
        {
            // Check if any tracks are still playing and how many samples are waiting to render
            size_t TracksPlaying = 0;
            ALuint SamplesToDo = BufferSamples - SamplesInBuffer;
            for(std::vector<MidiTrack>::iterator i = Tracks.begin(),
                                                 end = Tracks.end();i != end;i++)
            {
                if(i->Offset < i->data.size())
                {
                    SamplesToDo = std::min<ALuint>(SamplesToDo, i->SamplesLeft);
                    TracksPlaying++;
                }
            }
            if(TracksPlaying == 0)
                break;

            if(SamplesToDo == 0)
            {
                ProcessMidi();
                continue;
            }

            // Render samples
            WriteSamples(SamplesToDo, Buffer);
            Buffer += SamplesToDo*2;
            SamplesInBuffer += SamplesToDo;

            for(std::vector<MidiTrack>::iterator i = Tracks.begin(),
                                                 end = Tracks.end();i != end;i++)
            {
                if(i->Offset < i->data.size())
                    i->SamplesLeft -= SamplesToDo;
            }
        }

        return SamplesInBuffer;
    }

    void WriteSamples(ALuint count, short *buffer)
    { fluid_synth_write_s16(fluidSynth, count, buffer, 0, 2, buffer, 1, 2); }
    void WriteSamples(ALuint count, float *buffer)
    { fluid_synth_write_float(fluidSynth, count, buffer, 0, 2, buffer, 1, 2); }

    void ProcessMidi()
    {
        ALuint newtempo = 0;

        // Process more events
        std::vector<MidiTrack>::iterator i=Tracks.begin(), end=Tracks.end();
        while(i != end)
        {
            if(i->Offset >= i->data.size() || i->SamplesLeft >= 1.)
            {
                i++;
                continue;
            }

            if(i->data.size() - i->Offset < 3)
            {
                i->Offset = i->data.size();
                i++;
                continue;
            }

            ALubyte event = i->data[i->Offset++];
            ALubyte parm1, parm2;
            if(!(event&0x80))
            {
                event = i->LastEvent;
                i->Offset--;
            }
            if((event&MIDI_EVENT_MASK) != MIDI_SPECIAL)
                i->LastEvent = event;
            parm1 = i->data[i->Offset];
            parm2 = i->data[i->Offset+1];

            int channel = event&MIDI_CHANNEL_MASK;
            switch(event&MIDI_EVENT_MASK)
            {
                case MIDI_NOTEOFF:
                    fluid_synth_noteoff(fluidSynth, channel, parm1);
                    i->Offset += 2;
                    break;
                case MIDI_NOTEON:
                    fluid_synth_noteon(fluidSynth, channel, parm1, parm2);
                    i->Offset += 2;
                    break;
                case MIDI_POLYPRESS:
                    i->Offset += 2;
                    break;

                case MIDI_CTRLCHANGE:
                    fluid_synth_cc(fluidSynth, channel, parm1, parm2);
                    i->Offset += 2;
                    break;
                case MIDI_PRGMCHANGE:
                    fluid_synth_program_change(fluidSynth, channel, parm1);
                    i->Offset += 1;
                    break;

                case MIDI_CHANPRESS:
                    fluid_synth_channel_pressure(fluidSynth, channel, parm1);
                    i->Offset += 1;
                    break;

                case MIDI_PITCHBEND:
                    fluid_synth_pitch_bend(fluidSynth, channel, (parm1&0x7F) | ((parm2&0x7F)<<7));
                    i->Offset += 2;
                    break;

                case MIDI_SPECIAL:
                    switch(event)
                    {
                        case MIDI_SYSEX:
                        {
                            unsigned long len = i->ReadVarLen();
                            if(i->data.size() - i->Offset < len)
                            {
                                i->Offset = i->data.size();
                                break;
                            }

                            char *data = reinterpret_cast<char*>(&i->data[i->Offset]);
                            fluid_synth_sysex(fluidSynth, data, len, NULL, NULL, NULL, false);

                            i->Offset += len;
                            break;
                        }
                        case MIDI_SYSEXEND:
                        {
                            unsigned long len = i->ReadVarLen();
                            if(i->data.size() - i->Offset < len)
                            {
                                i->Offset = i->data.size();
                                break;
                            }
                            i->Offset += len;
                            break;
                        }

                        case MIDI_SONGPOS:
                            i->Offset += 2;
                            break;

                        case MIDI_SONGSEL:
                            i->Offset += 1;
                            break;

                        case MIDI_META:
                        {
                            ALubyte metatype = i->data[i->Offset++];
                            unsigned long val = i->ReadVarLen();

                            if(i->data.size() - i->Offset < val)
                            {
                                i->Offset = i->data.size();
                                break;
                            }

                            if(metatype == MIDI_META_EOT)
                            {
                                i->Offset = i->data.size();
                                break;
                            }

                            if(metatype == MIDI_META_TEMPO && val >= 3)
                            {
                                newtempo = (i->data[i->Offset] << 16) |
                                           (i->data[i->Offset+1] << 8) |
                                           (i->data[i->Offset+2]);
                            }

                            i->Offset += val;
                            break;
                        }

                        default:
                            /* The rest of the special events don't have any
                             * data bytes */
                            break;
                    }
                    break;

                default:
                    /* Shouldn't ever get to here */
                    break;
            }

            unsigned long val = i->ReadVarLen();
            i->SamplesLeft += val * samplesPerTick;
        }
        if(newtempo)
            UpdateTempo(newtempo);
    }

    void UpdateTempo(ALuint tempo)
    {
        ALdouble sampletickrate = sampleRate / (1000000. / tempo) / Divisions;

        for(std::vector<MidiTrack>::iterator i = Tracks.begin(),
                                             end = Tracks.end();i != end;i++)
        {
            if(i->Offset >= i->data.size())
                continue;
            i->SamplesLeft = i->SamplesLeft / samplesPerTick * sampletickrate;
        }
        samplesPerTick = sampletickrate;
    }

    void SetupSynth()
    {
        fluidSettings = new_fluid_settings();
        if(fluidSettings)
        {
            fluid_settings_setnum(fluidSettings, "synth.gain", 1.0);
            fluid_settings_setstr(fluidSettings, "synth.reverb.active", "yes");
            fluid_settings_setstr(fluidSettings, "synth.chorus.active", "yes");
            fluid_settings_setint(fluidSettings, "synth.polyphony", 256);
            fluid_settings_setnum(fluidSettings, "synth.sample-rate", (double)sampleRate);

            fluidSynth = new_fluid_synth(fluidSettings);
        }
    }
};
static DecoderDecl<fluidStream,0> fluidStream_decoder;
