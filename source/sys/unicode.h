


/*******************************************************
 * Copyright (C) 2015 DMD 'Ware <dmdware@gmail.com>
 * 
 * This file is part of Caloric Metabolic Graph.
 * 
 * You are entitled to use this source code to learn.
 *
 * You are not entitled to duplicate or copy this source code 
 * into your own projects, commercial or personal, UNLESS you 
 * give credit.
 *
 *******************************************************/




#include "../platform.h"
#include "../ustring.h"

int ValidateUTF8(char unsigned *buff, int len);
void dump_unicode(char unsigned *buff, int len);
//unsigned int *ToUTF32(const unsigned char *utf8, int len);
unsigned int *ToUTF32(const unsigned char *utf8);
unsigned char *ToUTF8(const unsigned int *unicode);
void print_char(int pos, int len, unsigned int ch);
void dump_unicode_string(unsigned int *str);
int UTF8Len(unsigned char ch);
int UTF32Len(const unsigned int* ustr);
unsigned char *raw2utf8(unsigned ch);
unsigned char *ToUTF8_2(const unsigned int *unicode);