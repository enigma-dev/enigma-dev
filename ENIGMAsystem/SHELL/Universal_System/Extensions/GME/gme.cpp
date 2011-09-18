#include "gme.h"
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
using std::string;
#include "../../../Audio_Systems/audio_mandatory.h"

struct wave_header {
    wave_header(
        bool uncompressed,
        uint16_t channels,
        uint32_t sample_rate,
        uint32_t bits_per_second,
        uint16_t frame_size,
        uint16_t bits_per_sample,
        uint32_t data_length
    ) :
        file_length(sizeof(wave_header) + data_length - 8),
        fmt_length(16),
        uncompressed(uncompressed),
        channels(channels),
        sample_rate(sample_rate),
        bits_per_second(bits_per_second),
        frame_size(frame_size),
        bits_per_sample(bits_per_sample),
        data_length(data_length) {
		memcpy(wave_sig, "RIFF", 4);
		memcpy(type, "WAVE", 4);
		memcpy(fmt_sig, "fmt ", 4);
		memcpy(data_sig, "data", 4);
    }
    
    char wave_sig[4];
    uint32_t file_length;
    char type[4];
    
    char fmt_sig[4];
    uint32_t fmt_length;
    uint16_t uncompressed;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t bits_per_second;
    uint16_t frame_size;
    uint16_t bits_per_sample;
    
    char data_sig[4];
    uint32_t data_length;
};

int sound_add_from_gme(string filename) {
	typedef signed short sample_t;
	const int sample_rate = 44100;
	const int channels = 1;
	const int track = 0;
	const int buffer_size = 1024;

	Music_Emu *emu;
	gme_open_file("test.nsf", &emu, sample_rate);

	gme_info_t *info;
	gme_track_info(emu, &info, track);
	wave_header header(
		true, channels,
		sample_rate, sample_rate * channels * sizeof(sample_t),
		channels * sizeof(sample_t),
		sizeof(sample_t) * 8,
		sample_rate * sizeof(sample_t) * info->play_length / 1000.0
	);
	gme_free_info(info);

	std::vector<char> data;
	// (x + buffer_size) & ~(buffer_size - 1) aligns to a multiple of buffer_size
	data.reserve((sizeof(header) + header.data_length + buffer_size) & ~(buffer_size - 1));
	data.insert(data.end(), (char*)&header, (char*)&header + sizeof(header));

	gme_start_track(emu, track);
	while (!gme_track_ended(emu) && data.size() < header.data_length) {
		sample_t buffer[buffer_size / sizeof(sample_t)];
		gme_play(emu, sizeof(buffer) / sizeof(sample_t), buffer);
		
		data.insert(data.end(), buffer, buffer + buffer_size);
	}

	gme_delete(emu);

	return enigma::sound_add_from_buffer(
		enigma::sound_allocate(), &data.front(), sizeof(header) + header.data_length
	);
}
