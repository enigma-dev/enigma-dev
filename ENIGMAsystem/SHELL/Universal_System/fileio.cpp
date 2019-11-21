/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

// Simple, intuitive, integer based file I/O

#include <stdio.h> //fstream can get staked
#include <string> //We will use string, though
using namespace std;

#include "darray.h" //Simpler vector with logarithmic time
#include "fileio.h" //Simpler vector with logarithmic time

#include "Widget_Systems/widgets_mandatory.h"

// Implement a storage class for file information

// We're going to set a standard at this point. On file close, the pointer shall be zeroed,
// and the array shall be traversed from end to start in search of the lowest legal high ID
// if the file's ID matches the high ID.

namespace enigma
{
  struct openFile
  {
    FILE *f;      //FILE we opened, or NULL if it has been closed.
    string sdata; //Use varies depending on type.
    unsigned spos;      //position in sdata string
    bool eof;

    openFile(): f(NULL), sdata(), spos(0), eof(false) {};
    openFile(FILE* a,string b): f(a), sdata(b), spos(0), eof(false) {};
  };
  varray<openFile> files; //Use a dynamic array to store as many files as the user cares to open
  int file_highid = 0; //This isn't what GM does, but it's not a bad idea. GM checks for the smallest unused ID.
  extern string gameInfoText;
}


// Function family file_text*
// Uses enigma::files[] for storage, treats sdata as last read string.

#include "estring.h"

namespace enigma_user
{

int file_text_open_read(string fname) // Opens the file with the indicated name for reading. The function returns the id of the file that must be used in the other functions. You can open multiple files at the same time (32 max). Don't forget to close them once you are finished with them.
{
  FILE *a = fopen(fname.c_str(),"rt"); //Read as text file
  if (!a)      //Failure
    return -1; //Behavior for fail is -1 return
  enigma::files[enigma::file_highid] = enigma::openFile(a,""); //Store it in the lowest available ID, highid
  file_text_readln(enigma::file_highid);
  return enigma::file_highid++; //Return our index and increment it for next time
}

int file_text_open_write(string fname) // Opens the indicated file for writing, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
{
  FILE *a = fopen(fname.c_str(),"wt"); //Write as text file
  if (!a)      //Failure
    return -1; //Behavior for fail is -1 return
  enigma::files[enigma::file_highid] = enigma::openFile(a,fname); //Store it in the lowest available ID, highid
  return enigma::file_highid++; //Return our index and increment it for next time
}

int file_text_open_append(string fname) // Opens the indicated file for appending data at the end, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
{
  FILE *a = fopen(fname.c_str(),"at"); //Append as text file
  if (!a)      //Failure
    return -1; //Behavior for fail is -1 return
  enigma::files[enigma::file_highid] = enigma::openFile(a,fname); //Store it in the lowest available ID, highid
  return enigma::file_highid++; //Return our index and increment it for next time
}

void file_text_close(int fileid) // Closes the file with the given file id.
{
   if(fileid == -1) {
    #ifdef DEBUG_MODE
      enigma_user::show_error("Cannot close an unopened file.",false);
    #endif
    return;
  }

  fclose(enigma::files[fileid].f);
  enigma::files[fileid].f = NULL;

  //Determine new high id: Lowest possible
  for (int i = enigma::file_highid; i >= 0; i--)
    if (enigma::files[i].f == NULL)
      enigma::file_highid = i;
}

void file_text_write_string(int fileid,string str) { // Writes the string to the file with the given file id.
  fwrite(str.c_str(),1,str.length(),enigma::files[fileid].f);
}

void file_text_write_real(int fileid,double x) // Write the real value to the file with the given file id.
{
  const enigma::openFile &mf = enigma::files[fileid];
  fprintf(mf.f, " %.16g", x);
}

void file_text_writeln(int fileid) // Write a newline character to the file.
{
  const enigma::openFile &mf = enigma::files[fileid];
  fputc('\n',mf.f);
}

string file_text_read_string(int fileid) { // Reads a string from the file with the given file id and returns this string. A string ends at the end of line.
  enigma::openFile &mf = enigma::files[fileid];
  if (!mf.sdata[mf.spos]) return "";
  string strr = mf.sdata.substr(mf.spos);
  mf.spos = mf.sdata.length();
  if (feof(mf.f))
    mf.eof = true;
  return strr;
}

string file_text_read_all(int fileid) {
  string contents;
  fseek(enigma::files[fileid].f, 0, SEEK_END);
  contents.resize(ftell(enigma::files[fileid].f));
  fseek(enigma::files[fileid].f, 0, SEEK_SET);
  fread(&contents[0],1,contents.size(),enigma::files[fileid].f);
  enigma::files[fileid].eof = true;
  return contents;
}

bool file_text_eoln(int fileid)
{
    enigma::openFile &mf = enigma::files[fileid];
    return (mf.spos >= mf.sdata.length());
}

inline bool is_whitespace(char x) { return x == ' ' or x == '\t' or x == '\r' or x == '\n'; }

double file_text_read_real(int fileid) { // Reads a real value from the file and returns this value.
  enigma::openFile &mf = enigma::files[fileid];
  double r1;
  int apos;
  if (mf.spos >= mf.sdata.length()) return -1;
  while (is_whitespace(mf.sdata[mf.spos])) {
    mf.spos++;
    if (mf.spos >= mf.sdata.length()) return -1;
  }
  if (sscanf(mf.sdata.substr(mf.spos).c_str(),"%lf%n",&r1,&apos),apos)
    mf.spos += apos;
  if (mf.spos >= mf.sdata.length() && feof(mf.f))
    mf.eof = true;
  return r1;
}

void file_text_readln(int fileid) // Skips the rest of the line in the file and starts at the start of the next line.
{
  if (feof(enigma::files[fileid].f))
    enigma::files[fileid].eof = true;
  string ret;
  char buf[BUFSIZ];
    buf[0] = 0;
  while (fgets(buf,BUFSIZ,enigma::files[fileid].f))
  {
    ret += buf;
    if (ret[ret.length()-1] == '\n' or ret[ret.length()-1] == '\r')
      break;
    buf[0] = 0;
  }
  size_t dp;
  for (dp = ret.length()-1; dp != size_t(-1) and (ret[dp] == '\n' or ret[dp] == '\r'); dp--);
  ret.erase(dp+1);
  enigma::files[fileid].sdata = ret;
  enigma::files[fileid].spos = 0;
}

bool file_text_eof(int fileid) { // Returns whether we reached the end of the file.
  return (enigma::files[fileid].eof);
}

void load_info(string fname) {
	int file = file_text_open_read(fname);
	enigma::gameInfoText = file_text_read_all(file);
	file_text_close(file);
}

}

// Binary file manipulation

namespace enigma_user
{

int file_bin_open(string fname,int mode) // Opens the file with the indicated name. The mode indicates what can be done with the file: 0 = reading, 1 = writing, 2 = both reading and writing). When the file does not exist it is created. The function returns the id of the file that must be used in the other functions.
{
  FILE *a;
  if (mode & 2) {
    a = fopen(fname.c_str(),"r+b"); //Open for reading + writing if it exists, failing otherwise
    if (!a) a = fopen(fname.c_str(),"w+b"); //Open for writing + reading, overwriting if it exists or creating otherwise
  } else a = fopen(fname.c_str(),(mode & 1) ? "wb" : "rb");

  if (!a)      //Failure
    return -1; //Behavior for fail is -1 return

  enigma::files[enigma::file_highid] = enigma::openFile(a,fname); //Store it in the lowest available ID, highid
  return enigma::file_highid++; //Return our index and increment it for next time
}

bool file_bin_rewrite(int fileid) // Rewrites the file with the given file id, that is, clears it and starts writing at the start.
{
  enigma::openFile &mf = enigma::files[fileid];
  mf.f = freopen (mf.sdata.c_str(), "wb", mf.f);

  if (mf.f == NULL) {
    #ifdef DEBUGMODE
      enigma_user::show_error("Failed to reopen binary file. Sure it's a binary file? Drive been removed?",false);
    #endif
    return false;
  }
  return true;
}

void file_bin_close(int fileid) // Closes the file with the given file id.
{
  fclose(enigma::files[fileid].f);
  enigma::files[fileid].f = NULL;

  //Determine new high id: Lowest possible
  for (int i = enigma::file_highid; i >= 0; i--)
    if (enigma::files[i].f == NULL)
      enigma::file_highid = i;
}

size_t file_bin_size(int fileid) // Returns the size (in bytes) of the file with the given file id.
{
  size_t sp = ftell(enigma::files[fileid].f);
  fseek(enigma::files[fileid].f, 0, SEEK_END);
  size_t ret = ftell(enigma::files[fileid].f);
  fseek(enigma::files[fileid].f, sp, SEEK_SET);
  return ret;
}

size_t file_bin_position(int fileid) { // Returns the current position (in bytes; 0 is the first position) of the file with the given file id.
  return ftell(enigma::files[fileid].f);
}

void file_bin_seek(int fileid,size_t pos) { // Moves the current position of the file to the indicated position. To append to a file move the position to the size of the file before writing.
  fseek(enigma::files[fileid].f, pos, SEEK_SET);
}

void file_bin_write_byte(int fileid,unsigned char byte) { // Writes a byte of data to the file with the given file id.
  fputc(byte, enigma::files[fileid].f);
}

int file_bin_read_byte(int fileid) { // Reads a byte of data from the file and returns this.
  return fgetc(enigma::files[fileid].f);
}

}
