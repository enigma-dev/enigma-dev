/*
 * Copyright (C) 2014 Seth N. Hetu
 *
 * This file is a part of the ENIGMA Development Environment.
 *
 * ENIGMA is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the license or any later version.
 *
 * This application and its source code is distributed AS-IS, WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this code. If not, see <http://www.gnu.org/licenses/>
 */

#include "Widget_Systems/widgets_mandatory.h"
#include "image_formats.h"
#include "nlpo2.h"

#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>

namespace {
const unsigned int ERR_SUCCESS          = 0; //No error (easy boolean checK)
const unsigned int ERR_FILE_CANT_OPEN   = 1;
const unsigned int ERR_OUT_OF_BYTES     = 2;
const unsigned int ERR_BAD_MAG_NUM      = 3;
const unsigned int ERR_BAD_GIF_VERS     = 4;
const unsigned int ERR_NONZERO_PIXEL_ASPECT_RATIO = 5;
const unsigned int ERR_ZERO_IMAGES                = 14;
const unsigned int ERR_UNKNOWN_CONTROL_CODE       = 6;
const unsigned int ERR_INTERLACED_IMAGE           = 7;
const unsigned int ERR_EXPECTED_CLEAR_CODE        = 8;
const unsigned int ERR_OUT_OF_BITS_IN_BYTESTREAM  = 9;
const unsigned int ERR_OVERSCAN                  = 10;
const unsigned int ERR_INDEX_COUNT_MISMATCH      = 11;
const unsigned int ERR_BITS_PAST_8               = 12;
const unsigned int ERR_CODEBITS_PAST_12          = 13;

const char* ERRMSG_SUCCESS = "Success! Not an error!";
const char* ERRMSG_FILE_CANT_OPEN = "Couldn't open or read file.";
const char* ERRMSG_OUT_OF_BYTES = "File is out of bytes.";
const char* ERRMSG_BAD_MAG_NUM = "Bad GIF magic number (probably not a GIF).";
const char* ERRMSG_BAD_GIF_VERS = "Bad GIF version number (this is weird!).";
const char* ERRMSG_NONZERO_PIXEL_ASPECT_RATIO = "We don't support pixel aspect ratios other than 0.";
const char* ERRMSG_UNKNOWN_CONTROL_CODE = "Unknown control code.";
const char* ERRMSG_INTERLACED_IMAGE = "We don't support interlaced images.";
const char* ERRMSG_EXPECTED_CLEAR_CODE = "The clear code was expected; image may be corrupt.";
const char* ERRMSG_OUT_OF_BITS_IN_BYTESTREAM = "Ran out of bits in the bytestream! Wacky!";
const char* ERRMSG_OVERSCAN = "Overscan when compiling image data.";
const char* ERRMSG_INDEX_COUNT_MISMATCH = "Index count mismatch";
const char* ERRMSG_UNKNOWN = "Unknown error message! What did you do???";
const char* ERRMSG_BITS_PAST_8 = "Self-reported past 8 bits in a byte.";
const char* ERRMSG_CODEBITS_PAST_12 = "Self-reported past 12 bits in a control code.";

const unsigned int MaxCodeSize = 12;

inline void clearmem(unsigned char* &out) {
  delete[] out;
  out = 0;
}

struct LogicalScreen {
  LogicalScreen() : canvasWidth(0), canvasHeight(0), gctFlag(false), gctSize(0), bgColorIndex(0) {}
  unsigned int canvasWidth;
  unsigned int canvasHeight;
  bool gctFlag;
  unsigned int gctSize;
  unsigned int bgColorIndex;
};

typedef std::vector<unsigned int> ColorTuple;

//Return 0b111 for 3, etc.
#define GetMask(size) ((1 << size) - 1)

//Turns 1 into [1], etc. Pads up to clearCode
std::vector<ColorTuple> buildColorTable(size_t colorTableSize, unsigned int clearCode, unsigned int eofCode)
{
  std::vector<ColorTuple> res;
  res.reserve(eofCode+1);
  for (size_t i=0; i<colorTableSize; i++) {
    res.push_back(ColorTuple(1, res.size()));
  }
  unsigned int nxtClr = colorTableSize;
  while (nxtClr<=eofCode) {
    res.push_back(ColorTuple());
    nxtClr += 1;
  }
  return res;
}

//Does checking; returns false if it can't read all sub-blocks.
bool skipSubBlocks(const unsigned char* bytes, size_t& pos, size_t length) 
{
  unsigned int sz = 0;
  for (;;) {
    if (pos+1>length) { return false; }
    sz = bytes[pos++];
    if (sz==0) { break; }
    if (pos+sz>length) { return false; }
    pos += sz;
  }
  return true;
}

//Should be faster. Do NOT pass currCodeSize by reference!
//Returns an error code, or ERR_SUCCESS on success.
unsigned int readBits2(unsigned char* bytes, size_t& pos, const size_t size, unsigned int& subBlockEnd, unsigned int& currBit, unsigned int currCodeSize, unsigned int& res)
{
  if (currCodeSize>MaxCodeSize) { return ERR_CODEBITS_PAST_12; }
  if (currBit>8) { return ERR_BITS_PAST_8; }

  //Do we need to advance a byte?
  if (currBit==8) {
    currBit = 0;
    pos++;
    if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
    //Do we need to advance a block?
    if (pos>subBlockEnd) {
      subBlockEnd = pos + bytes[pos];
      pos++;
      if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
    }
  }

  //The max code size is 12. So, at most, we read a prefix (from the current byte), a middle (the entire second byte) and a suffix (from the third byte).
  if (currCodeSize<=(8-currBit)) {
    //We can read the entire thing from the current byte!
    res = bytes[pos]&GetMask(currCodeSize);
    bytes[pos] >>= currCodeSize;
    currBit += currCodeSize;
  } else {
    //We need the current byte, and then some.
    unsigned int read = 8-currBit;
    res = bytes[pos]&GetMask(read);
    currCodeSize -= read;

    //Advance a byte
    currBit = 0;
    pos++;
    if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
    //Do we need to advance a block?
    if (pos>subBlockEnd) {
      subBlockEnd = pos + bytes[pos];
      pos++;
      if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
    }

    //We might need all or part of this byte.
    if (currCodeSize<8) {
      //We need part of it.
      res |= ((bytes[pos]&GetMask(currCodeSize))<<read);
      bytes[pos] >>= currCodeSize;
      currBit += currCodeSize;
    } else {
      //We need all of it.
      res |= bytes[pos]<<read;
      read += 8;
      currCodeSize -= 8;
      currBit = 8;

      //There may be a few more bits.
      if (currCodeSize>0) {
        //Advance a byte
        currBit = 0;
        pos++;
        if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
        //Do we need to advance a block?
        if (pos>subBlockEnd) {
          subBlockEnd = pos + bytes[pos];
          pos++;
          if (pos>size) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
        }

        //Read. There will never be a need to advance at this point.
        res |= ((bytes[pos]&GetMask(currCodeSize))<<read);
        bytes[pos] >>= currCodeSize;
        currBit += currCodeSize;
      }
    }
  }

  return ERR_SUCCESS;
}

unsigned char* read_entire_file(const char* filename, size_t& size) 
{
  std::ifstream input(filename, std::ios::binary|std::ios::ate);
  if (input.good()) {
    size = input.tellg();
    unsigned char* bytes = new unsigned char[size];
    input.seekg(0, std::ios::beg);
    input.read(reinterpret_cast<char*>(bytes), size);
    if (input) { return bytes; }
  }
  return 0;
}


} //End un-named namespace

namespace enigma
{
std::vector<RawImage> image_load_gif(const std::filesystem::path& filename)
{
  // Read the entire file into a byte array. This is reasonable because we will output width*height*4 bytes, and the 
  // GIF file will be noticeably less (it's compressed, indexed color, and no alpha).
  size_t pos = 0;
  size_t size = 0;
  std::vector<RawImage> res;

  //File input
  std::unique_ptr<unsigned char> bytes_c(read_entire_file(filename.u8string().c_str(), size));
  unsigned char *bytes = bytes_c.get();
  if (!bytes) { errno = ERR_FILE_CANT_OPEN; return res; }

  //
  //Phase 1: A few "check only" things.
  // TODO: Combine as many of the length checks as possible.
  //

  //Read the magic number.
  if (pos+3>size) { errno = ERR_OUT_OF_BYTES; return res; }
  std::string magNum = "XXX";
  magNum[0] = static_cast<char>(bytes[pos++]);
  magNum[1] = static_cast<char>(bytes[pos++]);
  magNum[2] = static_cast<char>(bytes[pos++]);
  if (magNum != "GIF") { errno = ERR_BAD_MAG_NUM; return res; }

  //Read the gif version
  if (pos+3>size) { errno = ERR_OUT_OF_BYTES; return res; }
  std::string gifVers = "XXX";
  gifVers[0] = static_cast<char>(bytes[pos++]);
  gifVers[1] = static_cast<char>(bytes[pos++]);
  gifVers[2] = static_cast<char>(bytes[pos++]);
  if (gifVers!="87a" && gifVers!="89a") { errno = ERR_BAD_GIF_VERS; return res; }

  //Read the description of the logical screen.
  if (pos+7>size) { errno = ERR_OUT_OF_BYTES; return res; }
  LogicalScreen screen;
  screen.canvasWidth = bytes[pos] | (bytes[pos+1]<<8);
  pos += 2;
  screen.canvasHeight = bytes[pos] | (bytes[pos+1]<<8);
  pos += 2;
  unsigned int pb1 = bytes[pos++];
  screen.gctFlag = pb1&0x80;
  screen.gctSize = 2<<(pb1 & 0x7);
  screen.bgColorIndex = bytes[pos++];
  if (bytes[pos++] != 0) { errno = ERR_NONZERO_PIXEL_ASPECT_RATIO; return res; }

  //Read the table of global colors (optional).
  size_t globalColorStart=0;
  if (screen.gctFlag) {
    if (pos+(screen.gctSize*3)>size) { errno = ERR_OUT_OF_BYTES; return res; }
    globalColorStart = pos;
    pos += (screen.gctSize*3);
  }

  //
  //Phase 2: Scan to the end and get the total image count.
  //

  {
  int num_images = 0;
  for (size_t myPos = pos;;) {
    if (myPos+1>size) { errno = ERR_OUT_OF_BYTES; return res; }
    unsigned int ctrlCode = bytes[myPos++];
    if (ctrlCode==0x21) { //It's an extension; skip it.
      if (myPos+2>size) { errno = ERR_OUT_OF_BYTES; return res; }
      unsigned int len = bytes[myPos+1]; //Length
      myPos += 2;
      if (myPos+len>size) { errno = ERR_OUT_OF_BYTES; return res; }
      myPos += len;
      if (!skipSubBlocks(bytes, myPos, size)) { errno = ERR_OUT_OF_BYTES; return res; }
    } else if (ctrlCode==0x3B) { //EOF; done;
      break;
    } else if (ctrlCode==0x2C) { //It's an image; skip and add one.
      num_images++;
      if (myPos+9>size) { errno = ERR_OUT_OF_BYTES; return res; }
      myPos += 9;
      unsigned int pb1 = bytes[myPos-1];
      if (pb1&0x80) { //Skip the local color table.
        unsigned int lctSize = 2<<(pb1 & 0x7);
        if (myPos+(lctSize*3)>size) { errno = ERR_OUT_OF_BYTES; return res; }
        myPos += lctSize*3;
      }
      //Skip the LZW min. code size.
      if (myPos+1>size) { errno = ERR_OUT_OF_BYTES; return res; }
      myPos++;
      if (!skipSubBlocks(bytes, myPos, size)) { errno = ERR_OUT_OF_BYTES; return res; }
    } else {
      errno = ERR_UNKNOWN_CONTROL_CODE;
      return res;
    }
  }
  if (!num_images) { errno = ERR_ZERO_IMAGES; return {}; }
  res.resize(num_images);
  }

  //
  //Phase 3: We now have an image count, so we can create our output buffer (and then write directly into it).
  //

  //At the same time, save our output properties.
  const unsigned int gif_width  = screen.canvasWidth;
  const unsigned int gif_height = screen.canvasHeight;
  const unsigned int final_size = gif_width * gif_height * 4;
  for (RawImage &frame : res) {
    frame.w = gif_width;
    frame.h = gif_height;
  }

  //Create the output buffer.
  unsigned char *out;
  res[0].pxdata = out = new unsigned char[final_size](); // Initialize to zero.
  if (screen.gctFlag) {
    unsigned char r = bytes[globalColorStart + screen.bgColorIndex*3 + 0];
    unsigned char g = bytes[globalColorStart + screen.bgColorIndex*3 + 1];
    unsigned char b = bytes[globalColorStart + screen.bgColorIndex*3 + 2];
    for (size_t y=0; y<gif_height; y++) {
      for (size_t x=0; x<gif_width; x++) {
        size_t pos2 = y*gif_width*4 + x*4;
        if (pos2+4>final_size) {
          clearmem(out);
          errno = ERR_OVERSCAN;
          return res;
        }
        out[pos2] = b;
        out[pos2+1] = g;
        out[pos2+2] = r;
        out[pos2+3] = 0xFF; //alpha
      }
    }
  }

  //How to dispose
  unsigned int disposalMethod = 0;
  unsigned int transpColor = 1<<14; //This will never be a Gif color index.

  //
  //Phase 4: Now read all image data, and decompress as you go.
  //

  unsigned int curr_img = 0;
  for (;;) {
    if (pos+1>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
    unsigned int ctrlCode = bytes[pos++];
    if (ctrlCode==0x21) { //It's an extension; skip it.
      if (pos+2>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
      ctrlCode = bytes[pos++]; //Extension control
      unsigned int extLen = bytes[pos++]; //Length
      if (pos+extLen>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
      if (ctrlCode==0xF9) { //Graphics control extension; we need a bit of data.
        disposalMethod = (bytes[pos]&0x1C)>>2;
        transpColor = (bytes[pos]&0x1) ? bytes[pos+3] : -1;
      }
      pos += extLen;
      if (!skipSubBlocks(bytes, pos, size)) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
    } else if (ctrlCode==0x3B) { //EOF; done;
      break;
    } else if (ctrlCode==0x2C) { //It's an image; read and decompress it.
      DEBUG_MESSAGE("[GIF] Reading image: " + std::to_string(curr_img+1) + " of " + std::to_string(res.size()), MESSAGE_TYPE::M_INFO);
      //Read top-level image properties.
      if (pos+9>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
      unsigned int left = bytes[pos] | (bytes[pos+1]<<8);
      pos += 2;
      unsigned int top = bytes[pos] | (bytes[pos+1]<<8);
      pos += 2;
      unsigned int width = bytes[pos] | (bytes[pos+1]<<8);
      pos += 2;
      unsigned int height = bytes[pos] | (bytes[pos+1]<<8);
      pos += 2;
      unsigned int pb1 = bytes[pos++];
      bool lctFlag = pb1&0x80;
      if (pb1&0x40) { clearmem(out); errno = ERR_INTERLACED_IMAGE; return res; }
      unsigned int lctSize = 2<<(pb1 & 0x7);

      //Read Local Color Table, if applicable.
      size_t localColorStart = globalColorStart;
      size_t colorTableSize = screen.gctSize;
      if (lctFlag) {
        if (pos+(lctSize*3)>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
        localColorStart = pos;
        pos += (lctSize*3);
        colorTableSize = lctSize;
      }

      //Read the lzw minimum code size.
      if (pos+1>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
      unsigned int lzwMinCodeSize = bytes[pos++];

      //Prepare to read the image data. We always need at least one byte (we check at the end of the loop).
      if (pos+1>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }
      unsigned int subBlockEnd = pos + bytes[pos];
      pos++;
      if (pos>subBlockEnd) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; } //Might be better as "not enough image data"?
      if (subBlockEnd+1>size) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }

      //More stuff.
      const unsigned int clearCode = 1 << lzwMinCodeSize;
      const unsigned int eofCode = clearCode + 1;
      std::vector<ColorTuple> currColorTable = buildColorTable(colorTableSize, clearCode, eofCode);
      ColorTuple prevTuple;
      ColorTuple currTuple;

      //Our image positioning information.
      unsigned int y=top;
      unsigned int x=left;

      //Read the image data, in blocks. Translate as we go.
      unsigned int currBit = 0;
      unsigned int currCodeSize = lzwMinCodeSize + 1;
      unsigned int nested = 0; //Sanity check.
      bool first = true;

      unsigned int err = 0;
      for (;;) {
        //Read the next control code
        unsigned int currCode = 0;
        err = readBits2(bytes, pos, size, subBlockEnd, currBit, currCodeSize, currCode);
        if (err!=ERR_SUCCESS) { clearmem(out); errno = err; return {}; }

        //Clear/EOF are special control codes.
        if (currCode==clearCode) {
          first = true;
          currCodeSize = lzwMinCodeSize + 1;
          currColorTable = buildColorTable(colorTableSize, clearCode, eofCode);
          continue;
        }
        if (currCode == eofCode) {
          break;
        }

        if (first) {
          first = false;
          currTuple = currColorTable[currCode];
        } else {
          //Normal loop
          if (currCode<currColorTable.size()) {
            //Add it to the index stream
            ColorTuple newCode = prevTuple;
            currTuple = currColorTable[currCode];
            unsigned int tmp = currColorTable[currCode][0];
            newCode.push_back(tmp);
            currColorTable.push_back(newCode);
          } else {
            //More complicated
            unsigned int tmp = prevTuple[0];
            ColorTuple newCode = prevTuple;
            newCode.push_back(tmp);
            currTuple = newCode;
            currColorTable.push_back(newCode);
          }

          //Increment codeSize?
          if (currColorTable.size()==static_cast<unsigned int>(1<<currCodeSize) && currCodeSize<12) {
            currCodeSize += 1;
          }
        }

        //Add the pixels.
        nested += currTuple.size();
        for (std::vector<unsigned int>::const_iterator it=currTuple.begin(); it!=currTuple.end(); it++) {
          //Set1
          if ((*it)!=transpColor) {
            size_t pos2 = y*gif_width*4 + x*4;
            if (pos2+4>final_size) { clearmem(out); errno = ERR_OVERSCAN; return res; }
            out[pos2] = bytes[localColorStart + (*it)*3 + 2];
            out[pos2+1] = bytes[localColorStart + (*it)*3 + 1];
            out[pos2+2] = bytes[localColorStart + (*it)*3 + 0];
            out[pos2+3] = 0xFF; //alpha
          }

          //Increment
          x++;
          if (x-left>=width) {
            x = left;
            y++;
          }
        }

        //Finally
        prevTuple = currTuple;
      }

      //Skip the remaining null terminator.
      pos = subBlockEnd+1;

      //Skip any remaining sub-blocks (should effectively skip a single "0").
      if (!skipSubBlocks(bytes, pos, size)) { clearmem(out); errno = ERR_OUT_OF_BYTES; return res; }

      //Make sure we read enough colors.
      if (nested != width*height) { 
        DEBUG_MESSAGE("[GIF] Index mismatch: " + std::to_string(nested)  + " : " + std::to_string(width*height), MESSAGE_TYPE::M_ERROR);
        clearmem(out);
        errno = ERR_INDEX_COUNT_MISMATCH;
        return res; 
      }

      //We're done! React to the disposal method.
      if (curr_img+1<res.size()) {
        if (disposalMethod==1) {
          //Leave the background in place (i.e., repaint it).
          memcpy(res[curr_img+1].pxdata, res[curr_img].pxdata, final_size);
        } else if (disposalMethod==2) {
          //Restore to background color (NOTE: We initialize to background color, just do nothing.).
        } else if (disposalMethod==3) {
          //Restore to the previous state (image, in this case).
          //NOTE: Mostly untested; few GIFs use this.
          memcpy(res[curr_img+1].pxdata, res[curr_img-1].pxdata, final_size);
        }
      }


      //Finally:
      disposalMethod = 0;
      transpColor = 1<<14;
      curr_img++;
      res[curr_img].pxdata = out = new unsigned char[final_size](); // Initialize to zero.
    } else {
      DEBUG_MESSAGE("[GIF] Unknown control code: " + std::to_string(ctrlCode), MESSAGE_TYPE::M_ERROR);
      clearmem(out);
      errno = ERR_UNKNOWN_CONTROL_CODE;
      return res;
    }
  }

  DEBUG_MESSAGE("[GIF] All GIF sub-images saved.", MESSAGE_TYPE::M_INFO);
  return res;
}

const char* load_gif_error_text(unsigned int err)
{
  switch (err) {
    case ERR_SUCCESS: return ERRMSG_SUCCESS;
    case ERR_FILE_CANT_OPEN: return ERRMSG_FILE_CANT_OPEN;
    case ERR_OUT_OF_BYTES: return ERRMSG_OUT_OF_BYTES;
    case ERR_BAD_MAG_NUM: return ERRMSG_BAD_MAG_NUM;
    case ERR_BAD_GIF_VERS: return ERRMSG_BAD_GIF_VERS;
    case ERR_NONZERO_PIXEL_ASPECT_RATIO: return ERRMSG_NONZERO_PIXEL_ASPECT_RATIO;
    case ERR_UNKNOWN_CONTROL_CODE: return ERRMSG_UNKNOWN_CONTROL_CODE;
    case ERR_INTERLACED_IMAGE: return ERRMSG_INTERLACED_IMAGE;
    case ERR_EXPECTED_CLEAR_CODE: return ERRMSG_EXPECTED_CLEAR_CODE;
    case ERR_OUT_OF_BITS_IN_BYTESTREAM: return ERRMSG_OUT_OF_BITS_IN_BYTESTREAM;
    case ERR_OVERSCAN: return ERRMSG_OVERSCAN;
    case ERR_INDEX_COUNT_MISMATCH: return ERRMSG_INDEX_COUNT_MISMATCH;
    case ERR_BITS_PAST_8: return ERRMSG_BITS_PAST_8;
    case ERR_CODEBITS_PAST_12: return ERRMSG_CODEBITS_PAST_12;
    default: return ERRMSG_UNKNOWN;
  }
}

} //namespace enigma


//Remove our macro.
#undef GetMask

