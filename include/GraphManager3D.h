
#ifndef _GRAPH_MANAGER_3D_H_
#define _GRAPH_MANAGER_3D_H_

#include "GraphicsLibrary/Core.h"

extern double cx;
extern double cy;
extern double cz;
extern double dist;
extern double scale;

extern double xyRotate;
extern double zRotate;

extern double dx;
extern double dy;

extern double vx;
extern double vy;
extern double vz;

extern double d;

extern double xVecx;
extern double xVecy;
extern double xVecz;

extern double yVecx;
extern double yVecy;
extern double yVecz;

void grapherLine(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen );

void drawGraph3DFunction( drawData drawInfo );

void drawGraph3D(drawData * graphArea);

void catchWindow( drawData * callingWindow );

void drag3DGraphArea( drawData * graphArea );

void zoom3DGraphArea( drawData * graphArea );

void grapher3DRecenter( drawData * graphArea );

void grapher3DResizeFunction();

void setup3DGraph();

#endif // _GRAPH_MANAGER_3D_H_