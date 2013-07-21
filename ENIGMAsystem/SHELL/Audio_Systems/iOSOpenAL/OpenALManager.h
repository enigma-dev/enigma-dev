/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#import <Foundation/Foundation.h>
#import "OpenAl/alc.h"
#import "OpenAl/al.h"
#import <AudioToolbox/AudioToolbox.h>
#include "iphoneOpenAL.h"

@interface OpenALManager : NSObject {
	// OpenAl variables
	ALCcontext* mContext;
	ALCdevice* mDevice;
	NSMutableDictionary* soundDictionary; //Dictionary of sound name to id
	NSMutableArray* bufferStorageArray; //used for sotring sound data when sounds are being loaded
}
+(AudioFileID)openAudioFile:(NSString*)filePath;
-(void)initOpenAL;
-(void)loadASoundOpenAL:(NSString*)soundname;
-(void)stopSound:(NSString*)soundKey;
-(void)playSound:(NSString*)soundKey;
-(void)cleanUpOpenAL;
@end

