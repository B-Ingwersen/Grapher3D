SOURCE_DIR = src
INCLUDE_DIR = include
OUTPUT_DIR = obj
GRAPHICS_LIBRARY_DIR = ../GraphicsLibrary
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR) -I$(GRAPHICS_LIBRARY_DIR)/include

LIBS = -lSDL2 -lpthread

_DEPS = Math/ExpressionAnalyzer.h Math/Objects.h DrawFunctions.h GraphData.h GraphManager2D.h GraphManager3D.h InputManager.h SideBar.h UserInterface.h
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = Grapher3D.o Math/ExpressionAnalyzer.o Math/Objects.o DrawFunctions.o GraphData.o GraphManager2D.o GraphManager3D.o InputManager.o SideBar.o UserInterface.o
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(GRAPHICS_LIBRARY_DIR)/obj/GraphicsLibrary.o :

Grapher3D : $(OBJ) $(GRAPHICS_LIBRARY_DIR)/obj/GraphicsLibrary.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
