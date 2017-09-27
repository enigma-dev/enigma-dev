/** Copyright (C) 2009-2013 Josh Ventura
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

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <libgen.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <fts.h>
#include <stdlib.h>
#include <limits.h>

#include <string>
#include "PFfilemanip.h"
using namespace std;

/* UNIX-ready port of file manipulation */

struct dupslash
{
    bool operator()(char a, char b) const
    {
        return (a == '/' && b == '/');
    }
};

namespace enigma_user
{

int file_exists(std::string fname)
{
    std::string str(fname);

    struct stat sb;
    return (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISREG(sb.st_mode));
}

int file_delete(std::string fname)
{
    std::string str(fname);

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISREG(sb.st_mode))
    {
        return (remove((char *)str.c_str()) == 0);
    }

    return 0;
}

int file_rename(std::string oldname,std::string newname)
{
    std::string str1(oldname);
    std::string str2(newname);

    str1.erase(unique(str1.begin(),str1.end(),dupslash()),str1.end());
    str2.erase(unique(str2.begin(),str2.end(),dupslash()),str2.end());

    size_t slash1;
    size_t slash2;
    std::string dir1(str1);
    std::string dir2(str2);

    if (dir1.find('/') != std::string::npos)
        slash1 = dir1.find_last_of("/");
    else
    {
        dir1 = std::string("");
        slash1 = 0;
    }

    if (dir2.find('/') != std::string::npos)
        slash2 = dir2.find_last_of("/");
    else
    {
        dir2 = std::string("");
        slash2 = 0;
    }

    int proceed = (dir1.substr(0,slash1) != dir2.substr(0,slash2));

    std::string prestr1 = str1;
    std::string prestr2 = str2;

    std::string pardir1(dirname((char *)str1.c_str()));
    if (!pardir1.empty())
    {
        while (*pardir1.rbegin() == '/')
        {
            pardir1.erase(pardir1.size()-1);
        }
    }
    struct stat pexist1;
    if (stat((char *)pardir1.c_str(),&pexist1) == 0 &&
        S_ISDIR(pexist1.st_mode))
    {
        char actpath1[PATH_MAX+1];
        char *ptr1 = realpath((char *)pardir1.c_str(),actpath1);
        if (ptr1 != NULL)
        {
            std::string pardir2(dirname((char *)str2.c_str()));
            if (!pardir2.empty())
            {
                while (*pardir2.rbegin() == '/')
                {
                    pardir2.erase(pardir2.size()-1);
                }
            }
            struct stat pexist2;
            if (stat((char *)pardir2.c_str(),&pexist2) == 0 &&
                S_ISDIR(pexist2.st_mode))
            {
                char actpath2[PATH_MAX+1];
                char *ptr2 = realpath((char *)pardir2.c_str(),actpath2);
                if (ptr2 != NULL)
                {
                    proceed = (std::string(actpath1) != std::string(actpath2));
                }
            }
        }
    }

    str1 = prestr1;
    str2 = prestr2;

    if (proceed || basename((char *)str1.c_str()) != basename((char *)str2.c_str()))
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISREG(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            return (rename((char *)str1.c_str(),(char *)str2.c_str()) == 0);
        }
    }

    return 0;
}

int file_copy(std::string fname,std::string newname)
{
    std::string str1(fname);
    std::string str2(newname);

    str1.erase(unique(str1.begin(),str1.end(),dupslash()),str1.end());
    str2.erase(unique(str2.begin(),str2.end(),dupslash()),str2.end());

    size_t slash1;
    size_t slash2;
    std::string dir1(str1);
    std::string dir2(str2);

    if (dir1.find('/') != std::string::npos)
        slash1 = dir1.find_last_of("/");
    else
    {
        dir1 = std::string("");
        slash1 = 0;
    }

    if (dir2.find('/') != std::string::npos)
        slash2 = dir2.find_last_of("/");
    else
    {
        dir2 = std::string("");
        slash2 = 0;
    }

    int proceed = (dir1.substr(0,slash1) != dir2.substr(0,slash2));

    std::string prestr1 = str1;
    std::string prestr2 = str2;

    std::string pardir1(dirname((char *)str1.c_str()));
    if (!pardir1.empty())
    {
        while (*pardir1.rbegin() == '/')
        {
            pardir1.erase(pardir1.size()-1);
        }
    }
    struct stat pexist1;
    if (stat((char *)pardir1.c_str(),&pexist1) == 0 &&
        S_ISDIR(pexist1.st_mode))
    {
        char actpath1[PATH_MAX+1];
        char *ptr1 = realpath((char *)pardir1.c_str(),actpath1);
        if (ptr1 != NULL)
        {
            std::string pardir2(dirname((char *)str2.c_str()));
            if (!pardir2.empty())
            {
                while (*pardir2.rbegin() == '/')
                {
                    pardir2.erase(pardir2.size()-1);
                }
            }
            struct stat pexist2;
            if (stat((char *)pardir2.c_str(),&pexist2) == 0 &&
                S_ISDIR(pexist2.st_mode))
            {
                char actpath2[PATH_MAX+1];
                char *ptr2 = realpath((char *)pardir2.c_str(),actpath2);
                if (ptr2 != NULL)
                {
                    proceed = (std::string(actpath1) != std::string(actpath2));
                }
            }
        }
    }

    str1 = prestr1;
    str2 = prestr2;

    if (proceed || basename((char *)str1.c_str()) != basename((char *)str2.c_str()))
    {
        struct stat sb1;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISREG(sb1.st_mode))
        {
            double ret = 0;
            size_t sz1 = sb1.st_size;

            std::ifstream srce((char *)str1.c_str(),std::ios::binary);
            ret = (srce.tellg() >= 0);

            std::ofstream dest((char *)str2.c_str(),std::ios::binary);
            ret = (dest.tellp() >= 0);

            dest << srce.rdbuf();
            ret = (srce.tellg()-dest.tellp() == 0);

            struct stat sb2;
            if (stat((char *)str2.c_str(),&sb2) == 0 &&
                S_ISREG(sb2.st_mode))
            {
                size_t sz2 = sb2.st_size;
                return ((ret == 1) ||
                    (sz1 == 0 && sz2 == 0));
            }
        }
    }

    return 0;
}

int directory_exists(std::string dname)
{
    std::string str(dname);

    if (!str.empty())
    {
        while (*str.rbegin() == '/')
        {
            str.erase(str.size()-1);
        }
    }

    struct stat sb;
    return (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISDIR(sb.st_mode));
}

int directory_create(std::string dname)
{
    std::string str(dname);

    if (!str.empty())
    {
        while (*str.rbegin() == '/')
        {
            str.erase(str.size()-1);
        }
    }

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) != 0)
    {
        return (mkdir((char *)str.c_str(),0777) == 0);
    }

    return 0;
}

int directory_destroy(std::string dname)
{
    std::string str(dname);

    if (!str.empty())
    {
        while (*str.rbegin() == '/')
        {
            str.erase(str.size()-1);
        }
    }

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISDIR(sb.st_mode))
    {
        FTS *ftsp = NULL;
        FTSENT *curr;

        char *files[] = {(char *)str.c_str(),NULL};
        ftsp = fts_open(files,FTS_NOCHDIR|FTS_PHYSICAL|FTS_XDEV,NULL);
        if (!ftsp)
        {
            return 0;
            goto finish;
        }

        while ((curr = fts_read(ftsp)))
        {
            switch (curr->fts_info)
            {
                case FTS_NS:
                case FTS_DNR:
                case FTS_ERR:
                    return 0;
                    break;

                case FTS_DC:
                case FTS_DOT:
                case FTS_NSOK:
                    break;

                case FTS_D:
                    break;

                case FTS_DP:
                case FTS_F:
                case FTS_SL:
                case FTS_SLNONE:
                case FTS_DEFAULT:
                    if (remove(curr->fts_accpath) < 0)
                    {
                        return 0;
                    }
                    break;
            }
        }

        finish:
            if (ftsp)
            {
                fts_close(ftsp);
            }

        return 1;
    }

    return 0;
}

int directory_rename(std::string oldname,std::string newname)
{
    std::string str1(oldname);
    std::string str2(newname);

    str1.erase(unique(str1.begin(),str1.end(),dupslash()),str1.end());
    str2.erase(unique(str2.begin(),str2.end(),dupslash()),str2.end());

    if (!str1.empty())
    {
        while (*str1.rbegin() == '/')
        {
            str1.erase(str1.size()-1);
        }
    }
    if (!str2.empty())
    {
        while (*str2.rbegin() == '/')
        {
            str2.erase(str2.size()-1);
        }
    }

    size_t slash1;
    size_t slash2;
    std::string dir1(str1);
    std::string dir2(str2);

    if (dir1.find('/') != std::string::npos)
        slash1 = dir1.find_last_of("/");
    else
    {
        dir1 = std::string("");
        slash1 = 0;
    }

    if (dir2.find('/') != std::string::npos)
        slash2 = dir2.find_last_of("/");
    else
    {
        dir2 = std::string("");
        slash2 = 0;
    }

    int proceed1 = (str1 != str2.substr(0,str1.length()));
    int proceed2 = (dir1.substr(0,slash1) == dir2.substr(0,slash2));

    std::string prestr1 = str1;
    std::string prestr2 = str2;
    std::string poststr1 = str1;
    std::string poststr2 = str2;

    std::string pardir1(dirname((char *)str1.c_str()));
    if (!pardir1.empty())
    {
        while (*pardir1.rbegin() == '/')
        {
            pardir1.erase(pardir1.size()-1);
        }
    }
    struct stat pexist1;
    if (stat((char *)pardir1.c_str(),&pexist1) == 0 &&
        S_ISDIR(pexist1.st_mode))
    {
        char actpath1[PATH_MAX+1];
        char *ptr1 = realpath((char *)pardir1.c_str(),actpath1);
        if (ptr1 != NULL)
        {
            std::string pardir2(dirname((char *)str2.c_str()));
            if (!pardir2.empty())
            {
                while (*pardir2.rbegin() == '/')
                {
                    pardir2.erase(pardir2.size()-1);
                }
            }
            struct stat pexist2;
            if (stat((char *)pardir2.c_str(),&pexist2) == 0 &&
                S_ISDIR(pexist2.st_mode))
            {
                char actpath2[PATH_MAX+1];
                char *ptr2 = realpath((char *)pardir2.c_str(),actpath2);
                if (ptr2 != NULL)
                {
                    std::string parbas1(std::string("/")+basename((char *)poststr1.c_str()));
                    std::string parbas2(std::string("/")+basename((char *)poststr2.c_str()));

                    proceed1 = (std::string(actpath1+parbas1) != std::string(actpath2+parbas2).substr(0,std::string(actpath1+parbas1).length()));
                    proceed2 = (std::string(actpath1) == std::string(actpath2));
                }
            }
        }
    }

    str1 = prestr1;
    str2 = prestr2;

    if (proceed1 || (proceed2 && basename((char *)str1.c_str()) != basename((char *)str2.c_str())))
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISDIR(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            return (rename((char *)str1.c_str(),(char *)str2.c_str()) == 0);
        }
    }

    return 0;
}

int directory_copy(std::string dname,std::string newname)
{
    std::string str1(dname);
    std::string str2(newname);

    str1.erase(unique(str1.begin(),str1.end(),dupslash()),str1.end());
    str2.erase(unique(str2.begin(),str2.end(),dupslash()),str2.end());

    if (!str1.empty())
    {
        while (*str1.rbegin() == '/')
        {
            str1.erase(str1.size()-1);
        }
    }
    if (!str2.empty())
    {
        while (*str2.rbegin() == '/')
        {
            str2.erase(str2.size()-1);
        }
    }

    size_t slash1;
    size_t slash2;
    std::string dir1(str1);
    std::string dir2(str2);

    if (dir1.find('/') != std::string::npos)
        slash1 = dir1.find_last_of("/");
    else
    {
        dir1 = std::string("");
        slash1 = 0;
    }

    if (dir2.find('/') != std::string::npos)
        slash2 = dir2.find_last_of("/");
    else
    {
        dir2 = std::string("");
        slash2 = 0;
    }

    int proceed1 = (str1 != str2.substr(0,str1.length()));
    int proceed2 = (dir1.substr(0,slash1) == dir2.substr(0,slash2));

    std::string prestr1 = str1;
    std::string prestr2 = str2;
    std::string poststr1 = str1;
    std::string poststr2 = str2;

    std::string pardir1(dirname((char *)str1.c_str()));
    if (!pardir1.empty())
    {
        while (*pardir1.rbegin() == '/')
        {
            pardir1.erase(pardir1.size()-1);
        }
    }
    struct stat pexist1;
    if (stat((char *)pardir1.c_str(),&pexist1) == 0 &&
        S_ISDIR(pexist1.st_mode))
    {
        char actpath1[PATH_MAX+1];
        char *ptr1 = realpath((char *)pardir1.c_str(),actpath1);
        if (ptr1 != NULL)
        {
            std::string pardir2(dirname((char *)str2.c_str()));
            if (!pardir2.empty())
            {
                while (*pardir2.rbegin() == '/')
                {
                    pardir2.erase(pardir2.size()-1);
                }
            }
            struct stat pexist2;
            if (stat((char *)pardir2.c_str(),&pexist2) == 0 &&
                S_ISDIR(pexist2.st_mode))
            {
                char actpath2[PATH_MAX+1];
                char *ptr2 = realpath((char *)pardir2.c_str(),actpath2);
                if (ptr2 != NULL)
                {
                    std::string parbas1(std::string("/")+basename((char *)poststr1.c_str()));
                    std::string parbas2(std::string("/")+basename((char *)poststr2.c_str()));

                    proceed1 = (std::string(actpath1+parbas1) != std::string(actpath2+parbas2).substr(0,std::string(actpath1+parbas1).length()));
                    proceed2 = (std::string(actpath1) == std::string(actpath2));
                }
            }
        }
    }

    str1 = prestr1;
    str2 = prestr2;

    if (proceed1 || (proceed2 && basename((char *)str1.c_str()) != basename((char *)str2.c_str())))
    {
        struct stat sb1;
        struct stat sb2;
        if (stat((char *)str1.c_str(),&sb1) == 0 &&
            S_ISDIR(sb1.st_mode) &&
            stat((char *)str2.c_str(),&sb2) != 0)
        {
            if (!directory_create((char *)str2.c_str()))
                return 0;

            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir((char *)str1.c_str())) != NULL)
            {
                while ((ent = readdir(dir)) != NULL)
                {
                    if (ent->d_name != std::string("."))
                    {
                        if (ent->d_name != std::string(".."))
                        {
                            std::string copy_sorc = str1+"/"+ent->d_name;
                            std::string copy_dest = str2+"/"+ent->d_name;

                            struct stat sb3;
                            if (stat((char *)copy_sorc.c_str(),&sb3) == 0 &&
                                S_ISDIR(sb3.st_mode))
                            {
                                if (!directory_copy((char *)copy_sorc.c_str(),(char *)copy_dest.c_str()))
                                    return 0;
                            }

                            struct stat sb4;
                            if (stat((char *)copy_sorc.c_str(),&sb4) == 0 &&
                                S_ISREG(sb4.st_mode))
                            {
                                if (!file_copy((char *)copy_sorc.c_str(),(char *)copy_dest.c_str()))
                                    return 0;
                            }
                        }
                    }
                }

                closedir(dir);
            }
            else
            {
                return 0;
            }

            return 1;
        }
    }

    return 0;
}

}
