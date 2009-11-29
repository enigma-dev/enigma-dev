/**********************************************************************************************\
**                                                                                            **
**  Copyright (C) 2008 Dylan Fortune                                                          **
**                                                                                            **
**  This file is a part of the ENIGMA Development Environment.                                **
**                                                                                            **
**  ENIGMA is free software: you can redistribute it and/or modify it under the               **
**  terms of the GNU General Public License as published by the Free Software                 **
**  Foundation, version 3 of the license or any later version.                                **
**                                                                                            **
**  This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; without  **
**  even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the **
**  GNU General Public License for more details.                                              **
**                                                                                            **
**  You should have recieved a copy of the GNU General Public License along with this code.   **
**  If not, see <http://www.gnu.org/licenses/>                                                **
**                                                                                            **
**  ENIGMA is an environment designed to create games and other programs with a high-level,   **
**  compilable language. Developers of ENIGMA or anything associated with ENIGMA are in no    **
**  way responsible for its users or applications created by its users, or damages caused     **
**  the environment or programs made in the environment.                                      **
**                                                                                            **
\**********************************************************************************************/

/*******************
* Words of wisdom: *
***********************************************************
* EDITING THIS FILE CAN CAUSE SEVERE INJURY OR EVEN DEATH *
* ...to any games produced with it.                       *
*                                                         *
* If you do not know what you are doing, DO NOT EDIT THIS *
* FILE.                                                   * 
**********************************************************/


//all this stuff works like a charm
class imap
{
  private:
     object** memberlist;                       //The shit list
     object** kill_list;                        //The shit hit list
     
     int first, last, count, killcount, *memberids;
     
     void kill_list_add(object* what)
     {
      if (killcount)
      {
       object** oldkills=kill_list;
       kill_list=new object*[killcount+1];
       for (int i=0;i<killcount;i++) kill_list[i]=oldkills[i];
       kill_list[killcount]=what;
       killcount++;
       delete[] oldkills;
      }
      else 
      {
       kill_list=new object*[1];
       kill_list[0]=what;
       killcount=1;
      }
     }
     
  public:
     imap (void) { count=0; killcount=0; }                    //constructor, make sure it's marked empty
     
     object*& operator[] (unsigned int id)       //Do all the fancy  allocation when called with []
     {
       if (count==0 || first > id || last < id)  //if its OOB
       {
         object** oldlist=memberlist;            //Save the old data
         int* oldids=memberids;
         memberlist=new object*[count+1];        //Allocate more space
         memberids=new int[count+1];
         for (int i=0; i<count; i++)             //Copy over pointers
         {
           memberids[i]=oldids[i];
           memberlist[i]=oldlist[i];
         }
         
         if (count==0) { first=id; last=id; }    //if it's empty, set the first and last to this id, since it's the only one 
         else if (id<first) first=id;            //Make sure the bounds are correct now that we've added something
         else if (id>last) last=id;
         
         delete[] oldids;                        //Free the old data now that we've picked through it
         delete[] oldlist;
         
         count++;                                //indicate one more element successfully added
         memberids[count-1]=id;                  //index its id
         
       }
       for (int i=0; i<count; i++)               //loop through to find element by id
        if (memberids[i]==id)
        return memberlist[i];
       return memberlist[0];
     }
     
     object* operator() (int index)              //Absolute index (not id based)
     {
       if (count)                                //make sure there's actually something in the damn map
       return memberlist[index];
       return NULL;                              //may as well
     }
     
     void erase(int id)                          //erase element by id
     {
        int* oldids=memberids;                   //make a copy of the old lists
        object** oldlist=memberlist;
        memberids=new int [count-1];             //allocate less memory
        memberlist=new object* [count-1]; 
        int i=0;
        for (i=0; i<count; i++)           //copy the first half of the list (before id in question)
        {
          if (oldids[i]==id) break;
          memberlist[i]=oldlist[i]; 
          memberids[i]=oldids[i];
        }
        
        //Don't kill the old one right away...
        kill_list_add(oldlist[i]);
        i++;
        
        while (i<count)                //copy the rest of the list
        {
          memberlist[i-1]=oldlist[i]; 
          memberids[i-1]=oldids[i];
          i++;
        }
        count--;                      //show that the count is one less
        delete[] oldids;              //clear the old lists now that we're done copying
        delete[] oldlist;
      }
      int begin() { return 0; }       ///just for compatibility
      int end() { return count; }     //bleh
      int size() { return count; }    //also bleh
      int find(int id)    //loop through to find element
      {
        int i;
        for (i=0;i<count;i++)
        if (memberids[i]==id) break;   //if you found it, exit, otherwise you'll be returning count, which is imap::end() 
        return i;
      }
      
      void cleanup()
      {
        if (killcount>0)
        {
           for (int i=0;i<killcount;i++) delete kill_list[i];
           killcount=0;
           delete[] kill_list;
        }
      }     
      
      ~imap()
      {
        for (int i=0;i<count;i++) delete memberlist[i];
        delete[] memberids;
        delete[] memberlist;
        delete[] kill_list;
      }
} instance_list;

struct iiterator  //placeholder iterator to tape compatibility
{
   int value;                   //It's really just an integer
   object* second;                     //only with a "second"
   
   int operator= (int a)                      //Set to zero, mostly
   { value=a; second=instance_list(a); }
   
   int operator++ (int a)                     //increment
   { value++; second=instance_list(value); }
   
   bool operator< (int val)                   //Duh
   { return value<val; }
   
   bool operator== (int val)                  //...still duh
   { return value==val; }
};
