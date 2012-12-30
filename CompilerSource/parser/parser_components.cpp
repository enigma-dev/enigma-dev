// Delete me
#include <string>
#include <fstream>
#include <general/darray.h>
using std::string;
using std::ofstream;
namespace syncheck {
  string syerr;
  int syntacheck(string) { return 0; }
  void addscr(string) {}
}
string parser_main(string, struct parsed_event*) { return ""; }
int parser_secondary(string& , string&, struct parsed_object *, struct parsed_object *, struct parsed_event *) {}
void print_to_file(string,string,unsigned int&,varray<string>&,int,ofstream&) {}
void parser_init() {}
