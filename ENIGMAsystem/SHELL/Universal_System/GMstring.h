/*********************************************************************************\
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
\*********************************************************************************/

/**Sprite adding functions*****************************************************\

bool is_string(enigma::variant val)
bool is_real(enigma::variant val)
std::string chr(char val)
char cchr(double val)
double ord(char* str)
double real(std::string str)
std::string string(double val)
int string_length(std::string str)
int string_pos(std::string substr,std::string str)
std::string string_copy(std::string str,double index,double count)
std::string string_char_at(std::string str,double index)
std::string string_delete(std::string str,double index,double count)
std::string string_insert(std::string substr,std::string str,double index)
std::string string_replace(std::string str,std::string substr,std::string newstr)
std::string string_replace_all(std::string str,std::string substr,std::string newstr)
int string_count(std::string substr,std::string str)
std::string string_lower(std::string str)
std::string string_upper(std::string str)
std::string string_repeat(std::string str,double count)
std::string string_letters(std::string str)
std::string string_digits(std::string str)
std::string string_lettersdigits(std::string str)

\******************************************************************************/




bool is_string(enigma::variant val)     { return val.type; }
bool is_string(var& val)                { return val.values[0]->type; }

bool is_real(enigma::variant val)       { return !(val.type); }
bool is_real(var& val)                  { return !(val.values[0]->type); }



std::string chr(char val)
{ 
   char ordinal=(char) enigma::negmod(val,255);
   std::string ret;
   ret=ordinal;
   return ret;                         
}

char cchr(double val)
{
    return (char) enigma::negmod(val,255);
}



double ord(char str)
{
  return (double) str;
}
double ord(char* str)
{
  return (double) str[0];
}
double ord(std::string str)
{
  return (double) str[0];
}



double real(std::string str)
{
  return atof(str.c_str());
}
double real(char* str)
{
  return atof(str);
}
double real(double str)
{
  return str;
}
double real(var& str)
{
  if (str.values[0]->type==1)
  {
    std::string strn=(std::string)str;
    return atof(strn.c_str());
  }
  else
  {
    return (double) str;
  }
}



int string_length(std::string str)
{
    return str.length();
}
int string_length(char* str)
{
    return strlen(str);
}


int string_pos(std::string substr,std::string str)
{
    unsigned int a=str.find(substr,0);
    if (a==std::string::npos)
    return 0;
    return a+1;
}
int string_pos(char* substr,std::string str)
{
    unsigned int a=str.find(substr,0);
    if (a==std::string::npos)
    return 0;
    return a+1;
}


std::string string_copy(std::string str,double index,double count)
{
   std::string strn=str;
   unsigned int indx=(unsigned int)index-1; if (indx<0) indx=0;
   
   if (indx>strn.length())
   return "";
   
   return strn.substr(indx,(int)count);
}


std::string string_char_at(std::string str,double index)
{
    unsigned int n=(unsigned int)index-1;
    
    if (n==std::string::npos) n=0;
    
    if (n>=str.length())
    return "";
    
    std::string ret=""; 
    ret+=str[n];
    
    return ret;
}


std::string string_delete(std::string str,double index,double count)
{
   std::string strn=str;
   int x=(int) index-1; if (x<0) x=0;
   int c=(int) count;   if (c<0) c=0;
   return strn.erase(x,c);
}



std::string string_insert(std::string substr,std::string str,double index)
{
    std::string strn=str;
    unsigned int x=(unsigned int) index-1; if (x==std::string::npos) x=0; if (x>strn.length()) x=strn.length(); 
    return strn.insert(x,substr);
}



std::string string_replace(std::string str,std::string substr,std::string newstr)
{
    std::string strr=str;
    int sublen=substr.length();
    int pos=0;
    if ((pos=strr.find(substr,pos)) != -1)
    {
        strr.erase(pos,sublen);
        strr.insert(pos,newstr);
    }
    
    return strr;
}


std::string string_replace_all(std::string str,std::string substr,std::string newstr)
{
    std::string strr=str;
    int sublen=substr.length(),
        newlen=newstr.length();
    int pos=0;
    while ((pos=strr.find(substr,pos)) != -1)
    {
        strr.erase(pos,sublen);
        strr.insert(pos,newstr);
        pos+=newlen;
    }
    
    return strr;
}


int string_count(std::string substr,std::string str)
{
    int sublen=substr.length();
    int pos=-sublen, occ=0;
    while ((pos=str.find(substr,pos+sublen))>=0) occ++;
    return occ;
}


std::string string_lower(std::string str)
{
    int len=str.length()-1;
    int i=-1;
    while (++i<len)
    {
       if (str[i]>=65 && str[i]<=91)
          str[i]+=32;
    }
    return str;
} 
std::string string_upper(std::string str)
{
    int len=str.length()-1;
    int i=-1;
    while (++i<len)
    {
       if (str[i]>=97 && str[i]<=122)
          str[i]-=32;
    }
    return str;
} 


std::string string_repeat(std::string str,double count)
{
    std::string ret="";
    for (int i=0;i<count;i++)
    {
        ret.append(str);
    }
    return ret;
}


std::string string_letters(std::string str)
{
    std::string ret="";
    unsigned int len=str.length()-1;
    for (unsigned int i=0; i<=len; i++)
    {
       if ((str[i]>=65 && str[i]<=91) || (str[i]>=97 && str[i]<=122))
       ret+=str[i];
    }
    return ret;
}


std::string string_digits(std::string str)
{
    std::string ret="";
    int len=str.length()-1;
    for (int i=0; i<=len; i++)
    {
       if (str[i]>=48 && str[i]<=57)
       ret+=str[i];
    }
    return ret;
}


std::string string_lettersdigits(std::string str)
{
    std::string ret="";
    int len=str.length()-1;
    
    for (int i=0; i<=len; i++)
    {
       if ((str[i]>=65 && str[i]<=91) || (str[i]>=97 && str[i]<=122) || (str[i]>=48 && str[i]<=57))
       ret+=str[i];
    }
    return ret;
}
