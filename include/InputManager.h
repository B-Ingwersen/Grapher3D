
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "Math/Objects.h"

extern unsigned char openParenthesis[];
extern unsigned char closeParenthesis[];
extern unsigned char comma[];
extern unsigned char equalSign[];
extern char curve[];
extern char param[];
extern char point[];
extern char vector[];
extern int nameIndex;
extern char currentName[8];

mathObject * processMathObject( char * text, int len );

bool checkNameRepeat( char * text );

void generateNewName( char * text );

mathObject * processMathInput( char * text );

void addDataBarExpression( drawData * dataBar );

void grapherGlobalKeyboardHandler( int keycode );

#endif // _INPUT_MANAGER_H_