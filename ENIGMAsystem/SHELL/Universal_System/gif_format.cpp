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

#include "gif_format.h"
#include "nlpo2.h"

#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>

#include <cassert> //TEMP

namespace {
const unsigned int ERR_SUCCESS          = 0; //No error (easy boolean checK)
const unsigned int ERR_FILE_CANT_OPEN   = 1;
const unsigned int ERR_OUT_OF_BYTES     = 2;
const unsigned int ERR_BAD_MAG_NUM      = 3;
const unsigned int ERR_BAD_GIF_VERS     = 4;
const unsigned int ERR_NONZERO_PIXEL_ASPECT_RATIO = 5;
const unsigned int ERR_UNKNOWN_CONTROL_CODE       = 6;
const unsigned int ERR_INTERLACED_IMAGE           = 7;
const unsigned int ERR_EXPECTED_CLEAR_CODE        = 8;
const unsigned int ERR_OUT_OF_BITS_IN_BYTESTREAM  = 9;
const unsigned int ERR_OVERSCAN                  = 10;
const unsigned int ERR_INDEX_COUNT_MISMATCH      = 11;

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


struct ColorRGB {
  ColorRGB(unsigned int r, unsigned int g, unsigned int b) : r(r), g(g), b(b) {}
  unsigned int r;
  unsigned int g;
  unsigned int b;
};

struct LogicalScreen {
  LogicalScreen() : canvasWidth(0), canvasHeight(0), gctFlag(false), clrRes(0), sortFlag(false), gctSize(0), bgColorIndex(0), pixelAspectRatio(0) {}
  unsigned int canvasWidth;
  unsigned int canvasHeight;
  bool gctFlag;
  unsigned int clrRes;
  bool sortFlag;
  unsigned int gctSize;
  unsigned int bgColorIndex;
  unsigned int pixelAspectRatio;
};

struct ColorTuple {
  explicit ColorTuple(unsigned int first) { val.push_back(first); }
  ColorTuple() {}
  std::vector<unsigned int> val;
};

struct Image {
  Image() : left(0), top(0), width(0), height(0), lctFlag(false), interlaceFlag(0), sortFlag(0), reservedBits(0), lctSize(0), lzwMinCodeSize(0) {}
  unsigned int left;
  unsigned int top;
  unsigned int width;
  unsigned int height;
  bool lctFlag;
  bool interlaceFlag;
  bool sortFlag;
  unsigned int reservedBits;
  unsigned int lctSize;
  std::vector<ColorRGB> localColorTable;
  unsigned int lzwMinCodeSize;
  std::vector<unsigned char*> bytes;
  std::vector<unsigned int> sizes;
  //std::vector<unsigned int> ctrlCodes;
  std::vector<ColorTuple> indices;
};

//Can help with debugging.
std::string to_string(const std::vector<ColorTuple>& src) 
{
  std::stringstream res;
  res <<"[";
  std::string comma1 = "";
  for (std::vector<ColorTuple>::const_iterator it=src.begin(); it!=src.end(); it++) {
    res <<comma1 <<"[";
    std::string comma2 = "";
    for (std::vector<unsigned int>::const_iterator it2=it->val.begin(); it2!=it->val.end(); it2++) {
      res <<comma2 <<(*it2);
      comma2 = ",";
    }
    res <<"]";
    comma1 = " , ";
  }
  res <<"]";
  return res.str();
}


//Turns 1 into [1], etc. Pads up to clearCode
std::vector<ColorTuple> buildColorTable(const std::vector<ColorRGB>& colorTable, unsigned int clearCode, unsigned int eofCode)
{
  std::vector<ColorTuple> res;
  for (std::vector<ColorRGB>::const_iterator it=colorTable.begin(); it!=colorTable.end(); it++) {
    res.push_back(ColorTuple(res.size()));
  }
  unsigned int nxtClr = colorTable.size();
  while (nxtClr<=eofCode) {
    res.push_back(ColorTuple());
    nxtClr += 1;
  }
  return res;
}

unsigned int readChar(std::ifstream& input, unsigned int& res) 
{
  char buff[1] = {0};
  input.read(buff,1);
  if (!input) { return ERR_OUT_OF_BYTES; }
  res = static_cast<unsigned int>(buff[0])&0xFF;
  return ERR_SUCCESS;
}


unsigned int subByte(unsigned int src, unsigned int orig, unsigned int length, unsigned int& res) 
{
  //TODO: Bounds check.
  res = 0;
  for (size_t i=0; i<length; i++) {
    res <<= 1;
    unsigned int next = (src>>(7 - (orig+i)))&0x1;
    res |= next;
  }
  return ERR_SUCCESS;
}


unsigned int readInt(std::ifstream& input, unsigned int length, unsigned int& res)
{
  res = 0;
  unsigned int count = 0;
  unsigned int err=ERR_SUCCESS;
  unsigned int next = 0;
  for (unsigned int i=0; i<length; i++) {
    if ((err=readChar(input, next))) { return err; }
    res = res | (next<<(8*count));
    count += 1;
  }
  return ERR_SUCCESS;
}

unsigned int readString(std::ifstream& input, unsigned int length, std::string& res)
{
  unsigned int next = 0;
  unsigned int err=ERR_SUCCESS;
  while (length>0) {
    length -= 1;
    if ((err=readChar(input, next))) { return err; }
    res += std::string(1, static_cast<char>(next));
  }
  return ERR_SUCCESS;
}

unsigned int readSubBlocks(std::ifstream& input, std::vector<unsigned char*>& res, std::vector<unsigned int>& sizes)
{
  unsigned int sz = 0;
  unsigned int err = ERR_SUCCESS;
  for (;;) {
    if ((err=readInt(input, 1, sz))) { return err; }
    if (sz==0) { break; }
    unsigned char* buff = new unsigned char[sz];
    input.read(reinterpret_cast<char*>(buff),sz);
    if (!input) { return ERR_OUT_OF_BYTES; }
    res.push_back(buff);
    sizes.push_back(sz);
  }
  return ERR_SUCCESS;
}

unsigned int readBits(const std::vector<unsigned char*>& bytes, const std::vector<unsigned int>& sizes, unsigned int& currBytestream, unsigned int& currByte, unsigned int& currBit, unsigned int& currCodeSize, unsigned int& res)
{
  res = 0;
  for (size_t i=0; i<currCodeSize; i++) {
    //Can read?
    if (currBytestream>=bytes.size()) { return ERR_OUT_OF_BITS_IN_BYTESTREAM; }
    //Get the bit, append it
    unsigned int nextBit = bytes[currBytestream][currByte]&0x1;
    res |= (nextBit<<i);
    //Increment
    bytes[currBytestream][currByte]>>=1;
    currBit += 1;
    if (currBit==8) {
      currBit = 0;
      currByte += 1;
      if (currByte>=sizes[currBytestream]) {
        currByte = 0;
        currBytestream += 1;
      }
    }
  }
  return ERR_SUCCESS;
}

unsigned int readMagNum(std::ifstream& input) 
{
  //Read the magic number.
  std::string magNum;
  unsigned int err=ERR_SUCCESS;
  if ((err=readString(input, 3, magNum))) { return err; }
  if (magNum != "GIF") { return ERR_BAD_MAG_NUM; }
  return ERR_SUCCESS;
}

unsigned int readGifVers(std::ifstream& input) 
{
  //Read the gif version
  std::string gifVers;
  unsigned int err=ERR_SUCCESS;
  if ((err=readString(input, 3, gifVers))) { return err; }
  if (gifVers!="87a" && gifVers!="89a") { return ERR_BAD_GIF_VERS; }
  return ERR_SUCCESS;
}

unsigned int readLogicalScreenDesc(std::ifstream& input, LogicalScreen& screen)
{
  //Read the logical screen descriptor
  unsigned int err = ERR_SUCCESS;
  if ((err=readInt(input, 2, screen.canvasWidth))) { return err; }
  if ((err=readInt(input, 2, screen.canvasHeight))) { return err; }

  unsigned int pb1 = 0;
  if ((err=readInt(input, 1, pb1))) { return err; }

  unsigned int thing=0;
  if ((err=subByte(pb1, 0, 1, thing))) { return err; }
  screen.gctFlag = thing>0;

  if ((err=subByte(pb1, 1, 3, screen.clrRes))) { return err; }

  thing=0;
  if ((err=subByte(pb1, 4, 1, thing))) { return err; }
  screen.sortFlag = thing>0;

  if ((err=subByte(pb1, 5, 3, screen.gctSize))) { return err; }
  screen.gctSize = static_cast<unsigned int>(pow(2, screen.gctSize+1));

  if ((err=readInt(input, 1, screen.bgColorIndex))) { return err; }
  if ((err=readInt(input, 1, screen.pixelAspectRatio))) { return err; }
  if (screen.pixelAspectRatio != 0) { return ERR_NONZERO_PIXEL_ASPECT_RATIO; }

  return ERR_SUCCESS;
}

unsigned int readColorTable(std::ifstream& input, std::vector<ColorRGB>& colors, bool ctExists, unsigned int ctSize)
{
  unsigned int r=0;
  unsigned int g=0;
  unsigned int b=0;
  unsigned int err = ERR_SUCCESS;
  if (ctExists) {
    for (unsigned int i=0; i<ctSize; i++) {
      if ((err=readInt(input, 1, r))) { return err; }
      if ((err=readInt(input, 1, g))) { return err; }
      if ((err=readInt(input, 1, b))) { return err; }
      colors.push_back(ColorRGB(r,g,b));
    }
  }
  return ERR_SUCCESS;
}

unsigned int readExtension(std::ifstream& input) //, Extension& ext)
{
  unsigned int ctrl = 0;
  unsigned int length = 0;
  unsigned int garbage = 0;

  unsigned int err = ERR_SUCCESS;
  if ((err=readInt(input, 1, ctrl))) { return err; }
  if ((err=readInt(input, 1, length))) { return err; }
  if ((err=readInt(input, length, garbage))) { return err; }

  std::vector<unsigned char*> trash;
  std::vector<unsigned int> trash2;
  if ((err=readSubBlocks(input, trash, trash2))) { return err; }
  for (std::vector<unsigned char*>::const_iterator it=trash.begin(); it!=trash.end(); it++) { delete *it; }
  return ERR_SUCCESS;
}

unsigned int readImage(std::ifstream& input, Image& img)
{
  //Read basic image properties.
  unsigned int err = ERR_SUCCESS;
  if ((err=readInt(input, 2, img.left))) { return err; }
  if ((err=readInt(input, 2, img.top))) { return err; }
  if ((err=readInt(input, 2, img.width))) { return err; }
  if ((err=readInt(input, 2, img.height))) { return err; }
  unsigned int pb1 = 0;
  if ((err=readInt(input, 1, pb1))) { return err; }

  unsigned int tmp = 0;
  if ((err=subByte(pb1, 0, 1, tmp))) { return err; }
  img.lctFlag = tmp>0;

  tmp = 0;
  if ((err=subByte(pb1, 1, 1, tmp))) { return err; }
  img.interlaceFlag = tmp>0;

  if (img.interlaceFlag>0) { return ERR_INTERLACED_IMAGE; }

  tmp = 0;
  if ((err=subByte(pb1, 2, 1, tmp))) { return err; }
  img.sortFlag = tmp>0;

  if ((err=subByte(pb1, 3, 2, img.reservedBits))) { return err; }

  if ((err=subByte(pb1, 5, 3, img.lctSize))) { return err; }
  img.lctSize = static_cast<unsigned int>(pow(2, img.lctSize+1));

  //Read Local Color Table, if applicable.
  if ((err=readColorTable(input, img.localColorTable, img.lctFlag, img.lctSize))) { return err; }

  //One more isolated property.
  if ((err=readInt(input, 1, img.lzwMinCodeSize))) { return err; }

  //Read the image data, in blocks.
  if ((err=readSubBlocks(input, img.bytes, img.sizes))) { return err; }
  return ERR_SUCCESS;
}


} //End un-named namespace


unsigned int load_gif_file(const char* filename, unsigned char*& out, unsigned int& gif_width, unsigned int& gif_height, unsigned int& image_width, unsigned int& image_height, int& num_images)
{
  //Open it in binary mode.
  std::ifstream input(filename, std::ios::binary);
  if (!input.good()) { return ERR_FILE_CANT_OPEN; }

  //Read a few "check only" things.
  unsigned int err = ERR_SUCCESS;
  if ((err=readMagNum(input))) { return err; }
  if ((err=readGifVers(input))) { return err; }

  //Read the description of the logical screen.
  LogicalScreen screen;
  if ((err=readLogicalScreenDesc(input, screen))) { return err; }

  //Read the table of global colors (optional).
  std::vector<ColorRGB> globalColors;
  if ((err=readColorTable(input, globalColors, screen.gctFlag, screen.gctSize))) { return err; }

  //Now we enter into a processing loop based on control codes.
  std::vector<Image> images;
  for (;;) {
    unsigned int ctrlCode = 0;
    if ((err=readInt(input, 1, ctrlCode))) { return err; }
    if (ctrlCode==0x21) {
      //It's an extension.
      //Extension ext;
      if ((err=readExtension(input/*, ext*/))) { return err; }
      std::cerr <<"Skipping extension\n";
    } else if (ctrlCode==0x3B) {
      std::cerr <<"EOF found; file structure is valid.\n";
      break;
    } else if (ctrlCode==0x2C) {
      //It's an image.
      images.push_back(Image());
      if ((err=readImage(input, images.back()))) { return err; }
    } else {
      return ERR_UNKNOWN_CONTROL_CODE;
    }
  }

  //Convert the byte stream into control codes, and convert those into index codes (easier to do both at once).
  //const unsigned int clearCode = static_cast<unsigned int>(pow(2, img.lzwMinCodeSize));
  //const unsigned int eofCode = clearCode + 1;
  const unsigned int MaxCodeSize = 12;
  unsigned int count = 0;

  //We do this per-image; we can tile it later.
  for (std::vector<Image>::iterator it=images.begin(); it!=images.end(); it++) {
    Image& img = *it;
    count += 1;
    unsigned int currBytestream = 0;
    unsigned int currByte = 0;
    unsigned int currBit = 0;
    const unsigned int clearCode = static_cast<unsigned int>(pow(2, img.lzwMinCodeSize));
    const unsigned int eofCode = clearCode + 1;
    const std::vector<ColorRGB>& colorTable = img.lctFlag ? img.localColorTable : globalColors;
    std::vector<ColorTuple> currColorTable = buildColorTable(colorTable, clearCode, eofCode);
    const unsigned int startCodeSize = img.lzwMinCodeSize + 1;
    unsigned int currCodeSize = startCodeSize;
    std::cerr <<"Reading image: " <<count <<" of " <<images.size() <<"\n";
    bool first = true; 

    //Decompress as we go.
    for (;;) {
      //Read the next control code
      unsigned int oldCodeSize = currCodeSize;
      unsigned int currCode = 0;
      if ((err=readBits(img.bytes, img.sizes, currBytestream, currByte, currBit, currCodeSize, currCode))) { return err; }
      //std::cerr <<"Read: " <<currCode <<", color table is: " <<to_string(currColorTable) <<"\n";

      if (oldCodeSize==MaxCodeSize && currCode!=clearCode) { return ERR_EXPECTED_CLEAR_CODE; }
      if (currCode==clearCode) {
        //print("  Clear code")
        currColorTable = buildColorTable(colorTable, clearCode, eofCode);
        //nextCodeSize = startCodeSize;
      }
      if (currCode == eofCode) {
        //print("  EOF code")
        break;
      }
      //img.ctrlCodes.push_back(currCode);

      //Decompress as we go.
      ColorTuple newCode;
      if (currCode==clearCode || currCode==eofCode) {
        continue;
      }
      if (first) {
        first = false;
        img.indices.push_back(currColorTable[currCode]);
      } else {
        //Normal loop
        if (currCode<currColorTable.size()) {
          //Add it to the index stream
          newCode = img.indices[img.indices.size()-1];
          img.indices.push_back(currColorTable[currCode]);
          unsigned int tmp = currColorTable[currCode].val[0];
          newCode.val.push_back(tmp);
          currColorTable.push_back(newCode);
        } else {
          //More complicated
          unsigned int tmp = img.indices[img.indices.size()-1].val[0];
          newCode = img.indices[img.indices.size()-1];
          newCode.val.push_back(tmp);
          img.indices.push_back(newCode);
          currColorTable.push_back(newCode);
        }

        //Increment codeSize?
        if (currColorTable.size()-1 == static_cast<unsigned int>(pow(2, currCodeSize) -1)) {
          std::cerr <<"  Increase code size!\n";
          currCodeSize += 1;
        }
      }
    }

    //Internal check
    unsigned int nested = 0;
    for (std::vector<ColorTuple>::const_iterator it2=img.indices.begin(); it2!=img.indices.end(); it2++) {
      nested += it2->val.size();
    }
    std::cerr <<"So far, read: " <<nested <<" of " <<img.width*img.height <<" indices.\n";
    if (nested != img.width*img.height) { return ERR_INDEX_COUNT_MISMATCH; }
  }

  //For now, just put 5 images per row.
  const unsigned int imgs_per_row = images.size();

  //Save our output properties.
  num_images = images.size();
  gif_width = screen.canvasWidth * imgs_per_row;
  gif_height = screen.canvasHeight * static_cast<unsigned int>(ceil(num_images/static_cast<double>(imgs_per_row)));

  //Need to scale to factors of 2.
  image_width = nlpo2dc(gif_width) + 1;
  image_height = nlpo2dc(gif_height) + 1;

  //Create the output buffer.
  const unsigned int final_size = image_width*image_height*4;
  out = new unsigned char[final_size](); // Initialize to zero.

  //Save all images.
  count = 0;
  for (std::vector<Image>::iterator it=images.begin(); it!=images.end(); it++) {
    Image& img = *it;
    unsigned int outY = count / imgs_per_row;
    unsigned int outX = count % imgs_per_row;

    //TODO: (left,top) might not be (0,0)
    unsigned int y=img.top;
    unsigned int x=img.left;
    const std::vector<ColorRGB>& colorTable = img.lctFlag ? img.localColorTable : globalColors;
    for (std::vector<ColorTuple>::const_iterator it=img.indices.begin(); it!=img.indices.end(); it++) {
      for (std::vector<unsigned int>::const_iterator it2=it->val.begin(); it2!=it->val.end(); it2++) {
        //Set1
        size_t pos = (outY*screen.canvasHeight+y)*image_width*4 + (outX*screen.canvasWidth+x)*4;
        if (pos+4>final_size) { delete []out; out=0; return ERR_OVERSCAN; }
        out[pos++] = colorTable[*it2].b;
        out[pos++] = colorTable[*it2].g;
        out[pos++] = colorTable[*it2].r;
        out[pos++] = 0xFF; //alpha

        //Increment
        x++;
        if (x>=img.width) {
          x = img.left;
          y++;
        }
      }
    }
    count += 1;
  }

  //TODO: We've leaked all the buffered "bytes" arrays. This is easy to clean up, but I'd like to fold it into the "read" step if possible.
  std::cerr <<"All GIF sub-images saved.\n";
  return ERR_SUCCESS;
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
    default: return ERRMSG_UNKNOWN;
  }
}

