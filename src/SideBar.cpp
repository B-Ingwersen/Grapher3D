#include "SideBar.h"
#include "GraphData.h"
#include "GraphicsLibrary/Extensions/BasicGUIElements.h"
#include "GraphicsLibrary/Extensions/Text.h"
#include "GraphicsLibrary/Extensions/WindowShadows.h"

int currentColor = 5;
int graphColors[] = { 0xFF0000, 0x00DD00, 0x0000EE, 0xFF8C00, 0xE76AA1 };
char deleteButtonText[] = "X";

int generateNextColor() {
	currentColor++;
	if (currentColor > 4) {
		currentColor = 0;
	}
	return graphColors[currentColor];
}

void deleteElem( int index ) {
	delete graphObjects[ index ].text;
	(mathObjTypes[graphObjects[ index ].objType].deleteFunction)( graphObjects[ index ].mathElem );

	int i;
	for ( i = index; i < numOfGraphObjs - 1; i++ ) {
		copyMathObject( graphObjects + i, graphObjects + i + 1 );
	}
	numOfGraphObjs--;
}

void deleteElemHandler( drawData * deleteButton ) {
	drawData * elemBar = getParentWindow(deleteButton);
	int index = elemBar -> arguments[7];
	deleteElem( index );
	int i;
	for ( i = 0; i < numOfGraphObjs - index; i++ ) {
		((drawData*)((elemBar + i) -> dataPointer) + 2) -> dataPointer = graphObjects[index + i].text;
		((drawData*)((elemBar + i) -> dataPointer) + 1) -> arguments[4] = ((drawData*)((elemBar + i + 1) -> dataPointer) + 1) -> arguments[4];
		(elemBar + i) -> arguments[0] = (elemBar + i + 1) -> arguments[0];
	}
	getParentWindow( elemBar ) -> arguments[0] -= 1;
	deleteWindow( elemBar + i );
	displayWindow( sideBar, onScreen );
	drawGraph( graphArea );
}

void changeElemVisible( drawData * selector ) {
	mathObject * obj = &graphObjects[ getParentWindow( selector ) -> arguments[7] ];
	if ( obj -> visible ) {
		obj -> visible = false;
		selector -> colorInfo[0] = 0xD0;
		changeBackgroundColor( selector, 0xD0 );
		displayWindow( selector, onScreen );
	}
	else {
		obj -> visible = true;
		selector -> colorInfo[0] = 0xA0;
		changeBackgroundColor( selector, 0xA0 );
		displayWindow( selector, onScreen );
	}
	drawGraph( graphArea );
}

void addSideBarElem(mathObject * obj) {
	drawData * newElemBar = createWindow( sideBar, 5, 5 + 20 * numOfGraphObjs, sideBar -> arguments[3] - 18, 15 );
	int color = graphObjects[numOfGraphObjs].color;
	int r = ((( color >> 16 ) & 0xFF) + 230 * 4) / 5;
	int g = ((( color >> 8 ) & 0xFF)  + 230 * 4) / 5;
	int b = ((( color >> 0 ) & 0xFF)  + 230 * 4) / 5;
	color = (r << 16) + (g << 8) + b;
	addRectangle( 0, 0, sideBar -> arguments[3], 15, color, newElemBar );
	drawData * equText = addText2( obj -> text, 17, 2 + 9, sideBar -> arguments[3], 9, 0, 0, newElemBar );

	newElemBar -> arguments[7] = numOfGraphObjs;
	drawData * selector = createButton( newElemBar, 1, 1, 13, 13, noText, changeElemVisible, 1 );
	changeBackgroundColor( selector, 0xA0 );
	selector -> colorInfo[0] = 0xA0;
	addWindowShadow2( selector, newElemBar, 2, 60 );

	drawData * deleteButton = createButton( newElemBar, sideBar -> arguments[3] - 33, 1, 13, 13, noText, deleteElemHandler, 1 );
	addText2( deleteButtonText, 4, 2 + 9, 13, 9, 0, 0, deleteButton );
	addWindowShadow2( deleteButton, newElemBar, 2, 60 );

	displayWindow( newElemBar, onScreen );
}
