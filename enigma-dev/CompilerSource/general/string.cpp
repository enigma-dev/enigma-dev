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

#include <string>
using namespace std;

string tostring(int val)
{
  char a[12];
  sprintf(a,"%d",val);
  return a;
}


int string_pos(string substr, string str)
{
    unsigned int a=str.find(substr,0);
    if (a==string::npos)
    return 0;
    return a+1;
}
int string_pos(char* substr, string str)
{
    unsigned int a=str.find(substr,0);
    if (a== string::npos)
    return 0;
    return a+1;
}


string string_copy(string str,double index,double count)
{
    string strn=str;
   unsigned int indx=(unsigned int)index-1; if (indx==string::npos) indx=0;

   if (indx>strn.length())
   return "";

   return strn.substr(indx,(int)count);
}



string string_delete(string str,double index,double count)
{
    string strn=str;
   int x=(int) index-1; if (x<0) x=0;
   int c=(int) count;   if (c<0) c=0;
   return strn.erase(x,c);
}




string string_insert(string substr, string str,double index)
{
    string strn=str;
    unsigned int x=(unsigned int) index-1; if (x==string::npos) x=0; if (x>strn.length()) x=strn.length();
    return strn.insert(x,substr);
}



string string_replace(string str, string substr, string newstr)
{
     string strr=str;
    int sublen=substr.length();
    int pos=0;
    if ((pos=strr.find(substr,pos)) != -1)
    {
        strr.erase(pos,sublen);
        strr.insert(pos,newstr);
    }

    return strr;
}





string string_replace_all(string str, string substr, string newstr)
{
     string strr=str;
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


int string_count(char c, char* str)
{
    int occ = 0;
    while (*str) occ+=*str++==c;
    return occ;
}

int string_count(string substr, string str)
{
    int sublen=substr.length();
    int pos=-sublen, occ=0;
    while ((pos=str.find(substr,pos+sublen))>=0) occ++;
    return occ;
}





string arraybounds_as_str(string str)
{
  string ret;
  for (unsigned int i=0;i<str.length();i++)
  {
    if (str[i]=='[') ret+="arrayb";
    else if (str[i]&-2==40) ret+=str[i]==')'?"eparenth":"fparenth";
  }
  return ret;
}






string string_letters(string str)
{
     string ret;
    int len=str.length()-1;
    for (int i=0; i<=len; i++)
    {
       if ((str[i]>=65 && str[i]<=91) || (str[i]>=97 && str[i]<=122))
       ret+=str[i];
    }
    return ret;
}

string string_digits(string str)
{
     string ret;
    int len=str.length()-1;
    for (int i=0; i<=len; i++)
    {
       if (str[i]>=48 && str[i]<=57)
       ret+=str[i];
    }
    return ret;
}

string string_lettersdigits(string str)
{
     string ret;
    int len=str.length()-1;

    for (int i=0; i<=len; i++)
    {
       if ((str[i]>=65 && str[i]<=91) || (str[i]>=97 && str[i]<=122) || (str[i]>=48 && str[i]<=57))
       ret+=str[i];
    }
    return ret;
}
