


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



#ifndef BRUSH_H
#define BRUSH_H

#include "vec3f.h"
#include "plane3f.h"
#include "polygon.h"
#include "triangle.h"

void MakeHull(Vec3f* norms, float* ds, const Vec3f pos, const float radius, const float height);
void MakeHull(Vec3f* norms, float* ds, const Vec3f pos, const float hwx, const float hwz, const float height);
void MakeHull(Vec3f* norms, float* ds, const Vec3f pos, const Vec3f vmin, const Vec3f vmax);
bool LineInterHull(const Vec3f* line, const Vec3f* norms, const float* ds, const int numplanes);
bool LineInterHull(const Vec3f* line, Plane3f* planes, const int numplanes, Vec3f* intersect);
bool LineInterHull(const Vec3f* line, const Vec3f* norms, const float* ds, const int numplanes, Vec3f* intersect);

#endif
