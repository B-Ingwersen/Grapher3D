#ifndef _GRAPH_DATA_H_
#define _GRAPH_DATA_H_

#include "Math/Objects.h"

#define GRAPH_TYPE_2D 0
#define GRAPH_TYPE_3D 1

extern char gridValTextBuffer[1024];
extern int graphType;
extern int numOfGraphObjs;

extern mathObject * graphObjects;
extern drawData * graphArea;

extern drawData * sideBar;
extern drawData * dataBar;
extern void (*drawGraph)( drawData * graphArea );
extern void (*grapherRecenterFunction)( drawData * graphArea);
extern int graphRedraw[2];

#endif // _GRAPH_DATA_H_