/*********************************************************
Created by: icuurd12b42 (icuurd12b42@yahoo.ca)
			www.coolflamesproductions.com

This code is free to use, as long as you give credit
you must abide by the FMOD licence agreement located here
http://www.fmod.org/ to use the fmodex dll.
And take note of the mp3 licence agreement reference by the site

You can change this code if you need but must keep this header
at the top of the file and add your own comments below

If you make code changes for your own purpose, you must still
credit me. And you must not use the same dll name but your own
dll name so not to potentially create problems.

Cheers!

v4.28
**********************************************************/

/*********************************************************
Modified for ENIGMA by: Harijs Grînbergs 2011
**********************************************************/
#include <stdlib.h>

namespace enigma
{
  int audiosystem_initialize() { return 0; }
  void audiosystem_update() {}
  int sound_add_from_buffer(int id, void* buffer, size_t size) { return -1; }
  void audiosystem_cleanup() {}
}

static inline double min(double x,double y) { return x<y?x:y; }
static inline double max(double x,double y) { return x>y?x:y; }

//#pragma warning(disable : 4996)

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#include <windows.h>
//#include <windowsx.h>
#include "additional/FMOD/inc/fmod.h" // FIX ME: additional is missing
#include "Universal_System/var4.h"
#include "Universal_System/Extensions/DataStructures/include.h"
#include <string>
using namespace std;
//#include "FMOD_main.h"
#include <stdio.h>

#ifndef ___SHA1_HDR___
#define ___SHA1_HDR___

#if !defined(SHA1_UTILITY_FUNCTIONS) && !defined(SHA1_NO_UTILITY_FUNCTIONS)
#define SHA1_UTILITY_FUNCTIONS
#endif

#ifdef SHA1_UTILITY_FUNCTIONS
//#include <stdio.h>  // Needed for file access and sprintf
//#include <string.h> // Needed for strcat and strcpy
#endif

#ifdef _MSC_VER
//#include <stdlib.h>
#endif

// You can define the endian mode in your files, without modifying the SHA1
// source files. Just #define SHA1_LITTLE_ENDIAN or #define SHA1_BIG_ENDIAN
// in your files, before including the SHA1.h header file. If you don't
// define anything, the class defaults to little endian.

#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#define SHA1_LITTLE_ENDIAN
#endif

// Same here. If you want variable wiping, #define SHA1_WIPE_VARIABLES, if
// not, #define SHA1_NO_WIPE_VARIABLES. If you don't define anything, it
// defaults to wiping.

#if !defined(SHA1_WIPE_VARIABLES) && !defined(SHA1_NO_WIPE_VARIABLES)
#define SHA1_WIPE_VARIABLES
#endif

/////////////////////////////////////////////////////////////////////////////
// Define 8- and 32-bit variables

#ifndef UINT_32

#ifdef _MSC_VER

#define UINT_8  unsigned __int8
#define UINT_32 unsigned __int32

#else

#define UINT_8 unsigned char

#if (ULONG_MAX == 0xFFFFFFFF)
#define UINT_32 unsigned long
#else
#define UINT_32 unsigned int
#endif

#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Declare SHA1 workspace

typedef union
{
	UINT_8  c[64];
	UINT_32 l[16];
} SHA1_WORKSPACE_BLOCK;

class CSHA1
{
public:
#ifdef SHA1_UTILITY_FUNCTIONS
	// Two different formats for ReportHash(...)
	enum
	{
		REPORT_HEX = 0,
		REPORT_DIGIT = 1
	};
#endif

	// Constructor and Destructor
	CSHA1();
	~CSHA1();

	UINT_32 m_state[5];
	UINT_32 m_count[2];
	UINT_32 __reserved1[1];
	UINT_8  m_buffer[64];
	UINT_8  m_digest[20];
	UINT_32 __reserved2[3];

	void Reset();

	// Update the hash value
	void Update(UINT_8 *data, UINT_32 len);


	// Finalize hash and report
	void Final();

	// Report functions: as pre-formatted and raw data
#ifdef SHA1_UTILITY_FUNCTIONS
	void ReportHash(char *szReport, unsigned char uReportType = REPORT_HEX);
#endif
	void GetHash(UINT_8 *puDest);

private:
	// Private SHA-1 transformation
	void Transform(UINT_32 *state, UINT_8 *buffer);

	// Member variables
	UINT_8 m_workspace[64];
	SHA1_WORKSPACE_BLOCK *m_block; // SHA1 pointer to the byte array above
};

#endif


#ifdef SHA1_UTILITY_FUNCTIONS
#define SHA1_MAX_FILE_BUFFER 8000
#endif

// Rotate x bits to the left
#ifndef ROL32
#ifdef _MSC_VER
#define ROL32(_val32, _nBits) _rotl(_val32, _nBits)
#else
#define ROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))
#endif
#endif

#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (m_block->l[i] = \
	(ROL32(m_block->l[i],24) & 0xFF00FF00) | (ROL32(m_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (m_block->l[i])
#endif

#define SHABLK(i) (m_block->l[i&15] = ROL32(m_block->l[(i+13)&15] ^ m_block->l[(i+8)&15] \
	^ m_block->l[(i+2)&15] ^ m_block->l[i&15],1))

// SHA-1 rounds
#define _R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define _R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define _R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }

CSHA1::CSHA1()
{
	m_block = (SHA1_WORKSPACE_BLOCK *)m_workspace;

	Reset();
}

CSHA1::~CSHA1()
{
	Reset();
}

void CSHA1::Reset()
{
	// SHA1 initialization constants
	m_state[0] = 0x67452301;
	m_state[1] = 0xEFCDAB89;
	m_state[2] = 0x98BADCFE;
	m_state[3] = 0x10325476;
	m_state[4] = 0xC3D2E1F0;

	m_count[0] = 0;
	m_count[1] = 0;
}

void CSHA1::Transform(UINT_32 *state, UINT_8 *buffer)
{
	// Copy state[] to working vars
	UINT_32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

	memcpy(m_block, buffer, 64);

	// 4 rounds of 20 operations each. Loop unrolled.
	_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
	_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
	_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
	_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
	_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
	_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
	_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
	_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
	_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
	_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
	_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
	_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
	_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
	_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
	_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
	_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
	_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
	_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
	_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
	_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

	// Add the working vars back into state
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	// Wipe variables
#ifdef SHA1_WIPE_VARIABLES
	a = b = c = d = e = 0;
#endif
}

// Use this function to hash in binary data and strings
void CSHA1::Update(UINT_8 *data, UINT_32 len)
{
	UINT_32 i, j;

	j = (m_count[0] >> 3) & 63;

	if((m_count[0] += len << 3) < (len << 3)) m_count[1]++;

	m_count[1] += (len >> 29);

	if((j + len) > 63)
	{
		i = 64 - j;
		memcpy(&m_buffer[j], data, i);
		Transform(m_state, m_buffer);

		for( ; i + 63 < len; i += 64) Transform(m_state, &data[i]);

		j = 0;
	}
	else i = 0;

	memcpy(&m_buffer[j], &data[i], len - i);
}


void CSHA1::Final()
{
	UINT_32 i;
	UINT_8 finalcount[8];

	for(i = 0; i < 8; i++)
		finalcount[i] = (UINT_8)((m_count[((i >= 4) ? 0 : 1)]
			>> ((3 - (i & 3)) * 8) ) & 255); // Endian independent

	Update((UINT_8 *)"\200", 1);

	while ((m_count[0] & 504) != 448)
		Update((UINT_8 *)"\0", 1);

	Update(finalcount, 8); // Cause a SHA1Transform()

	for(i = 0; i < 20; i++)
	{
		m_digest[i] = (UINT_8)((m_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
	}

	// Wipe variables for security reasons
#ifdef SHA1_WIPE_VARIABLES
	i = 0;
	memset(m_buffer, 0, 64);
	memset(m_state, 0, 20);
	memset(m_count, 0, 8);
	memset(finalcount, 0, 8);
	Transform(m_state, m_buffer);
#endif
}

#ifdef SHA1_UTILITY_FUNCTIONS
// Get the final hash as a pre-formatted string
void CSHA1::ReportHash(char *szReport, unsigned char uReportType)
{
	unsigned char i;
	char szTemp[16];

	if(szReport == NULL) return;

	if(uReportType == REPORT_HEX)
	{
		sprintf(szTemp, "%02X", m_digest[0]);
		strcat(szReport, szTemp);

		for(i = 1; i < 20; i++)
		{
			sprintf(szTemp, "%02X", m_digest[i]);
			strcat(szReport, szTemp);
		}
	}
	else if(uReportType == REPORT_DIGIT)
	{
		sprintf(szTemp, "%u", m_digest[0]);
		strcat(szReport, szTemp);

		for(i = 1; i < 20; i++)
		{
			sprintf(szTemp, "%u", m_digest[i]);
			strcat(szReport, szTemp);
		}
	}
	else strcpy(szReport, "Error: Unknown report type!");
}
#endif

// Get the raw message digest
void CSHA1::GetHash(UINT_8 *puDest)
{
	memcpy(puDest, m_digest, 20);
}

//float degtorad(DWORD degrees)
//{
//        return (double) (degrees % 360) * (float)(3.1415926535f / 180.0f);
//}


#define GlobalAllocPtr(flags,cb) (GlobalLock(GlobalAlloc((flags),(cb))))
#define GlobalFreePtr(lp) (GlobalUnlockPtr(lp),(BOOL)GlobalFree(GlobalPtrHandle(lp)))
#define GlobalPtrHandle(lp) ((HGLOBAL)GlobalHandle(lp))
#define GlobalUnlockPtr(lp) GlobalUnlock(GlobalPtrHandle(lp))

static UINT lasterror = FMOD_OK;
#define FMODASSERT(x) {lasterror = x; if(lasterror!=FMOD_OK) {return (double) 0;}}
const int POLYGONSPERBLOCKER = 24;
const int VERTICESPERBLOCKER = 24;

//goto FMODSoundAdd where it is allocated and initialised
typedef struct myinfo
{
    UINT                    effects;
    char                    file[2048];
    bool                    threed;
    bool                    streamed;
    float                   maxvolume;
    float                   group;
    FMOD_CHANNEL *          channel;
    FMOD_DSP *              chorus;
    FMOD_DSP *              echo;
    FMOD_DSP *              flanger;
    FMOD_DSP *              gargle;
    FMOD_DSP *              reverb;
    FMOD_DSP *              compressor;
    FMOD_DSP *              equalizer;
    float                   minmaxset;
    float                   mind;
    float                   maxd;
    float                   coneset;
 	float 					insideconeangle;
  	float 					outsideconeangle;
  	float 					outsidevolume;
    float                   maxdopplerset;
 	float 					maxdoppler;
 	float 					streamready;
    bool                    gotlen;
    UINT                    len;
	FMOD_DSP * moreeffects[10];

} myinfo;


extern "C" {
static char curpassword[256];
static BOOL inited = false;
//static BOOL silent = false;
static FMOD_SYSTEM *mainsystem;
static FMOD_CHANNELGROUP *priority;
static FMOD_CHANNELGROUP *effects;
static FMOD_CHANNELGROUP *ambientmusic;
static FMOD_CHANNELGROUP *ambienteffects;
static float worldscale = 1;
static BOOL distanceonly[4] = {1,1,1,1};
static float deltatime = 0;
static DWORD *geometry = NULL;
static DWORD maxgeometry = 0;
static DWORD curgeometry = 0;
static float samplebuffer[16384];

 double FMODBlockersFree()
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    for (UINT i = 0; i< curgeometry;i++)
    {
        if(*(geometry+i)) FMOD_Geometry_Release((FMOD_GEOMETRY *)(*(geometry+i)));
	}
	GlobalFreePtr(geometry);
    geometry = NULL;
    curgeometry = 0;
	return (double) 1;
}

 double FMODfree(void)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}

    if(geometry) FMODBlockersFree();
    geometry = NULL;

	FMOD_ChannelGroup_Release(priority);
	FMOD_ChannelGroup_Release(effects);
	FMOD_ChannelGroup_Release(ambientmusic);
	FMOD_ChannelGroup_Release(ambienteffects);
	FMOD_CHANNELGROUP *g;
	FMOD_System_GetMasterChannelGroup(mainsystem,&g);
	FMOD_ChannelGroup_Release(g);
	//FMOD_System_Close(mainsystem);
	FMOD_System_Release(mainsystem);
	inited = false;
    return (double) 1;
}


typedef struct mydecrypt
{
    char                   hash1[41];
    char                   hash2[41];
    int                    pat;
    //FILE* dfile;
} mydecrypt;

FMOD_RESULT F_CALLBACK myopen(const char *name, int unicode, unsigned int *filesize, void **handle, void **userdata)
{
    *userdata = NULL;
    if (name)
    {
        //FILE *fp = NULL;
		HANDLE fp = NULL;
		//fp = fopen(name, "rb");
		fp = CreateFileA(name, FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
        if (!fp)
        {
            return FMOD_ERR_FILE_NOTFOUND;
        }

        //fseek(fp, 0, SEEK_END);
        //*filesize = ftell(fp);
        *filesize = SetFilePointer (fp, 0, NULL, FILE_END);

        //fseek(fp,-12,SEEK_END);
        SetFilePointer (fp, -12, NULL, FILE_END);

		char passwordtag[] =  "XMODSIMPLEPW\0\0";
		//fgets(passwordtag,13,fp);
		DWORD bytesread = 0;
		ReadFile(fp,passwordtag,12, &bytesread,NULL);


		//MessageBoxA(GetActiveWindow(),passwordtag,(LPCSTR)"Debug",MB_ICONINFORMATION);
		if(strcmp(passwordtag,"FMODSIMPLEPW") == 0)
	    {
            *filesize = *filesize-53;
            //fseek(fp,-52,SEEK_END);
            SetFilePointer (fp, -52, NULL, FILE_END);
			char hash[] =  "1234567890123456789012345678901234567890\0\0";
			//fgets(hash,41,fp);
			ReadFile(fp,hash,40, &bytesread,NULL);

			char password[256];
			strcpy(password,curpassword);
			char hash1[41];
			memset(hash1,0,41);
			CSHA1 sha1;
			sha1.Update((unsigned char *)password, strlen(password));
			sha1.Final();
			sha1.ReportHash(hash1, CSHA1::REPORT_HEX);
			//MessageBoxA(GetActiveWindow(),hash,"read",MB_ICONINFORMATION);
			if(strcmp(hash,hash1)!=0)
			{
				//MessageBoxA(GetActiveWindow(),hash1,"generated",MB_ICONINFORMATION);
				//fclose (fp);
				CloseHandle((HANDLE)fp);

				*filesize = 0;
				return FMOD_ERR_FILE_BAD;
			}
			//MessageBoxA(GetActiveWindow(),hash1,"debug",MB_samplerateICONINFORMATION);
			CSHA1 sha2;

			char revp[1024];
			memset(revp,0,1024);
			strcpy(revp,password);
			strrev(revp);
			//MessageBoxA(GetActiveWindow(),revp,"debug",MB_ICONINFORMATION);
			char hash2[41];
			memset(hash2,0,41);

			sha2.Update((unsigned char *)revp, strlen(revp));
			sha2.Final();
			sha2.ReportHash(hash2, CSHA1::REPORT_HEX);

            //MessageBoxA(GetActiveWindow(),passwordtag,(LPCSTR)"FOUND",MB_ICONINFORMATION);
            mydecrypt *md = (mydecrypt *)GlobalAllocPtr(GMEM_FIXED|GMEM_ZEROINIT, sizeof(mydecrypt));
			strcpy(md->hash1,hash1);
			strcpy(md->hash2,hash2);
			md->pat = 0;
//			char f[256];
//			strcpy(f,name);
//			strcat(f,".tmp.vaw");
//			md->dfile = fopen(f,"wb");
			*userdata = md;

		}
        //fseek(fp, 0, SEEK_SET);
		SetFilePointer (fp, 0, NULL, FILE_BEGIN);
        *handle = fp;
    }

    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK myclose(void *handle, void *userdata)
{
	if(userdata)
	{
//      mydecrypt *md = (mydecrypt *) userdata;
//		fclose((FILE *)md->dfile);
		GlobalFreePtr(userdata);
		userdata = NULL;
	}
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }

	CloseHandle((HANDLE)handle);
    //fclose((FILE *)handle);

    return FMOD_OK;
}

//static DWORD buff[4096];
//static DWORD br = 0;
FMOD_RESULT F_CALLBACK myread(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }
	//MessageBoxA(GetActiveWindow(),(LPCSTR)buffer,(LPCSTR)"Debugb0",MB_ICONINFORMATION);

    if (bytesread)
    {
		//int fat = ftell((FILE *)handle);
		DWORD fat = SetFilePointer((HANDLE) handle, 0,NULL, FILE_CURRENT);
		//MessageBoxA(GetActiveWindow(),(LPCSTR)buffer,(LPCSTR)"Debugb1",MB_ICONINFORMATION);
        //*bytesread = (int)fread(buffer, 1, sizebytes, (FILE *)handle);
        if(!ReadFile((HANDLE) handle,buffer,(DWORD)sizebytes, (DWORD*)bytesread,NULL))
        {
            return FMOD_ERR_FILE_EOF;
		}
		//*bytesread = (int) br;

		//MessageBoxA(GetActiveWindow(),(LPCSTR)buffer,(LPCSTR)"Debugb2",MB_ICONINFORMATION);

		if(userdata)
		{
			//MessageBoxA(GetActiveWindow(),(LPCSTR)buffer,(LPCSTR)"Debugb3",MB_ICONINFORMATION);
			mydecrypt *md = (mydecrypt *) userdata;
			char *bytes = (char*) buffer;
			md->pat = fat % 40;
			int rol = fat;
			for(UINT i=0;i<*bytesread;i++)
			{
				//(*(bytes+i)) = ((*(bytes+i)) xor md->hash1[(int)(fabs(sin(degtorad(rol)))*40)]) xor md->hash2[md->pat];
				//*bytes = ((*bytes) xor md->hash1[(int)(fabs(sin(degtorad(rol)))*40)]) xor md->hash2[md->pat];
				//*bytes = *bytes xor 'A';
				//*bytes = (*bytes) xor md->hash2[md->pat];
				*bytes = ((*bytes) ^ md->hash1[md->pat] ) ^ md->hash2[md->pat];

				bytes++;
				md->pat++;
				if(md->pat >= 40) md->pat = 0;
				rol++;
			}
			if (*bytesread < sizebytes)
        	{
            	return FMOD_ERR_FILE_EOF;
        	}
//			fwrite(buffer, 1, sizebytes, (FILE *)md->dfile);
			//MessageBoxA(GetActiveWindow(),(LPCSTR)buffer,(LPCSTR)"Debugb4",MB_ICONINFORMATION);
			return FMOD_OK; //always ok.. sorry, not other way for it to work right
    	}
        if (*bytesread < sizebytes)
        {
            return FMOD_ERR_FILE_EOF;
        }
	}
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK myseek(void *handle, unsigned int pos, void *userdata)
{
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }

    //fseek((FILE *)handle, pos, SEEK_SET);
    if(pos != SetFilePointer((HANDLE) handle, pos, NULL, FILE_BEGIN))
    	return FMOD_ERR_FILE_COULDNOTSEEK;

    return FMOD_OK;
}
 double FMODGetNumInstances(void)
{
	if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
	int c = 0;
	FMODASSERT(FMOD_System_GetChannelsPlaying(mainsystem, &c));
	return (double) c;
}

char tagname[2048];
char tagdata[2048];
/*
FMOD_DSP_RESAMPLER samplemethod = (FMOD_DSP_RESAMPLER)1;
double FMODInternalSetResampler(FMOD_DSP_RESAMPLER resamplemethod)
{
    //no need if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}

    if(resamplemethod>= FMOD_DSP_RESAMPLER_MAX) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(resamplemethod<0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    int samplerate;
    FMOD_SOUND_FORMAT format;
    int numoutputchannels;
    int maxinputchannels;
    FMOD_DSP_RESAMPLER rm;
    int bits;

    FMODASSERT(FMOD_System_GetSoftwareFormat(mainsystem,
        &samplerate,
        &format,
        &numoutputchannels,
        &maxinputchannels,
        &rm,
        &bits));

    FMODASSERT(FMOD_System_SetSoftwareFormat(mainsystem,
        samplerate,
        format,
        numoutputchannels,
        maxinputchannels,
        (FMOD_DSP_RESAMPLER) (DWORD)resamplemethod));
    return (double)1;
}
*/
 double FMODinit(double maxsounds, double supportwebfiles = 0)
{
    if(inited) {FMODASSERT(FMOD_ERR_INITIALIZED);}

    memset(curpassword,0,256);
    memset(samplebuffer,0,16384*sizeof(float));
    memset(tagname,0,2048);
    memset(tagdata,0,2048);

    FMOD_RESULT result;
     unsigned int     version;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
    int              numdrivers;


    /*
        Create a System object and initialize.
    */

    FMODASSERT(FMOD_System_Create(&mainsystem));		// Create the main system object.

    FMODASSERT(FMOD_System_GetVersion(mainsystem,&version));

    if (version < FMOD_VERSION)
    {
        MessageBeep(MB_ICONWARNING);
		MessageBoxA(GetActiveWindow(),"initFMOD-Incompatible fmodex.dll found. It's possible another game or product is running using and older version of this dll or the supplied fmod dll is incompatible.", "FMOD Error", MB_ICONSTOP);
        FMODASSERT(FMOD_ERR_VERSION);
    }
    FMODASSERT(FMOD_System_GetNumDrivers(mainsystem,&numdrivers));

    if (numdrivers == 0)
    {
        FMODASSERT(FMOD_System_SetOutput(mainsystem,FMOD_OUTPUTTYPE_NOSOUND));
    }
    else
    {
        FMODASSERT(FMOD_System_GetDriverCaps(mainsystem, 0, &caps, 0, 0, &speakermode));
        FMODASSERT(FMOD_System_SetSpeakerMode(mainsystem,speakermode));       /* Set the user selected speaker mode. */

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
            MessageBeep(MB_ICONWARNING);
            MessageBoxA(GetActiveWindow(),"You've turned windows 'Acceleration' too low and will experience major slow downs. You can fix this in the control panel's hardware acceleration options. Location varies depending your Windows version.", "FMOD Warning", MB_ICONWARNING);
            //Dont care if this fails
            FMOD_System_SetDSPBufferSize(mainsystem,1024, 10);    /* At 48khz, the latency between issuing an fmod command and hearing it will now be about 213ms. */
        }
        //Dont care if this fails
        FMOD_System_SetHardwareChannels(mainsystem, 32, 64, 32, 64);
        FMOD_System_SetSoftwareChannels(mainsystem,100);
    }
//    if(FMODInternalSetResampler(samplemethod) == 0) return 0;
    result = FMOD_System_Init(mainsystem,(int)maxsounds, FMOD_INIT_NORMAL /*|FMOD_INIT_VOL0_BECOMES_VIRTUAL*/ , 0);	// Initialize FMOD.
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
       FMOD_System_SetSpeakerMode(mainsystem,FMOD_SPEAKERMODE_STEREO);
       result = FMOD_System_Init(mainsystem,(int)maxsounds, FMOD_INIT_NORMAL /*| FMOD_INIT_VOL0_BECOMES_VIRTUAL*/ , 0);
    }
    FMODASSERT(result);

    if(supportwebfiles)
        FMODASSERT(FMOD_System_SetStreamBufferSize(mainsystem,64*1024, FMOD_TIMEUNIT_RAWBYTES));

    FMODASSERT(FMOD_System_CreateChannelGroup(mainsystem, "priority", &priority));
    FMODASSERT(FMOD_System_CreateChannelGroup(mainsystem, "effects", &effects));
    FMODASSERT(FMOD_System_CreateChannelGroup(mainsystem, "ambientmusic", &ambientmusic));
    FMODASSERT(FMOD_System_CreateChannelGroup(mainsystem, "ambienteffects", &ambienteffects));
    FMOD_CHANNELGROUP *maingrp;
	FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem, &maingrp));
	FMODASSERT(FMOD_ChannelGroup_AddGroup(maingrp,priority));
	FMODASSERT(FMOD_ChannelGroup_AddGroup(maingrp,effects));
	FMODASSERT(FMOD_ChannelGroup_AddGroup(maingrp,ambientmusic));
	FMODASSERT(FMOD_ChannelGroup_AddGroup(maingrp,ambienteffects));

	if(!supportwebfiles)
		FMODASSERT(FMOD_System_SetFileSystem(mainsystem,myopen, myclose, myread, myseek,2048));

    inited = true;
    return (double) 1;
}


 double FMODSoundAddEffect(double sound, double effect, double pos)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    if((pos<0)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if((pos>9)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if((effect<FMOD_DSP_TYPE_MIXER)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if((effect>FMOD_DSP_TYPE_LOWPASS_SIMPLE)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    void *a = NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    myinfo *mi = (myinfo *) a;

 	FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,(FMOD_DSP_TYPE)(DWORD)effect,&mi->moreeffects[(int)pos]));

    return (double) 1;
}

const UINT se_none= 0;
const UINT se_chorus = 1;
const UINT se_echo= 2;
const UINT se_flanger= 4;
const UINT se_gargle= 8;
const UINT se_reverb= 16;
const UINT se_compressor= 32;
const UINT se_equalizer= 64;

 double FMODSoundSetEffects(double sound, double effects)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a =NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    myinfo *mi = (myinfo *) a;
    mi->effects = (UINT)effects;
	if(effects != se_none)
	{
		FMOD_DSP *dsp;
        if((UINT)effects & se_chorus)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_CHORUS,&dsp));
    		mi->chorus = dsp;
		}
        if((UINT)effects & se_echo)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_ECHO,&dsp));
    		mi->echo = dsp;
		}
        if((UINT)effects & se_flanger)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_FLANGE,&dsp));
			mi->flanger = dsp;
		}
        if((UINT)effects & se_gargle)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_DISTORTION,&dsp));
    		mi->gargle = dsp;
		}
        if((UINT)effects & se_reverb)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_REVERB,&dsp));
			mi->reverb = dsp;
		}
        if((UINT)effects & se_compressor)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_COMPRESSOR,&dsp));
    		mi->compressor = dsp;
		}
        if((UINT)effects & se_equalizer)
        {
			FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,FMOD_DSP_TYPE_HIGHPASS,&dsp));
    		mi->equalizer = dsp;
		}
	}

    return (double) 1;
}

 double FMODSoundSetGroup(double sound, double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    if(group<1 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    myinfo *mi = (myinfo *) a;
    mi->group = (float)group;
    return (double) 1;
}

 double FMODMasterSetVolume(double volume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_CHANNELGROUP *maingrp = NULL;
	FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem, &maingrp));
	if(maingrp==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMODASSERT(FMOD_ChannelGroup_SetVolume(maingrp,(float)max(0,min(volume,1))));
    return (double) 1;
}

 double FMODGroupSetVolume(double group, double volume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}


    if(group == 0)
    {
        return (double) FMODMasterSetVolume(volume);
    }
	else if(group == 1)
    {
        FMODASSERT(FMOD_ChannelGroup_SetVolume(priority,(float)max(0,min(volume,1))));
    }
    else if(group == 2)
    {
        FMODASSERT(FMOD_ChannelGroup_SetVolume(effects,(float)max(0,min(volume,1))));
    }
    else if(group == 3)
    {
        FMODASSERT(FMOD_ChannelGroup_SetVolume(ambientmusic,(float)max(0,min(volume,1))));
    }
    else if(group == 4)
    {
        FMODASSERT(FMOD_ChannelGroup_SetVolume(ambienteffects,(float)max(0,min(volume,1))));
    }
    return (double) 1;
}


 double FMODSoundSetMaxVolume(double sound, double volume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    void *a=NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    myinfo *mi = (myinfo *) a;
    mi->maxvolume = (float)max(0,min(volume,1));
    return (double) 1;
}


 double FMODSoundInstanciate(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a=NULL;


    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

//MessageBoxA(GetActiveWindow(),"","",0);
    FMOD_CHANNEL *channel = NULL;
    //char t[] = "123456789";
//    wsprintf( t, "%ld", (int)FMODGetNumInstances());
//	MessageBoxA(GetActiveWindow(),t,"",0);
    FMODASSERT(FMOD_System_PlaySound(mainsystem, FMOD_CHANNEL_FREE, (FMOD_SOUND *)(DWORD)sound, true /*true*/, &channel));
    //wsprintf(t,"%ld", (int) FMODGetNumInstances());
	//MessageBoxA(GetActiveWindow(),t,"",0);

	myinfo *mi = (myinfo *) a;
    mi->channel = channel;
    FMODASSERT(FMOD_Channel_SetVolume(channel,mi->maxvolume));
    if(mi->group == 1)
    {
        FMODASSERT(FMOD_Channel_SetChannelGroup(channel,priority));
        FMODASSERT(FMOD_Channel_SetPriority(channel,0));
    }
    else if(mi->group == 2)
    {
        FMODASSERT(FMOD_Channel_SetChannelGroup(channel,effects));
        FMODASSERT(FMOD_Channel_SetPriority(channel,64));
    }
    else if(mi->group == 3)
    {
        FMODASSERT(FMOD_Channel_SetChannelGroup(channel,ambientmusic));
        FMODASSERT(FMOD_Channel_SetPriority(channel,128));
    }
    else if(mi->group == 4)
    {
        FMODASSERT(FMOD_Channel_SetChannelGroup(channel,ambienteffects));
		FMODASSERT(FMOD_Channel_SetPriority(channel,192));
    }

    if(mi->effects)
    {
		if(mi->chorus)
		{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->chorus,0));
		}
    	if(mi->echo)
    	{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->echo,0));
		}
		if(mi->gargle)
    	{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->gargle,0));
		}
		if(mi->flanger)
    	{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->flanger,0));
		}
		if(mi->reverb)
    	{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->reverb,0));
		}
		if(mi->compressor)
    	{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->compressor,0));
		}
		if(mi->equalizer)
		{
	        FMODASSERT(FMOD_Channel_AddDSP(channel,mi->equalizer,0));
		}
	}
	for(int i =0; i<10; i++)
	{
		if(mi->moreeffects[i] != NULL) {{FMODASSERT(FMOD_Channel_AddDSP(channel,mi->moreeffects[i],0))}};
	}

    return (double) (DWORD) channel;

}
 double FMODSoundLoop(double sound, double paused = 0)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_CHANNEL * channel = (FMOD_CHANNEL *)(DWORD) FMODSoundInstanciate(sound);
	if(channel == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	if(mi->threed == TRUE  && mi->streamready == 0.0f)
	{
		MessageBoxA(GetActiveWindow(),"Trying to loop a 3d sound in 2d mode",mi->file,MB_ICONINFORMATION);
	}

    FMODASSERT(FMOD_Channel_SetLoopCount(channel,-1));
    FMODASSERT(FMOD_Channel_SetMode(channel,FMOD_LOOP_NORMAL));
    if(!paused) {FMODASSERT(FMOD_Channel_SetPaused(channel,false));}
    return (double) (DWORD) channel;
}
 double FMODSoundPlay(double sound, double paused = 0)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_CHANNEL * channel = (FMOD_CHANNEL *)(DWORD) FMODSoundInstanciate(sound);
	if(channel == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
    FMODASSERT(FMOD_Channel_SetMode(channel,FMOD_LOOP_OFF));
	if(mi->threed == TRUE && mi->streamready == 0.0f)
	{
		MessageBoxA(GetActiveWindow(),"Trying to play a 3d sound in 2d mode",mi->file,MB_ICONINFORMATION);
	}

    if(!paused) FMODASSERT(FMOD_Channel_SetPaused(channel,false));
    return (double) (DWORD) channel;
}
 double FMODSoundLoop3d(double sound, double x, double y, double z, double paused)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_CHANNEL * channel = (FMOD_CHANNEL *)(DWORD) FMODSoundInstanciate(sound);
	if(channel == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	if(mi->threed == FALSE && mi->streamready == 0.0f)
	{
		MessageBoxA(GetActiveWindow(),"Trying to loop a non 3d sound in 3d mode",mi->file,MB_ICONINFORMATION);
	}
    FMODASSERT(FMOD_Channel_SetLoopCount(channel,-1));
    FMODASSERT(FMOD_Channel_SetMode(channel,FMOD_3D_WORLDRELATIVE | FMOD_LOOP_NORMAL));

    FMOD_VECTOR pos;
    pos.x = (float)x;//* worldscale;
    pos.y = (float)y;//* worldscale;
    pos.z = (float)z;//* worldscale;
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
	FMODASSERT(FMOD_Channel_Set3DAttributes(channel,&pos,&vel));


	if(mi->minmaxset)
	{
		FMODASSERT(FMOD_Channel_Set3DMinMaxDistance(channel, mi->mind,mi->maxd));
		//FMODASSERT(FMOD_Sound_Set3DMinMaxDistance((FMOD_SOUND *)(DWORD)sound, mi->mind,mi->maxd));
	}
	if(mi->coneset)
	{
		FMODASSERT(FMOD_Channel_Set3DConeSettings(channel, mi->insideconeangle,mi->outsideconeangle,mi->outsidevolume));
		//FMODASSERT(FMOD_Sound_Set3DConeSettings((FMOD_SOUND *)(DWORD)sound,  mi->insideconeangle,mi->outsideconeangle,mi->outsidevolume));
	}
	if(mi->maxdopplerset)
	{
		FMODASSERT(FMOD_Channel_Set3DDopplerLevel(channel, mi->maxdoppler));
	}
	if(!paused) FMODASSERT(FMOD_Channel_SetPaused(channel,false));
    return (double) (DWORD) channel;
}

 double FMODSoundPlay3d(double sound, double x, double y, double z, double paused)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	if(mi->threed == FALSE && mi->streamready == 0.0f)
	{
		MessageBoxA(GetActiveWindow(),"Trying to play a non 3d sound in 3d mode",mi->file,MB_ICONINFORMATION);
	}
    FMOD_CHANNEL * channel = (FMOD_CHANNEL *)(DWORD) FMODSoundInstanciate(sound);
	if(channel == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMODASSERT(FMOD_Channel_SetMode(channel,FMOD_3D_WORLDRELATIVE | FMOD_LOOP_OFF));

    FMOD_VECTOR pos;
    pos.x = (float)x;//* worldscale;
    pos.y = (float)y;//* worldscale;
    pos.z = (float)z;//* worldscale;
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
	FMODASSERT(FMOD_Channel_Set3DAttributes(channel,&pos,&vel));


	if(mi->minmaxset)
	{
		FMODASSERT(FMOD_Channel_Set3DMinMaxDistance(channel, mi->mind,mi->maxd));
		//FMODASSERT(FMOD_Sound_Set3DMinMaxDistance((FMOD_SOUND *)(DWORD)sound, mi->mind,mi->maxd));
	}
	if(mi->coneset)
	{
		FMODASSERT(FMOD_Channel_Set3DConeSettings(channel, mi->insideconeangle,mi->outsideconeangle,mi->outsidevolume));
		//FMODASSERT(FMOD_Sound_Set3DConeSettings((FMOD_SOUND *)(DWORD)sound,  mi->insideconeangle,mi->outsideconeangle,mi->outsidevolume));
	}
	if(mi->maxdopplerset)
	{
		FMODASSERT(FMOD_Channel_Set3DDopplerLevel(channel, mi->maxdoppler));
	}

	if(!paused) FMODASSERT(FMOD_Channel_SetPaused(channel,false));
    return (double) (DWORD) channel;
}


 double FMODInstanceSet3dPosition(double channel,double x,double y,double z)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(channel>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_VECTOR pos = {0,0,0};

    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
    if(deltatime)
    {
		FMODASSERT(FMOD_Channel_Get3DAttributes((FMOD_CHANNEL*)(DWORD)channel,&pos,0));
		vel.x = ((float)x-pos.x)*worldscale * deltatime;
		vel.y = ((float)y-pos.y)*worldscale * deltatime;
		vel.z = ((float)z-pos.z)*worldscale * deltatime;
	}
    pos.x = (float)x;//* worldscale;
    pos.y = (float)y;//* worldscale;
    pos.z = (float)z;//* worldscale;
	FMODASSERT(FMOD_Channel_Set3DAttributes((FMOD_CHANNEL*)(DWORD)channel,&pos,&vel));

	return (double) 1;
}
 double FMODSetPassword(LPCSTR password)
{
	strcpy(curpassword,password);
	return (double) 1;
}
 double FMODSoundIsStreamed(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	return (double) mi->streamed;
}
 double FMODSoundIs3d(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	return (double) mi->threed;
}
 double FMODSoundAdd(string soundfile, double threed = 0, double streamed = 0)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_SOUND *sound = NULL;

    DWORD flags = FMOD_LOOP_NORMAL | FMOD_2D | FMOD_SOFTWARE|FMOD_ACCURATETIME;
    if(threed)
	{
        //MessageBoxA(GetActiveWindow(),soundfile,(LPCSTR)"3d",MB_ICONINFORMATION);
		flags = FMOD_LOOP_NORMAL | FMOD_3D | FMOD_SOFTWARE | FMOD_3D_LINEARROLLOFF|FMOD_ACCURATETIME;
	}
	else
	{
        //		MessageBoxA(GetActiveWindow(),soundfile,(LPCSTR)"2D",MB_ICONINFORMATION);
	}
    if(streamed)
    {
	    FMODASSERT(FMOD_System_CreateStream(mainsystem, soundfile.c_str(), flags, 0, &sound));
        //FMODASSERT(FMOD_System_CreateSound(mainsystem, soundfile, flags| FMOD_CREATESTREAM | FMOD_NONBLOCKING, 0, &sound));
    //char t[] = "123456789";
    //wsprintf( t, "%ld", (int)sound);
//	MessageBoxA(GetActiveWindow(),t,"",0);

    }
	else
    {
		FMODASSERT(FMOD_System_CreateSound(mainsystem, soundfile.c_str(), flags, 0, &sound));
	}

	if(sound == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	//MessageBoxA(GetActiveWindow(),"2","",0);

    myinfo *mi = (myinfo *)GlobalAllocPtr(GMEM_FIXED|GMEM_ZEROINIT,(sizeof(myinfo)));
	//MessageBoxA(GetActiveWindow(),"3","",0);
    if(mi == NULL) {FMODASSERT(FMOD_ERR_MEMORY);}
    mi->maxvolume = 1;
    strcpy(mi->file,soundfile.c_str());
    mi->threed = threed != 0.0f;
    mi->streamed = streamed != 0.0f;
	//all other members set by GMEM_ZEROINIT
	//MessageBoxA(GetActiveWindow(),"4","",0);

    FMODASSERT(FMOD_Sound_SetUserData(sound, (void *)mi));
	//MessageBoxA(GetActiveWindow(),"5","",0);

	return (double) (DWORD) sound;
}



 double FMODListenerSetNumber(double number)
{
	if(number <1 || number > 4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	FMODASSERT(FMOD_System_Set3DNumListeners(mainsystem, (int)number));
    return (double) 1;
}


 double FMODListenerSet3dPosition(double number, double x, double y, double z)
{
	if(number <1 || number > 4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_VECTOR pos = {0,0,0};

    FMOD_VECTOR forward        = { 0.0f, 0.0f, -1.0f };
    FMOD_VECTOR up             = { 0.0f, -1.0f, 0.0f };
    if(distanceonly[(int)number-1]) up.y = 0;
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
    if(deltatime)
    {
		FMODASSERT(FMOD_System_Get3DListenerAttributes(mainsystem,(int)number-1,&pos,0,0,0));
		vel.x = ((float)x-pos.x)*worldscale * deltatime;
		vel.y = ((float)y-pos.y)*worldscale * deltatime;
		vel.z = ((float)z-pos.z)*worldscale * deltatime;
	}
    pos.x = (float)x;//* worldscale;
    pos.y = (float)y;//* worldscale;
    pos.z = (float)z;//* worldscale;

	FMODASSERT(FMOD_System_Set3DListenerAttributes(mainsystem, (int)number-1,&pos,&vel,&forward,&up));
    return (double) 1;
}

 double FMODListenerSet3dPositionEx(double number, double x, double y, double z, double fx, double fy, double fz, double ux, double uy, double uz)
{
	if(number <1 || number > 4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_VECTOR pos = {0,0,0};

    FMOD_VECTOR forward = {(float)fx, (float)fy, (float)fz};
    FMOD_VECTOR up = {(float)ux, (float)uy, (float)uz};
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
    if(deltatime)
    {
		FMODASSERT(FMOD_System_Get3DListenerAttributes(mainsystem,(int)number-1,&pos,0,0,0));
		vel.x = ((float)x-pos.x)*worldscale * deltatime;
		vel.y = ((float)y-pos.y)*worldscale * deltatime;
		vel.z = ((float)z-pos.z)*worldscale * deltatime;
	}
    pos.x = (float)x;//* worldscale;
    pos.y = (float)y;//* worldscale;
    pos.z = (float)z;//* worldscale;

	FMODASSERT(FMOD_System_Set3DListenerAttributes(mainsystem, (int)number-1,&pos,&vel,&forward,&up));
    return (double) 1;
}

 double FMODSetWorldScale(double scale)
{

	//FMODASSERT(FMOD_System_Set3DSettings(mainsystem, 1,scale,1));
    worldscale = (float)scale;
    return (double) 1;
}

 double FMODSoundSet3dMinMaxDistance(double sound, double Min, double Max)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a = NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *mi = (myinfo *) a;
	mi->minmaxset = TRUE;
	mi->mind = max(0,(float)Min /**worldscale*/);
	mi->maxd = max((float)Max/**worldscale*/,mi->mind);

    return (double) 1;
}


 double FMODUpdate(void)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
	FMODASSERT(FMOD_System_Update(mainsystem));
	return (double) 1;
}

 double FMODSoundFree(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    void *a = NULL;

    FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a);
	if(a!=NULL)
	{

		myinfo *mi = (myinfo *) a;
		if(mi->effects)
		{
			if(mi->chorus)
			{
	        	FMOD_DSP_Release(mi->chorus);
			}
	    	if(mi->echo)
	    	{
		        FMOD_DSP_Release(mi->echo);
			}
			if(mi->gargle)
	    	{
		        FMOD_DSP_Release(mi->gargle);
			}
			if(mi->flanger)
	    	{
		        FMOD_DSP_Release(mi->flanger);
			}
			if(mi->reverb)
	    	{
		        FMOD_DSP_Release(mi->reverb);
			}
			if(mi->compressor)
		 	{
			    FMOD_DSP_Release(mi->compressor);
			}
			if(mi->equalizer)
			{
	 	       FMOD_DSP_Release(mi->equalizer);
			}
			for(int i =0; i<10; i++)
			{
				if(mi->moreeffects[i] != NULL) FMOD_DSP_Release(mi->moreeffects[i]);
			}
			GlobalFreePtr(a);
	    }
	}
    DWORD t = GetTickCount()+5000;
    do
    {
        FMOD_OPENSTATE openstate = FMOD_OPENSTATE_READY;
        UINT result = FMOD_Sound_GetOpenState((FMOD_SOUND*)(DWORD)sound,&openstate, 0, 0);

        if (openstate == FMOD_OPENSTATE_READY || openstate == FMOD_OPENSTATE_ERROR || result != FMOD_OK || GetTickCount()>t)
        {
            break;
        }
        FMODUpdate();
        Sleep(10);
    } while (1);
    FMOD_Sound_Release((FMOD_SOUND*)(DWORD)sound);
    return (double) 1;
}

 double FMODAllStop(void)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_CHANNELGROUP * maingrp = NULL;
	FMOD_System_GetMasterChannelGroup(mainsystem, &maingrp);
    FMODASSERT(FMOD_ChannelGroup_Stop(maingrp));
    //FMOD_System_Close(mainsystem);
    return (double) 1;
}

 double FMODGroupStop(double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(group == 0)
    {
        return (double) FMODAllStop();
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_Stop(priority));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_Stop(effects));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_Stop(ambientmusic));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_Stop(ambienteffects));
	}
	return (double) 1;
}



 double FMODInstanceStop(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMODASSERT(FMOD_Channel_Stop((FMOD_CHANNEL *) (DWORD) instance));
    return (double) 1;
}

 double FMODInstanceIsPlaying(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMOD_BOOL p = 0;
    FMODASSERT(FMOD_Channel_IsPlaying((FMOD_CHANNEL *) (DWORD) instance,&p))
    return (double) p;
}


 double FMODAllSetMuted(double mute)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_CHANNELGROUP * maingrp = NULL;
	FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem, &maingrp));
    if(maingrp==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_ChannelGroup_SetMute(maingrp, (FMOD_BOOL)mute));
    return (double) 1;
}

 double FMODGroupSetMuted(double group, double mute)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(group == 0)
    {
		return (double) FMODAllSetMuted(mute);
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_SetMute(priority, (FMOD_BOOL)mute));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_SetMute(effects, (FMOD_BOOL)mute));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_SetMute(ambientmusic, (FMOD_BOOL)mute));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_SetMute(ambienteffects, (FMOD_BOOL)mute));
	}
	return (double) 1;
}

 double FMODInstanceSetMuted(double instance, double mute)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMODASSERT(FMOD_Channel_SetMute((FMOD_CHANNEL *) (DWORD) instance, (FMOD_BOOL)mute));
    return (double) 1;
}

 double FMODAllSetPaused(double Pause)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_CHANNELGROUP * maingrp = NULL;
	FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem, &maingrp));
    if(maingrp==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_ChannelGroup_SetPaused(maingrp, (FMOD_BOOL)Pause));
    return (double) 1;
}

 double FMODGroupSetPaused(double group, double Pause)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(group == 0)
    {
		return (double) FMODAllSetPaused(Pause);
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_SetPaused(priority, (FMOD_BOOL)Pause));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_SetPaused(effects, (FMOD_BOOL)Pause));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_SetPaused(ambientmusic, (FMOD_BOOL)Pause));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_SetPaused(ambienteffects, (FMOD_BOOL)Pause));
	}
	return (double) 1;
}

 double FMODInstanceSetPaused(double instance, double Pause)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMODASSERT(FMOD_Channel_SetPaused((FMOD_CHANNEL *) (DWORD) instance, (FMOD_BOOL)Pause));
    return (double) 1;
}


 double FMODInstanceSetVolume(double instance, double volume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMOD_SOUND * sound = NULL;
	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *) (DWORD) sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	myinfo *d = (myinfo*) a;
	FMODASSERT(FMOD_Channel_SetVolume((FMOD_CHANNEL *) (DWORD) instance, max(0,min((float)volume, 1))*d->maxvolume));
	return (double) 1;
}

 double FMODSetDopplerFPS(double fps)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    deltatime = 0;
    if(fps==0) return (double) 1;
    deltatime = (float)1/(float)fps * (float)1000;
    deltatime = (float)1000/(float)deltatime;
	return (double) 1;
}

 double FMODListenerHearsDistanceOnly(double number, double t)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(number<1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(number>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	distanceonly[(int)number-1] = (BOOL) t;
	return (double) 1;
}
 double FMODGetLastError(void)
{
	return (double) lasterror;
}


 double FMODSoundSet3dCone(double sound, double insideconeangle, double outsideconeangle, double outsidevolume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a = NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	myinfo *mi = (myinfo *) a;
	mi->coneset = TRUE;
	mi->insideconeangle = (float)insideconeangle;
	mi->insideconeangle = (float)outsideconeangle;
	mi->outsidevolume = max(min((float)outsidevolume,1),0);

    return (double) 1;
}

 double FMODInstanceSet3dConeOrientation(double instance, double x, double y, double z)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_VECTOR v = {(float)x,(float)y,(float)z};
    FMODASSERT(FMOD_Channel_Set3DConeOrientation((FMOD_CHANNEL *) (DWORD) instance, &v));

    return (double) 1;
}

 double FMODSoundSet3dDopplerMax(double sound, double dmax)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a = NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	myinfo *mi = (myinfo *) a;
	mi->maxdopplerset = TRUE;
	mi->maxdoppler = max(0,min((float)dmax,5));

    return (double) 1;
}

 double FMODInstanceSetFrequency(double instance, double freq)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMODASSERT(FMOD_Channel_SetFrequency((FMOD_CHANNEL *) (DWORD) instance, (float)freq));

    return (double) 1;
}
 double FMODInstanceGetFrequency(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float f = 0;
    FMODASSERT(FMOD_Channel_GetFrequency((FMOD_CHANNEL *) (DWORD) instance, &f));

    return (double) f;
}

 double FMODGroupSetFrequency(double group, double freq)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_OverrideFrequency(g, (float)freq));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_OverrideFrequency(priority, (float)freq));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_OverrideFrequency(effects, (float)freq));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_OverrideFrequency(ambientmusic, (float)freq));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_OverrideFrequency(ambienteffects, (float)freq));
	}
	return (double) 1;
}



 double FMODGroupGetVolume(double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    float vol = 0;
    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT (FMOD_ChannelGroup_GetVolume(g, &vol));
	}
    else if(group == 1)
    {
		FMODASSERT (FMOD_ChannelGroup_GetVolume(priority, &vol));
	}
    else if(group == 2)
    {
		FMODASSERT (FMOD_ChannelGroup_GetVolume(effects, &vol));
	}
    else if(group == 3)
    {
		FMODASSERT (FMOD_ChannelGroup_GetVolume(ambientmusic, &vol));
	}
    else if(group == 4)
    {
		FMODASSERT (FMOD_ChannelGroup_GetVolume(ambienteffects, &vol));
	}
	return (double) vol;

}
 double FMODGroupGetPitch(double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    float vol = 0;
    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT (FMOD_ChannelGroup_GetPitch(g, &vol));
	}
    else if(group == 1)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPitch(priority, &vol));
	}
    else if(group == 2)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPitch(effects, &vol));
	}
    else if(group == 3)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPitch(ambientmusic, &vol));
	}
    else if(group == 4)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPitch(ambienteffects, &vol));
	}
	return (double) vol;

}

 double FMODGroupGetPaused(double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_BOOL vol = 0;
    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT (FMOD_ChannelGroup_GetPaused(g, &vol));
	}
    else if(group == 1)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPaused(priority, &vol));
	}
    else if(group == 2)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPaused(effects, &vol));
	}
    else if(group == 3)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPaused(ambientmusic, &vol));
	}
    else if(group == 4)
    {
		FMODASSERT (FMOD_ChannelGroup_GetPaused(ambienteffects, &vol));
	}
	return (double) vol;
}

 double FMODGroupGetMuted(double group)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_BOOL vol = 0;
    if(group == 0)
    {
		FMOD_CHANNELGROUP *g= NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT (FMOD_ChannelGroup_GetMute(g, &vol));
	}
    else if(group == 1)
    {
		FMODASSERT (FMOD_ChannelGroup_GetMute(priority, &vol));
	}
    else if(group == 2)
    {
		FMODASSERT (FMOD_ChannelGroup_GetMute(effects, &vol));
	}
    else if(group == 3)
    {
		FMODASSERT (FMOD_ChannelGroup_GetMute(ambientmusic, &vol));
	}
    else if(group == 4)
    {
		FMODASSERT (FMOD_ChannelGroup_GetMute(ambienteffects, &vol));
	}
	return (double) vol;
}

 double FMODGroupSetPitch(double group,double vol)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g=NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT (FMOD_ChannelGroup_SetPitch(g, (float)vol));
	}
    else if(group == 1)
    {
		FMODASSERT (FMOD_ChannelGroup_SetPitch(priority, (float)vol));
	}
    else if(group == 2)
    {
		FMODASSERT (FMOD_ChannelGroup_SetPitch(effects, (float)vol));
	}
    else if(group == 3)
    {
		FMODASSERT (FMOD_ChannelGroup_SetPitch(ambientmusic, (float)vol));
	}
    else if(group == 4)
    {
		FMODASSERT (FMOD_ChannelGroup_SetPitch(ambienteffects, (float)vol));
	}
	return (double) 1;

}

 double FMODInstanceGetVolume(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMOD_SOUND * sound = NULL;
	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	void *a=NULL;
	float v = 0;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *) (DWORD) sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	myinfo *d = (myinfo*) a;

	FMODASSERT(FMOD_Channel_GetVolume((FMOD_CHANNEL *) (DWORD) instance, &v));
	v /= d->maxvolume;
	return (double) v;
}

 double FMODInstanceGetSound(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_SOUND * sound = NULL;
	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	return (double)(DWORD) sound;
}

 double FMODInstanceGetSpeakerMix(double instance,
        float *  frontleft,
        float *  frontright,
        float *  center,
        float *  lfe,
        float *  backleft,
        float *  backright,
        float *  sideleft,
        float *  sideright)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	FMODASSERT(FMOD_Channel_GetSpeakerMix((FMOD_CHANNEL *) (DWORD) instance,
        frontleft,
        frontright,
        center,
        lfe,
        backleft,
        backright,
        sideleft,
        sideright));
	return 1;
}

 double FMODSoundGetMaxVolume(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *) (DWORD) sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    myinfo *mi = (myinfo*) a;
	return (double)mi->maxvolume;
}

 double FMODSoundGetMaxDist(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	void *a = NULL;
    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *) (DWORD) sound, &a));
	if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    myinfo *mi = (myinfo*) a;
	return (double)mi->maxd;
}

 double FMODInstanceGetPaused(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	FMOD_BOOL v = 0;
	FMODASSERT(FMOD_Channel_GetPaused((FMOD_CHANNEL *) (DWORD) instance, &v));
	return (double) v;
}

 double FMODInstanceGetMuted(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	FMOD_BOOL v = 0;
	FMODASSERT(FMOD_Channel_GetMute((FMOD_CHANNEL *) (DWORD) instance, &v));
	return (double) v;
}

 double FMODSoundGetLength(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    void *a = NULL;

    FMODASSERT(FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a));
    if(a==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    myinfo *mi = (myinfo *) a;

    if(mi->gotlen)
        return (double)mi->len;

	UINT l;
    l = 0;
	FMODASSERT(FMOD_Sound_GetLength((FMOD_SOUND *) (DWORD) sound, &l, FMOD_TIMEUNIT_MS));
    mi->gotlen = true;
    mi->len = l;
	return (double) l;
}

 double FMODInstanceGetPosition(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	UINT p;
	p = 0;
    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	//FMODASSERT(FMOD_Sound_GetLength(sound, &l, FMOD_TIMEUNIT_MS));
	FMODASSERT(FMOD_Channel_GetPosition((FMOD_CHANNEL *) (DWORD) instance, &p, FMOD_TIMEUNIT_MS));
	return (double) p/max(FMODSoundGetLength((double)(DWORD)sound),1);
    //return (double) p/max(l,1);
}

 double FMODInstanceSetPosition(double instance, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	if(p<0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(p>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	//UINT l = 0;
    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	//FMODASSERT(FMOD_Sound_GetLength(sound, &l, FMOD_TIMEUNIT_MS));
	//FMODASSERT(FMOD_Channel_SetPosition((FMOD_CHANNEL *) (DWORD) instance, (UINT)(p*l), FMOD_TIMEUNIT_MS));

    FMODASSERT(FMOD_Channel_SetPosition((FMOD_CHANNEL *) (DWORD) instance, (UINT)(p*FMODSoundGetLength((double)(DWORD)sound)), FMOD_TIMEUNIT_MS));
	return (double) 1;
}



 double FMODInstanceSetPan(double instance, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	if(p<-1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}};
	if(p>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}};
 	FMODASSERT(FMOD_Channel_SetPan((FMOD_CHANNEL *) (DWORD) instance, (float)p));
	return (double) 1;
}

 double FMODInstanceGetPan(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	float p = 0;
 	FMODASSERT(FMOD_Channel_GetPan((FMOD_CHANNEL *) (DWORD) instance, &p));
	return (double) p;
}

 double FMODInstanceSetLoopCount(double instance, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	if(p<-1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
 	FMODASSERT(FMOD_Channel_SetLoopCount((FMOD_CHANNEL *) (DWORD) instance, (int)p));
	return (double) 1;
}
 double FMODInstanceSet3dMinMaxDistance(double instance, double Min, double Max)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMODASSERT(FMOD_Channel_Set3DMinMaxDistance((FMOD_CHANNEL *)(DWORD)instance, (float)Min, (float) Max));
    return (double) 1;
}
 double FMODInstanceSet3dDopplerMax(double instance, double doppler)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMODASSERT(FMOD_Channel_Set3DDopplerLevel((FMOD_CHANNEL *)(DWORD)instance, (float)doppler));
    return (double) 1;
}
 double FMODInstanceSet3dCone(double instance, double insideconeangle, double outsideconeangle, double outsidevolume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


	FMODASSERT(FMOD_Channel_Set3DConeSettings((FMOD_CHANNEL *)(DWORD)instance, (float)insideconeangle,(float)outsideconeangle,(float)outsidevolume));
    return (double) 1;
}

 double FMODInstanceGetLoopCount(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	int p = 0;
 	FMODASSERT(FMOD_Channel_GetLoopCount((FMOD_CHANNEL *) (DWORD) instance, &p));
	return (double) p;
}

 double FMODBlockersInit(double NumBlockers, double xs, double ys, double zs)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry != NULL) FMODBlockersFree();

	float mzs = max((float)zs,100.0f);
	float mys = max((float)ys,mzs);
	float mxs = max((float)xs,mys);
	float v = (1.5f*(mxs)/2.0f);
    FMODASSERT(FMOD_System_SetGeometrySettings(mainsystem, v));
    geometry = (DWORD*) GlobalAllocPtr(GMEM_FIXED |GMEM_ZEROINIT, sizeof(DWORD)*(UINT)NumBlockers);
    if(geometry == NULL) {FMODASSERT(FMOD_ERR_MEMORY);}
    maxgeometry = (UINT)NumBlockers;
    curgeometry = 0;
	return (double) 1;
}


 double FMODBlockerAdd(double x, double y, double z, double xs, double ys, double zs, double xe, double ye, double ze)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
	FMOD_GEOMETRY *g = NULL;
	if(curgeometry >= maxgeometry) {FMODASSERT(FMOD_ERR_MEMORY);}
	FMODASSERT(FMOD_System_CreateGeometry(mainsystem, 6, 24, &g));
	*(geometry + curgeometry) = (DWORD)g;
	curgeometry++;

	float dxs,dxe,dys,dye,dzs,dze;
	dxs = (float)xs-(float)x;
	dxe = (float)xe-(float)x;
	dys = (float)ys-(float)y;
	dye = (float)ye-(float)y;
	dzs = (float)zs-(float)z;
	dze = (float)ze-(float)z;

	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
    {
        { dxe, dys, dzs}, { dxe, dys,  dze}, { dxe,  dye,  dze}, { dxe,  dye, dzs}, //+X face
        {dxs, dys, dzs}, {dxs, dys,  dze}, {dxs,  dye,  dze}, {dxs,  dye, dzs}, //-X face
        {dxs,  dye, dzs}, { dxe,  dye, dzs}, { dxe,  dye,  dze}, {dxs,  dye,  dze}, //+Y face
        {dxs, dys, dzs}, { dxe, dys, dzs}, { dxe, dys,  dze}, {dxs, dys,  dze}, //-Y face
        {dxs, dys,  dze}, {dxs,  dye,  dze}, { dxe,  dye,  dze}, { dxe,  dys, dze}, //+Z face
        {dxs, dys, dzs}, {dxs,  dye, dzs}, { dxe,  dye, dzs}, { dxe, dys, dzs}, //-Z face
    };

	int pi = 0;
	for(int i = 0; i < 5; ++i)
	{
    	FMOD_Geometry_AddPolygon(g, 1, 1, 1, 4, &cube [4 * i], &pi);
	}

	FMOD_VECTOR v = {(float)x,(float)y,(float)z};
    FMODASSERT(FMOD_Geometry_SetPosition(g, &v));
	return (double) (DWORD)g;

}

 double FMODBlockerSet3dPosition(double blocker, double x, double y, double z)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_VECTOR v = {(float)x,(float)y,(float)z};
    FMODASSERT(FMOD_Geometry_SetPosition((FMOD_GEOMETRY*)(DWORD)blocker, &v));
	return (double) 1;
}
 double FMODBlockerSet3dOrientation(double blocker, double fx, double fy, double fz, double ux, double uy, double uz)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_VECTOR fv = {(float)fx,(float)fy,(float)fz};
    FMOD_VECTOR uv = {(float)ux,(float)uy,(float)uz};
    FMODASSERT(FMOD_Geometry_SetRotation((FMOD_GEOMETRY*)(DWORD)blocker, &fv, &uv));
	return (double) 1;
}
 double FMODBlockerSet3dScale(double blocker, double sx, double sy, double sz)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_VECTOR v = {(float)sx,(float)sy,(float)sz};
    FMODASSERT(FMOD_Geometry_SetScale((FMOD_GEOMETRY*)(DWORD)blocker, &v));
    return (double) 1;
}
 double FMODBlockerSetEnabled(double blocker, double enabled)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMODASSERT(FMOD_Geometry_SetActive((FMOD_GEOMETRY*)(DWORD)blocker, (FMOD_BOOL) enabled));
	return (double) 1;
}

 double FMODBlockerGetEnabled(double blocker)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_BOOL enabled = 0;
    FMODASSERT(FMOD_Geometry_GetActive((FMOD_GEOMETRY*)(DWORD)blocker, &enabled));
	return (double) enabled;
}

 double FMODBlockerGetStrength(double blocker)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    int polies = 0;
    FMODASSERT(FMOD_Geometry_GetNumPolygons((FMOD_GEOMETRY*)(DWORD)blocker, &polies));
    if(polies>0)
    {
		float s = 0;
		float d1 = 0;
		FMOD_BOOL d2 = 0;
	    FMODASSERT(FMOD_Geometry_GetPolygonAttributes((FMOD_GEOMETRY*)(DWORD)blocker, 0, &s, &d1, &d2));
	    return (double) s;
	}
	return (double) 0;
}

 double FMODBlockerSetStrength(double blocker,double strength)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(geometry == NULL){{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(blocker >0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    int polies = 0;
    FMODASSERT(FMOD_Geometry_GetNumPolygons((FMOD_GEOMETRY*)(DWORD)blocker, &polies));
    if(polies>0)
    {
		for(int i = 0; i<polies;i++)
		{
	    	FMODASSERT(FMOD_Geometry_SetPolygonAttributes((FMOD_GEOMETRY*)(DWORD)blocker, i, (float)strength, (float)strength, 1));
		}
	    return (double) 1;
	}
	return (double) 0;
}

 double FMODGroupSetPan(double group, double pan)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(pan <-1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(pan >1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_OverridePan(g, (float)pan));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_OverridePan(priority, (float)pan));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_OverridePan(effects, (float)pan));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_OverridePan(ambientmusic, (float)pan));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_OverridePan(ambienteffects, (float)pan));
	}
	return (double) 1;
}



 double FMODInstanceGetMaxDist(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float v = 0;
    FMODASSERT(FMOD_Channel_Get3DMinMaxDistance((FMOD_CHANNEL *) (DWORD) instance, 0, &v));
	return (double) v;
}

 double FMODSoundGetNumChannels(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	int ch = 0;
    FMODASSERT(FMOD_Sound_GetFormat((FMOD_SOUND *) (DWORD) sound, NULL, NULL, &ch, 0));

	return (double) ch;
}



 double FMODInstanceSoundGetLength(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	//UINT l;
	//l = 0;
    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	//FMODASSERT(FMOD_Sound_GetLength(sound, &l, FMOD_TIMEUNIT_MS));
	//return (double) l;
    return FMODSoundGetLength((double)(DWORD)sound);
}
 double FMODInstanceGetWaveSnapshot(double instance, double channel, double size, LPSTR Buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    FMODASSERT(FMOD_Channel_GetWaveData((FMOD_CHANNEL*)(DWORD)instance, samplebuffer,(int)size,(int) channel));
    int i;
    for (i=0;i<size;i++)
    {
        //samplebuffer[i] = (float)i/size;
        if(samplebuffer[i]>1) samplebuffer[i] = 1;
        if(samplebuffer[i]<-1) samplebuffer[i] = -1;
        Buffer[i] = (char)((samplebuffer[i])*120+127);
    }
    return (double) 1;
}

FMOD_DSP_FFT_WINDOW spectrumwindow = FMOD_DSP_FFT_WINDOW_RECT;

 double FMODSpectrumSetSnapshotType(double snapshottype)
{
    spectrumwindow = (FMOD_DSP_FFT_WINDOW)max(0,min((int)FMOD_DSP_FFT_WINDOW_MAX-1,(int) snapshottype));
    return (double)1;
}

 double FMODInstanceGetSpectrumSnapshot(double instance, double channel, double size, LPSTR Buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMODASSERT(FMOD_Channel_GetSpectrum((FMOD_CHANNEL*)(DWORD)instance, samplebuffer,(int)size,(int) channel, spectrumwindow));
    int i;
    float maxv = 0;
    for (i=0;i<size;i++)
    {
        maxv = max(samplebuffer[i],maxv);
    }
    if(maxv == 0) return (double) 0;
    for (i=0;i<size;i++)
    {
        //samplebuffer[i] = (float)i/size;
        Buffer[i] = (char)((samplebuffer[i]/maxv)*255);
    }

    return (double) maxv;
}


 double FMODGroupGetWaveSnapshot(double group, double channel, double size, LPSTR Buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_GetWaveData(g, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(priority, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(effects, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(ambientmusic, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(ambienteffects, samplebuffer,(int)size,(int) channel));
	}

    int i;
    for (i=0;i<size;i++)
    {
        if(samplebuffer[i]>1) samplebuffer[i] = 1;
        if(samplebuffer[i]<-1) samplebuffer[i] = -1;
        //samplebuffer[i] = (float)i/size;
        Buffer[i] = (char)((samplebuffer[i])*120+127);
    }
    return (double) 1;
}

 double FMODGroupGetSpectrumSnapshot(double group, double channel, double size, LPSTR Buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(g, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(priority, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(effects, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(ambientmusic, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(ambienteffects, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}

    int i;
    float maxv = 0;
    for (i=0;i<size;i++)
    {
        maxv = max(samplebuffer[i],maxv);
    }
    if(maxv == 0) return (double) 0;
    for (i=0;i<size;i++)
    {
        //samplebuffer[i] = (float)i/size;
        Buffer[i] = (char)((samplebuffer[i]/maxv)*255);
    }

    return (double) maxv;
}

 double FMODInstanceGetWaveSnapshot2(double instance, double channel, double size)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    FMODASSERT(FMOD_Channel_GetWaveData((FMOD_CHANNEL*)(DWORD)instance, samplebuffer,(int)size,(int) channel));
    int i;
    for (i=0;i<size;i++)
    {
        if(samplebuffer[i]>1) samplebuffer[i] = 1;
        if(samplebuffer[i]<-1) samplebuffer[i] = -1;
    }
    return (double) 1;
}

 double FMODInstanceGetSpectrumSnapshot2(double instance, double channel, double size)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMODASSERT(FMOD_Channel_GetSpectrum((FMOD_CHANNEL*)(DWORD)instance, samplebuffer,(int)size,(int) channel, spectrumwindow));

    return (double) 1;
}


 double FMODGroupGetWaveSnapshot2(double group, double channel, double size)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_GetWaveData(g, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(priority, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(effects, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(ambientmusic, samplebuffer,(int)size,(int) channel));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_GetWaveData(ambienteffects, samplebuffer,(int)size,(int) channel));
	}
    int i;
    for (i=0;i<size;i++)
    {
        if(samplebuffer[i]>1) samplebuffer[i] = 1;
        if(samplebuffer[i]<-1) samplebuffer[i] = -1;
    }
    return (double)1;
}

 double FMODGroupGetSpectrumSnapshot2(double group, double channel, double size)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(channel >8) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(g, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(priority, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(effects, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(ambientmusic, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_GetSpectrum(ambienteffects, samplebuffer,(int)size,(int) channel, spectrumwindow));
	}


    return (double) 1;
}

 double FMODNormalizeSpectrumData(double startpos, double size)
{
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos >=8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    int i =0;
    float maxv = 0;
    int endpos = (int)min(startpos+size,8192);
    for (i=(int)startpos;i<endpos;i++)
    {
        maxv = max(samplebuffer[i],maxv);
    }

    if(maxv<=0) return (double) 1;
    for (i=(int)startpos;i<endpos;i++)
    {
        //samplebuffer[i] = (float)i/size;
        samplebuffer[i] = (samplebuffer[i]/maxv);
    }

    return (double) maxv;

}

 double FMODNormalizeWaveData(double startpos, double size)
{
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos >=16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    float maxv = 0;
    float minv = 0;
    int i =0;
    int endpos = (int)min(startpos+size,16384);
    for (i=(int)startpos;i<endpos;i++)
    {
        maxv = max(samplebuffer[i],maxv);
        minv = min(samplebuffer[i],minv);
    }
    if(maxv == 0) return (double) 0;

    maxv = maxv-minv;
    for (i=(int)startpos;i<endpos;i++)
    {
        //samplebuffer[i] = (float)i/size;
        samplebuffer[i] = (samplebuffer[i]/maxv);
        if(samplebuffer[i]>1) samplebuffer[i] = 1;
        if(samplebuffer[i]<-1) samplebuffer[i] = -1;
    }

    return (double) maxv;

}

 double FMODGetSnapshotEntry(double pos)
{
	if(pos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(pos >=16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    return (double) samplebuffer[(int)pos];

}

 double FMODGetSpectrumBuffer(double startpos, double size, LPSTR Buffer)
{
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos >=8192) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    int endpos = (int)min(startpos+size,8192);
    int i = 0;
    for (i=(int)startpos;i<endpos;i++)
    {
        //samplebuffer[i] = (float)i/size;
        Buffer[i] = (char)((samplebuffer[i])*255);
    }
    return (double)1;

}

 double FMODGetWaveBuffer(double startpos, double size, LPSTR Buffer)
{
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos >=16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    int endpos = (int)min(startpos+size,16384);
    int i = 0;
    for (i=(int)startpos;i<endpos;i++)
    {
        //samplebuffer[i] = (float)i/size;
        Buffer[i] = (char)((samplebuffer[i])*120+127);
    }
    return (double)1;

}

 double FMODEncryptFile(LPCSTR sourcename, LPCSTR destname, LPCSTR password)
{
char hash1[1024];
memset(hash1,0,1024);
CSHA1 sha1;
sha1.Update((unsigned char *)password, strlen(password));
sha1.Final();
sha1.ReportHash(hash1, CSHA1::REPORT_HEX);
//MessageBoxA(GetActiveWindow(),hash1,"debug",MB_ICONINFORMATION);
CSHA1 sha2;

char revp[1024];
memset(revp,0,1024);
strcpy(revp,password);
strrev(revp);
//MessageBoxA(GetActiveWindow(),revp,"debug",MB_ICONINFORMATION);
char hash2[1024];
memset(hash2,0,1024);

sha2.Update((unsigned char *)revp, strlen(revp));
sha2.Final();
sha2.ReportHash(hash2, CSHA1::REPORT_HEX);
//MessageBoxA(GetActiveWindow(),hash2,"debug",MB_ICONINFORMATION);
//return;
	DeleteFileA(destname);

	FILE* sfile = fopen(sourcename, "rb");
	if (sfile == NULL)
	{
        MessageBoxA(GetActiveWindow(),sourcename,"Error Opening",MB_ICONINFORMATION);
        return (double)0;
	}
	fseek(sfile,-12,SEEK_END);
	char passwordtag[] =  "XMODSIMPLEPW\0\0";
	fgets(passwordtag,13,sfile);
	//MessageBoxA(GetActiveWindow(),passwordtag,"debug",MB_ICONINFORMATION);
	if(strcmp(passwordtag,"FMODSIMPLEPW") == 0)
	{
        fclose(sfile);
        MessageBoxA(GetActiveWindow(),sourcename,"File is protected already!",MB_ICONINFORMATION);
        return (double)0;
	}
	fseek(sfile,0,SEEK_SET);
	FILE* dfile = fopen(destname, "wb");
	if (dfile == NULL)
	{
		fclose(sfile);
        MessageBoxA(GetActiveWindow(),destname,"Error Opening",MB_ICONINFORMATION);
        return (double)0;
	}
	char c;
	int ln = strlen(hash2);
	int at = 0;
	DWORD rol = 0;
	while (!(feof(sfile)))
	{
        c = fgetc(sfile );
        //fputc((c xor hash2[at]) xor hash1[(int)(fabs(sin(degtorad(rol)))*lnr)],dfile);
        fputc((c ^ hash2[at]) ^ hash1[at],dfile);
        //fputc(c xor 'A',dfile);
        //fputc(c,dfile);
        //fputc(c xor hash2[at],dfile);
        at++;
        if(at >= ln) at = 0;
        c = 0;
        rol++;
	}
	fputs(hash1, dfile);
	fputs("FMODSIMPLEPW", dfile);
	fclose(sfile);
	fclose(dfile);
    return (double)1;
}


 double FMODInstanceAddEffect(double instance, double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    if((effect<FMOD_DSP_TYPE_MIXER)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if((effect>FMOD_DSP_TYPE_LOWPASS_SIMPLE)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_DSP *eff = NULL;
 	FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,(FMOD_DSP_TYPE)(DWORD)effect,&eff));
    if(eff==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_Channel_AddDSP((FMOD_CHANNEL*)(DWORD) instance, eff,0));

    return (double) (DWORD) eff;
}

 double FMODGroupAddEffect(double group, double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(group<0 || group>4) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    FMOD_DSP *eff = NULL;
 	FMODASSERT(FMOD_System_CreateDSPByType(mainsystem,(FMOD_DSP_TYPE)(DWORD)effect,&eff));
    if(eff==NULL){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    if(group == 0)
    {
		FMOD_CHANNELGROUP *g = NULL;
		FMODASSERT(FMOD_System_GetMasterChannelGroup(mainsystem,&g));
 		if(g==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

		FMODASSERT(FMOD_ChannelGroup_AddDSP(g, eff,0));
	}
    else if(group == 1)
    {
		FMODASSERT(FMOD_ChannelGroup_AddDSP(priority, eff,0));
	}
    else if(group == 2)
    {
		FMODASSERT(FMOD_ChannelGroup_AddDSP(effects, eff,0));
	}
    else if(group == 3)
    {
		FMODASSERT(FMOD_ChannelGroup_AddDSP(ambientmusic, eff,0));
	}
    else if(group == 4)
    {
		FMODASSERT(FMOD_ChannelGroup_AddDSP(ambienteffects, eff,0));
	}
    return (double) (DWORD) eff;
}

 double FMODEffectFree(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_DSP_Release((FMOD_DSP *)(DWORD) effect));
    return (double)1;
}

 double FMODSoundSetLoopCount(double sound, double count)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    if(!(count>=-1)) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	FMODASSERT(FMOD_Sound_SetLoopCount((FMOD_SOUND *) (DWORD) sound, (int) count));
	return (double)1;
}

 double FMODSoundGetLoopCount(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	int ct;
    ct = -1;
	FMODASSERT(FMOD_Sound_GetLoopCount((FMOD_SOUND *) (DWORD) sound, &ct));
	return (double) ct;
}

 double FMODSoundSetLoopPoints(double sound, double start, double end)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	if(start<0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(start>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(end<=start) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(end>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	//UINT l = 0;

	//FMODASSERT(FMOD_Sound_GetLength((FMOD_SOUND *) (DWORD) sound, &l, FMOD_TIMEUNIT_MS));
	//FMODASSERT(FMOD_Sound_SetLoopPoints((FMOD_SOUND *) (DWORD) sound, (UINT)(start*l), FMOD_TIMEUNIT_MS, (UINT)(end*l), FMOD_TIMEUNIT_MS));
    UINT l = (UINT)FMODSoundGetLength((double)(DWORD)sound);
    FMODASSERT(FMOD_Sound_SetLoopPoints((FMOD_SOUND *) (DWORD) sound, (UINT)(start*l), FMOD_TIMEUNIT_MS, (UINT)(end*l), FMOD_TIMEUNIT_MS));
	return (double) 1;
}


 double FMODInstanceSetSpeakerMix(double instance,
  double  frontleft,
  double  frontright,
  double  center,
  double  lfe,
  double  backleft,
  double  backright,
  double  sideleft,
  double  sideright)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	FMODASSERT(FMOD_Channel_SetSpeakerMix((FMOD_CHANNEL *) (DWORD) instance,
        max(0,min(5,(float)frontleft)),
        max(0,min(5,(float)frontright)),
        max(0,min(5,(float)center)),
        max(0,min(5,(float)lfe)),
        max(0,min(5,(float)backleft)),
        max(0,min(5,(float)backright)),
        max(0,min(5,(float)sideleft)),
        max(0,min(5,(float)sideright))
    ));
	return (double) 1;

}

 double FMODInstanceSetLoopPoints(double instance, double start, double end)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	if(start<0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(start>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(end<=start) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(end>1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	//UINT l = 0;
    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	//FMODASSERT(FMOD_Sound_GetLength(sound, &l, FMOD_TIMEUNIT_MS));
    UINT l = (UINT)FMODSoundGetLength((double)(DWORD)sound);
	FMODASSERT(FMOD_Channel_SetLoopPoints((FMOD_CHANNEL *) (DWORD) instance, (UINT)(start*l), FMOD_TIMEUNIT_MS, (UINT)(end*l), FMOD_TIMEUNIT_MS));
	return (double) 1;
}

 double FMODInstanceGetAudibility(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float a = 0;
    FMODASSERT(FMOD_Channel_GetAudibility((FMOD_CHANNEL *) (DWORD) instance, &a));
	return (double) a;
}

HANDLE updatethread = 0;
int threadstate = 0;
DWORD WINAPI UpdateFunction( LPVOID lpParam )
{
    while(threadstate == 1)
    {
        Sleep(16);
        if(inited)
            FMODUpdate();
    }
    threadstate = 2;
    ExitThread(0);
}
 double FMODUpdateTakeOverWhileLocked()
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(updatethread) {FMODASSERT(FMOD_ERR_INITIALIZED);}
    DWORD   dontcare = 0;
    threadstate = 1;
    updatethread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            UpdateFunction,       // thread function name
            0,          // argument to thread function
            0,                      // use default creation flags
            &dontcare);   // returns the thread identifier
    return (double) 1;
}

 double FMODUpdateTakeOverDone()
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!updatethread) {FMODASSERT(FMOD_ERR_INITIALIZED);}
    threadstate = 0;
    while(threadstate != 2) Sleep(4);
    CloseHandle(updatethread);
    updatethread = 0;
    threadstate = 0;

    return (double) 1;
}


 double FMODInstanceSet3DPanLevel(double instance, double panlevel)
{

    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float a = max(0,min(1,(float)panlevel));
    FMODASSERT(FMOD_Channel_Set3DPanLevel((FMOD_CHANNEL *) (DWORD) instance, a));
	return (double) 1;

}

 double FMODInstanceGet3DPanLevel(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float a = 0;
    FMODASSERT(FMOD_Channel_Get3DPanLevel((FMOD_CHANNEL *) (DWORD) instance, &a));
	return (double) a;
}

 double FMODInstanceSet3DSpread(double instance, double spreadangle)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float a = max(0,min(360,(float)spreadangle));
    FMODASSERT(FMOD_Channel_Set3DSpread((FMOD_CHANNEL *) (DWORD) instance, a));
	return (double) a;
}

 double FMODInstanceGet3DSpread(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float a = 0;
    FMODASSERT(FMOD_Channel_Get3DSpread((FMOD_CHANNEL *) (DWORD) instance, &a));
	return (double) a;
}


 double FMODInstanceGetNextTag(double instance)
{
    tagname[0] = 0;
    tagdata[0] = 0;
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_TAG tag;
    if (FMOD_Sound_GetTag(sound, 0, -1, &tag) != FMOD_OK)
    {
        return (double)0;
    }
    if(tag.name == NULL) return (double) 2;
    int l; l = min(2047, strlen(tag.name)+1);
    lstrcpynA(tagname,
            tag.name,
            l);
    tagname[l+1] = 0;
    if(tag.data == NULL) return (double) 2;
    if (tag.datatype == FMOD_TAGDATATYPE_STRING)
    {
        l = min(2047, strlen((char*) tag.data)+1);
        lstrcpynA(tagdata,
            (char*)tag.data,
            l);
        tagdata[l+1] = 0;
        return (double)1;
    }
    else if (tag.datatype == FMOD_TAGDATATYPE_FLOAT)
    {
        float *f = (float*)tag.data;
        wsprintfA(tagdata,"%f",*f);
        return (double)1;
    }
    else
    {
        int *v = (int*)tag.data;
        wsprintfA(tagdata,"%d",*v);
        return (double)1;
    }
    return (double)2;
}

 double FMODGetTagName(LPSTR buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    lstrcpyA(buffer,tagname);
//MessageBoxA(GetActiveWindow(),buffer,(LPCSTR)"DebugGetName",MB_ICONINFORMATION);
    return (double)lstrlenA(tagname);
}
 double FMODGetTagData(LPSTR buffer)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    lstrcpyA(buffer,tagdata);
    return (double)lstrlenA(tagdata);
}



 double FMODInstanceAsyncOK(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}


    FMOD_SOUND * sound = NULL;

	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
	if(sound==NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_OPENSTATE openstate = FMOD_OPENSTATE_READY;
    FMOD_BOOL starving = false;
    FMODASSERT(FMOD_Sound_GetOpenState(sound,&openstate,NULL,&starving));
    if(openstate == FMOD_OPENSTATE_ERROR) FMODASSERT(FMOD_ERR_HTTP_TIMEOUT);

    return (double) !starving;

}

 double FMODSoundAsyncReady(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_OPENSTATE openstate = FMOD_OPENSTATE_READY;
    FMODASSERT(FMOD_Sound_GetOpenState((FMOD_SOUND *)(DWORD)sound,&openstate,NULL,NULL));
    if(openstate == FMOD_OPENSTATE_ERROR) FMODASSERT(FMOD_ERR_HTTP_TIMEOUT);
    if(openstate == FMOD_OPENSTATE_READY)
    {
        void *a = NULL;
        FMOD_Sound_GetUserData((FMOD_SOUND *)(DWORD)sound, &a);
        if(a==NULL)
        {
            myinfo *mi = (myinfo *)GlobalAllocPtr(GMEM_FIXED|GMEM_ZEROINIT,(sizeof(myinfo)));
	       //MessageBoxA(GetActiveWindow(),"3","",0);
            if(mi == NULL) {FMODASSERT(FMOD_ERR_MEMORY);}
            mi->maxvolume = 1;
            mi->streamed =(true);
            FMODASSERT(FMOD_Sound_SetUserData((FMOD_SOUND *)(DWORD)sound, (void *)mi));
        }

        return (double)1;
    }
    return 0;
}



 double FMODSoundAddAsyncStream(LPCSTR soundfile, double threed)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}

    FMOD_SOUND *sound = NULL;

    DWORD flags = FMOD_LOOP_NORMAL | FMOD_2D | FMOD_SOFTWARE |FMOD_ACCURATETIME;
    if(threed)
	{
        //MessageBoxA(GetActiveWindow(),soundfile,(LPCSTR)"3d",MB_ICONINFORMATION);
		flags = FMOD_LOOP_NORMAL | FMOD_3D | FMOD_SOFTWARE | FMOD_3D_LINEARROLLOFF|FMOD_ACCURATETIME;
	}
    FMODASSERT(FMOD_System_CreateSound(mainsystem, soundfile, flags| FMOD_CREATESTREAM | FMOD_NONBLOCKING, 0, &sound));
    //char t[] = "123456789";
    //wsprintf( t, "%ld", (int)sound);
//	MessageBoxA(GetActiveWindow(),t,"",0);

	if(sound == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	//MessageBoxA(GetActiveWindow(),"2","",0);


	//MessageBoxA(GetActiveWindow(),"5","",0);

	return (double) (DWORD) sound;
}

 double FMODSoundGetMusicNumChannels(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	int numchannels;
    numchannels = 0;
	FMODASSERT(FMOD_Sound_GetMusicNumChannels((FMOD_SOUND *) (DWORD) sound, &numchannels));
	return (double) numchannels;
}

 double FMODSoundGetMusicChannelVolume(double sound, double channel)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	float volume;
    volume = 0;
	FMODASSERT(FMOD_Sound_GetMusicChannelVolume((FMOD_SOUND *) (DWORD) sound, (int)(DWORD)channel, &volume));
	return (double) volume;
}

 double FMODSoundSetMusicChannelVolume(double sound, double channel, double volume)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

	FMODASSERT(FMOD_Sound_SetMusicChannelVolume((FMOD_SOUND *) (DWORD) sound, (int)(DWORD)channel, (float)max(0,min(volume,1))));
	return (double) 1;
}


 double FMODEffectGetActive(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_BOOL v = 0;
	FMODASSERT(FMOD_DSP_GetActive((FMOD_DSP *)(DWORD) effect, &v));
    return (double)(DWORD)(int)v;
}
 double FMODEffectSetActive(double effect, double v)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_DSP_SetActive((FMOD_DSP *)(DWORD) effect, (FMOD_BOOL)(int)(DWORD) v));
    return (double)1;
}

 double FMODEffectGetBypass(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_BOOL v = 0;
	FMODASSERT(FMOD_DSP_GetBypass((FMOD_DSP *)(DWORD) effect, &v));
    return (double)(DWORD)(int)v;
}
 double FMODEffectSetBypass(double effect, double v)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_DSP_SetBypass((FMOD_DSP *)(DWORD) effect, (FMOD_BOOL)(int)(DWORD) v));
    return (double)1;
}

 double FMODEffectGetDefaultFr(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    return (double)fr;
}
 double FMODEffectGetDefaultVol(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    return (double)v;
}
 double FMODEffectGetDefaultPan(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    return (double)pn;
}

 double FMODEffectSetDefaultFr(double effect, double val)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    fr = (float)val;
	FMODASSERT(FMOD_DSP_SetDefaults((FMOD_DSP *)(DWORD) effect,fr,v,pn,pr));
    return (double)1;
}
 double FMODEffectSetDefaultVol(double effect, double val)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    v = (float)val;
	FMODASSERT(FMOD_DSP_SetDefaults((FMOD_DSP *)(DWORD) effect,fr,v,pn,pr));
    return (double)1;
}
 double FMODEffectSetDefaultPan(double effect, double val)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float fr,v,pn;
    fr = 0;
    v = 0;
    pn = 0;
    int pr = 0;
	FMODASSERT(FMOD_DSP_GetDefaults((FMOD_DSP *)(DWORD) effect,&fr,&v,&pn,&pr));
    pn = (float)val;
	FMODASSERT(FMOD_DSP_SetDefaults((FMOD_DSP *)(DWORD) effect,fr,v,pn,pr));
    return (double)1;
}

 double FMODEffectGetNumParams(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    int np = 0;
	FMODASSERT(FMOD_DSP_GetNumParameters((FMOD_DSP *)(DWORD) effect, &np));
    return (double)(DWORD) np;
}

 double FMODEffectGetParamValue(double effect, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float value = 0;
    //char *  valuestr;
    //int  valuestrlen;
    char t[17];
	FMODASSERT(FMOD_DSP_GetParameter((FMOD_DSP *)(DWORD) effect, (int)(DWORD)p, &value, t, 16));
    return (double) value;
}
 double FMODEffectGetParamValueStr(double effect, double p, LPSTR str)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    float value = 0;

	FMODASSERT(FMOD_DSP_GetParameter((FMOD_DSP *)(DWORD) effect, (int)(DWORD)p, &value, str, 16));
    return (double)1;
}
 double FMODEffectSetParamValue(double effect, double p, double v)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_DSP_SetParameter((FMOD_DSP *)(DWORD) effect, (int)(DWORD)p, (float) v));
    return (double)1;
}

 double FMODEffectGetParamName(double effect, double p, LPSTR str)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    //char name[17];
    char label[17];
    char desc[256];
    float min = 0;
    float max = 0;
	FMODASSERT(FMOD_DSP_GetParameterInfo((FMOD_DSP *)(DWORD) effect, (int)(DWORD) p, str, label, desc, 256, &min, &max));
    return (double)1;
}
 double FMODEffectGetParamLabel(double effect, double p, LPSTR str)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    char name[17];
    //char label[17];
    char desc[256];
    float min = 0;
    float max = 0;
	FMODASSERT(FMOD_DSP_GetParameterInfo((FMOD_DSP *)(DWORD) effect, (int)(DWORD) p, name, str, desc, 256, &min, &max));
    return (double)1;
}
 double FMODEffectGetParamDesc(double effect, double p, LPSTR str)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    char name[17];
    char label[17];
    //char desc[256];
    float min = 0;
    float max = 0;
	FMODASSERT(FMOD_DSP_GetParameterInfo((FMOD_DSP *)(DWORD) effect, (int)(DWORD) p, name, label, str, 256, &min, &max));
    return (double)1;
}
 double FMODEffectGetParamMin(double effect, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    char name[17];
    char label[17];
    char desc[256];
    float min = 0;
    float max = 0;
	FMODASSERT(FMOD_DSP_GetParameterInfo((FMOD_DSP *)(DWORD) effect, (int)(DWORD) p, name, label, desc, 256, &min, &max));
    return (double)min;
}
 double FMODEffectGetParamMax(double effect, double p)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    char name[17];
    char label[17];
    char desc[256];
    float min = 0;
    float max = 0;
	FMODASSERT(FMOD_DSP_GetParameterInfo((FMOD_DSP *)(DWORD) effect, (int)(DWORD) p, name, label, desc, 256, &min, &max));
    return (double)max;
}
 double FMODEffectGetSpeakerActive(double effect, double speaker)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    //FMOD_SPEAKER  speaker,
    FMOD_BOOL active = 0;
	FMODASSERT(FMOD_DSP_GetSpeakerActive((FMOD_DSP *)(DWORD) effect,(FMOD_SPEAKER) (int)(DWORD) speaker, &active));
    return (double)(DWORD)active;
}
 double FMODEffectSetSpeakerActive(double effect, double speaker, double active)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    //FMOD_SPEAKER  speaker,
    //FMOD_BOOL active = 0;
	FMODASSERT(FMOD_DSP_SetSpeakerActive((FMOD_DSP *)(DWORD) effect,(FMOD_SPEAKER) (int)(DWORD) speaker, (FMOD_BOOL)(int)(DWORD)active));
    return (double)1;
}
 double FMODEffectReset(double effect)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(effect>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	FMODASSERT(FMOD_DSP_Reset((FMOD_DSP *)(DWORD) effect));
    return (double)1;
}


 double FMODInstanceGetPitch(double instance)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_SOUND * sound = NULL;
	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
    if(sound == NULL) return 0;
    float frequency;
    float volume;
    float pan;
    int priority;

	FMODASSERT(FMOD_Sound_GetDefaults(sound, &frequency, &volume, &pan, &priority));
    double ifr = FMODInstanceGetFrequency(instance);
    if(ifr == 0) return 0;

	return (double) ifr/frequency;
}

 double FMODInstanceSetPitch(double instance, double pitch)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(instance>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}

    FMOD_SOUND * sound = NULL;
	FMODASSERT(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL *) (DWORD) instance, &sound));
    if(sound == NULL) return 0;
    float frequency;
    float volume;
    float pan;
    int priority;

	FMODASSERT(FMOD_Sound_GetDefaults(sound, &frequency, &volume, &pan, &priority));
    return FMODInstanceSetFrequency(instance, frequency * pitch);
}

//using namespace std;
//void ds_list_add(const unsigned int id, const variant val);

double FMODSnapShotToArray(double startpos, double size, float array[])
{
	if(size <1) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(size >16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos <0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
	if(startpos >=16384) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}

    int endpos = (int)min(startpos+size,16384);
    int i = 0;
    float minv,maxv;

    minv = 9999;
    maxv = -9999;
	unsigned int ds_size=0;
    for (i=(int)startpos;i<endpos;i++)
    {
        //samplebuffer[i] = (float)i/size;
        minv = min(minv,samplebuffer[i]);
        maxv = max(maxv,samplebuffer[i]);
        array[ds_size]=samplebuffer[i];
        ds_size+=1;
    }
    return (double)(maxv-minv);

}
#define OUTPUTRATE          48000

 double FMODCreateSoundFromMicInput()
{

    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    FMOD_CREATESOUNDEXINFO exinfo;

    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 1;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = OUTPUTRATE;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;
    FMOD_SOUND * sound = NULL;

    FMODASSERT(FMOD_System_CreateSound(mainsystem, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound));
	if(sound == NULL) {{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
	//MessageBoxA(GetActiveWindow(),"2","",0);

    myinfo *mi = (myinfo *)GlobalAllocPtr(GMEM_FIXED|GMEM_ZEROINIT,(sizeof(myinfo)));
	//MessageBoxA(GetActiveWindow(),"3","",0);
    if(mi == NULL) {FMODASSERT(FMOD_ERR_MEMORY);}
    mi->maxvolume = 1;
    strcpy(mi->file,"");
    mi->threed =(0);
    mi->streamed =(0);
	//all other members set by GMEM_ZEROINIT
	//MessageBoxA(GetActiveWindow(),"4","",0);

    FMODASSERT(FMOD_Sound_SetUserData(sound, (void *)mi));
    return (double) (DWORD) sound;
}

 double FMODRecordStart(double sound)
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}
    if(!(sound>0)){{FMODASSERT(FMOD_ERR_INVALID_HANDLE);}}
    FMOD_CHANNEL * channel;
    FMODASSERT(FMOD_System_RecordStart(mainsystem, 0, (FMOD_SOUND *)(DWORD) sound, TRUE));
    Sleep(200);
    FMODUpdate();
    FMODASSERT(FMOD_System_PlaySound(mainsystem, FMOD_CHANNEL_REUSE, (FMOD_SOUND *)(DWORD) sound, FALSE, &channel));
    return (double) (DWORD) channel;
}

 double FMODRecordStop()
{
    if(!inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}

    FMODASSERT(FMOD_System_RecordStop(mainsystem, 0));
    return 1;
}

/*
 double FMODSetResampler(double resamplemethod)
{
    //if(inited) {{FMODASSERT(FMOD_ERR_INITIALIZATION);}}

    if(resamplemethod>= FMOD_DSP_RESAMPLER_MAX) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    if(resamplemethod<0) {{FMODASSERT(FMOD_ERR_INVALID_PARAM);}}
    samplemethod = (FMOD_DSP_RESAMPLER) (DWORD) resamplemethod;
    return (double)1;
}
*/
}
