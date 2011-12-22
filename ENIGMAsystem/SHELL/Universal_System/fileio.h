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
**  with this code. If not, see <http:
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/* Simple, intuitive, integer based file I/O */

int     file_text_open_read(string fname);
int     file_text_open_write(string fname);
int     file_text_open_append(string fname);
void    file_text_close(int fileid);
void    file_text_write_string(int fileid, string str);
void    file_text_write_real(int fileid, double x);
void    file_text_writeln(int fileid);
string file_text_read_string(int fileid);
double  file_text_read_real(int fileid);
void    file_text_readln(int fileid);
bool    file_text_eof(int fileid);
bool file_text_eoln(int fileid);

int     file_bin_open(string fname,int mode);
bool    file_bin_rewrite(int fileid);
void    file_bin_close(int fileid);
size_t  file_bin_size(int fileid);
size_t  file_bin_position(int fileid);
void    file_bin_seek(int fileid,size_t pos);
void    file_bin_write_byte(int fileid,unsigned char byte);
int     file_bin_read_byte(int fileid);
