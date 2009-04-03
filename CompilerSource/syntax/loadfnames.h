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

/*ERROR CODES
 * -1 File read error
 * 0  No error
 * 1  Unexpected end of File
 * 2  Program error: String not null terminated
 * 3  Memory error
 * 4  
 * 5  Identifier too long
 */
int load_fnames()
{
  FILE *a=fopen("fnames","rb");
  if (a==NULL) return -1;
  {
    unsigned int clen,flags,argc,temc;
    char name[1024]; char rchr;
    while (!feof(a))
    {
      if (!fread(&clen,sizeof(clen),1,a)) { fclose(a); return 1; }
      if (clen > 1024) { fclose(a); return 5; }
      if (fread(name,sizeof(char),clen,a) != clen) { fclose(a); return 1; }
      if (name[clen-1]!=0) { fclose(a); return 2; }
      if (!fread(&flags,sizeof(flags),1,a)) { fclose(a); return 1; }
      
      varray<externs> *f=&(extarray[name]);
      if (f==NULL) return 3;
      externs *s=&((*f)[f->size]);
      s->flags=flags;
      
      if (!fread(&argc,sizeof(argc),1,a)) { fclose(a); return 1; }
      for (unsigned int i=0;i<argc;i++) if (fread(&rchr,sizeof(char),1,a))
      s->fargs[s->fargs.size]=rchr; else { fclose(a); return 1; }
      if (!fread(&temc,sizeof(temc),1,a)) { fclose(a); return 1; }
      for (unsigned int i=0;i<temc;i++) if (fread(&rchr,sizeof(char),1,a))
      s->targs[s->targs.size]=rchr; else { fclose(a); return 1; }
    }
  }
  fclose(a);
  return 0;
}
