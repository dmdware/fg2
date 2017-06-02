


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



#include "../../app/appmain.h"
#include "../../gui/gui.h"
#include "../keymap.h"
#include "../../math/camera.h"
#include "../../render/screenshot.h"
#include "../../gui/widgets/winw.h"
#include "../../debug.h"
#include "../../window.h"
#include "../../language.h"
#include "../widgets/image.h"
#include "../../render/graph.h"

//not engine
#include "ggui.h"
#include "messbox.h"
#include "../../app/appmain.h"

//bool g_canselect = true;

char g_lastsave[DMD_MAX_PATH+1];

void Resize_Fullscreen(Widget* w)
{
	w->m_pos[0] = 0;
	w->m_pos[1] = 0;
	w->m_pos[2] = (float)g_width-1;
	w->m_pos[3] = (float)g_height-1;

#if 1
	if(w->m_type == WIDGET_IMAGE)
	{
		Texture* tex = &g_texture[w->m_tex];
	
		w->m_texc[0] = w->m_pos[0] / (float)tex->width;
		w->m_texc[1] = w->m_pos[1] / (float)tex->height;
		w->m_texc[2] = w->m_pos[2] / (float)tex->width;
		w->m_texc[3] = w->m_pos[3] / (float)tex->height;
	}
#endif
}

void Resize_FullscreenSq(Widget* w)
{
	float minsz = fmin((float)g_width-1, (float)g_height-1);

	w->m_pos[0] = g_width/2.0f - minsz/2.0f;
	w->m_pos[1] = g_height/2.0f - minsz/2.0f;
	w->m_pos[2] = g_width/2.0f + minsz/2.0f;
	w->m_pos[3] = g_height/2.0f + minsz/2.0f;

#if 0
	if(w->m_type == WIDGET_IMAGE)
	{
		Texture* tex = &g_texture[w->m_tex];
	
		w->m_texc[0] = w->m_pos[0] / (float)tex->width;
		w->m_texc[1] = w->m_pos[1] / (float)tex->height;
		w->m_texc[2] = w->m_pos[2] / (float)tex->width;
		w->m_texc[3] = w->m_pos[3] / (float)tex->height;
	}
#endif
}

void Resize_AppLogo(Widget* w)
{
	w->m_pos[0] = 30;
	w->m_pos[1] = 30;
	w->m_pos[2] = 200;
	w->m_pos[3] = 200;
}

void Resize_AppTitle(Widget* w)
{
	w->m_pos[0] = 30;
	w->m_pos[1] = 30;
	w->m_pos[2] = (float)g_width-1;
	w->m_pos[3] = 100;
}


void Resize_MenuItem(Widget* w)
{
	int row;
	sscanf(w->m_name.c_str(), "%d", &row);
#if 0
	Font* f = &g_font[w->m_font];

	//w->m_pos[0] = g_width/2 - f->gheight*4;
	w->m_pos[0] = f->gheight*4;
	w->m_pos[1] = g_height/2 - f->gheight*4 + f->gheight*1.5f*row;
	w->m_pos[2] = w->m_pos[0] + f->gheight * 10;
	w->m_pos[3] = w->m_pos[1] + f->gheight;
#else
	w->m_pos[0] = 50;
	w->m_pos[2] = 230;
	w->m_pos[1] = row * 150 + 50;
	w->m_pos[3] = w->m_pos[1] + 100;
#endif
}

void Resize_LoadingStatus(Widget* w)
{
	w->m_pos[0] = g_width/2 - 64;
	w->m_pos[1] = g_height/2;
	w->m_pos[2] = g_width;
	w->m_pos[3] = g_height;
	w->m_tpos[0] = g_width/2;
	w->m_tpos[1] = g_height/2;
}

void Resize_WinText(Widget* w)
{
	Widget* parent = w->m_parent;

	w->m_pos[0] = parent->m_pos[0];
	w->m_pos[1] = parent->m_pos[1];
	w->m_pos[2] = w->m_pos[0] + 400;
	w->m_pos[3] = w->m_pos[1] + 2000;
}

void Resize_CenterWin(Widget* w)
{

	w->m_pos[0] = 0;
	w->m_pos[1] = g_height - 200;
	w->m_pos[2] = 400;
	w->m_pos[3] = g_height;
}

void Resize_SvList(Widget* w)
{

	w->m_pos[0] = 0;
	w->m_pos[1] = g_height - 300;
	w->m_pos[2] = 550;
	w->m_pos[3] = g_height;
}

//TODO make window resize functions fit window to screen, no matter how small it is, but be more centered on larger screens
//TODO scroll bars on smaller screens
void Resize_CenterWin2(Widget* w)
{

	w->m_pos[0] = g_width/2 - 250 + 60;
	w->m_pos[1] = g_height - 300;
	w->m_pos[2] = g_width/2 + 200 + 60;
	w->m_pos[3] = g_height;
}

void Click_Options()
{
	GUI* gui = &g_gui;
	gui->hideall();
	gui->show("options");
}

void Resize_JoinCancel(Widget* w)
{
	w->m_pos[0] = g_width/2 - 75;
	w->m_pos[1] = g_height/2 + 100 - 30;
	w->m_pos[2] = g_width/2 + 75;
	w->m_pos[3] = g_height/2 + 100 - 0;
	CenterLabel(w);
}

void Resize_CopyLogo(Widget* w)
{
	w->m_pos[0] = g_width - 260 + 16;
	w->m_pos[1] = g_height - 60 - 16;
	w->m_pos[2] = w->m_pos[0] + 64;
	w->m_pos[3] = w->m_pos[1] + 64;
}

void EnumDisplay()
{
	g_resolution.clear();
	
	for(int i=0; i<SDL_GetNumDisplayModes(0); i++)
	{
		SDL_DisplayMode mode;
		SDL_GetDisplayMode(0, i, &mode);
		
		bool found = false;
		
		for(auto rit=g_resolution.begin(); rit!=g_resolution.end(); rit++)
		{
			if(rit->width == mode.w &&
			   rit->height == mode.h)
			{
				found = true;
				break;
			}
		}
		
		if(found)
			continue;
		
#ifdef PLATFORM_MOBILE
		//landscape only
		if(mode.h > mode.w)
			continue;
#endif
		
		Resolution res;
		res.width = mode.w;
		res.height = mode.h;
		g_resolution.push_back(res);
	}
}

void Click_Settings()
{
	GUI* gui = &g_gui;
	gui->show("settings");
}

void Click_OpenLoss()
{
	GUI* gui = &g_gui;
	gui->show("loss");
}

void Click_OpenGain()
{
	GUI* gui = &g_gui;
	gui->show("gain");
}

void Click_OpenReset()
{
	GUI* gui = &g_gui;
	gui->show("reset");
}

void Resize_Input(Widget* w)
{
	Widget* parw = w->m_parent;
	int row;
	sscanf(w->m_name.c_str(), "%d", &row);
#if 0
	Font* f = &g_font[w->m_font];

	//w->m_pos[0] = g_width/2 - f->gheight*4;
	w->m_pos[0] = f->gheight*4;
	w->m_pos[1] = g_height/2 - f->gheight*4 + f->gheight*1.5f*row;
	w->m_pos[2] = w->m_pos[0] + f->gheight * 10;
	w->m_pos[3] = w->m_pos[1] + f->gheight;
#else
	w->m_pos[0] = 2 + parw->m_pos[0];
	w->m_pos[2] = parw->m_pos[2] - 2;
	w->m_pos[1] = 2 + parw->m_pos[1] + row * 40;
	w->m_pos[3] = w->m_pos[1] + 32;
#endif
}

void Click_SaveSet()
{
	GUI* gui = &g_gui;
	gui->hide("settings");
	Win* setwin = (Win*)gui->get("settings");
	DropList* genderbox = (DropList*)setwin->get("1");
	EditBox* agebox = (EditBox*)setwin->get("3");
	EditBox* heightbox = (EditBox*)setwin->get("5");
	EditBox* weightbox = (EditBox*)setwin->get("7");

	int gender = genderbox->m_selected;
	float age = StrToFloat(agebox->m_value.rawstr().c_str());
	float heightcm = StrToFloat(heightbox->m_value.rawstr().c_str());
	float weightlbs = StrToFloat(weightbox->m_value.rawstr().c_str());

	if(g_graph.points.size())
	{
		SaveAdd("");
	}

	g_gender = gender;
	g_age = age;
	g_heightcm = heightcm;
	g_weightkcal = weightlbs * 3500.0f;

	SaveSettings();
}

void Click_Reset()
{
	GUI* gui = &g_gui;
	gui->hide("reset");

	char datfull[DMD_MAX_PATH+1];
	FullWritePath("data.dat", datfull);
	unlink(datfull);

	exit(0);
}

void Click_CancelReset()
{
	GUI* gui = &g_gui;
	gui->hide("reset");
}

void Click_SaveLoss()
{
	GUI* gui = &g_gui;
	gui->hide("loss");

	if(g_graph.points.size() == 0)
	{
		RichText m = RichText("Must enter settings first!");
		Mess(&m);
		return;
	}
	
	Win* losswin = (Win*)gui->get("loss");
	EditBox* lossbox = (EditBox*)losswin->get("1");
	EditBox* notebox = (EditBox*)losswin->get("3");

	char note[32];
	strncpy(note, notebox->m_value.rawstr().c_str(), 31);
	float loss = StrToFloat(lossbox->m_value.rawstr().c_str());

	SaveAdd("");
	g_weightkcal -= loss;
	SaveAdd(note);
}

void Click_SaveGain()
{
	GUI* gui = &g_gui;
	gui->hide("gain");

	if(g_graph.points.size() == 0)
	{
		RichText m = RichText("Must enter settings first!");
		Mess(&m);
		return;
	}
	
	Win* gainwin = (Win*)gui->get("gain");
	EditBox* gainbox = (EditBox*)gainwin->get("1");
	EditBox* notebox = (EditBox*)gainwin->get("3");

	char note[32];
	strncpy(note, notebox->m_value.rawstr().c_str(), 31);
	float gain = StrToFloat(gainbox->m_value.rawstr().c_str());
	
	SaveAdd("");
	g_weightkcal += gain;
	SaveAdd(note);
}

void Resize_Window(Widget* w)
{
	w->m_pos[0] = 100;
	w->m_pos[1] = 100;
	w->m_pos[2] = 650;
	w->m_pos[3] = 600;
}

void FillMenu()
{
	GUI* gui = &g_gui;

	// Main ViewLayer
	gui->add(new ViewLayer(gui, "main"));
	ViewLayer* mainview = (ViewLayer*)gui->get("main");
	mainview->add(new Image(mainview, "", "gui/mmbg.jpg", false, Resize_Fullscreen));
	//mainview->add(new Image(mainview, "", "gui/centerp/app.png", true, Resize_AppLogo));
	//mainview->add(new Text(mainview, "app title", RichText(TITLE), MAINFONT32, Resize_AppTitle));
#if 0
	//mainview->add(new Link(mainview, "0", (STRTABLE[STR_TUTORIAL]), MAINFONT16, Resize_MenuItem, Click_OpenTut));
	mainview->add(new Link(mainview, "2", (STRTABLE[STR_LOADGAME]), MAINFONT16, Resize_MenuItem, Click_LoadGame));
	mainview->add(new Link(mainview, "3", (STRTABLE[STR_HOSTGAME]), MAINFONT16, Resize_MenuItem, Click_HostGame));
	mainview->add(new Link(mainview, "4", (STRTABLE[STR_JOINGAME]), MAINFONT16, Resize_MenuItem, Click_ListHosts));
#ifndef PLATFORM_MOBILE
	mainview->add(new Link(mainview, "5", (STRTABLE[STR_MAPED]), MAINFONT16, Resize_MenuItem, Click_OpenEd));
#endif
	//mainview->add(new Link(mainview, "6", RichText("Load Mod"), MAINFONT16, Resize_MenuItem, Click_OpenMod));
	mainview->add(new Link(mainview, "6", (STRTABLE[STR_OPTIONS]), MAINFONT16, Resize_MenuItem, Click_Options));
#ifndef PLATFORM_MOBILE
	mainview->add(new Link(mainview, "7", (STRTABLE[STR_QUIT]), MAINFONT16, Resize_MenuItem, Click_Quit));
#endif
#endif
	mainview->add(new InsDraw(mainview, DrawGraphs));
	mainview->add(new Button(mainview, "0", "gui/transp.png", RichText("Settings"), RichText("Settings"), MAINFONT8, BUST_LINEBASED, Resize_MenuItem, Click_Settings, NULL, NULL, NULL, NULL, -1, NULL));
	mainview->add(new Button(mainview, "1", "gui/transp.png", RichText("Caloric Gain"), RichText("Caloric Gain"), MAINFONT8, BUST_LINEBASED, Resize_MenuItem, Click_OpenGain, NULL, NULL, NULL, NULL, -1, NULL));
	mainview->add(new Button(mainview, "2", "gui/transp.png", RichText("Caloric Loss"), RichText("Caloric Loss"), MAINFONT8, BUST_LINEBASED, Resize_MenuItem, Click_OpenLoss, NULL, NULL, NULL, NULL, -1, NULL));
	mainview->add(new Button(mainview, "3", "gui/transp.png", RichText("Reset Data"), RichText("Reset Data"), MAINFONT8, BUST_LINEBASED, Resize_MenuItem, Click_OpenReset, NULL, NULL, NULL, NULL, -1, NULL));

	mainview->add(new Image(mainview, "", "gui/transp.png", true, Resize_CopyLogo));

#if 0
	char infostr[512];
	char verstr[32];
	VerStr(VERSION, verstr);
	sprintf(infostr, "%s (c) DMD 'Ware 2015\n%s %s", 
		STRTABLE[STR_COPYRIGHT].rawstr().c_str(), 
		STRTABLE[STR_VERSION].rawstr().c_str(), 
		verstr);
	mainview->add(new TextBlock(mainview, "copy", RichText(infostr), MAINFONT16, Resize_CopyInfo, 0.2f, 1.0f, 0.2f, 1.0f));
#endif

	gui->add(new Win(gui, "settings", Resize_Window));
	gui->add(new Win(gui, "gain", Resize_Window));
	gui->add(new Win(gui, "loss", Resize_Window));
	gui->add(new Win(gui, "reset", Resize_Window));
	
	Win* setwin = (Win*)gui->get("settings");
	setwin->add(new Text(setwin, "0", RichText("Gender:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	setwin->add(new DropList(setwin, "1", FONT_EUROSTILE32, Resize_Input, NULL));
	DropList* gender = (DropList*)setwin->get("1");
	gender->m_options.push_back(RichText("Male"));
	gender->m_options.push_back(RichText("Female"));
	setwin->add(new Text(setwin, "2", RichText("Age:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	setwin->add(new EditBox(setwin, "3", RichText("25"), FONT_EUROSTILE32, Resize_Input, false, 6, NULL, NULL, -1));
	setwin->add(new Text(setwin, "4", RichText("Height (centimeters):"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	setwin->add(new EditBox(setwin, "5", RichText("160"), FONT_EUROSTILE32, Resize_Input, false, 6, NULL, NULL, -1));
	setwin->add(new Text(setwin, "6", RichText("Weight (pounds):"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	setwin->add(new EditBox(setwin, "7", RichText("200"), FONT_EUROSTILE32, Resize_Input, false, 6, NULL, NULL, -1));
	setwin->add(new Button(setwin, "9", "gui/transp.png", RichText("Save"), RichText("Save"), FONT_EUROSTILE32, BUST_LINEBASED, Resize_Input, Click_SaveSet, NULL, NULL, NULL, NULL, -1, NULL));

	Win* gainwin = (Win*)gui->get("gain");
	gainwin->add(new Text(gainwin, "0", RichText("Calories gained:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	gainwin->add(new EditBox(gainwin, "1", RichText("250"), FONT_EUROSTILE32, Resize_Input, false, 6, NULL, NULL, -1));
	gainwin->add(new Text(gainwin, "2", RichText("Note:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	gainwin->add(new EditBox(gainwin, "3", RichText("Energy bar"), FONT_EUROSTILE32, Resize_Input, false, 32, NULL, NULL, -1));
	gainwin->add(new Button(gainwin, "5", "gui/transp.png", RichText("Add data"), RichText("Add data"), FONT_EUROSTILE32, BUST_LINEBASED, Resize_Input, Click_SaveGain, NULL, NULL, NULL, NULL, -1, NULL));

	Win* losswin = (Win*)gui->get("loss");
	losswin->add(new Text(losswin, "0", RichText("Calories lost:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	losswin->add(new EditBox(losswin, "1", RichText("250"), FONT_EUROSTILE32, Resize_Input, false, 6, NULL, NULL, -1));
	losswin->add(new Text(losswin, "2", RichText("Note:"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	losswin->add(new EditBox(losswin, "3", RichText("Ran for 5 minutes"), FONT_EUROSTILE32, Resize_Input, false, 32, NULL, NULL, -1));
	losswin->add(new Button(losswin, "5", "gui/transp.png", RichText("Add data"), RichText("Add data"), FONT_EUROSTILE32, BUST_LINEBASED, Resize_Input, Click_SaveLoss, NULL, NULL, NULL, NULL, -1, NULL));

	Win* reswin = (Win*)gui->get("reset");
	reswin->add(new Text(reswin, "0", RichText("Are you sure?"), FONT_EUROSTILE32, Resize_Input, false, 1, 1, 1, 0.7f));
	reswin->add(new Button(reswin, "2", "gui/transp.png", RichText("Reset data"), RichText("Reset data"), FONT_EUROSTILE32, BUST_LINEBASED, Resize_Input, Click_Reset, NULL, NULL, NULL, NULL, -1, NULL));
	reswin->add(new Button(reswin, "4", "gui/transp.png", RichText("Cancel"), RichText("Cancel"), FONT_EUROSTILE32, BUST_LINEBASED, Resize_Input, Click_CancelReset, NULL, NULL, NULL, NULL, -1, NULL));
}

void MouseLDown()
{
	if(g_mode == APPMODE_PLAY ||
		g_mode == APPMODE_EDITOR)
	{
		g_mousestart = g_mouse;
        
	}
}

void MouseLUp()
{
	if(g_mode == APPMODE_EDITOR)
	{
	}
	else if(g_mode == APPMODE_PLAY)
	{
	}
}

void MouseMove(InEv* ie)
{
	if(g_mode == APPMODE_PLAY || 
		g_mode == APPMODE_EDITOR)
	{
	}
}

void MouseRDown()
{
	if(g_mode == APPMODE_PLAY)
	{
	}
}

void MouseRUp()
{
	if(g_mode == APPMODE_PLAY)
	{
	}
}

void Resize_Logo(Widget *thisw)
{
	//float minsz = (float)fmin(g_width, g_height);
	float minsz = (float)120;

	thisw->m_pos[0] = g_width / 2 - minsz / 2;
	thisw->m_pos[1] = g_height / 2 - minsz / 2;
	thisw->m_pos[2] = g_width / 2 + minsz / 2;
	thisw->m_pos[3] = g_height / 2 + minsz / 2;
}

void Draw_LoadBar()
{
	float pct = (float)(g_lastLSp + g_lastLTex) / (float)(g_spriteload.size() + g_texload.size());

	EndS();
	UseS(SHADER_COLOR2D);
	Shader* s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	
	float bar[4] =
	{
		(float)g_width/2 - 50,
		(float)g_height/2 + 30,
		(float)g_width/2 + 100,
		(float)g_height/2 + 30 + 3
	};
	
	DrawSquare(0, 0, 0, 1, bar[0]-1, bar[1]-1, bar[2]+1, bar[3]+1, g_gui.m_crop);
	DrawSquare(0, 1, 0, 1, bar[0], bar[1], bar[0] + pct * (bar[2]-bar[0]), bar[3], g_gui.m_crop);


	EndS();
	CHECKGLERROR();
	Ortho(g_width, g_height, 1, 1, 1, 1);
}

void AnyKey(int k)
{
	if(g_mode == APPMODE_LOGO)
		SkipLogo();
}

void FillGUI()
{
	//DrawSceneFunc = DrawScene;
	//DrawSceneDepthFunc = DrawSceneDepth;

	Log("2.1\r\n");


	GUI* gui = &g_gui;

	//TODO get rid of unnecessary assign funcs and just assign vars manually
	gui->assignkey(SDL_SCANCODE_F1, SaveScreenshot, NULL);
	//gui->assignkey(SDL_SCANCODE_F2, LogPathDebug, NULL);
	gui->assignlbutton(MouseLDown, MouseLUp);
	gui->assignrbutton(MouseRDown, MouseRUp);
	gui->assignmousemove(MouseMove);
	gui->assignanykey(AnyKey, NULL);

	Log("2.1.3");

	Log("2.2");
	

	// Loading ViewLayer

	Log("2.2");
	
	
	gui->add(new ViewLayer(gui, "logo"));
	ViewLayer* logoview = (ViewLayer*)gui->get("logo");
	logoview->add(new Image(logoview, "", "gui/mmbg.jpg", false, Resize_Fullscreen));
	//logoview->add(new Image(logoview, "", "gui/centerp/gears-2.png", true, Resize_AppLogo));
	//logoview->add(new Text(logoview, "app title", RichText(TITLE), MAINFONT32, Resize_AppTitle));
	logoview->add(new Image(logoview, "logo", "gui/transp.png", true, Resize_Logo, 1, 1, 1, 0));

	gui->add(new ViewLayer(gui, "loading"));
	ViewLayer* loadview = (ViewLayer*)gui->get("loading");

	Log("2.3");
	

	loadview->add(new Image(loadview, "loadbg", "gui/mmbg.jpg", false, Resize_Fullscreen));
	//loadview->add(new Image(loadview, "", "gui/centerp/gears-2.png", true, Resize_AppLogo));
	//loadview->add(new Text(loadview, "app title", RichText(TITLE), MAINFONT32, Resize_AppTitle));
	loadview->add(new Text(loadview, "status", RichText("Loading..."), MAINFONT8, Resize_LoadingStatus));
	loadview->add(new InsDraw(loadview, Draw_LoadBar));

	Log("2.4");
	

	FillMenu();

	Log("2.5");
	

	Log("2.6");
	

	Log("2.7");
	
	FillMess();
	
	gui->hideall();
	//gui->show("loading");
	gui->show("logo");
}
