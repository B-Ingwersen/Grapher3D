void createUserInterface(void * argument) {
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