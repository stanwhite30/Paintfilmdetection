#ifndef CONVERT_H
#define CONVERT_H
#include <iconv.h>
#include <iostream>
#include <stdio.h>

int utf8togb2312( char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen);
#endif // CONVERT_H
