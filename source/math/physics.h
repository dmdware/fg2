


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



#define EPSILON			0.03125f //0.1f	//1.2f
#define	CLOSE_EPSILON	0.1f
#define FRICTION		1.5f
#define INVFRICTION		(1.0f/FRICTION)

class Vec3f;
class Triangle;

bool TriBoxOverlap(Vec3f vPos, Vec3f vMin, Vec3f vMax, Triangle tri);
bool TriBoxOverlap(Vec3f vCenter, Vec3f vRadius, Triangle tri);
bool TriBoxOverlap2(Vec3f vScaleDown, Vec3f vCenter, Triangle tri);
