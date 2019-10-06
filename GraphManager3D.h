double cx = 0;
double cy = 0;
double cz = 0;
double dist = 1;
double scale = 41;

double xyRotate = M_PI/4;
double zRotate = M_PI/4;

double dx;
double dy;

double vx;
double vy;
double vz;

double d;

double xVecx;
double xVecy;
double xVecz;

double yVecx;
double yVecy;
double yVecz;

void grapherLine(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen );

void drawGraph3DFunction( drawData drawInfo ) {
	drawData * graphArea = (drawData*)(drawInfo.dataPointer);
	Background( 0xFF, graphArea -> window, onScreen );

	dx = cos(xyRotate);
	dy = sin(xyRotate);

	vx = cos(xyRotate) * cos(zRotate);
	vy = sin(xyRotate) * cos(zRotate);
	vz = sin(zRotate);
	
	int width2 = graphArea -> arguments[3] / 2;
	int height2 = graphArea -> arguments[4] / 2;

	d = vx * cx + vy * cy + vz * cz;

	xVecx = -dy;
	xVecy = dx;
	xVecz = 0;

	yVecx = xVecz * vy - xVecy * vz;
	yVecy = xVecx * vz - xVecz * vx;
	yVecz = xVecy * vx - xVecx * vy;

	double x,y,z, t, pixX1,pixY1,pixX2,pixY2;

	x = 0; y = -10 * dist; z = 0;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX1 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY1 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	x = 0; y = 10 * dist; z = 0;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX2 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY2 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	grapherLine( pixX1, pixY1, pixX2, pixY2 , 0, graphArea -> window, onScreen );

	x = -10 * dist; y = 0; z = 0;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX1 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY1 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	x = 10 * dist; y = 0; z = 0;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX2 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY2 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	grapherLine( pixX1, pixY1, pixX2, pixY2 , 0, graphArea -> window, onScreen );

	x = 0; y = 0; z = -10 * dist;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX1 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY1 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	x = 0; y = 0; z = 10 * dist;
	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	pixX2 = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	pixY2 = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
	grapherLine( pixX1, pixY1, pixX2, pixY2 , 0, graphArea -> window, onScreen );

	int i;
	for ( i = 0; i < numOfGraphObjs; i++ ) {
		if ( graphObjects[i].d3D && graphObjects[i].visible ) {
			drawMathObject( graphObjects + i, graphArea, graphObjects[i].color, onScreen );
		}
	}

	DrawIntWindowShadow( graphArea, 20, 128, 20, 0, 20, 0, onScreen );
}

void drawGraph3D(drawData * graphArea) {
	addEventNoRepeat(drawGraph3DFunction, 100, NULL, graphArea, NULL, 0, NULL, graphRedraw );
}


void catchWindow( drawData * callingWindow ) {
	caughtWindow = callingWindow;
}
void drag3DGraphArea( drawData * graphArea ) {
	xyRotate -= ((double)(deltaMouseX)) / 250;
	zRotate += ((double)(deltaMouseY)) / 250;

	if (zRotate > M_PI/2) { zRotate = M_PI/2; }
	if (zRotate < -M_PI/2) { zRotate = -M_PI/2; }

	if (xyRotate < 0) { xyRotate += M_PI*2; }
	if (xyRotate > M_PI*2) { xyRotate -= M_PI*2; }

	drawGraph( graphArea );
}
void zoom3DGraphArea( drawData * graphArea ) {
	if ( scrollY != 0 ) {
		if (scrollY > 0) { dist *= 0.9; }
		if (scrollY < 0) { dist /= 0.9; }
		drawGraph( graphArea );
	}
}

void grapher3DRecenter( drawData * graphArea ) {
	cx = 0;
	cy = 0;
	cz = 0;
	dist = 1;
	xyRotate = M_PI/4;
	zRotate = M_PI/4;

	drawGraph(graphArea);
}

void grapher3DResizeFunction() {
	int width =  graphArea -> arguments[3];
	int height =  graphArea -> arguments[4];

	int dimension;
	if (width < height) { dimension = width; }
	else { dimension = height; }
	if (dimension < 100) { dimension = 100; }

	scale = dimension / 21;
	drawGraph( graphArea );
}

void setup3DGraph() {
	int width =  graphArea -> arguments[3];
	int height =  graphArea -> arguments[4];

	int dimension;
	if (width < height) { dimension = width; }
	else { dimension = height; }
	if (dimension < 100) { dimension = 100; }

	scale = dimension / 21;

	addWindowEventHandler( graphArea, MOUSE_SCROLL, zoom3DGraphArea );
	addWindowEventHandler( graphArea, MOUSE_DRAGGED, drag3DGraphArea );
	drawGraph = drawGraph3D;
	grapherRecenterFunction = grapher3DRecenter;
	sizeChangeFunction = grapher3DResizeFunction;
}
