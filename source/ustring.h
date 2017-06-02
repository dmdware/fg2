


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




#ifndef USTRING_H
#define USTRING_H

#include "platform.h"

//#define USTR_DEBUG

class UStr
{
public:
	unsigned int m_length;	//length doesn't count null-terminator
	unsigned int* m_data;

	UStr();
	~UStr();
	UStr(const UStr& original);
	UStr(const char* cstr);
	UStr(unsigned int k);
	UStr(unsigned int* k);
	UStr& operator=(const UStr &original);
	UStr operator+(const UStr &other);
	UStr substr(int start, int len) const;
	int firstof(UStr find) const;
	std::string rawstr() const;
};

#endif
