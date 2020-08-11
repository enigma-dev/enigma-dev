/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <string>
#include <vector>

// signed int for a POSIX pid_t
// unsigned long is Win32 DWORD
// process id are always signed
// negative is for error checks
// lmao i'm so fucking OCD haha
typedef unsigned long process_t;

namespace enigma {

// helper needed for insecure functions
bool string_has_whitespace(std::string str);

} // namespace enigma

namespace enigma_insecure {

std::string cmd_from_pid(process_t pid);
process_t process_execute(process_t ind, std::string command);
void process_execute_async(process_t ind, std::string command);
std::string echo(process_t ind, std::string expression);
process_t process_current(process_t ind);
process_t process_previous(process_t ind);
std::string process_output(process_t ind); // FIXME: don't need this
std::string process_evaluate(process_t ind);
void process_clear_pid(process_t ind);
void process_clear_out(process_t ind);
bool pid_exists(process_t pid);
bool pid_kill(process_t pid);

} // namespace enigma_insecure
