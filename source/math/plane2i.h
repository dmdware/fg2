


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



#ifndef PLANE2I_H
#define PLANE2I_H

#include "vec2i.h"
#include "physics.h"

class Plane2i
{
public:
	Vec2i m_normal;
	int m_d;

	bool operator==(const Plane2i p) const
	{
		if(m_normal.x == p.m_normal.x && m_normal.y == p.m_normal.y && m_d == p.m_d)
			return true;

		return false;
	}

	Plane2i();
	Plane2i(int x, int y, int d);
	~Plane2i();
};

int PlaneDistance(Vec2i normal, Vec2i point);

#endif
