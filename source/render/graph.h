


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






#ifndef GRAPH_H
#define GRAPH_H

#include "../platform.h"

class Graph
{
public:
	std::list<float> points;
	std::list<unsigned long long> timestamps;
	std::list<std::string> notes;

	Graph()
	{
	}
};

void DrawGraph(unsigned long long reqstart, Graph* g, float left, float top, float right, float bottom, float highest=0, float* color=NULL);
void DrawGraphs();
void LoadData();
void SaveSettings();
void SaveAdd(const char* paramnote);
void UpdGraph();

extern Graph g_graph;

extern int g_gender;
extern float g_age;
extern float g_heightcm;
extern double g_weightkcal;

#endif