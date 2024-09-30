# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Libraries and includes
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

# Executable name
TARGET = $(BINDIR)/MyProject

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Rule to create the target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(SDL2_LDFLAGS)

# Rule to compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -I$(INCDIR) -c $< -o $@

# Clean rule to remove compiled objects and executables
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean

