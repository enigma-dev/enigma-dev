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

char* __ENIGMA_returnstring;
std::string __ENIGMA_return_stlstring;
std::string __ENIGMA_empty_string="";

/**************************************************************************************\
|*Var is a versatile variant data type meant to allow for dynamically switching between
|*real and string types. The types used are actually double and std::string. Var will
|*behave as either of these very different types, and therefore must be handled with 
|*caution as it will easily confuse the compiler.
|*
|*The class itself is written by Josh @ Dreamland and debugged by Dylan Fortune, with  
|*much needed help from Dave.
\**************************************************************************************/

struct var;

struct __variant
{
       double doubleval;
       std::string stringval;
       bool type;
       
       operator double()
       {
          if (type==0)
          return doubleval;
          else 
          return 0;
       }
       operator std::string()
       {
          if (type==1)
          return stringval;
          else 
          return (std::string) "";
       }
       operator char*()
       {
          if (type==1)
          return (char*) stringval.c_str();
          else 
          return "";
       }
       
       __variant operator=(var& __varname);
       double operator=(double __realvalue)
       {
          type=       0;
          doubleval=  __realvalue;
          stringval=  "";
          
          return __realvalue;
       }
       int operator=(int __realvalue)
       {
          type=       0;
          doubleval=  __realvalue;
          stringval=  "";
          return  __realvalue;
       }
       char* operator=(char* __string)
       {
          type=       1;
          doubleval=  0;
          stringval=  __string;
          return __string;
       }
       std::string operator=(std::string __string)
       {
          type=       1;
          doubleval=  0;
          stringval= __string;
          return __string;
       }
       
       __variant(var& __varname);
       __variant(void)
       {
          type=       0;
          doubleval=  0;
          stringval=  "";
       }
       __variant(double __realvalue)
       {
          type=       0;
          doubleval=  __realvalue;
          stringval=  "";
       }
       __variant(int __realvalue)
       {
          type=       0;
          doubleval=  __realvalue;
          stringval=  "";
       }
       __variant(char* __string)
       {
          type=       1;
          doubleval=  0;
          stringval=  __string;
       }
       __variant(std::string __string)
       {
          type=       1;
          doubleval=  0;
          stringval= __string;
       }
       
       
} __ENIGMA_returnvalue;

struct var
{
    double *doubleval;
    std::string *stringval;
    int length, length2;
    int xindex, yindex;
    bool *type, initd;
    
    var&   operator() (double x,double y) 
    { 
          xindex=(int) x;
          yindex=(int) y;
          return *this;
    }
    var&   operator() (double x) 
    { 
          xindex=(int) x;
          yindex=0;
          return *this;
    }
    operator double()
    {
          int x=(int) xindex+1; if (x<1) x=1; xindex=0;
          int y=(int) yindex+1; if (y<1) y=1; yindex=0;
          int pos=(x-1)*length2+y-1;
          
          if (!initd) return 0;
          
          if (type[pos]==0)
          return doubleval[pos]; 
          else 
          return 0;
    }
    operator std::string()
    {
          int x=(int) xindex+1; if (x<1) x=1; xindex=0;
          int y=(int) yindex+1; if (y<1) y=1; yindex=0;
          int pos=(x-1)*length2+y-1;
          
          if (!initd) return "";
          
          if (type[pos]==1)
          return stringval[pos]; 
          else 
          return "";
    }
    char* c_str()
    {
          int x=(int) xindex+1; if (x<1) x=1; xindex=0;
          int y=(int) yindex+1; if (y<1) y=1; yindex=0;
          int pos=(x-1)*length2+y-1;
          
          if (!initd) return "";
          
          if (type[pos]==1)
          return (char*) stringval[pos].c_str(); 
          else 
          return "";
    }
    
    bool getType(void)
    {
         if (!initd) return 0;
         int x=(int) xindex+1; if (x<1) x=1;
         int y=(int) yindex+1; if (y<1) y=1;
         int pos=(x-1)*length2+y-1;
         return type[pos];
    }
    
    /*******************************************************
    * Handle array being too small in either direction
    ********************************************************/
    void resize(int x,int y)
    {
       if (!initd)
       {
           doubleval=new double[1];
           stringval=new std::string[1];      
           type     =new bool;
           
           length=1; length2=1;
           initd=1;
       }
       
       if (x>length)
       {
          void* tmpvar=doubleval;
          std::string* tmpstr=stringval;
          void* tmptyp=type;
          
          doubleval=(double*) malloc(sizeof(double)*(x+5)*length2);
             memcpy(doubleval,tmpvar,sizeof(double)*length*length2);
          stringval = new std::string[(x+5)*length2];
             for (int strmcpy=0; strmcpy<length*length2; strmcpy++) { stringval[strmcpy]=tmpstr[strmcpy]; }
          type=(bool*) malloc(sizeof(bool)*(x+5)*length2);
             memcpy(type,tmpvar,sizeof(bool)*length*length2);
          
          free(tmptyp);
          free(tmpstr);
          free(tmpvar);
          
          length=x+5;
       }
       if (y>length2)
       {
          double* tempvar=doubleval;
          std::string* tempstr=stringval;
          bool* tmptype=type;
          
          doubleval=(double*) malloc(sizeof(double)*y*length);
          stringval = new std::string[y*length];
          type=(bool*) malloc(sizeof(bool)*y*length);
          
          int newmargin=y-length2;
          
          int m2iyo;
          for (int array_2D_index_x=1; array_2D_index_x<=length; array_2D_index_x++)
          {
            for (int array_2D_index_y=1; array_2D_index_y<=y; array_2D_index_y++)
            { 
              if (array_2D_index_y<=length2)
              {
                doubleval[(array_2D_index_x-1)*y + array_2D_index_y-1]
                         =tempvar[(array_2D_index_x-1)*length2 + array_2D_index_y-1];
                stringval[(array_2D_index_x-1)*y + array_2D_index_y-1]
                         =tempstr[(array_2D_index_x-1)*length2 + array_2D_index_y-1];         
                type     [(array_2D_index_x-1)*y + array_2D_index_y-1]
                         =tmptype[(array_2D_index_x-1)*length2 + array_2D_index_y-1];
              }
              else
              {
                doubleval[(array_2D_index_x-1)*y + array_2D_index_y-1]=0;
                stringval[(array_2D_index_x-1)*y + array_2D_index_y-1]="";
                type     [(array_2D_index_x-1)*y + array_2D_index_y-1]=0;
              }
            }
          }
          
          free(tmptype);
          free(tempstr);
          free(tempvar);
          length2=y;
       }
    }
    
    
    
    /*******************************************************
    * Handle operator=
    ********************************************************/
    var& operator= (var& value)
    {
       //Copy the indexes and make sure they are positive, and begin at one.
       //This is so you can determine the position of a vector in a 1D array.
       //Set the global indexes to zero in case operator() is not called again.
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       //Make sure there is enough room. That is, the indexes must be less than the two length variables.
       resize(x,y);
       
       //get the position in the (one dimensional) array var contains of the two dimensional indexes
       int pos=(x-1)*length2+y-1;
       
       
       //Get the same positions for the var you are setting to.
       int x2=(int) value.xindex+1; if (x2<1) x2=1; value.xindex=0;
       int y2=(int) value.yindex+1; if (y2<1) y2=1; value.yindex=0;
       
       int pos2=(x2-1)*value.length2+y2-1;
       
       //Make sure the var is big enough. Else assume zero or error.
       if (x2>value.length && y2>value.length2)
       {
           #if ASSUMEZERO==1
           show_error("Array index out of bounds.",0);
           #endif
           type[pos]=0;
           doubleval[pos] = 0;
           stringval[pos] = "";
           return *this;
       }
       
       //Set the type to the value's type, set the values.
       type[pos]=value.type[pos2];
       doubleval[pos] = value.doubleval[pos2];
       stringval[pos] = value.stringval[pos2];
       
       return value;
    }
    __variant operator= (__variant value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       type[pos]=value.type;
       doubleval[pos] = value.doubleval;
       stringval[pos] = value.stringval;
       
       return value;
    }
    double operator= (double value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       type[pos]=0;
       doubleval[pos] = value;
       stringval[pos] = "";
       
       return value;
    }
    double operator= (int value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       type[pos]=0;
       doubleval[pos] = value;
       stringval[pos] = "";
       
       return value;
    }
    char* operator= (char* value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       type[pos]=1;
       doubleval[pos] = 0;
       stringval[pos] = value;
       
       return value;
    }
    std::string operator= (std::string value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       type[pos]=1;
       doubleval[pos] = 0;
       stringval[pos] = value;
       
       return value;
    }
    
    /*****************************************************
    * Handle += and incrimenting
    *****************************************************/
    
    var& operator+= (var& value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       
       int x2=(int) value.xindex+1; if (x2<1) x2=1; value.xindex=0;
       int y2=(int) value.yindex+1; if (y2<1) y2=1; value.yindex=0;
       
       int pos2=(x2-1)*value.length2+y2-1;
       
       if (x2>value.length && y2>value.length2)
       {
           #if ASSUMEZERO==1
           show_error("Array index out of bounds.",0);
           #endif
           return *this;
       }
       
       //Set the type to the value's type, set the values.
       if (type[pos]==0)
       {
          if (value.type[pos2]==0)
          {
             doubleval[pos]+=value.doubleval[pos2];
          }
          #if SHOWERRORS
          else
          {
              show_error("Cannot add string to real.",0);
          }
          #endif
       }
       else
       {
          if (value.type[pos2]==1)
          {
             stringval[pos]+=value.stringval[pos2];
          }
          #if SHOWERRORS
          else
          {
              show_error("Cannot add real to string.",0);
          }
          #endif
       }
       
       return value;
    }
    __variant operator+= (__variant value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
            if (value.type==0)
            {
                doubleval[pos]+=value.doubleval;
            }
            #if SHOWERRORS
            else
            {
                show_error("Cannot add string to real.",0);
            }
            #endif
       }
       else
       {
           if (value.type==1)
            {
                stringval[pos]+=value.stringval;
            }
            #if SHOWERRORS
            else
            {
                show_error("Cannot add real to string.",0);
            }
            #endif
       }
       
       return value;
    }
    double operator+= (double value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] += value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot add real to string.",0);
       }
       #endif
       return value;
    }
    double operator+= (int value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] += value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot add real to string.",0);
       }
       #endif
       return value;
    }
    char* operator+= (char* value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==1)
       {
         stringval[pos]+= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot add string to real.",0);
       }
       #endif
       
       return value;
    }
    std::string operator+= (std::string value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==1)
       {
         stringval[pos]+= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot add string to real.",0);
       }
       #endif
       
       return value;
    }
    
    double operator++ (int something)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] += 1;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot increment a string.",0);
       }
       #endif
       return doubleval[pos]-1;
    }
    double operator++ ()
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] += 1;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot increment a string.",0);
       }
       #endif
       return doubleval[pos];
    }
    
    
    
    /*****************************************************
    * Handle -= and decrementing
    *****************************************************/
    
    var& operator-= (var& value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       
       int x2=(int) value.xindex+1; if (x2<1) x2=1; value.xindex=0;
       int y2=(int) value.yindex+1; if (y2<1) y2=1; value.yindex=0;
       
       int pos2=(x2-1)*value.length2+y2-1;
       
       if (x2>value.length && y2>value.length2)
       {
           #if ASSUMEZERO==1
           show_error("Array index out of bounds.",0);
           #endif
           return *this;
       }
       
       //Set the type to the value's type, set the values.
       if (type[pos]==0)
       {
          if (value.type[pos2]==0)
          {
             doubleval[pos]-=value.doubleval[pos2];
          }
          #if SHOWERRORS
          else
          {
              show_error("Cannot subtract string from real.",0);
          }
          #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot subtract from a string.",0);
       }
       #endif
       
       return value;
    }
    __variant operator-= (__variant value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
            if (value.type==0)
            {
                doubleval[pos]-=value.doubleval;
            }
            #if SHOWERRORS
            else
            {
                show_error("Cannot add string to real.",0);
            }
            #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot subtract from a string.",0);
       }
       #endif
       
       return value;
    }
    double operator-= (double value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] -= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot subtract real from string.",0);
       }
       #endif
       return value;
    }
    double operator-= (int value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] -= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot subtract real from string.",0);
       }
       #endif
       return value;
    }
    char* operator-= (char* value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot subtract strings.",0);
       #endif
       
       return (char*) stringval[pos].c_str();
    }
    std::string operator-= (std::string value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot subtract strings.",0);
       #endif
       
       return stringval[pos];
    }
    
    double operator--(int something)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] -= 1;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot decrement a string.",0);
       }
       #endif
       return doubleval[pos]+1;
    }
    double operator-- ()
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] -= 1;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot decrement a string.",0);
       }
       #endif
       return doubleval[pos];
    }
    
    
    /*****************************************************
    * Handle /= and decrementing
    *****************************************************/
    
    var& operator/= (var& value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       
       int x2=(int) value.xindex+1; if (x2<1) x2=1; value.xindex=0;
       int y2=(int) value.yindex+1; if (y2<1) y2=1; value.yindex=0;
       
       int pos2=(x2-1)*value.length2+y2-1;
       
       if (x2>value.length && y2>value.length2)
       {
           #if ASSUMEZERO==1
           show_error("Array index out of bounds.",0);
           #endif
           return *this;
       }
       
       if (type[pos]==0)
       {
          if (value.type[pos2]==0)
          {
             doubleval[pos]/=value.doubleval[pos2];
          }
          #if SHOWERRORS
          else
          {
              show_error("Cannot divide real by string.",0);
          }
          #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot divide a string.",0);
       }
       #endif
       
       return value;
    }
    __variant operator/= (__variant value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
            if (value.type==0)
            {
                doubleval[pos]/=value.doubleval;
            }
            #if SHOWERRORS
            else
            {
                show_error("Cannot divide real by string.",0);
            }
            #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot divide a string.",0);
       }
       #endif
       
       return value;
    }
    double operator/= (double value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] /= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot divide a string.",0);
       }
       #endif
       return value;
    }
    double operator/= (int value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] /= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot divide a string.",0);
       }
       #endif
       return value;
    }
    char* operator/= (char* value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot divide by a string.",0);
       #endif
       
       return (char*) stringval[pos].c_str();
    }
    std::string operator/= (std::string value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot divide by a string.",0);
       #endif
       
       return stringval[pos];
    }
    
    
    
    
    
    
    /*****************************************************
    * Handle *= and decrementing
    *****************************************************/
    
    var& operator*= (var& value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       
       int x2=(int) value.xindex+1; if (x2<1) x2=1; value.xindex=0;
       int y2=(int) value.yindex+1; if (y2<1) y2=1; value.yindex=0;
       
       int pos2=(x2-1)*value.length2+y2-1;
       
       if (x2>value.length && y2>value.length2)
       {
           #if ASSUMEZERO==1
           show_error("Array index out of bounds.",0);
           #endif
           return *this;
       }
       
       if (type[pos]==0)
       {
          if (value.type[pos2]==0)
          {
             doubleval[pos]*=value.doubleval[pos2];
          }
          #if SHOWERRORS
          else
          {
              show_error("Cannot multiply real by string.",0);
          }
          #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot multiply a string.",0);
       }
       #endif
       
       return value;
    }
    __variant operator*= (__variant value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
            if (value.type==0)
            {
                doubleval[pos]*=value.doubleval;
            }
            #if SHOWERRORS
            else
            {
                show_error("Cannot multiply real by string.",0);
            }
            #endif
       }
       #if SHOWERRORS
       else
       {
          show_error("Cannot multiply a string.",0);
       }
       #endif
       
       return value;
    }
    double operator*= (double value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] *= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot multiply a string.",0);
       }
       #endif
       return value;
    }
    double operator*= (int value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       
       resize(x,y);
       
       int pos=(x-1)*length2+y-1;
       
       if (type[pos]==0)
       {
         doubleval[pos] *= value;
       }
       #if SHOWERRORS
       else
       {
         show_error("Cannot multiply a string.",0);
       }
       #endif
       return value;
    }
    char* operator*= (char* value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot multiply by a string.",0);
       #endif
       
       return (char*) stringval[pos].c_str();
    }
    std::string operator*= (std::string value)
    {
       int x=(int) xindex+1; if (x<1) x=1; xindex=0;
       int y=(int) yindex+1; if (y<1) y=1; yindex=0;
       int pos=(x-1)*length2+y-1;
       #if SHOWERRORS
       show_error("Cannot multiply by a string.",0);
       #endif
       
       return stringval[pos];
    }
    
    
    
    var(void)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=0;
    }
    var(__variant x)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=1;
       
       resize(0,0);
       type[0]=x.type;
       doubleval[0]=x.doubleval;
       stringval[0]=x.stringval;
    }
    var(double x)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=1;
       
       resize(0,0);
       type[0]=0;
       doubleval[0]=x;
       stringval[0]="";
    }
    var(int x)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=1;
       
       resize(0,0);
       type[0]=0;
       doubleval[0]=x;
       stringval[0]="";
    }
    var(std::string x)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=1;
       
       resize(0,0);
       type[0]=1;
       doubleval[0]=0;
       stringval[0]=x;
    }
    var(char* x)
    {
       length=1;
       length2=1;
       xindex=0;
       yindex=0;
       initd=1;
       
       resize(0,0);
       type[0]=1;
       doubleval[0]=0;
       stringval[0]=x;
    }
} __ENIGMA_returnvar;

__variant::__variant(var& varn)
{
   type=       varn.getType();
   if (type==0) {
   doubleval=  (double)varn;
   stringval=  ""; }
   else {
   doubleval=  0;
   stringval=  (std::string)varn; }
}                                                    
__variant __variant::operator=(var& __varname)
       {
          int x=(int) __varname.xindex+1; if (x<1) x=1; __varname.xindex=0;
          int y=(int) __varname.yindex+1; if (y<1) y=1; __varname.yindex=0;
          int pos=(x-1)*__varname.length2+y-1;
          
          if (!__varname.initd) return 0;
          
          type=       __varname.type[pos];
          doubleval=  __varname.doubleval[pos];
          stringval=  __varname.stringval[pos];
          
          return (__variant)__varname;
       }
