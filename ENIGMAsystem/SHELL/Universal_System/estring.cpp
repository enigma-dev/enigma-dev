/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include <string>
#include <cstdlib>
#include "var4.h"

const char ldgrs[256]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,
0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

bool is_string(variant val) { return val.type;  }
bool is_real(variant val)   { return !val.type; }
string chr(char val){
	char ret[2]={val,0};
	return ret;
}

int ord(char str)        { return str; }
int ord(const char* str) { return *str; }
int ord(string str)      { return str[0]; }

double real(string str) { return atof(str.c_str()); }
double real(char* str)  { return atof(str); }
double real(double str) { return str; }
double real(variant str){ return str.type ? atof(((string)str).c_str()) : (double) str; }
double real(var& str)   { return (*str).type ? atof(((string)str).c_str()) : (double) str; }

size_t string_length(string str) { return str.length(); }
size_t string_length(const char* str)
{
	const char* s = str;
	while (*s++);
	return str - s;
}

int string_pos(string substr,string str){
	return str.find(substr,0)+1;
}

string string_copy(string str,double index,double count){
	unsigned int indx=index<=1?0:(int)index-1;
	return indx>str.length()?"":str.substr(indx,(int)count);
}

string string_char_at(string str,double index){
	unsigned int n=index<=1?0:(unsigned int)index-1;
	char ret[2]={n<str.length()?str[n]:0,0};
	return ret;
}

string string_delete(string str,double index,double count){
	return str.erase(index<2?0:(size_t)index-1,count<1?0:(size_t)count);
}

string string_insert(string substr,string str,double index){
	if(index<=1) return str.insert(0,substr);
	size_t x=(size_t)index-1;
	return str.insert(x>str.length()?str.length():x,substr);
}

string string_replace(string str,string substr,string newstr){
	size_t pos=str.find(substr,0);
	return pos==(size_t)-1?str:str.replace(pos,substr.length(),newstr);
}

string string_replace_all(string str,string substr,string newstr){
	int pos=0,sublen=substr.length(),newlen=newstr.length();
    while((pos=str.find(substr,pos))!=-1){
		str.replace(pos,sublen,newstr);
		pos+=newlen;
	}
	return str;
}

int string_count(string substr,string str){
	int sublen=substr.length(),pos=-sublen,occ=0;
    while((pos=str.find(substr,pos+sublen))!=-1) occ++;
    return occ;
}

string string_lower(string str){
	int len=str.length()-1;
	for(int i=0;i<len;i++)
		if(ldgrs[(unsigned char)str[i]]&2) str[i]-=32;
	return str;
}

string string_upper(string str){
	int len=str.length()-1;
	for(int i=0;i<len;i++)
		if(ldgrs[(unsigned char)str[i]]&1) str[i]-=32;
	return str;
}

string string_repeat(string str,int count){
	string ret;
	for(int i=count;i;i--) ret.append(str);
	return ret;
}

string string_letters(string str){
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&3) ret+=*c;
	return ret;
}

string string_digits(string str){
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&4) ret+=*c;
	return ret;
}

string string_lettersdigits(string str){
	string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]) ret+=*c;
	return ret;
}
