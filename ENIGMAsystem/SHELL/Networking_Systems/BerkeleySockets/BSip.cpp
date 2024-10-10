/** Copyright (C) 2010 IsmAvatar
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include "BSip.h"
#include <curl/curl.h>
#define IPLEN 24

char ip[IPLEN];
int p = 0;

int handle_data(void *ptr, int size, int nmemb, void *stream) {
 int i, numbytes = size * nmemb;
 char *str = (char *)ptr;
 for (i = 0; i < numbytes && p < IPLEN - 1; i++)
  ip[p++] = str[i];
 ip[p] = '\0';
}

