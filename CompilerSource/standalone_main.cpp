/** Copyright (C) 2008-2012 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "darray.h"


#include "syntax/syncheck.h"
  #include "parser/parser.h"
  #include "parser/object_storage.h"
  #include "compiler/compile_common.h"
  // #include "syntax/checkfile.h"


int m_prog_loop_cfp();

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

extern const char* establish_bearings(const char* compiler);
extern void print_definition(string);


extern  int ext_count;

#include "settings-parse/crawler.h"
#include "filesystem/file_find.h"

#include <Storage/definition.h>
#include <languages/lang_CPP.h>

extern int cfile_parse_main();
extern jdi::definition *enigma_type__var, *enigma_type__variant, *enigma_type__varargs;

inline string fc(const char* fn)
{
    FILE *pt = fopen(fn,"rb");
    if (pt==NULL) return "";
    else {
      fseek(pt,0,SEEK_END);
      size_t sz = ftell(pt);
      fseek(pt,0,SEEK_SET);

      char a[sz+1];
      sz = fread(a,1,sz,pt);
      fclose(pt);

      a[sz] = 0;
      return a;
    }
}

inline void povers(string n)
{
  jdi::definition *d = main_context->get_global()->look_up(n);
  if (d) cout << d->toString();
  else cout << n << ": Not found" << endl;
}

const char* libInit(struct EnigmaCallbacks* ecs);
void        libFree();
syntax_error *definitionsModified(const char*,const char*);

#include "OS_Switchboard.h"
#include "general/bettersystem.h"
#include <System/builtins.h>

void do_cli(jdi::context &ct);
syntax_error *syntaxCheck(int script_count, const char* *script_names, const char* code);
int compileEGMf(deprecated::JavaStruct::EnigmaStruct *es, const char* exe_filename, int mode);
int main(int argc, char* argv[])
{
  puts("Attempting to run");
  /*e_execp("gcc -E -x c++ -v blank.txt","");*/
  //e_exec("gcc -E -x c++ -v blank.txt");

  const char *ic = libInit(NULL);
  if (ic) {
    cout << ic << endl;
    return 0;
  }

  for (int i = 0; i < 1; ++i)
  current_language->definitionsModified(NULL, ((string) "%e-yaml\n"
    "---\n"
    "target-windowing: " +  (CURRENT_PLATFORM_ID==OS_WINDOWS ? "Win32" : CURRENT_PLATFORM_ID==OS_MACOSX ? "Cocoa" : "xlib")  + "\n"
    /* "target-graphics: OpenGL\n"
    "target-audio: OpenAL\n"
    "target-collision: BBox\n"
    /* Straight from LGM on Linux */
    "treat-literals-as: 0\n"
    "sample-lots-of-radios: 0\n"
    "inherit-equivalence-from: 0\n"
    "sample-checkbox: on\n"
    "sample-edit: DEADBEEF\n"
    "sample-combobox: 0\n"
    "inherit-strings-from: 0\n"
    "inherit-escapes-from: 0\n"
    "inherit-increment-from: 0\n"
    " \n"
    "target-audio: OpenAL\n"
    "target-windowing: xlib\n"
    "target-compiler: gcc\n"
    "target-graphics: OpenGL\n"
    "target-widget: None\n"
    "target-collision: BBox\n"
    "target-networking: None\n"
    /* */
    ).c_str());
  //mainr(argc,argv);

  string in2 = fc("./CompilerSource/test_gml.h");
  cout << "Check file:" << endl << in2 << endl;
  const char *name = "my_script";
  syntax_error* a = syntaxCheck(1,&name,in2.c_str());
  printf("Line %d, position %d (absolute index %d): %s\r\n",a->line,a->position,a->absolute_index,a->err_str);
  {
    jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());
    current_language->quickmember_integer("sprite0");
  }

  EnigmaStruct es;

  GmObject obj = {0,0,0,0,0,0,0,0,0,0,0};
  MainEvent mev = {0,0,0};
  Event ev = {0,0};
  ev.code = "//NOTICE: test code!!!";
  ev.id = 10; // EV_MOUSE_ENTER

  mev.id = 6; // EV_MOUSE
  mev.eventCount = 1;
  mev.events = &ev;

  obj.mainEventCount = 1;
  obj.mainEvents = &mev;
  obj.name = "obj_boobs";
  obj.spriteId = obj.parentId = obj.maskId = -1;

  es.gmObjects = &obj;
  es.gmObjectCount = 1;

  /*SubImage subimages = { 32, 32, new char[32*32*4], 32*32*4 };
  Sprite sprite = {"spr_0", 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 32, 32, &subimages, 1, NULL, 0};
  Sprite sprites[2] = { sprite, sprite };
  es.spriteCount = 2; es.sprites = sprites;
  es.filename = "coolio.gmk";
  es.fileVersion = 800;*/

  do_cli(*main_context);

  // current_language->compile(&es, "/tmp/coolio.exe", 0);

  libFree();

  //getchar();
  return 0;
}


#include <cstring>
#include <System/lex_cpp.h>
#include <General/parse_basics.h>

#ifdef __linux__d
#include <ncurses.h>
#else
static char getch() {
  int c = fgetc(stdin);
  int ignore = c;
  while (ignore != '\n')
    ignore = fgetc(stdin);
  return c;
}
#endif

using namespace jdip;
void do_cli(context &ct) {
  char c = ' ';
  macro_map undamageable = ct.get_macros();
  while (c != 'q' and c != '\n') { switch (c) {
    case 'd': {
        bool justflags; justflags = false;
        if (false) { case 'f': justflags = true; }
        cout << "Enter the item to define:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        size_t start, e = 0;
        while (is_useless(buf[e]) or buf[e] == ':') ++e;
        definition* def = ct.get_global();
        while (buf[e] and def) {
          if (!is_letter(buf[e])) { cout << "Unexpected symbol '" << buf[e] << "'" << endl; break; }
          start = e;
          while (is_letterd(buf[++e]));
          if (def->flags & DEF_SCOPE) {
            string name(buf+start, e-start);
            definition_scope::defiter it = ((definition_scope*)def)->members.find(name);
            if (it == ((definition_scope*)def)->members.end()) {
              cout << "No `" << name << "' found in scope `" << def->name << "'" << endl;
              def = NULL;
              break;
            }
            def = it->second;
          }
          else {
            cout << "Definition `" << def->name << "' is not a scope" << endl;
            def = NULL;
            break;
          }
          while (is_useless(buf[e]) or buf[e] == ':') ++e;
        }
        if (def and e) {
          if (justflags) {
            map<int, string> flagnames;
            DEF_FLAGS d = DEF_TYPENAME;
            switch (d) {
              case DEF_TYPENAME: flagnames[DEF_TYPENAME] = "DEF_TYPENAME";
              case DEF_NAMESPACE: flagnames[DEF_NAMESPACE] = "DEF_NAMESPACE";
              case DEF_CLASS: flagnames[DEF_CLASS] = "DEF_CLASS";
              case DEF_ENUM: flagnames[DEF_ENUM] = "DEF_ENUM";
              case DEF_UNION: flagnames[DEF_UNION] = "DEF_UNION";
              case DEF_SCOPE: flagnames[DEF_SCOPE] = "DEF_SCOPE";
              case DEF_TYPED: flagnames[DEF_TYPED] = "DEF_TYPED";
              case DEF_FUNCTION: flagnames[DEF_FUNCTION] = "DEF_FUNCTION";
              case DEF_VALUED: flagnames[DEF_VALUED] = "DEF_VALUED";
              case DEF_DEFAULTED: flagnames[DEF_DEFAULTED] = "DEF_DEFAULTED";
              case DEF_EXTERN: flagnames[DEF_EXTERN] = "DEF_EXTERN";
              case DEF_TEMPLATE: flagnames[DEF_TEMPLATE] = "DEF_TEMPLATE";
              case DEF_TEMPPARAM: flagnames[DEF_TEMPPARAM] = "DEF_TEMPPARAM";
              case DEF_HYPOTHETICAL: flagnames[DEF_HYPOTHETICAL] = "DEF_HYPOTHETICAL";
              case DEF_TEMPSCOPE: flagnames[DEF_TEMPSCOPE] = "DEF_TEMPSCOPE";
              case DEF_PRIVATE: flagnames[DEF_PRIVATE] = "DEF_PRIVATE";
              case DEF_PROTECTED: flagnames[DEF_PROTECTED] = "DEF_PROTECTED";
              case DEF_INCOMPLETE: flagnames[DEF_INCOMPLETE] = "DEF_INCOMPLETE";
              case DEF_ATOMIC: flagnames[DEF_ATOMIC] = "DEF_ATOMIC";
              default: ;
            }
            bool hadone = false;
            for (int i = 1; i < (1 << 30); i <<= 1)
              if (def->flags & i)
                cout << (hadone? " | " : "  ") << flagnames[i], hadone = true;
            cout << endl;
          }
          else
            cout << def->toString() << endl;
        }
      } break;

    case 'e': {
        bool eval, coerce, render, show;
        eval = true,
        coerce = false;
        render = false;
        show = false;
        if (false) { case 'c': eval = false; coerce = true;  render = false; show = false;
        if (false) { case 'r': eval = false; coerce = false; render = true;  show = false;
        if (false) { case 's': eval = false; coerce = false; render = false; show = true;
        } } }
        cout << "Enter the expression to evaluate:" << endl << ">> " << flush;
        char buf[4096]; cin.getline(buf, 4096);
        llreader llr(buf, strlen(buf));
        AST a;
        lexer_cpp c_lex(llr, undamageable, "User expression");
        token_t dummy = c_lex.get_token_in_scope(ct.get_global());
        if (!a.parse_expression(dummy, &c_lex, ct.get_global(), precedence::all, def_error_handler)) {
          if (render) {
            cout << "Filename to render to:" << endl;
            cin.getline(buf, 4096);
            a.writeSVG(buf);
          }
          if (eval) {
            value v = a.eval();
            cout << "Value returned: " << v.toString() << endl;
          }
          if (coerce) {
            full_type t = a.coerce();
            cout << "Type of expression: " << t.toString() << endl;
          }
          if (show) {
            a.writeSVG("/tmp/anus.svg");
            if (system("xdg-open /tmp/anus.svg"))
              cout << "Failed to open." << endl;
          }
        } else cout << "Bailing." << endl;
      } break;

    case 'h':
      cout <<
      "'c' Coerce an expression, printing its type"
      "'d' Define a symbol, printing it recursively\n"
      "'e' Evaluate an expression, printing its result\n"
      "'f' Print flags for a given definition\n"
      "'h' Print this help information\n"
      "'m' Define a macro, printing a breakdown of its definition\n"
      "'r' Render an AST representing an expression\n"
      "'s' Render an AST representing an expression and show it\n"
      "'q' Quit this interface\n";
    break;


    default: cout << "Unrecognized command. Empty command or 'q' to quit." << endl << endl; break;
    case ' ': cout << "Commands are single-letter; 'h' for help." << endl << "Follow commands with ENTER on non-unix." << endl;
  }
  cout << "> " << flush;
  c = getch();
  }
  cout << endl << "Goodbye" << endl;
}
