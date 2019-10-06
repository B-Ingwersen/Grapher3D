#define GRAPH_TYPE_2D 0
#define GRAPH_TYPE_3D 1

char gridValTextBuffer[1024];

int graphType = GRAPH_TYPE_2D;

int numOfGraphObjs = 0;
mathObject * graphObjects;

drawData * graphArea;
drawData * sideBar;
drawData * dataBar;
void (*drawGraph)( drawData * graphArea );
void (*grapherRecenterFunction)( drawData * graphArea);
int graphRedraw[2];