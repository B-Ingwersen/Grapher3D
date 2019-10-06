double gXCenter = 0;
double gYCenter = 0;
double pixScale = 100; // pixels per unit

void printFloat( double value, int x, int y, char * textBuffer, int maxLen, drawData * window );

void drawGraph2DFunction( drawData drawInfo ) {
	drawData * graphArea = (drawData*)(drawInfo.dataPointer);
	Background( 0xFF, graphArea -> window, onScreen );
	char * gridTextVals = gridValTextBuffer;

	double scale = 1 / pixScale;

	int width = graphArea -> arguments[3];
	int height = graphArea -> arguments[4];

	int yPixCenter = height / 2 + gYCenter * pixScale;
	int xPixCenter = width / 2 - gXCenter * pixScale;

	double mag = log( scale ) / log( 10 );
	double pixMag = -mag;
	double gridPixScale = pow( 10, pixMag - floor(pixMag) + 1);
	double gridInterval = pow( 10, 1 - floor(pixMag) );
	if ( pixMag - floor(pixMag) < log(5) / log (10) ) {
		gridPixScale *= 5;
		gridInterval *= 5;
	}
	else if ( pixMag - floor(pixMag) < log(2) / log (10) ) {
		gridPixScale *= 2;
		gridInterval *= 2;
	}

	double subGridPixScale = gridPixScale / 5;
	int imin = - xPixCenter / subGridPixScale - 1;
	int imax = (width - xPixCenter) / subGridPixScale + 1;
	int i;
	for ( i = imin; i < imax; i++ ) {
		Line( xPixCenter + subGridPixScale * i, 0, xPixCenter + subGridPixScale * i, graphArea -> arguments[4], 0xE0E0E0, graphArea -> window, onScreen );
	}

	int jmin = - yPixCenter / subGridPixScale - 1;
	int jmax = (height - yPixCenter) / subGridPixScale + 1;
	int j;
	for ( j = jmin; j < jmax; j++ ) {
		Line( 0, yPixCenter + subGridPixScale * j, graphArea -> arguments[3], yPixCenter + subGridPixScale * j + 1, 0xE0E0E0, graphArea -> window, onScreen );
	}

	imin = - xPixCenter / gridPixScale - 1;
	imax = (width - xPixCenter) / gridPixScale + 1;
	for ( i = imin; i < imax; i++ ) {
		if ( i % 2 == 0 ) {
			Line( xPixCenter + gridPixScale * i, 12, xPixCenter + gridPixScale * i, graphArea -> arguments[4], 0xA0A0A0, graphArea -> window, onScreen );
			if ( xPixCenter + gridPixScale * i - 24 > 0 && xPixCenter + gridPixScale * i - 24 < graphArea -> arguments[3] ) {
			printFloat( i * gridInterval, xPixCenter + gridPixScale * i - 24, 2, gridTextVals + 8 * (i - imin), 8, graphArea ); }
		}
		else {
			Line( xPixCenter + gridPixScale * i, 0, xPixCenter + gridPixScale * i, graphArea -> arguments[4], 0xA0A0A0, graphArea -> window, onScreen );
		}
	}

	jmin = - yPixCenter / gridPixScale - 1;
	jmax = (height - yPixCenter) / gridPixScale + 1;
	for ( j = jmin; j < jmax; j++ ) {
		if ( j % 2 == 0 ) {
			Line( 48, yPixCenter + gridPixScale * j, graphArea -> arguments[3], yPixCenter + gridPixScale * j + 1, 0xA0A0A0, graphArea -> window, onScreen );
			if ( yPixCenter + gridPixScale * j - 5 > 0 && yPixCenter + gridPixScale * j - 5 < graphArea -> arguments[4] ) {
			printFloat( -j * gridInterval, 2, yPixCenter + gridPixScale * j - 5, gridTextVals + 8 * (j - jmin) + 512, 8, graphArea ); }
		}
		else {
			Line( 0, yPixCenter + gridPixScale * j, graphArea -> arguments[3], yPixCenter + gridPixScale * j + 1, 0xA0A0A0, graphArea -> window, onScreen );
		}
	}

	Rectangle( 48, yPixCenter - 1, graphArea -> arguments[3] - 48, 2, 0, graphArea -> window, onScreen );
	Rectangle( xPixCenter - 1, 12, 2, graphArea -> arguments[4] - 12, 0, graphArea -> window, onScreen );

	for ( i = 0; i < numOfGraphObjs; i++ ) {
		if ( graphObjects[i].d2D && graphObjects[i].visible ) {
			drawMathObject( graphObjects + i, graphArea, graphObjects[i].color, onScreen );
		}
	}

	DrawIntWindowShadow( graphArea, 20, 128, 20, 0, 20, 0, onScreen );
}

void drawGraph2D(drawData * graphArea) {
	addEventNoRepeat(drawGraph2DFunction, 100, NULL, graphArea, NULL, 0, NULL, graphRedraw );
}


void drag2DGraphArea( drawData * graphArea ) {
	gXCenter -= deltaMouseX / pixScale;
	gYCenter += deltaMouseY / pixScale;
	drawGraph( graphArea );
}

void zoom2DGraphArea( drawData * graphArea ) {
	if ( scrollY != 0 ) {
		int windowMouseX = mouseX - graphArea -> window -> x;
		int windowMouseY = mouseY - graphArea -> window -> y;

		double scale = 1 / pixScale;

		int width = graphArea -> arguments[3];
		int height = graphArea -> arguments[4];

		int yPixCenter = height / 2 + gYCenter * pixScale;
		int xPixCenter = width / 2 - gXCenter * pixScale;

		double zoom = 1.05;
		if ( scrollY >= 1 ) { 
			gXCenter += (windowMouseX - width / 2) * ( 1 / pixScale - 1 / (zoom * pixScale) );
			gYCenter -= (windowMouseY - height / 2) * ( 1 / pixScale - 1 / (zoom * pixScale) );
			pixScale *= zoom;
		}
		else if ( scrollY <= -1 ) {
			gXCenter += (windowMouseX - width / 2) * ( 1 / pixScale - zoom / pixScale );
			gYCenter -= (windowMouseY - height / 2) * ( 1 / pixScale - zoom / pixScale );
			pixScale /= zoom;
		}
		drawGraph( graphArea );
	}
}

void grapher2DRecenter( drawData * graphArea ) {
	gXCenter = 0;
	gYCenter = 0;
	pixScale = 100;

	drawGraph(graphArea);
}

void grapher2DResizeFunction() {
	drawGraph( graphArea );
	addNoOpEvent();
}

void setup2DGraph() {
	addWindowEventHandler( graphArea, MOUSE_SCROLL, zoom2DGraphArea );
	addWindowEventHandler( graphArea, MOUSE_DRAGGED, drag2DGraphArea );
	drawGraph = drawGraph2D;
	grapherRecenterFunction = grapher2DRecenter;
	sizeChangeFunction = grapher2DResizeFunction;
}
