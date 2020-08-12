

#ifndef _MATH___OBJECTS_H_
#define _MATH___OBJECTS_H_

#include "GraphicsLibrary/Core.h"
#include "Math/ExpressionAnalyzer.h"

#define POINT2D 0
#define POINT3D 1
#define VECTOR 2
#define EXPRESSION2D 3
#define EXPRESSION3D 4
#define CURVE 5
#define PARAMETRIC 6

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

struct mathObjType {
	void (* drawFunction)( drawData drawInformation );
	void (* deleteFunction)( void * mathElem );	
};

extern mathObjType mathObjTypes[];

void copyMathObject( mathObject * obj1, mathObject * obj2 );

void deleteCurve( void * mathElem );
void deleteParam( void * mathElem );
void deletePoint( void * mathElem );
void deleteExpression( void * mathElem );
void deleteVector( void * object );
void deleteMathObjectNothing( void * object );

void drawPoint2D( drawData drawInfo );
void drawPoint3D( drawData drawInfo );
void drawVector( drawData drawInfo );
void draw2DExpression( drawData drawInfo );
void draw3DExpression( drawData drawInfo );
void drawCurve( drawData drawInfo );
void drawParam( drawData drawInfo );

void drawMathObject( mathObject * obj, drawData * graphArea, int color, screenData * screen );

#endif // _MATH___OBJECTS_H_