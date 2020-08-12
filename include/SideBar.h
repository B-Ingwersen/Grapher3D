
#ifndef _SIDE_BAR_H_
#define _SIDE_BAR_H_

#include "Math/Objects.h"
#include "GraphicsLibrary/Core.h"

extern int currentColor;
extern int graphColors[];
extern char deleteButtonText[];

int generateNextColor();

void deleteElem( int index );

void deleteElemHandler( drawData * deleteButton );

void changeElemVisible( drawData * selector );

void addSideBarElem(mathObject * obj);

#endif // _SIDE_BAR_H_