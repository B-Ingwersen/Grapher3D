
#ifndef _MATH___EXPRESSION_ANALYZER_H_
#define _MATH___EXPRESSION_ANALYZER_H_

#define valueType 0
#define predefFuncType 1
#define predefVarType 2
#define operationType 3
#define openGroupType 4
#define closeGroupType 5
#define processType 6

struct predefFunc {
	char name[8];
	int len;
	double (*func)(double);
};

struct predefVar {
	char name[8];
	int len;
	double val;
};

struct operation {
	char name[8];
	int len;
};

struct funcComponent {
	int compType;
	int index;
};

struct mathExpression {
	int * data;
	double * results;
	funcComponent * funcComps;
	int funcCompLen;
	int textLen;
	int color;
};

struct processedGroup {
	funcComponent * g;
	funcComponent * g1;
	funcComponent * g2;
	funcComponent * g3;
	funcComponent * g4;
	funcComponent * g5;

	int valBase;
	int varBase;
	int funcBase;
	int powBase;
	int prodBase;
	int sumBase;
	int finalBase;

	int nSubGroups;
	int nVals;
	int nVars;
	int nPows;
	int nFuncs;
	int nProds;
	int nSums;

	int len;
	int len2;
	int len3;
	int len4;
	int len5;

	int base3;
	int powDIBase;

	int * varRefs;
	processedGroup ** subGroups;
	int totalLen;

	int dataIndex;
	int totalDataLen;
};

struct numberAnalysis {
	double val;
	bool sucess;
	int textLen;
};

extern int numOfPredefFuncs;
extern predefFunc predefFunctions[];

extern int numOfVars;
extern predefVar predefVariables[];

extern operation operations[];

int getStringLen( char * text );

bool compareDataNull( char * data, char * source );

void copyString( char * dest, char * source );

void clearPredefVars();

void printFuncComps( funcComponent * comps, int len );

void deleteMathExpression( mathExpression * elem );

void deleteProcGroup( processedGroup * procGroup );

processedGroup * processGroup( funcComponent * group, int len, double * values );

double evaluateGroup( int * group, double * results );

double evalMathExpression( mathExpression * function );

mathExpression * analyzeMathExpression( char * text, int len );

numberAnalysis analyzeNumber( char * text, int len );

#endif // _MATH___EXPRESSION_ANALYZER_H_