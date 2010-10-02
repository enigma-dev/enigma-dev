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

#import "OpenALManager.h"
#import "OpenAl/alc.h"
#import "OpenAl/al.h"
#import <AudioToolbox/AudioToolbox.h>


@implementation OpenALManager


// start up openAL
-(void)initOpenAL
{
	
	soundDictionary=[[NSMutableDictionary alloc] init];
	bufferStorageArray=[[NSMutableArray alloc] init];
	
	// Initialization
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	if (mDevice) {
		// use the device to make a context
		mContext=alcCreateContext(mDevice,NULL);
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
	}
}

// open the audio file
// returns a big audio ID struct
+(AudioFileID)openAudioFile:(NSString*)filePath
{
	AudioFileID outAFID;
	// use the NSURl instead of a cfurlref cuz it is easier
	NSURL * afUrl = [NSURL fileURLWithPath:filePath];
	
	// do some platform specific stuff..
#if TARGET_OS_IPHONE
	OSStatus result = AudioFileOpenURL((CFURLRef)afUrl, kAudioFileReadPermission, 0, &outAFID);
#else
	OSStatus result = AudioFileOpenURL((CFURLRef)afUrl, fsRdPerm, 0, &outAFID);
#endif
	if (result != 0) NSLog(@"cannot openf file: %@",filePath);
	return outAFID;
}

+(UInt32)audioFileSize:(AudioFileID)fileDescriptor
{
	UInt64 outDataSize = 0;
	UInt32 thePropSize = sizeof(UInt64);
	OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
	if(result != 0) NSLog(@"cannot find file size");
	return (UInt32)outDataSize;
}

// the main method: grab the sound ID from the library
// and start the source playing
- (void)playSound:(NSString*)soundKey
{
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return;
	NSUInteger sourceID = [numVal unsignedIntValue];
	alSourcePlay(sourceID);
}

- (void)stopSound:(NSString*)soundKey
{
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return;
	NSUInteger sourceID = [numVal unsignedIntValue];
	alSourceStop(sourceID);
}



-(void)loadASoundOpenAL:(NSString*)soundname {
	// get the full path of the file
	NSString* fileName = [[NSBundle mainBundle] pathForResource:soundname ofType:@"caf"];
	// first, open the file
	AudioFileID fileID = [OpenALManager openAudioFile:fileName];
	// find out how big the actual audio data is
	UInt32 fileSize = [OpenALManager audioFileSize:fileID];
	
	// this is where the audio data will live for the moment
	unsigned char * outData = malloc(fileSize);
	
	// this where we actually get the bytes from the file and put them
	// into the data buffer
	OSStatus result = noErr;
	result = AudioFileReadBytes(fileID, false, 0, &fileSize, outData);
	AudioFileClose(fileID); //close the file
	
	if (result != 0) NSLog(@"cannot load effect: %@",fileName);
	
	NSUInteger bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	// jam the audio data into the new buffer
	alBufferData(bufferID,AL_FORMAT_STEREO16,outData,fileSize,44100); 
	
	// save the buffer so I can release it later
	[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	NSUInteger sourceID;
	
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	if (true) alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	// store this for future use
	[soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:soundname];	
	
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
}	
	-(void)cleanUpOpenAL
	{
		// delete the sources
		for (NSNumber * sourceNumber in [soundDictionary allValues]) {
			NSUInteger sourceID = [sourceNumber unsignedIntegerValue];
			alDeleteSources(1, &sourceID);
		}
		[soundDictionary removeAllObjects];
		
		// delete the buffers
		for (NSNumber * bufferNumber in bufferStorageArray) {
			NSUInteger bufferID = [bufferNumber unsignedIntegerValue];
			alDeleteBuffers(1, &bufferID);
		}
		[bufferStorageArray removeAllObjects];
		
		// destroy the context
		alcDestroyContext(mContext);
		// close the device
		alcCloseDevice(mDevice);
	}

+(void)loadStaticSound:(NSString*)soundname {
	NSLog(soundname);
	// get the full path of the file
	NSString* fileName = [[NSBundle mainBundle] pathForResource:soundname ofType:@"caf"];
	// first, open the file
	AudioFileID fileID = [self openAudioFile:fileName];
	// find out how big the actual audio data is
	UInt32 fileSize = [self audioFileSize:fileID];
	
	// this is where the audio data will live for the moment
	unsigned char * outData = malloc(fileSize);
	
	// this where we actually get the bytes from the file and put them
	// into the data buffer
	OSStatus result = noErr;
	result = AudioFileReadBytes(fileID, false, 0, &fileSize, outData);
	AudioFileClose(fileID); //close the file
	
	if (result != 0) NSLog(@"cannot load effect: %@",fileName);
	
	NSUInteger bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	// jam the audio data into the new buffer
	alBufferData(bufferID,AL_FORMAT_STEREO16,outData,fileSize,44100); 
	
	// save the buffer so I can release it later
	//[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	NSUInteger sourceID;
	
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	if (true) alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	// store this for future use
	//[soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:soundname];	
	
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
}	

@end
//OpenALManager* manager;

void loadstaticSound(const char* filename) {
	[OpenALManager loadStaticSound:[NSString stringWithCString:filename length:strlen(filename)]];
	//[OpenALManager loadStaticSound:@"awesome.mp3"];

}
