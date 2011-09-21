#include <gme/gme.h>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
using std::string;
#include "../../../Audio_Systems/audio_mandatory.h"

typedef signed short sample_t;

size_t callback(void *userdata, void *buffer, size_t size) {
	Music_Emu *emu = (Music_Emu*)userdata;
	if (!gme_track_ended(emu)) {
		gme_play(emu, size / sizeof(sample_t), (sample_t*)buffer);
		return size;
	}
	else
		return 0;
}

void cleanup(void *userdata) {
	Music_Emu *emu = (Music_Emu*)userdata;
	gme_delete(emu);
}

int sound_add_from_gme(string filename, int track) {
	Music_Emu *emu;
	gme_open_file(filename.c_str(), &emu, 44100);
	if (!emu)
		return -1;

	gme_start_track(emu, track - 1);
	const int sid = enigma::sound_allocate();
	return enigma::sound_add_from_stream(sid,callback,cleanup,(void*)emu)?-1:sid;
}
