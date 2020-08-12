
#ifndef _DRAW_FUNCTIONS_H_
#define _DRAW_FUNCTIONS_H_

#include "GraphicsLibrary/Core.h"

void drawGrapherLine(drawData drawInformation);

void grapherLine(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen );

void drawCurve( drawData drawInfo );

void drawParam( drawData drawInfo );

void drawPoint2D( drawData drawInfo );

void drawPoint3D( drawData drawInfo );

void draw2DExpression( drawData drawInformation );

void drawVector( drawData drawInfo );

void draw3DExpression( drawData drawInfo );

void printFloat( double value, int x, int y, char * textBuffer, int maxLen, drawData * window );

#endif // _DRAW_FUNCTIONS_H_
