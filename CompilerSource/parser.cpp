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

#include <map>
#include <string>

using namespace std;

#include "string.h"
#include "parser_components.h"

std::string __codebuffer, __syntaxbuffer, *__varnames, *__varnamestars, *__varnametags, *__varnamearrays;
std::map<int,std::string> __stringincode,__withs,__globals,__globalstars,__globaltags,__globalarrays,__fnames;
int _errorlast=0, _error_position=-1, __varcount=0,__fcount=0,__withcount=0,__globalcount=0,__globalscrcount=0;

std::string parser_main(std::string code, int argument2=0,int argument3=0)
{
    /* Open the file. */
    __codebuffer="";
    __syntaxbuffer="";
    _errorlast=0;
    _error_position=0;

    /* Load the code into memory.
    *******************************/
    parser_buffer_remove_linecomments(code);
    //The old call is parser_buffer_UNCcode(textfileread);


    /* Tokenize it.
    ****************/
    parser_develop_token_stream();


    /* Doesn't really matter where you do this, so just do it here:
    |* Remove any zeroes from the beginnings of numbers, or surprise! Octal!
    ************************************************************************/

    for (unsigned int i=0;(i=__syntaxbuffer.find("0",i))!=string::npos;i++)
    {
      while ((i<(__codebuffer.length()-1)) && (__codebuffer[i]=='0') && (__codebuffer[i+1]=='0'))
      {
        __codebuffer . erase(i,1);
        __syntaxbuffer.erase(i,1);
      }
      while (__syntaxbuffer[i]=='0') i++;
    }


    //printf("seg11\r\n");  //std::cout << __codebuffer << "\r\n"; system("PAUSE");


    /*Parse all if/with/case/while/until/repeat statements
    ********************************************************/
    unsigned int strpos;
    strpos=1;
    std::string strseg1[4];
    while (strpos<string_length(__codebuffer))
    {
        strseg1[0]=string_copy(__codebuffer,strpos,4);
        strseg1[1]=string_copy(__codebuffer,strpos,6);
        strseg1[2]=string_copy(__codebuffer,strpos,7);
        strseg1[3]=string_copy(__codebuffer,strpos,8);
        if (strseg1[0] =="<if>"
           ||strseg1[1]=="<with>"
           ||strseg1[1]=="<case>"
           ||strseg1[2]=="<while>"
           ||strseg1[2]=="<until>"
           ||strseg1[3]=="<repeat>"
           ||strseg1[3]=="<return>")
        {
           if (string_copy(__codebuffer,strpos,7)=="<until>")
           {
              parser_parse_statement(strpos,argument2);
           }

           strpos=parser_parse_statement(strpos,argument2);
        }
        else if (string_copy(__codebuffer,strpos,5)=="<for>")
        {
           parser_check_for_loop(strpos);
        }
        strpos+=1;
    }


    //printf("seg12\r\n");
    //std::cout << __codebuffer << "\r\n"; system("PAUSE");

    strpos=0;
    int strpos2=0;
    __codebuffer=string_replace_all(__codebuffer,":","");
    __syntaxbuffer=string_replace_all(__syntaxbuffer,":","");
    while (string_pos((char*)"<switch>",__codebuffer))
    {
        strpos=string_pos((char*)"<switch>",__codebuffer);
        if (strpos==0||strpos2>100)
        break;
        parser_parse_statement(strpos,argument2);
        parser_convert_switch(strpos,strpos2);
        strpos2++;
    }


    //printf("seg13\r\n");
    //std::cout << __codebuffer << "\r\n"; system("PAUSE");

    //printf(__syntaxbuffer.c_str()); system("PAUSE");

    parser_add_semicolons(/*argument2,argument3*/);

    //printf(__syntaxbuffer.c_str()); system("PAUSE");

    // Great time to get with() vars, now that the code is legible.
    //*****************************************************************/
    //parser_get_withvars();


    //std::cout << "segment 14 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_recode_repeat();

    //std::cout << "segment 15 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_manage_multidimensional();

    //std::cout << "segment 15.5 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_destroy_who();

    /* Great time to handle with(); right before
    |* closing, now that about everything's done
    ******************************************************************/
    parser_get_withcodes();

    //std::cout << "segment 16 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_infinite_arguments("min(");
    parser_infinite_arguments("max(");

    //std::cout << "segment 17 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_num_arguments("choose(");
    parser_num_arguments("median(");
    parser_num_arguments("mean(");

    //std::cout << "segment 18 \r\n" << __codebuffer << "\r\n" << __syntaxbuffer << "\r\n"; system("PAUSE");

    parser_replace(";","; ");

    //std::cout << "segment 19 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    string_replace_stack("  "," ");

    //std::cout << "segment 20 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_replace(" {","{");
    parser_replace("{ ","{");
    parser_replace("{"," { ");

    //std::cout << "segment 21 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_remove_for_newlines();



    //std::cout << "segment 22 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_replace(" ","\r");
    parser_replace("}","}\r");

    //std::cout << "segment 23 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    //parser_fix_typecasts();

    //std::cout << "segment 24 \r\n" << __codebuffer << "\r\n"; system("PAUSE");

    parser_parse_dots();


    parser_replace("<div>"," div ");
    parser_replace("<==>","==");
    parser_replace("<!=>","!=");
    parser_replace("<=>","=");
    parser_replace("<+=>","+=");
    parser_replace("<*=>","*=");
    parser_replace("</=>","/=");
    parser_replace("<-=>","-=");
    parser_replace("<&&>","&&");
    parser_replace("<||>","||");
    parser_replace("<^^>","^^");
    parser_replace("<&>","&");
    parser_replace("<|>","|");
    parser_replace("<^>","^");
    parser_replace("<!>","!");
    parser_replace("<%>","%");
    parser_replace("<~>","~");
    parser_replace("<$>","0x");
    parser_replace("<&=>","&=");
    parser_replace("<|=>","|=");
    parser_replace("<^=>","^=");
    parser_replace("<if>","if ");
    parser_replace("<for>","for ");
    parser_replace("<while>","while ");
    parser_replace("<switch>","switch ");
    parser_replace("<do>","do ");
    parser_replace("<then>","");
    parser_replace("<break>","break; ");
    parser_replace("<continue>","continue; ");
    parser_replace("<case>","case ");
    parser_replace("<default>","default");
    parser_replace("<else>","else ");
    parser_replace("<exit>","return 0;\r\n");
    parser_replace("<return>","return ");
    parser_replace("/","/(double)");
    parser_replace("/(double)=","/=");
    parser_replace("<g=>"," >=");
    parser_replace("<rs=>"," >>=");
    parser_replace("<rsh>"," >>");
    parser_replace("<gt>"," >");
    parser_replace("<l=>","<=");
    parser_replace("<ls=>","<<=");
    parser_replace("<lsh>","<<");
    parser_replace("<lt>","<");

    parser_replace(" >=",">=");
    parser_replace(" >>=",">>=");
    parser_replace(" >>",">>");
    parser_replace(" >",">");

    parser_replace_until();

    parser_replace("<var>","var ");
    parser_replace("<double>","double ");
    parser_replace("<float>","float ");
    parser_replace("<int>","int ");
    parser_replace("<char>","char ");
    parser_replace("<bool>","bool ");
    parser_replace("<cstring>","std::string ");
    parser_replace("<long>","long ");
    parser_replace("<short>","short ");
    parser_replace("<signed>","signed ");
    parser_replace("<unsigned>","unsigned ");
    parser_replace("<const>","const ");
    parser_replace("<static>","static ");
    parser_replace("<localv>","");
    parser_replace("<new>","new ");
    parser_replace("<delete>","delete []");
    parser_replace("<withiter>","enigma::withiter ");

    //parser_unhandle_new();

    //std::cout << "segment 25 \r\n" << __codebuffer << "\r\n";  system("PAUSE");



    parser_restore_strings();
    parser_concatenate_strings();










    /*std::cout << "Parse complete. Will now print code.\r\n";
    system("PAUSE");
    std::cout<<"\r\n\r\n\r\n\r\n";
    std::cout << string_replace_all(__codebuffer,"\r","\r\n") << "\r\n" << "\r\n\r\n\r\n";
    system("PAUSE");*/

    unsigned int the_last_pos=0;
    while ((the_last_pos=__codebuffer.find("\r",the_last_pos)) != std::string::npos)
    { __codebuffer.replace(the_last_pos,1,"\r\n"); the_last_pos+=2; }

    parser_restore_languages();

    return __codebuffer;
}








std::string file_parse(std::string filename,std::string outname)
{
    FILE* in=fopen(filename.c_str(),"rb");
    if (in==NULL) return "//Failed to parse file. Sorry.";
    
    fseek(in,0,SEEK_END);
    int size=ftell(in);
    fseek(in,0,SEEK_SET);

    char* in2=new char[size+1];
    fread(in2,1,size,in);
    fclose(in);

    in2[size]='\0';
    std::string retval=parser_main(in2);
    delete []in2;

    return retval;
}
