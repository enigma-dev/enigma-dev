/*
  The MIT License

  Copyright (c) 2010-2013 IsmAvatar <IsmAvatar@gmail.com> and Robert B. Colton

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "BS_ip.h"
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

