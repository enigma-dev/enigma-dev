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
#include "var_cr3.h"

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

bool is_string(enigma::variant val){return val.type;}
bool is_string(var& val){return val.values[0]->type;}
bool is_real(enigma::variant val){return !val.type;}
bool is_real(var& val){return !val.values[0]->type;}

std::string chr(char val){
	char ret[2]={val,0};
	return ret;
}

int ord(char str){return str;}
int ord(char* str){return *str;}
int ord(std::string str){return str[0];}

double real(std::string str){return atof(str.c_str());}
double real(char* str){return atof(str);}
double real(double str){return str;}
double real(var& str){return str.values[0]->type?atof(((std::string)str).c_str()):(double)str;}

size_t string_length(std::string str){return str.length();}
size_t string_length(char* str){
	char*s=str;
	while(*s++);
	return s-str;
}

int string_pos(std::string substr,std::string str){
	return str.find(substr,0)+1;
}

std::string string_copy(std::string str,double index,double count){
	unsigned int indx=index<=1?0:(int)index-1;
	return indx>str.length()?"":str.substr(indx,(int)count);
}

std::string string_char_at(std::string str,double index){
	unsigned int n=index<=1?0:(unsigned int)index-1;
	char ret[2]={n<str.length()?str[n]:0,0};
	return ret;
}

std::string string_delete(std::string str,double index,double count){
	return str.erase(index<2?0:(size_t)index-1,count<1?0:(size_t)count);
}

std::string string_insert(std::string substr,std::string str,double index){
	if(index<=1) return str.insert(0,substr);
	size_t x=(size_t)index-1;
	return str.insert(x>str.length()?str.length():x,substr);
}

std::string string_replace(std::string str,std::string substr,std::string newstr){
	size_t pos=str.find(substr,0);
	return pos==(size_t)-1?str:str.replace(pos,substr.length(),newstr);
}

std::string string_replace_all(std::string str,std::string substr,std::string newstr){
	int pos=0,sublen=substr.length(),newlen=newstr.length();
    while((pos=str.find(substr,pos))!=-1){
		str.replace(pos,sublen,newstr);
		pos+=newlen;
	}
	return str;
}

int string_count(std::string substr,std::string str){
	int sublen=substr.length(),pos=-sublen,occ=0;
    while((pos=str.find(substr,pos+sublen))!=-1) occ++;
    return occ;
}

std::string string_lower(std::string str){
	int len=str.length()-1;
	for(int i=0;i<len;i++)
		if(ldgrs[(unsigned char)str[i]]&2) str[i]-=32;
	return str;
}

std::string string_upper(std::string str){
	int len=str.length()-1;
	for(int i=0;i<len;i++)
		if(ldgrs[(unsigned char)str[i]]&1) str[i]-=32;
	return str;
}

std::string string_repeat(std::string str,int count){
	std::string ret;
	for(int i=count;i;i--) ret.append(str);
	return ret;
}

std::string string_letters(std::string str){
	std::string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&3) ret+=*c;
	return ret;
}

std::string string_digits(std::string str){
	std::string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]&4) ret+=*c;
	return ret;
}

std::string string_lettersdigits(std::string str){
	std::string ret;
	for(const char*c=str.c_str();*c;c++)
		if(ldgrs[(unsigned char)*c]) ret+=*c;
	return ret;
}
