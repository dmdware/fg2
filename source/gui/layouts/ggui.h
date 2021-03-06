


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



#ifndef GGUI_H
#define GGUI_H

#include "../../math/vec3f.h"

class ViewLayer;
class Widget;

#if 0
extern bool g_canselect;
#endif

#define LEFT_PANEL_WIDTH	200

extern char g_lastsave[DMD_MAX_PATH+1];

void Resize_Fullscreen(Widget* w);
void Resize_FullscreenSq(Widget* w);
void Resize_AppLogo(Widget* w);
void Resize_AppTitle(Widget* w);
void FillGUI();
void Resize_MenuItem(Widget* w);
void Resize_CenterWin(Widget* w);
void EnumDisplay();

void Resize_Window(Widget* w);

#endif	//GGUI_H
