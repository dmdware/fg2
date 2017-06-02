


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
#include "graph.h"
#include "shader.h"
#include "../math/vec3f.h"
#include "../window.h"
#include "../gui/gui.h"

Graph g_graph;
int g_gender = 1;
float g_age = 25;
float g_heightcm = 160;
double g_weightkcal = 200 * 3500.0f;

void DrawGraph(unsigned long long reqstart, Graph* g, float left, float top, float right, float bottom, float highest, float* color)
{
	EndS();
	UseS(SHADER_COLOR2D);
	Shader* s = &g_shader[g_curS];

	char note[32] = "";
	g->points.push_back(g_weightkcal);
	g->timestamps.push_back(GetTicks());
	g->notes.push_back(note);
	
	g->points.push_back(g_weightkcal);
	g->timestamps.push_back(GetTicks());
	g->notes.push_back(note);

	//float highest = 0;
	float lowest = -1;
	unsigned long long timestart = 0;
	unsigned long long timeend = 0;

	{
		auto pit=g->points.begin();
		auto tit=g->timestamps.begin();

		for(; pit!=g->points.end(); pit++, tit++)
		{
			if(*tit < reqstart)
				continue;

			if(*pit < lowest || lowest < 0)
				lowest = *pit;

			if(*pit > highest)
				highest = *pit;
		
			if(timestart == 0 || *tit < timestart)
				timestart = *tit;

			if(*tit > timeend)
				timeend = *tit;
		}
	}

	unsigned long long timelength = timeend - timestart;

	//float xadv = (right-left) / (float)(g->points.size()-1);
	float yadv = (bottom-top) / (highest-lowest);

	std::vector<Vec3f> ps;
	ps.reserve( g->points.size() );
	
	auto pit=g->points.begin();
	auto tit=g->timestamps.begin();

	left += 100;

	for(; pit!=g->points.end(); pit++, tit++)
	{
		float x = (right-left) * (float)(*tit - timestart) / (float)timelength;
		ps.push_back( Vec3f( left + x, top + (highest - *pit)*yadv, 0.0f ) );
	}

	left -= 100;

	if(!color)
		glUniform4f(s->slot[SSLOT_COLOR], 0.1f, 0.9f, 0.1f, 1.0f);
	else
		glUniform4f(s->slot[SSLOT_COLOR], color[0], color[1], color[2], 1.0f);
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	
	char highc[32];
	sprintf(highc, "%f", (float)(highest)/3500);
	char lowc[32];
	sprintf(lowc, "%f", (float)(highest+lowest)/2/3500);
	RichText highr = RichText(highc);
	RichText lowr = RichText(lowc);

	if(ps.size() <= 0)
		goto end;

#ifdef PLATFORM_GL14
	glVertexPointer(3, GL_FLOAT, sizeof(float)*0, &ps[0]);
#endif
	
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, sizeof(float)*0, &ps[0]);
#endif
	//glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, &vertices[2]);
	//glTexCoordPointer(2, GL_FLOAT, sizeof(float)*0, &vertices[2]);
	glDrawArrays(GL_LINE_STRIP, 0, ps.size());
	CHECKGLERROR();

	EndS();	//corpd fix

	CHECKGLERROR();
	Ortho(g_width, g_height, 1.0f, 1.0f, 1.0f, 1.0f);
	
	DrawLine(MAINFONT8, left, top, &highr);
	DrawLine(MAINFONT8, left, (top + bottom) / 2, &lowr);

	auto nit=g->notes.begin();
	tit=g->timestamps.begin();

	for(auto pit2=ps.begin(); pit2!=ps.end(); pit2++, nit++, tit++)
	{
		if(*tit < reqstart)
			continue;

		RichText r = RichText(nit->c_str());
		DrawLine(MAINFONT8, pit2->x, pit2->y, &r);
	}

end:
	g->points.pop_back();
	g->timestamps.pop_back();
	g->notes.pop_back();

	g->points.pop_back();
	g->timestamps.pop_back();
	g->notes.pop_back();
}

void DrawGraphs()
{
	DrawGraph(0, &g_graph, 0, 0, g_width, g_height/4);
	DrawGraph(GetTicks() - 1000*60*60*24*7, &g_graph, 0, g_height/4, g_width, g_height/4*2);
	DrawGraph(GetTicks() - 1000*60*60*24, &g_graph, 0, g_height/4*2, g_width, g_height/4*3);
	DrawGraph(GetTicks() - 1000*60*60*8, &g_graph, 0, g_height/4*3, g_width, g_height);

	EndS();
	UseS(SHADER_COLOR2D);

	Shader* s = &g_shader[g_curS];

	GUI* gui = &g_gui;
	float* crop = gui->m_crop;
	
	glUniform1f(s->slot[SSLOT_WIDTH], g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], g_height);
	glUniform4f(s->slot[SSLOT_COLOR], 0.5f, 0.5f, 0.5f, 1.0f);
	
	DrawLine(0.5f, 0.5f, 1.0f, 1.0f, 0, g_height/4, g_width, g_height/4, crop);
	DrawLine(0.5f, 0.5f, 1.0f, 1.0f, 0, g_height/4*2, g_width, g_height/4*2, crop);
	DrawLine(0.5f, 0.5f, 1.0f, 1.0f, 0, g_height/4*3, g_width, g_height/4*3, crop);

	EndS();
	Ortho(g_width, g_height, 1, 1, 1, 1);
	
	RichText r1 = RichText("Lifetime");
	RichText r2 = RichText("Last 7 Days");
	RichText r3 = RichText("Last 24 Hours");
	RichText r4 = RichText("Last 8 Hours");
	float green[4] = {0,1,0,1}; 

	DrawLine(MAINFONT8, 400, 0, &r1, green, -1);
	DrawLine(MAINFONT8, 400, g_height/4, &r2, green, -1);
	DrawLine(MAINFONT8, 400, g_height/4*2, &r3, green, -1);
	DrawLine(MAINFONT8, 400, g_height/4*3, &r4, green, -1);

	Graph* g = &g_graph;
	
	char c5[128];
	RichText r5;

	if(g->points.size() == 0)
		goto end;

	float lastweightkcal = *g->points.rbegin();
	unsigned long long timepass = 1000*60*60;
	double hourchangekcal = 0;
	float heightinch = g_heightcm * 0.393701f;

	if(g_gender != 1)	//female
	{
		 //BMR 655 + ( 4.35 x weight in pounds ) + ( 4.7 x height in inches ) - ( 4.7 x age in years )
		hourchangekcal = ( 655 + (4.35f * lastweightkcal / 3500.0f) + ( 4.7f * heightinch ) - (4.7f * g_age) ) * ((float)timepass / (24.0f * 60.0f * 60.0f * 1000.0f)) * 1.2f;
	}
	else
	{
		//BMR 66 + ( 6.23 x weight in pounds ) + ( 12.7 x height in inches ) - ( 6.8 x age in years )
		hourchangekcal = ( 66 + (6.23f * lastweightkcal / 3500.0f) + ( 12.7f * heightinch ) - (6.8f * g_age) ) * ((float)timepass / (24.0f * 60.0f * 60.0f * 1000.0f)) * 1.2f;
	}

	sprintf(c5, "loss rate: %lf kcal / hour    =     %lf kcal / sec    =     %lf lbs / sec      (%lf lbs / week)       ", 
		hourchangekcal, 
		(double)hourchangekcal / (double)60.0 / (double)60.0,
		(double)hourchangekcal / (double)60.0 / (double)60.0 / (double)3500.0,
		hourchangekcal * 24 * 7 / (double)3500.0f);
	r5 = RichText(c5);
	DrawLine(MAINFONT8, 100, g_height/4*3+30, &r5, NULL, -1);

	sprintf(c5, "current weight: %0.7lf lbs",
		(double)(g_weightkcal/3500));
	r5 = RichText(c5);
	DrawLine(MAINFONT8, 600, g_height/4*3+60, &r5, NULL, -1);

end:

	EndS();
	Ortho(g_width, g_height, 1, 1, 1, 1);
}

void UpdGraph()
{
	Graph* g = &g_graph;

	if(!g->points.size())
		return;

	unsigned long long lasttime = *g->timestamps.rbegin();
	unsigned long long timepass = GetTicks() - lasttime;
	double lastweightkcal = *g->points.rbegin();

	unsigned long long now = GetTicks();

	float changelbs = 0;
	double changekcal = 0;
	double heightinch = g_heightcm * 0.393701f;

	if(g_gender != 1)	//female
	{
		 //BMR 655 + ( 4.35 x weight in pounds ) + ( 4.7 x height in inches ) - ( 4.7 x age in years )
		changekcal = ( (double)655 + ((double)4.35f * (double)lastweightkcal / (double)3500.0f) + ( (double)4.7f * (double)heightinch ) - ((double)4.7f * g_age) ) * ((double)timepass / (24.0f * 60.0f * 60.0f * 1000.0f)) * 1.2f;
	}
	else
	{
		//BMR 66 + ( 6.23 x weight in pounds ) + ( 12.7 x height in inches ) - ( 6.8 x age in years )
		changekcal = ( (double)66 + ((double)6.23f * (double)lastweightkcal / (double)3500.0f) + ( (double)12.7f * (double)heightinch ) - ((double)6.8f * g_age) ) * ((double)timepass / (24.0f * 60.0f * 60.0f * 1000.0f)) * 1.2f;
	}

	//changelbs = - changekcal / 3500.0f;
	//g_weightlbs = lastweightlbs + changelbs;
	g_weightkcal = lastweightkcal - changekcal;
}

void LoadData()
{
	char datfull[DMD_MAX_PATH+1];
	FullWritePath("data.dat", datfull);
	FILE* fp = fopen(datfull, "rb");

	if(!fp)
		return;
	
	fread(&g_gender, sizeof(int), 1, fp);
	fread(&g_age, sizeof(float), 1, fp);
	fread(&g_heightcm, sizeof(float), 1, fp);
	
	Graph* g = &g_graph;

	while(!feof(fp))
	{
		char note[32];
		unsigned long long timestamp;
		float weightkcal;
		fread(&timestamp, sizeof(unsigned long long), 1, fp);
		fread(&weightkcal, sizeof(float), 1, fp);
		fread(note, sizeof(char), 32, fp);
		note[31] = 0;
		
		g->points.push_back(weightkcal);
		g->timestamps.push_back(timestamp);
		g->notes.push_back(note);
	}

	fclose(fp);
}

void SaveAdd(const char* paramnote)
{
	char datfull[DMD_MAX_PATH+1];
	FullWritePath("data.dat", datfull);
	FILE* fp = fopen(datfull, "ab");

	unsigned long long now = GetTicks();
	char note[32];
	strcpy(note, paramnote);
	fwrite(&now, sizeof(unsigned long long), 1, fp);
	float fkcal = g_weightkcal;
	fwrite(&fkcal, sizeof(float), 1, fp);
	fwrite(note, sizeof(char), 32, fp);

	Graph* g = &g_graph;

	g->points.push_back(g_weightkcal);
	g->timestamps.push_back(now);
	g->notes.push_back(note);

	fclose(fp);
}

void SaveSettings()
{	
	char datfull[DMD_MAX_PATH+1];
	FullWritePath("data.dat", datfull);
	FILE* fp = fopen(datfull, "wb");
	
	fwrite(&g_gender, sizeof(int), 1, fp);
	fwrite(&g_age, sizeof(float), 1, fp);
	fwrite(&g_heightcm, sizeof(float), 1, fp);
	
	Graph* g = &g_graph;
	auto wit = g->points.begin();
	auto tit = g->timestamps.begin();
	auto nit = g->notes.begin();

	while(wit != g->points.end())
	{
		char note[32];
		strncpy(note, nit->c_str(), 31);
		fwrite(&*tit, sizeof(unsigned long long), 1, fp);
		fwrite(&*wit, sizeof(float), 1, fp);
		fwrite(note, sizeof(char), 32, fp);

		wit++;
		tit++;
		nit++;
	}
	
	unsigned long long now = GetTicks();
	char note[32] = "Settings";
	fwrite(&now, sizeof(unsigned long long), 1, fp);
	float fkcal = g_weightkcal;
	fwrite(&fkcal, sizeof(float), 1, fp);
	fwrite(note, sizeof(char), 32, fp);

	if(g->points.size() == 0)
	{
		g->points.push_back(g_weightkcal);
		g->timestamps.push_back(now);
		g->notes.push_back(note);
	}

	fclose(fp);
}