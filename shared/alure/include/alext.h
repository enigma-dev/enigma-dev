#ifndef AL_ALEXT_H
#define AL_ALEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AL_LOKI_IMA_ADPCM_format
#define AL_LOKI_IMA_ADPCM_format 1
#define AL_FORMAT_IMA_ADPCM_MONO16_EXT           0x10000
#define AL_FORMAT_IMA_ADPCM_STEREO16_EXT         0x10001
#endif

#ifndef AL_LOKI_WAVE_format
#define AL_LOKI_WAVE_format 1
#define AL_FORMAT_WAVE_EXT                       0x10002
#endif

#ifndef AL_EXT_vorbis
#define AL_EXT_vorbis 1
#define AL_FORMAT_VORBIS_EXT                     0x10003
#endif

#ifndef AL_LOKI_quadriphonic
#define AL_LOKI_quadriphonic 1
#define AL_FORMAT_QUAD8_LOKI                     0x10004
#define AL_FORMAT_QUAD16_LOKI                    0x10005
#endif

#ifndef AL_EXT_float32
#define AL_EXT_float32 1
#define AL_FORMAT_MONO_FLOAT32                   0x10010
#define AL_FORMAT_STEREO_FLOAT32                 0x10011
#endif

#ifndef AL_EXT_double
#define AL_EXT_double 1
#define AL_FORMAT_MONO_DOUBLE_EXT                0x10012
#define AL_FORMAT_STEREO_DOUBLE_EXT              0x10013
#endif

#ifndef ALC_LOKI_audio_channel
#define ALC_LOKI_audio_channel 1
#define ALC_CHAN_MAIN_LOKI                       0x500001
#define ALC_CHAN_PCM_LOKI                        0x500002
#define ALC_CHAN_CD_LOKI                         0x500003
#endif

#ifndef ALC_ENUMERATE_ALL_EXT
#define ALC_ENUMERATE_ALL_EXT 1
#define ALC_DEFAULT_ALL_DEVICES_SPECIFIER        0x1012
#define ALC_ALL_DEVICES_SPECIFIER                0x1013
#endif

#ifndef AL_EXT_MCFORMATS
#define AL_EXT_MCFORMATS 1
#define AL_FORMAT_QUAD8                          0x1204
#define AL_FORMAT_QUAD16                         0x1205
#define AL_FORMAT_QUAD32                         0x1206
#define AL_FORMAT_REAR8                          0x1207
#define AL_FORMAT_REAR16                         0x1208
#define AL_FORMAT_REAR32                         0x1209
#define AL_FORMAT_51CHN8                         0x120A
#define AL_FORMAT_51CHN16                        0x120B
#define AL_FORMAT_51CHN32                        0x120C
#define AL_FORMAT_61CHN8                         0x120D
#define AL_FORMAT_61CHN16                        0x120E
#define AL_FORMAT_61CHN32                        0x120F
#define AL_FORMAT_71CHN8                         0x1210
#define AL_FORMAT_71CHN16                        0x1211
#define AL_FORMAT_71CHN32                        0x1212
#endif

#ifndef AL_EXT_MULAW_MCFORMATS
#define AL_EXT_MULAW_MCFORMATS 1
#define AL_FORMAT_MONO_MULAW                     0x10014
#define AL_FORMAT_STEREO_MULAW                   0x10015
#define AL_FORMAT_QUAD_MULAW                     0x10021
#define AL_FORMAT_REAR_MULAW                     0x10022
#define AL_FORMAT_51CHN_MULAW                    0x10023
#define AL_FORMAT_61CHN_MULAW                    0x10024
#define AL_FORMAT_71CHN_MULAW                    0x10025
#endif

#ifndef AL_EXT_IMA4
#define AL_EXT_IMA4 1
#define AL_FORMAT_MONO_IMA4                      0x1300
#define AL_FORMAT_STEREO_IMA4                    0x1301
#endif

#ifndef ALC_EXT_thread_local_context
#define ALC_EXT_thread_local_context 1
typedef ALCboolean  (ALC_APIENTRY*PFNALCSETTHREADCONTEXTPROC)(ALCcontext *context);
typedef ALCcontext* (ALC_APIENTRY*PFNALCGETTHREADCONTEXTPROC)(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
