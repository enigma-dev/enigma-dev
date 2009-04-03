/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Adam Domurad                                             **
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
            //uses: i, ii, aX1, aX2, bitMod, byteRef
            for (i=(unsigned int)(((unsigned int)aX1>>3));(unsigned int)i<=(unsigned int)((unsigned int)aX2>>3);i++)
                    {
                        byteRef=i+(int)ii*(aColl.byteDim);
                        if (i==(unsigned int)((unsigned int)aX1>>3))
                        {          
                           bitMod=255>>(((-(unsigned int)aX2-1)&7)*((unsigned int)(i*8+8)>(unsigned int)aX2));
                           bitMod&=255<<(((unsigned int)aX1)&7);
                           switch (Blend)
                           {
                                  case bm_normal:
                                  aColl.cMask[byteRef]=(255&bitMod);break;
                                  case bm_add:
                                  aColl.cMask[byteRef]=(255&bitMod);break;//no difference in a rectangle
                                  case bm_subtract:
                                  aColl.cMask[byteRef]&=~(255&bitMod);break;
                                  case bm_negate:
                                  aColl.cMask[byteRef]^=(255)&bitMod;break;
                           }     
                        }
                        else
                        {
                        if (i==(unsigned int)((unsigned int)aX2>>3))
                           {
                              bitMod=255>>((-(int)aX2-1)&7);
                              switch (Blend)
                              {
                                  case bm_normal:
                                  aColl.cMask[byteRef]=(255&bitMod);break;
                                  case bm_add:
                                  aColl.cMask[byteRef]=(255&bitMod);break;//no difference in a rectangle
                                  case bm_subtract:
                                  aColl.cMask[byteRef]&=~(255&bitMod);break;
                                  case bm_negate:
                                  aColl.cMask[byteRef]^=(255)&bitMod;break;
                               }     
                           }
                        else  
                            {
                               switch (Blend)
                               {
                                  case bm_normal:
                                  aColl.cMask[byteRef]=(255);break;
                                  case bm_add:
                                  aColl.cMask[byteRef]=(255);break;//no difference in a rectangle
                                  case bm_subtract:
                                  aColl.cMask[byteRef]&=(0);break;
                                  case bm_negate:
                                  aColl.cMask[byteRef]^=(255);break;
                               }
                            }
                        }
                    }
