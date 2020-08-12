
#ifndef _GRAPH_MANAGER_2D_H_
#define _GRAPH_MANAGER_2D_H_

extern double gXCenter;
extern double gYCenter;
extern double pixScale; // pixels per unit

void printFloat( double value, int x, int y, char * textBuffer, int maxLen, drawData * window );

void drawGraph2DFunction( drawData drawInfo );

void drawGraph2D(drawData * graphArea);

void drag2DGraphArea( drawData * graphArea );

void zoom2DGraphArea( drawData * graphArea );

void grapher2DRecenter( drawData * graphArea );

void grapher2DResizeFunction();

void setup2DGraph();

#endif // _GRAPH_MANAGER_2D_H_
