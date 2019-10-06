//g++ Grapher3D.cpp -lSDL2 -lpthread -o Grapher3D

#include <stdlib.h>
#include "../GraphicsLibrary/DrawingLibrary.h"
#include "../GraphicsLibrary/Extensions/Text.h"
#include "../GraphicsLibrary/Extensions/BasicGUIElements.h"
#include "../GraphicsLibrary/Extensions/WindowShadows.h"
#include "../GraphicsLibrary/Extensions/DataOperations.h"
#include "Math/ExpressionAnalyzer.h"
#include "Math/Objects.h"

/*#include <stdlib.h>
#include "../SDL/SDLDrawingLibrary.h"
#include "../SDL/extensions/Text.h"*/
//#include "../SDL/extensions/SDLBasicGUIElements.h"
/*#include "../SDL/extensions/SDLWindowShadows.h"
#include "../Libraries/mathStuff/mathStuff.h"*/

char gridValTextBuffer[1024];

#define GRAPH_TYPE_2D 0
#define GRAPH_TYPE_3D 1

int graphType = GRAPH_TYPE_2D;

int numOfGraphObjs = 0;
mathObject * graphObjects;

drawData * graphArea;
drawData * sideBar;
drawData * dataBar;
void (*drawGraph)( drawData * graphArea );
void (*grapherRecenterFunction)( drawData * graphArea);
int graphRedraw[2];

#include "GraphManager3D.h"
#include "GraphManager2D.h"

#include "DrawFunctions.h"
#include "SideBar.h"
#include "InputManager.h"

void printFloat( double value, int x, int y, char * textBuffer, int maxLen, drawData * window ) {
	int mag = floor( log( abs(value) ) / log ( 10 ) );
	float coef = value / pow( 10, mag );

	int digit = 6;
	textBuffer[ digit ] = 48 + abs(mag);
	digit--;
	if (mag < 0) {
		textBuffer[ digit ] = 45;
		digit--;
	}
	textBuffer[ digit ] = 101;

	if ( coef < 0 ) {
		textBuffer[0] = 45;
		coef *= -1;

		textBuffer[ 1 ] = 48 + (int)(coef);
		coef -= floor( coef );
		coef *= 10;
		textBuffer[ 2 ] = 46;

		int i;
		for ( i = 3; i < digit; i++ ) {
			textBuffer[ i ] = 48 + (int)(coef);
			coef -= floor( coef );
			coef *= 10;
		}
	}
	else {
		textBuffer[ 0 ] = 48 + (int)(coef);
		coef -= floor( coef );
		coef *= 10;
		textBuffer[ 1 ] = 46;

		int i;
		for ( i = 2; i < digit; i++ ) {
			textBuffer[ i ] = 48 + (int)(coef);
			coef -= floor( coef );
			coef *= 10;
		}
	}

	textBuffer[ 7 ] = 0;
	Text( textBuffer, x, y, 100, 1, 0, 0, window -> window, onScreen );
}

void grapherGlobalKeyboardHandler( int keycode ) {
	drawData * equationEnterBox = (drawData *)(dataBar -> dataPointer) + 3;
	int key = handleKeyDown(keycode);
	if ( CTRL_DOWN && !ALT_DOWN ) {
		if ( key == 32 ) {
			callWindowEventHandler( equationEnterBox, MOUSE_BUTTON );
		}
		else if ( key == 104 ) {
			//cout << "recenter" << endl;
			grapherRecenterFunction(graphArea);
		}
		else if ( key == 51 ) {
			if (graphType != GRAPH_TYPE_3D) {
				setup3DGraph();
				graphType = GRAPH_TYPE_3D;
				drawGraph( graphArea );
			}
		}
		else if ( key == 50 ) {
			if (graphType != GRAPH_TYPE_2D) {
				setup2DGraph();
				graphType = GRAPH_TYPE_2D;
				drawGraph( graphArea );
			}
		}
	}
}

void createProgramStart(void * argument) {
	graphObjects = new mathObject[128];

	int bottomBarSize = 44;
	int sideBarSize = 400;

	graphArea = createWindow( parentWindow, sideBarSize, 0, parentWindow -> arguments[3] - sideBarSize, parentWindow -> arguments[4] - bottomBarSize );
	setHandlerState( MOUSE_SCROLL, MOUSE_LOCATION_HANDLER );
	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	addWindowEventHandler( graphArea, MOUSE_BUTTON, catchWindow );
	setResizeFunc( graphArea, resizeHW );
	setup2DGraph();

	dataBar = createWindow( parentWindow, 0, parentWindow -> arguments[4] - bottomBarSize, parentWindow -> arguments[3], bottomBarSize );
	addBackground( 0xFF, dataBar );
	drawData * dataBarDrag = createWindow( dataBar, 0, 0, dataBar -> arguments[3], 8 );
	addBackground( 0xE0, dataBarDrag );
	drawData * equationEnterBox = createOneLineTextBox( dataBar, 0, 8, dataBar -> arguments[3], bottomBarSize - 8, 2, addDataBarExpression );
	addWindowShadow2( dataBarDrag, equationEnterBox, 10, 50 );
	setResizeFunc( dataBar, resizeBW );
	setResizeFunc( dataBarDrag, resizeW );
	setResizeFunc( equationEnterBox, resizeW );

	sideBar = createWindow( parentWindow, 0, 0, sideBarSize, parentWindow -> arguments[4] - bottomBarSize );
	addBackground( 0xFF, sideBar );
	setResizeFunc( sideBar, resizeH );
	drawData * sideBarDrag = createWindow( sideBar, sideBarSize - 8, 0, 8, sideBar -> arguments[4] );
	addBackground( 0xE0, sideBarDrag );
	setResizeFunc( sideBarDrag, resizeH );

	drawGraph( graphArea );

	displayWindow( parentWindow, onScreen );
	globalKeyboardHandler = grapherGlobalKeyboardHandler;
};

int main(int argc, char* args[])
{
	createDrawingEnvironment(createProgramStart, NULL);
	return 0;
};
