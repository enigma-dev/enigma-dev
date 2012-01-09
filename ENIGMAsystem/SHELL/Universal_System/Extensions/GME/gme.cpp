#include "../../../Audio_Systems/audio_mandatory.h"
#include <gme/gme.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using std::string;


typedef signed short sample_t;

void handle_error( const char* str )
{
        if (str)
        {
                printf( "\n GME Error: %s\n", str ); 
                exit( EXIT_FAILURE );
        }
}

struct gme_callback {
	Music_Emu *emu;
	int track;
};

size_t callback(gme_callback *userdata, void *buffer, size_t size) {
	Music_Emu *emu = userdata->emu;
	int track = userdata->track;

	if (!gme_track_ended(emu)) {
		handle_error( gme_play(emu, size / sizeof(sample_t), (sample_t*)buffer) );
		return size;
	}
	else {
		handle_error( gme_start_track(emu, track - 1) );
		return 0;
	}
}

void cleanup(gme_callback *userdata) {
	gme_delete(userdata->emu);
	delete userdata;
}

void seek(gme_callback *userdata, float position) {
	handle_error( gme_seek(userdata->emu,position*1000) );
}

int sound_add_from_gme(string filename, int track) {
	Music_Emu *emu;
	handle_error( gme_open_file(filename.c_str(), &emu, 44100) );
	if (!emu)
		return -1;

	handle_error( gme_start_track(emu, track - 1) );

	const int sid = enigma::sound_allocate();
	gme_callback *userdata = new gme_callback;
	userdata->emu = emu;
	userdata->track = track;

	return enigma::sound_add_from_stream(sid,(size_t (*)(void*, void*, size_t))callback,(void (*)(void*,float))seek,(void (*)(void*))cleanup,(void*)userdata)?-1:sid;
}
