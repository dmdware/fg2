


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



#ifndef POLYGON_H
#define POLYGON_H

#include "../platform.h"
#include "vec3f.h"

class Polyg	//name shortened due to naming conflict on Windows
{
public:
	std::list<Vec3f> m_edv;	//used for constructing the polygon on-the-fly
	Vec3f* m_drawoutva;		//used for drawing outline

	Polyg();
	~Polyg();
	Polyg(const Polyg& original);
	Polyg& operator=(const Polyg& original);
	void makeva();
	void freeva();
};

bool InsidePoly(Vec3f vIntersection, Vec3f Poly[], long verticeCount);
bool InterPoly(Vec3f vPoly[], Vec3f vLine[], int verticeCount, Vec3f* vIntersection=NULL);

#endif
