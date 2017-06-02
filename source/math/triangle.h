


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



#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3f.h"
#include "vec2f.h"

class Vec3f;

class Triangle
{
public:
	Vec3f m_vertex[3];
};

class Triangle2 : public Triangle
{
public:
	Vec2f m_texcoord[3];
};

#endif
