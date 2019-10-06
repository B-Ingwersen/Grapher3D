unsigned char openParenthesis[] = "(";
unsigned char closeParenthesis[] = ")";
unsigned char comma[] = ",";
unsigned char equalSign[] = "=";
char curve[] = "curve";
char param[] = "param";
char point[] = "pt";
char vector[] = "vec";

mathObject * processMathObject( char * text, int len ) {
	mathObject * obj = new mathObject;

	int i;
	for ( i = 0; i < numOfGraphObjs; i++ ) {
		if ( compareDataNull( text, graphObjects[i].name ) ) {
			copyMathObject( obj, graphObjects + i );
			obj -> color = generateNextColor();
			obj -> text = text;
			return obj;
		}
	}

	obj -> color = generateNextColor();
	obj -> text = text;

	if ( compareDataNull( text, curve )) {
		int i,j;
		numberAnalysis num;
		i = parseData( openParenthesis, (unsigned char *)(text), 1, len );
		if ( i == -1 ) { delete obj; return NULL;}

		clearPredefVars();
		predefVariables[0] = {{116,0}, 1, 0};

		i++;
		curveData * curve = new curveData;
		curve -> x = analyzeMathExpression( text + i, len - i );
		if (curve -> x == NULL ) { delete curve; delete obj; return NULL; }

		i += curve -> x -> textLen + 1;
		curve -> y = analyzeMathExpression( text + i, len - i );
		if (curve -> y == NULL ) { delete curve; delete obj; return NULL; }

		i += curve -> y -> textLen + 1;
		curve -> z = analyzeMathExpression( text + i, len - i );
		if (curve -> z == NULL ) { delete curve; delete obj; return NULL; }

		i += curve -> z -> textLen + 1;
		num = analyzeNumber( text + i, len - i );
		if (!num.sucess) { delete curve; delete obj; return NULL; }
		curve -> startX = num.val;

		i += num.textLen + 1;
		num = analyzeNumber( text + i, len - i );
		if (!num.sucess) { delete curve; delete obj; return NULL; }
		curve -> endX = num.val;
		curve -> nSteps = 500;

		obj -> mathElem = curve;
		obj -> d2D = false;
		obj -> d3D = true;
		obj -> objType = CURVE;
	}
	else if ( compareDataNull( text, param )) {
		int i;
		numberAnalysis num;
		i = parseData( openParenthesis, (unsigned char *)(text), 1, len );
		if ( i == -1 ) { delete obj; return NULL;}

		clearPredefVars();
		predefVariables[0] = {{116,0}, 1, 0};

		i++;
		paramData * param = new paramData;
		param -> x = analyzeMathExpression( text + i, len - i );
		if (param -> x == NULL ) { delete param; delete obj; return NULL; }

		i += param -> x -> textLen + 1;
		param -> y = analyzeMathExpression( text + i, len - i );
		if (param -> y == NULL ) { delete param; delete obj; return NULL; }

		i += param -> y -> textLen + 1;
		num = analyzeNumber( text + i, len - i );
		if (!num.sucess) { delete param; delete obj; return NULL; }
		param -> startX = num.val;

		i += num.textLen + 1;
		num = analyzeNumber( text + i, len - i );
		if (!num.sucess) { delete param; delete obj; return NULL; }
		param -> endX = num.val;
		param -> nSteps = 500;

		obj -> mathElem = param;
		obj -> d2D = true;
		obj -> d3D = false;
		obj -> objType = PARAMETRIC;
	}
	else if ( compareDataNull( text, point )) {
		int i,j;
		numberAnalysis num;
		i = parseData( openParenthesis, (unsigned char *)(text), 1, len );
		if ( i == -1 ) { delete obj; return NULL;}

		pointData * point = new pointData;
		i++;
		num = analyzeNumber( text + i, len - i ); if (!num.sucess) { delete point; delete obj; return NULL; }
		point -> x = num.val;
		
		i += num.textLen + 1;
		num = analyzeNumber( text + i, len - i ); if (!num.sucess) { delete point; delete obj; return NULL; }
		point -> y = num.val;

		i += num.textLen + 1;
		if ( text[i-1] == 44 ) {
			num = analyzeNumber( text + i, len - i ); if (!num.sucess) { delete point; delete obj; return NULL; }
			point -> z = num.val;

			obj -> d3D = true;
			obj -> d2D = false;
			obj -> objType = POINT3D;
		}
		else { 
			point -> z = 0;
			obj -> d2D = true;
			obj -> d3D = false;
			obj -> objType = POINT2D;
		}
		obj -> mathElem = point;
	}
	else if ( compareDataNull( text, vector )) {
		clearPredefVars();

		int i,j;
		numberAnalysis num;
		i = parseData( openParenthesis, (unsigned char *)(text), 1, len );
		if ( i == -1 ) { delete obj; return NULL;}

		vectorData * vector = new vectorData;
		i++;

		for (j = 0; j < 8; j++) {
			num = analyzeNumber( text + i, len - i ); if (!num.sucess) { delete vector; delete obj; return NULL; }
			vector -> comps[j] = num.val;
			cout << num.val << endl;
		
			i += num.textLen + 1;
			if (text[i-1] == 41) {break;}
		}
		obj -> mathElem = vector;
		obj -> objType = VECTOR;
	}

	else {
		if (graphType == GRAPH_TYPE_2D) {
			clearPredefVars();
			predefVariables[0] = {{120,0}, 1, 0};

			obj -> mathElem = analyzeMathExpression( text, getStringLen(text) );
			obj -> d2D = true;
			obj -> d3D = false;
			if (obj -> mathElem == NULL) { delete obj; return NULL; }
			obj -> objType = EXPRESSION2D;
		}
		else if (graphType == GRAPH_TYPE_3D) {
			clearPredefVars();
			predefVariables[0] = {{120,0}, 1, 0};
			predefVariables[1] = {{121,0}, 1, 0};

			obj -> mathElem = analyzeMathExpression( text, getStringLen(text) );
			obj -> d3D = true;
			obj -> d2D = false;
			if (obj -> mathElem == NULL) { delete obj; return NULL; }
			obj -> objType = EXPRESSION3D;
		}
	}

	return obj;
}

bool checkNameRepeat( char * text ) {
	int i;
	for ( i = 0; i < numOfGraphObjs; i++ ) {
		if ( compareDataNull( text, graphObjects[i].name ) ) {
			return false;
		}
	}
	for ( i = 0; i < numOfPredefFuncs; i++ ) {
		if ( compareDataNull( text, predefFunctions[i].name ) ) {
			return false;
		}
	}
	for ( i = 0; i < numOfVars; i++ ) {
		if ( compareDataNull( text, predefVariables[i].name ) ) {
			return false;
		}
	}

	return true;
}

int nameIndex = 0;
char currentName[8] = {96,0,0,0,0,0,0,0};
void generateNewName( char * text ) {
	while (true) {
		currentName[nameIndex]++;
		int i;
		for ( i = 6; i >= 0; i-- ) {
			if (currentName[i] > 122) {
				currentName[i] = 97;
				currentName[i - 1]++;
				if (i == nameIndex) { nameIndex++; }
			}
		}

		if ( checkNameRepeat( currentName ) ) {break;}
	}
	int i;
	for ( i = 0; i < 8; i++ ) {
		text[i] = currentName[i];
	}
}

mathObject * processMathInput( char * text ) {
	int len = getTextLen( text ) + 1;
	int i = parseData( equalSign, (unsigned char*)(text), 1, len);
	if ( i == -1 ) {
		mathObject * obj = processMathObject( text, len );
		if (obj == NULL) { return NULL; }
		generateNewName( obj -> name );

		int nameLen = getTextLen( obj -> name );
		memmove( text + nameLen + 1, text, len );

		int j;
		for (j = 0; j < nameLen; j++ ) {
			text[j] = obj -> name[j];
		}
		text[nameLen] = 61;

		return obj;
	}
	else {
		if ( i > 7 ) { return NULL; }
		char name[8];
		int j = 0;
		for ( j = 0; j < i; j++ ) {
			name[j] = text[j];
			if ( name[j] < 97 || name[j] > 122 ) { return NULL; }
		}
		name[i] = 0;
		if ( !checkNameRepeat( name ) ) { return NULL;}

		mathObject * obj = processMathObject( text + i + 1, len );
		if ( obj == NULL ) { return NULL; }
		else {
			for (j = 0; j < i + 1; j++) {
				obj -> name[j] = name[j];
			}
		}
		obj -> text = text;

		return obj;
	}
}

void addDataBarExpression( drawData * dataBar ) {
	char c = handleKeyDown( lastKeyDown );
	if ( c == '\n' ) {
		char * text = (char *) ( ( (drawData*)(dataBar -> dataPointer))[3].dataPointer );
		char * newText = new char[136];
		int i;
		for ( i = 0; i < 128; i++ ) {
			newText[i] = text[i];
		}

		if (numOfGraphObjs < 128) {
			mathObject * mathElem = processMathInput( newText );
			if ( mathElem != NULL ) {
				copyMathObject( graphObjects + numOfGraphObjs, mathElem );
				graphObjects[ numOfGraphObjs ].visible = true;
				addSideBarElem( graphObjects + numOfGraphObjs );

				numOfGraphObjs++;
				drawGraph( graphArea );

				memset( text, 0, 128 );
				dataBar -> arguments[7] = 0;
				dataBar -> arguments[8] = 0;
				delete mathElem;
			}
			else {
				changeBackgroundColor( dataBar, 0x80 );
			}
		}
		drawTextBoxCursor( dataBar );
	}
	else if ( c == 27 ) {
		keyboardWindowFocus = NULL;
		textBoxUnHighlight( dataBar );
	}
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
