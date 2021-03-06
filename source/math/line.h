


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



#ifndef LINE_H
#define LINE_H

#include "vec3f.h"

class Line
{
public:
	Vec3f m_vertex[2];

	Line();
	Line(Vec3f start, Vec3f end);
};

#endif
