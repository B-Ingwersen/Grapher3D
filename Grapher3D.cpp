//g++ Grapher3D.cpp -lSDL2 -lpthread -o Grapher3D

#include <stdlib.h>

// Current Graphics Library isn't a true linkable library, so the source code must be downloaded and the path of these files pointed to it
#include "../GraphicsLibrary/DrawingLibrary.h"
#include "../GraphicsLibrary/Extensions/Text.h"
#include "../GraphicsLibrary/Extensions/BasicGUIElements.h"
#include "../GraphicsLibrary/Extensions/WindowShadows.h"
#include "../GraphicsLibrary/Extensions/DataOperations.h"

#include "Math/ExpressionAnalyzer.h"
#include "Math/Objects.h"
#include "GraphData.h"
#include "GraphManager3D.h"
#include "GraphManager2D.h"
#include "DrawFunctions.h"
#include "SideBar.h"
#include "InputManager.h"
#include "UserInterface.h"

int main(int argc, char* args[])
{
	createDrawingEnvironment(createUserInterface, NULL);

	return 0;
};
