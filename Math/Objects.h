struct mathObject {
	int objType;

	bool visible;
	bool d3D;
	bool d2D;
	void (* drawFunction)( drawData drawInformation );
	void (* deleteFunction)( void * mathElem );
	int color;
	char * text;
	void * mathElem;

	char name[8];
	int nameLen;
};

void copyMathObject( mathObject * obj1, mathObject * obj2 ) {
	obj1 -> objType = obj2 -> objType;

	obj1 -> visible = obj2 -> visible;
	obj1 -> d3D = obj2 -> d3D;
	obj1 -> d2D = obj2 -> d2D;
	obj1 -> drawFunction = obj2 -> drawFunction;
	obj1 -> deleteFunction = obj2 -> deleteFunction;
	obj1 -> color = obj2 -> color;
	obj1 -> text = obj2 -> text;
	obj1 -> mathElem = obj2 -> mathElem;

	int i;
	for ( i = 0; i < 8; i++ ) {
		obj1 -> name[i] = obj2 -> name[i];
	}
	obj1 -> nameLen = obj2 -> nameLen;
}

#define POINT2D 0
#define POINT3D 1
#define VECTOR 2
#define EXPRESSION2D 3
#define EXPRESSION3D 4
#define CURVE 5
#define PARAMETRIC 6

struct curveData {
	mathExpression * x;
	mathExpression * y;
	mathExpression * z;

	int nSteps;
	double startX;
	double endX;
};

struct paramData {
	mathExpression * x;
	mathExpression * y;

	int nSteps;
	double startX;
	double endX;
};

struct pointData {
	double x;
	double y;
	double z;
};

struct vectorData {
	double comps[8];
	int numOfComps;
};

void deleteCurve( void * mathElem ) {
	curveData * curve = (curveData *)(mathElem);

	deleteMathExpression( curve -> x );
	deleteMathExpression( curve -> y );
	deleteMathExpression( curve -> z );

	delete curve;
}
void deleteParam( void * mathElem ) {
	paramData * param = (paramData *)(mathElem);

	deleteMathExpression( param -> x );
	deleteMathExpression( param -> y );

	delete param;
}
void deletePoint( void * mathElem ) {
	pointData * point = (pointData *)(mathElem);
	delete point;
}
void deleteExpression( void * mathElem ) {
	mathExpression * exp = (mathExpression *)(mathElem);
	deleteMathExpression( exp );
}
void deleteVector( void * object ) {
	vectorData * vector = (vectorData*)(object);
	delete vector;
}
void deleteMathObjectNothing( void * object ) {
}

struct mathObjType {
	void (* drawFunction)( drawData drawInformation );
	void (* deleteFunction)( void * mathElem );	
};

void drawPoint2D( drawData drawInfo );
void drawPoint3D( drawData drawInfo );
void drawVector( drawData drawInfo );
void draw2DExpression( drawData drawInfo );
void draw3DExpression( drawData drawInfo );
void drawCurve( drawData drawInfo );
void drawParam( drawData drawInfo );

mathObjType mathObjTypes[] = {
	{ drawPoint2D, deletePoint },
	{ drawPoint3D, deletePoint },
	{ drawVector, deleteVector },
	{ draw2DExpression, deleteExpression },
	{ draw3DExpression, deleteExpression },
	{ drawCurve, deleteCurve },
	{ drawParam, deleteParam },
};

void drawMathObject( mathObject * obj, drawData * graphArea, int color, screenData * screen ) {
	int arguments[3] = { graphArea -> arguments[3], graphArea -> arguments[4], color };
	addEventExecute( mathObjTypes[obj -> objType].drawFunction, 100, arguments, obj -> mathElem, graphArea -> window, 3, screen );
}
