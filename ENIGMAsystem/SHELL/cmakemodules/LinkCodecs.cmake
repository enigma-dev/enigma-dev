find_package(dumb REQUIRED)
if(DUMB_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${DUMB_LIBRARY}")
endif(DUMB_FOUND)

find_package(OGG REQUIRED)
find_package(FLAC REQUIRED)
if(FLAC_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${FLAC_LIBRARIES}")
endif(FLAC_FOUND)

find_package(Vorbis REQUIRED)
if(VORBIS_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${VORBIS_LIBRARY}")
endif(VORBIS_FOUND)

if(OGG_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${OGG_LIBRARY}")
endif(OGG_FOUND)

find_package(MODPLUG REQUIRED)
if(MODPLUG_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${MODPLUG_LIBRARIES}")
endif(MODPLUG_FOUND)

find_package(MPG123 REQUIRED)
if(MPG123_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${MPG123_LIBRARIES}")
endif(MPG123_FOUND)

find_package(SndFile REQUIRED)
if(SNDFILE_FOUND)
  target_link_libraries("AUDIO_${AUDIO}" "${SNDFILE_LIBRARIES}")
endif(SNDFILE_FOUND)
