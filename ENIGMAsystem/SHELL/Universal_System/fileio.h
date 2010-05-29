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

// Simple, untuitive, integer based file I/O

int     file_text_open_read(std::string fname);         // Opens the file with the indicated name for reading. The function returns the id of the file that must be used in the other functions. You can open multiple files at the same time (32 max);                                                   //. Don't forget to close them once you are finished with them.
int     file_text_open_write(std::string fname);        // Opens the indicated file for writing, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
int     file_text_open_append(std::string fname);       // Opens the indicated file for appending data at the end, creating it if it does not exist. The function returns the id of the file that must be used in the other functions.
void    file_text_close(int fileid);            // Closes the file with the given file id.
void    file_text_write_string(int fileid, std::string str); // Writes the string to the file with the given file id.
void    file_text_write_real(int fileid, double x);     // Write the real value to the file with the given file id.
void    file_text_writeln(int fileid);          // Write a newline character to the file.
std::string file_text_read_string(int fileid);  // Reads a string from the file with the given file id and returns this string. A string ends at the end of line.
double  file_text_read_real(int fileid);        // Reads a real value from the file and returns this value.
void    file_text_readln(int fileid);           // Skips the rest of the line in the file and starts at the start of the next line.
bool    file_text_eof(int fileid);              // Returns whether we reached the end of the file.

bool    file_exists(std::string fname);           // Returns whether the file with the given name exists (true);                                                   // or not (false);                                                   //.
void    file_delete(std::string fname);           // Deletes the file with the given name.
void    file_rename(std::string oldname,std::string newname); // Renames the file with name std::string olstd::string dname into std::string newname.
void    file_copy(std::string fname,std::string newname);     // Copies the file fname to the std::string newname.
bool    directory_exists(std::string dname);      // Returns whether the indicated directory does exist. The name must include the full path, not a relative path.
void    directory_create(std::string dname);      // Creates a directory with the given name (including the path towards it);                                                   // if it does not exist. The name must include the full path, not a relative path.
std::string file_find_first(std::string mask,int attr);   // Returns the name of the first file that satisfies the mask and the attributes. If no such file exists, the empty string is returned. The mask can contain a path and can contain wildchars, for example 'C:\temp\*.doc'. The attributes give the additional files you want to see. (So the normal files are always returned when they satisfy the mask.);                                                   // You can add up the following constants to see the type of files you want:

enum {
  fa_readonly  = 1,
  fa_hidden    = 2,
  fa_sysfile   = 4,
  fa_volumeid  = 8,
  fa_directory = 16,
  fa_archive   = 32
};

std::string file_find_next();                 // Returns the name of the next file that satisfies the previously given mask and the attributes. If no such file exists, the empty string is returned.
void file_find_close();                // Must be called after handling all files to free memory.
bool file_attributes(std::string fname,int attr); // Returns whether the file has all the attributes given in attr. Use a combination of the constants indicated above.

std::string filename_name(std::string fname);              // Returns the name part of the indicated file name, with the extension but without the path.
std::string filename_path(std::string fname);              // Returns the path part of the indicated file name, including the final backslash.
std::string filename_dir(std::string fname);               // Returns the directory part of the indicated file name, which normally is the same as the path except for the final backslash.
std::string filename_drive(std::string fname);             // Returns the drive information of the filename.
std::string filename_ext(std::string fname);               // Returns the extension part of the indicated file name, including the leading dot.
void   filename_change_ext(std::string fname,std::string newext); // Returns the indicated file name, with the extension (including the dot);                                                   // changed to the new extension. By using an empty string as the new extension you can remove the extension.

int     file_bin_open(std::string fname,int mode); // Opens the file with the indicated name. The mode indicates what can be done with the file: 0 = reading, 1 = writing, 2 = both reading and writing);                                                   //. When the file does not exist it is created. The function returns the id of the file that must be used in the other functions. You can open multiple files at the same time (32 max);                                                   //. Don't forget to close them once you are finished with them.
bool    file_bin_rewrite(int fileid);         // Reopens the file with the given id for writing. Returns whether successful.
void    file_bin_close(int fileid);           // Closes the file with the given file id.
size_t  file_bin_size(int fileid);            // Returns the size (in bytes);                                                   // of the file with the given file id.
size_t  file_bin_position(int fileid);        // Returns the current position (in bytes; 0 is the first position);                                                   // of the file with the given file id.
void    file_bin_seek(int fileid,int pos);        // Moves the current position of the file to the indicated position. To append to a file move the position to the size of the file before writing.
void    file_bin_write_byte(int fileid,unsigned char byte); // Writes a byte of data to the file with the given file id.
int     file_bin_read_byte(int fileid);       // Reads a byte of data from the file and returns this.

void export_include_file(std::string fname);                   // Exports the included file with the name fname. This must be a string variable, so don't forget the quotes.
void export_include_file_location(std::string fname,std::string location); // Exports the included file with the name fname to the given location. Location must contain the path and the filename.
void discard_include_file(std::string fname);                  // Discard the included file with the name fname, freeing the memory used. This must be a string variable, so don't forget the quotes.

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;


int parameter_count();
std::string parameter_string(int n);

std::string environment_get_variable(std::string name);
