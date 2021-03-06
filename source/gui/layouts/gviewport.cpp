


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





#include "../../render/shader.h"
#include "../../gui/gui.h"
#include "../../math/3dmath.h"
#include "../../window.h"
#include "../../platform.h"
#include "../../gui/font.h"
#include "../../math/camera.h"
#include "../../math/matrix.h"
#include "../../math/vec4f.h"
#include "../../math/brush.h"
#include "../../math/frustum.h"
#include "gviewport.h"
#include "../../debug.h"

VpType g_vptype[VIEWPORT_TYPES];
Viewport g_viewport[4];
//Vec3f g_focus;

VpType::VpType(Vec3f offset, Vec3f up, const char* label, bool axial)
{
	m_offset = offset;
	m_up = up;
	strcpy(m_label, label);
	m_axial = axial;
}

#if 0
Viewport::Viewport()
{
	m_drag = false;
	m_ldown = false;
	m_rdown = false;
}

Viewport::Viewport(int type)
{
	m_drag = false;
	m_ldown = false;
	m_rdown = false;
	m_mdown = false;
	m_type = type;
}

Vec3f Viewport::up()
{
	Player* py = &g_player[g_localP];
	Camera* c = &g_cam;

	Vec3f upvec = c->m_up;
	VpType* t = &g_vptype[m_type];

	if(t->m_axial)
		upvec = t->m_up;

	return upvec;
}

Vec3f Viewport::up2()
{
	Player* py = &g_player[g_localP];
	Camera* c = &g_cam;

	Vec3f upvec = c->up2();
	VpType* t = &g_vptype[m_type];

	if(t->m_axial)
		upvec = t->m_up;

	return upvec;
}

Vec3f Viewport::strafe()
{
	Vec3f upvec = up();
	VpType* t = &g_vptype[m_type];
	Vec3f sidevec = Normalize(Cross(Vec3f(0,0,0)-t->m_offset, upvec));

	//if(!t->m_axial)
	//	sidevec = c->m_strafe;

	return sidevec;
}

Vec3f Viewport::focus()
{
	Player* py = &g_player[g_localP];
	Camera* c = &g_cam;

	Vec3f viewvec = c->m_view;
	return viewvec;
}

Vec3f Viewport::viewdir()
{
	Vec3f focusvec = focus();
	Vec3f posvec = pos();
	//Vec3f viewvec = posvec + Normalize(focusvec-posvec);
	//return viewvec;
	return focusvec-posvec;
}

Vec3f Viewport::pos()
{
	Player* py = &g_player[g_localP];
	Camera* c = &g_cam;

	Vec3f posvec = c->m_pos;

#if 0
	if(g_projtype == PROJ_PERSP && !t->m_axial)
	{
		Vec3f dir = Normalize( c->m_view - c->m_pos );
		posvec = c->m_view - dir * 1000.0f / g_zoom;
	}
#endif

	VpType* t = &g_vptype[m_type];

	if(t->m_axial)
		posvec = c->m_view + t->m_offset;

	return posvec;
}
#endif

void DrawMinimap()
{
	//g_frustum.construct(projection.m_matrix, viewmat.m_matrix);

	CHECKGLERROR();

}

void DrawViewport(int which, int x, int y, int width, int height)
{
	//return;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	Viewport* v = &g_viewport[which];
	VpType* t = &g_vptype[v->m_type];

	if(which == VIEWPORT_ENTVIEW)
	{
		CHECKGLERROR();
	}

	if(which == VIEWPORT_MINIMAP)
	{
		StartTimer(TIMER_DRAWMINIMAP);

		CHECKGLERROR();

		StopTimer(TIMER_DRAWMINIMAP);
	}

	//glDisable(GL_DEPTH_TEST);
	glFlush();
	CHECKGLERROR();
}

#if 0
bool ViewportLDown(int which, int relx, int rely, int width, int height)
{
	//return false;

	Viewport* v = &g_viewport[which];
	v->m_ldown = true;
	v->m_lastmouse = Vec2i(relx, rely);
	v->m_curmouse = Vec2i(relx, rely);


	VpType* t = &g_vptype[v->m_type];

	//Log("vp["<<which<<"] l down");
	//

	float aspect = fabsf((float)width / (float)height);
	Matrix projection;

	bool persp = false;

#if 0
	if(v->!t->m_axial && g_projtype == PROJ_PERSP)
	{
		projection = PerspProj(FIELD_OF_VIEW, aspect, MIN_DISTANCE, MAX_DISTANCE);
		persp = true;
	}
	else
	{
		projection = OrthoProj(-PROJ_RIGHT*aspect/g_zoom, PROJ_RIGHT*aspect/g_zoom, PROJ_RIGHT/g_zoom, -PROJ_RIGHT/g_zoom, MIN_DISTANCE, MAX_DISTANCE);
	}
#endif

	//Vec3f viewvec = g_focus; //c->m_view;
	//Vec3f viewvec = c->m_view;
	Vec3f focusvec = v->focus();
	//Vec3f posvec = g_focus + t->m_offset;
	//Vec3f posvec = c->m_pos;
	Vec3f posvec = v->pos();

	//if(v->t->m_axial)
	{
		//	posvec = c->m_view + t->m_offset;
		//viewvec = posvec + Normalize(c->m_view-posvec);
	}

	//viewvec = posvec + Normalize(viewvec-posvec);
	//Vec3f posvec2 = c->lookpos() + t->m_offset;
	//Vec3f upvec = t->m_up;
	//Vec3f upvec = c->m_up;
	Vec3f upvec = v->up();

	//if(v->t->m_axial)
	//	upvec = t->m_up;

	Matrix viewmat = LookAt(posvec.x, posvec.y, posvec.z, focusvec.x, focusvec.y, focusvec.z, upvec.x, upvec.y, upvec.z);
	Matrix mvpmat;
	mvpmat.set(projection.m_matrix);
	mvpmat.postmult(viewmat);

	return true;
}


bool ViewportRDown(int which, int relx, int rely, int width, int height)
{
	Viewport* v = &g_viewport[which];
	v->m_rdown = true;
	v->m_lastmouse = Vec2i(relx, rely);
	v->m_curmouse = Vec2i(relx, rely);

	return true;
}

bool ViewportLUp(int which, int relx, int rely, int width, int height)
{
	Viewport* v = &g_viewport[which];
	VpType* t = &g_vptype[v->m_type];

	if(v->m_ldown)
	{
		//return true;
		v->m_ldown = false;

	}

	//g_sel1b = NULL;
	//g_dragV = -1;
	//g_dragS = -1;

	return false;
}

bool ViewportRUp(int which, int relx, int rely, int width, int height)
{
	Viewport* v = &g_viewport[which];
	VpType* t = &g_vptype[v->m_type];

	v->m_rdown = false;

	return false;
}

bool NULL(int which, int delta)
{
	Viewport* v = &g_viewport[which];
	VpType* t = &g_vptype[v->m_type];

#if 0
	//if(v->!t->m_axial)
	{
		g_zoom *= 1.0f + (float)delta / 10.0f;
		return true;
	}
#endif

	return false;
}

bool ViewportMousemove(int which, int relx, int rely, int width, int height)
{
	Viewport* v = &g_viewport[which];

	v->m_lastmouse = Vec2i(relx, rely);
	v->m_curmouse = Vec2i(relx, rely);

	return false;
}
#endif
