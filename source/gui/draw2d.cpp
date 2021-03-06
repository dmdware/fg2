


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
#include "../render/shader.h"
#include "draw2d.h"
#include "../utils.h"
#include "../window.h"

void DrawImage(unsigned int tex, float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom, float *crop)
{
	Shader *s;
	float v[2*6], t[2*6], newleft, newtop, newright, newbottom, newtexleft, newtextop, newtexright, newtexbottom;

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	s = &g_shader[g_curS];
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;
	newtexleft = texleft;
	newtextop = textop;
	newtexright = texright;
	newtexbottom = texbottom;

	if(newleft < crop[0])
	{
		newtexleft = texleft+(crop[0]-left)*(texright-texleft)/(right-left);
		newleft = crop[0];
	}
	else if(newleft > crop[2])
		return;

	if(newright < crop[0])
		return;
	else if(newright > crop[2])
	{
		newtexright = texleft+(crop[2]-left)*(texright-texleft)/(right-left);
		newright = crop[2];
	}

	if(newtop < crop[1])
	{
		newtextop = textop+(crop[1]-top)*(texbottom-textop)/(bottom-top);
		newtop = crop[1];
	}
	else if(newtop > crop[3])
		return;

	if(newbottom < crop[1])
		return;
	else if(newbottom > crop[3])
	{
		newtexbottom = textop+(crop[3]-top)*(texbottom-textop)/(bottom-top);
		newbottom = crop[3];
	}

	v[0] = newleft;
	v[1] = newtop;
	v[2] = newright;
	v[3] = newtop;
	v[4] = newright;
	v[5] = newbottom;
	v[6] = newright;
	v[7] = newbottom;
	v[8] = newleft;
	v[9] = newbottom;
	v[10] = newleft;
	v[11] = newtop;

	t[0] = newtexleft;
	t[1] = newtextop;
	t[2] = newtexright;
	t[3] = newtextop;
	t[4] = newtexright;
	t[5] = newtexbottom;
	t[6] = newtexright;
	t[7] = newtexbottom;
	t[8] = newtexleft;
	t[9] = newtexbottom;
	t[10] = newtexleft;
	t[11] = newtextop;

#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Rotate2(float cx, float cy, float angle, float* p)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  p[0] -= cx;
  p[1] -= cy;

  // rotate point
  float xnew = p[0] * c - p[1] * s;
  float ynew = p[0] * s + p[1] * c;

  // translate point back:
  p[0] = xnew + cx;
  p[1] = ynew + cy;
}

void DrawSphericalBlend(unsigned int difftex, unsigned int depthtex, unsigned int renderdepthtex, unsigned int renderfb, float basedepth,
						float cx, float cy,
						float pixradius, float angle,
						float texleft, float textop, float texright, float texbottom)
{
	Shader *s;
	float v[2*6], t[2*6];
	
	float top = cy - pixradius;
	float bottom = cy + pixradius;
	float left = cx - pixradius;
	float right = cx + pixradius;

	float topleft[2] = {left,top};
	float bottomleft[2] = {left,bottom};
	float topright[2] = {right,top};
	float bottomright[2] = {right,bottom};
	
	Rotate2(cx, cy, angle, topleft);
	Rotate2(cx, cy, angle, bottomleft);
	Rotate2(cx, cy, angle, topright);
	Rotate2(cx, cy, angle, bottomright);

#if 0
	v[0] = left;
	v[1] = top;
	v[2] = right;
	v[3] = top;
	v[4] = right;
	v[5] = bottom;
	v[6] = right;
	v[7] = bottom;
	v[8] = left;
	v[9] = bottom;
	v[10] = left;
	v[11] = top;
#else
	v[0] = topleft[0];
	v[1] = topleft[1];
	v[2] = topright[0];
	v[3] = topright[1];
	v[4] = bottomright[0];
	v[5] = bottomright[1];
	v[6] = bottomright[0];
	v[7] = bottomright[1];
	v[8] = bottomleft[0];
	v[9] = bottomleft[1];
	v[10] = topleft[0];
	v[11] = topleft[1];
#endif

	t[0] = texleft;
	t[1] = textop;
	t[2] = texright;
	t[3] = textop;
	t[4] = texright;
	t[5] = texbottom;
	t[6] = texright;
	t[7] = texbottom;
	t[8] = texleft;
	t[9] = texbottom;
	t[10] = texleft;
	t[11] = textop;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	UseS(SHADER_PARTICLEBLEND);
	//UseS(SHADER_ORTHO);
	s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	//glUniform1f(s->slot[SSLOT_WIDTH], (float)512);
	//glUniform1f(s->slot[SSLOT_HEIGHT], (float)512);
	glUniform1f(s->slot[SSLOT_SCREENMAPWIDTH], (float)2048);
	glUniform1f(s->slot[SSLOT_SCREENMAPHEIGHT], (float)2048);
	glUniform1f(s->slot[SSLOT_BASEDEPTH], basedepth);
	//glUniform1f(s->slot[SSLOT_BASEDEPTH], 0);
	glUniform4f(s->slot[SSLOT_COLOR], 1.0f, 1.0f, 1.0f, 1.0f);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderdepthtex);
	glUniform1i(s->slot[SSLOT_SCREENDEPTH], 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, difftex);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

#if 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glUniform1i(s->slot[SSLOT_SPRITEDEPTH], 1);
#endif
	
#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	
	
	EndS();

#if 0
	glEnable(GL_DEPTH_TEST);
	//glBindFramebuffer(GL_FRAMEBUFFER, renderfb);

	UseS(SHADER_PARTICLEBLENDDEPTH);
	//UseS(SHADER_ORTHO);
	s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	//glUniform1f(s->slot[SSLOT_WIDTH], (float)512);
	//glUniform1f(s->slot[SSLOT_HEIGHT], (float)512);
	glUniform1f(s->slot[SSLOT_SCREENMAPWIDTH], (float)2048);
	glUniform1f(s->slot[SSLOT_SCREENMAPHEIGHT], (float)2048);
	glUniform1f(s->slot[SSLOT_BASEDEPTH], basedepth);
	glUniform4f(s->slot[SSLOT_COLOR], 1.0f, 1.0f, 1.0f, 1.0f);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderdepthtex);
	glUniform1i(s->slot[SSLOT_SCREENDEPTH], 2);

	s = &g_shader[g_curS];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, difftex);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

#if 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glUniform1i(s->slot[SSLOT_SPRITEDEPTH], 1);
#endif
	
#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	EndS();
	glDisable(GL_DEPTH_TEST);
#endif
}

void DrawDepth(unsigned int difftex, unsigned int depthtex, unsigned int renderdepthtex, unsigned int renderfb, int basedepth, float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom)
{
	Shader *s;
	float v[2*6], t[2*6];

	v[0] = left;
	v[1] = top;
	v[2] = right;
	v[3] = top;
	v[4] = right;
	v[5] = bottom;
	v[6] = right;
	v[7] = bottom;
	v[8] = left;
	v[9] = bottom;
	v[10] = left;
	v[11] = top;

	//flip vertically because OpenGL has 0->1 upwards
	//texbottom = 1.0f - texbottom;
	//textop = 1.0f - textop;

	t[0] = texleft;
	t[1] = textop;
	t[2] = texright;
	t[3] = textop;
	t[4] = texright;
	t[5] = texbottom;
	t[6] = texright;
	t[7] = texbottom;
	t[8] = texleft;
	t[9] = texbottom;
	t[10] = texleft;
	t[11] = textop;
	
#if 1
	//actually, this one could also be done using the normal depth test
	//glEnable(GL_DEPTH_TEST);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//UseS(SHADER_DEEPORTHO);
	//UseS(SHADER_ORTHO);
	s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	//glUniform1f(s->slot[SSLOT_WIDTH], (float)512);
	//glUniform1f(s->slot[SSLOT_HEIGHT], (float)512);
	glUniform1f(s->slot[SSLOT_SCREENMAPWIDTH], (float)2048);
	glUniform1f(s->slot[SSLOT_SCREENMAPHEIGHT], (float)2048);
	glUniform4f(s->slot[SSLOT_COLOR], 1.0f, 1.0f, 1.0f, 1.0f);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, renderdepthtex);
	//glUniform1i(s->slot[SSLOT_SCREENDEPTH], 2);

	s = &g_shader[g_curS];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, difftex);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

#if 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glUniform1i(s->slot[SSLOT_SPRITEDEPTH], 1);
#endif

#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//EndS();
	
#endif

	return;

#if 0
	//flip vertically because OpenGL has 0->1 upwards
	texbottom = 1.0f - texbottom;
	textop = 1.0f - textop;

	t[0] = texleft;
	t[1] = textop;
	t[2] = texright;
	t[3] = textop;
	t[4] = texright;
	t[5] = texbottom;
	t[6] = texright;
	t[7] = texbottom;
	t[8] = texleft;
	t[9] = texbottom;
	t[10] = texleft;
	t[11] = textop;
#endif

	//to avoid using the render depth texture while writing to it, use the normal depth test
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, renderfb);

	UseS(SHADER_DEPTH);
	//UseS(SHADER_ORTHO);
	s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	//glUniform1f(s->slot[SSLOT_WIDTH], (float)512);
	//glUniform1f(s->slot[SSLOT_HEIGHT], (float)512);
	glUniform1f(s->slot[SSLOT_SCREENMAPWIDTH], (float)2048);
	glUniform1f(s->slot[SSLOT_SCREENMAPHEIGHT], (float)2048);
	glUniform4f(s->slot[SSLOT_COLOR], 1.0f, 1.0f, 1.0f, 1.0f);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, renderdepthtex);
	//glUniform1i(s->slot[SSLOT_SCREENDEPTH], 2);

	s = &g_shader[g_curS];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, difftex);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

#if 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glUniform1i(s->slot[SSLOT_SPRITEDEPTH], 1);
#endif
	
#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	EndS();

	glDisable(GL_DEPTH_TEST);
}

void DrawSquare(float r, float g, float b, float a, float left, float top, float right, float bottom, float *crop)
{
	Shader *s;
	float v[2*6], newleft, newtop, newright, newbottom;

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	s = &g_shader[g_curS];

	glUniform4f(s->slot[SSLOT_COLOR], r, g, b, a);

	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;

	if(newleft < crop[0])
		newleft = crop[0];
	else if(newleft > crop[2])
		return;

	if(newright < crop[0])
		return;
	else if(newright > crop[2])
		newright = crop[2];

	if(newtop < crop[1])
		newtop = crop[1];
	else if(newtop > crop[3])
		return;

	if(newbottom < crop[1])
		return;
	else if(newbottom > crop[3])
		newbottom = crop[3];

	v[0] = newleft;
	v[1] = newtop;
	v[2] = newright;
	v[3] = newtop;
	v[4] = newright;
	v[5] = newbottom;
	v[6] = newright;
	v[7] = newbottom;
	v[8] = newleft;
	v[9] = newbottom;
	v[10] = newleft;
	v[11] = newtop;
	
#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawLine(float r, float g, float b, float a, float x1, float y1, float x2, float y2, float *crop)
{
	float v[2*2], newx1, newy1, newx2, newy2, dx, dy, dot, r1, r2, rx, ry, dr, slope;
	Shader *s;

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	float minx = fmin(x1,x2);
	float miny = fmin(y1,y2);
	float maxx = fmax(x1,x2);
	float maxy = fmax(y1,y2);

	newx1 = minx;
	newy1 = miny;
	newx2 = maxx;
	newy2 = maxy;

	dx = maxx-minx;
	dy = maxy-miny;
	slope = dy/dx;

#if 0
	rx = (crop[2] - minx) / dx;
	ry = (crop[3] - miny) / dy;
	r1 = fmin(rx,ry);

	if(r1 <= 0.0f)
		return;
	if(r1 < 1.0f)
	{
		newx2 = minx + dx * r1;
		newy2 = miny + dy * r1;
	}
#endif

#if 0
	rx = (crop[0] - maxx) / -dx;
	ry = (crop[1] - maxy) / -dy;
	r2 = fmin(rx,ry);

	if(r2 <= 0.0f)
		return;
	if(r2 < 1.0f)
	{
		newx1 = maxx - dx * r2;
		newy1 = maxy - dy * r2;
	}
#endif

#if 0
	r1 = fmin(1.0f,r1);
	r2 = 1.0f - fmin(1.0f,r2);

	if(r2 >= r1)
		return;
#endif

	s = &g_shader[g_curS];
	glUniform4f(s->slot[SSLOT_COLOR], r, g, b, a);

	v[0] = newx1;
	v[1] = newy1;
	v[2] = newx2;
	v[3] = newy2;

#if 1
	v[0] = x1;
	v[1] = y1;
	v[2] = x2;
	v[3] = y2;
#endif

#if 0
	v[0] = minx;
	v[1] = miny;
	v[2] = maxx;
	v[3] = maxy;
#endif

#ifdef PLATFORM_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
#endif
	
	glDrawArrays(GL_LINES, 0, 2);
}
