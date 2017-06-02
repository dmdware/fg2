


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



#include "line.h"


Line::Line()
{
}

Line::Line(Vec3f start, Vec3f end)
{
	m_vertex[0] = start;
	m_vertex[1] = end;
}
