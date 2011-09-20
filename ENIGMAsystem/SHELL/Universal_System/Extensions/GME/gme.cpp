#include "gme.h"
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
using std::string;
#include "../../../Audio_Systems/audio_mandatory.h"

typedef signed short sample_t;
const int sample_rate = 44100;
const int channels = 1;
const int buffer_size = 1024;

size_t callback(void *userdata, void *buffer, size_t size) {
	Music_Emu *emu = (Music_Emu*)userdata;
	if (!gme_track_ended(emu)) {
		gme_play(emu, size / sizeof(sample_t), (sample_t*)buffer);
		return size;
	}
	else
		return 0;
}

int sound_add_from_gme(string filename, int track) {
	Music_Emu *emu;
	gme_open_file(filename.c_str(), &emu, sample_rate);
	gme_start_track(emu, track);

	return enigma::sound_add_from_stream(
		enigma::sound_allocate(), callback, (void*)emu
	);
}
