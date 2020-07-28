# Author: Aheyeu Anton
# aheyeant@fit.cvut.cz

BINARY = ./aheyeant
DOC = doc
RM = rm -rf
SRC = src/CExceptions.cpp src/CMap.cpp src/CPages.cpp src/main.cpp src/CSDLInitializer.cpp src/CControllers.cpp src/CTimer.cpp src/CImageLoader.cpp src/CPlayer.cpp src/CTails.cpp src/CInventory.cpp

CXX =g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb  
SDL = -lSDL2 

all: compile doc

# Delete binary and created documentation
clean:
	rm -r $(BINARY)
	rm -r $(DOC)

# Compile
compile: 
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BINARY) $(SDL) 

# Run the program
run:
	$(BINARY)

# Create documentation
doc: 
	doxygen

