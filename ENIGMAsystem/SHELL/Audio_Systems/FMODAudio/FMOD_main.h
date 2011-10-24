/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have received a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

//TO DO: ADD THE REMAINING FUNCTIONS. I WAS JUST LAZY AND COULDN'T FIND A H FROM CPP GENERATOR
extern "C" {
double FMODinit(double maxsounds, double supportwebfiles = 0);
double FMODSoundAdd(string soundfile, double threed = 0, double streamed = 0);
double FMODUpdate();
double FMODSoundFree(double sound);
double FMODfree();
double FMODSoundLoop(double sound, double paused = 0);
double FMODSoundPlay(double sound, double paused = 0);
double FMODSoundGetLength(double sound);
double FMODMasterSetVolume(double volume);
double FMODGroupSetVolume(double group, double volume);
double FMODSoundSetGroup(double sound, double group);
double FMODGroupSetPitch(double group,double vol);

double FMODInstanceSetPitch(double instance, double pitch);
double FMODInstanceStop(double instance);
double FMODInstanceSetPaused(double instance, double Pause);
double FMODInstanceGetPaused(double instance);
double FMODInstanceIsPlaying(double instance);
double FMODInstanceGetPosition(double instance);
double FMODInstanceSetPosition(double instance, double p);
double FMODInstanceSetPan(double instance, double p);
double FMODInstanceGetPan(double instance);
double FMODInstanceSetSpeakerMix(double instance,
  double  frontleft,
  double  frontright,
  double  center,
  double  lfe,
  double  backleft,
  double  backright,
  double  sideleft,
  double  sideright);
double FMODInstanceGetSpeakerMix(double instance,
  float *  frontleft,
  float *  frontright,
  float *  center,
  float *  lfe,
  float *  backleft,
  float *  backright,
  float *  sideleft,
  float *  sideright);


double FMODInstanceSetVolume(double instance, double volume);
double FMODInstanceAddEffect(double instance, double effect);
double FMODEffectFree(double effect);
double FMODEffectSetParamValue(double effect, double p, double v);
double FMODEffectGetParamValue(double effect, double p);

double FMODSnapShotToArray(double startpos, double size, float array[]);
double FMODInstanceGetSpectrumSnapshot2(double instance, double channel, double size);
double FMODGroupGetSpectrumSnapshot2(double group, double channel, double size);
double FMODNormalizeSpectrumData(double startpos, double size);

double FMODCreateSoundFromMicInput();
double FMODRecordStart(double sound);
double FMODRecordStop();
}
