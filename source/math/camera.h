


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



#ifndef CAMERA_H
#define CAMERA_H

#include "vec2f.h"
#include "vec3f.h"
#include "quaternion.h"

#define MOUSE_SENSITIVITY	0.0001f	//radians over pixels

class Camera
{
public:
	Vec3f m_pos;
	Vec3f m_view;
	Vec3f m_up;
	Vec3f m_strafe;
	Vec3f m_vel;
	Vec2f m_orientv;	// pitch, yaw
	Quaternion m_orientq;	//orientation quaternion
	bool m_grounded;

	Camera();
	Camera(float posx, float posy, float posz, float viewx, float viewy, float viewz, float upx, float upy, float upz);

	Vec3f up2();
	void position(float posx, float posy, float posz, float viewx, float viewy, float viewz, float upx, float upy, float upz);
	void rotateview(float angle, float x, float y, float z);
	void lookat(Vec3f at);
	void rotatebymouse(int dx, int dy);
	Vec3f lookpos();
	void grounded(bool ground);
	void rotateabout(Vec3f center, float rad, float x, float y, float z);
	void strafe(float speed);
	void move(float speed);
	void accelerate(float speed);
	void accelstrafe(float speed);
	void accelrise(float speed);
	void rise(float speed);
	void move(Vec3f delta);
	void moveto(Vec3f newpos);
	void limithvel(float limit);
	void stop();
	void calcstrafe();
	void calcyaw();
	void calcpitch();
	void frameupd();
	void friction();
	void friction2();
	float yaw();
	float pitch();
	Vec3f zoompos();
};

#endif
