
#include "GraphData.h"

char gridValTextBuffer[1024];
int graphType = GRAPH_TYPE_2D;
int numOfGraphObjs = 0;
int graphRedraw[2];

mathObject * graphObjects;

drawData * graphArea;
drawData * sideBar;
drawData * dataBar;
void (*drawGraph)( drawData * graphArea );
void (*grapherRecenterFunction)( drawData * graphArea);