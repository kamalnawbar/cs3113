# Source and target
SRCS = main.cpp
TARGET = raylib_app

# OS detection
UNAME_S := $(shell uname -s)

# Add CS3113 library if it exists
ifeq ($(wildcard CS3113/cs3113.cpp),CS3113/cs3113.cpp)
SRCS += CS3113/cs3113.cpp
endif

# Add Entity library if it exists
ifeq ($(wildcard CS3113/Entity.cpp),CS3113/Entity.cpp)
SRCS += CS3113/Entity.cpp
endif

# Add Map library if it exists
ifeq ($(wildcard CS3113/Map.cpp),CS3113/Map.cpp)
SRCS += CS3113/Map.cpp
endif

# Add Scene library if it exists
ifeq ($(wildcard CS3113/Scene.cpp),CS3113/Scene.cpp)
SRCS += CS3113/Scene.cpp
endif

# Add MenuScene if it exists
ifeq ($(wildcard CS3113/MenuScene.cpp),CS3113/MenuScene.cpp)
SRCS += CS3113/MenuScene.cpp
endif

# Add Level1 if it exists
ifeq ($(wildcard CS3113/Level1.cpp),CS3113/Level1.cpp)
SRCS += CS3113/Level1.cpp
endif

# Add Level2 if it exists
ifeq ($(wildcard CS3113/Level2.cpp),CS3113/Level2.cpp)
SRCS += CS3113/Level2.cpp
endif

# Add Level3 if it exists
ifeq ($(wildcard CS3113/Level3.cpp),CS3113/Level3.cpp)
SRCS += CS3113/Level3.cpp
endif

# Add GameOverScreen if it exists
ifeq ($(wildcard CS3113/GameOverScreen.cpp),CS3113/GameOverScreen.cpp)
SRCS += CS3113/GameOverScreen.cpp
endif

# Add WinScreen if it exists
ifeq ($(wildcard CS3113/WinScreen.cpp),CS3113/WinScreen.cpp)
SRCS += CS3113/WinScreen.cpp
endif

# Default values
CXX = g++
CXXFLAGS = -std=c++11

# Raylib configuration using pkg-config
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS = $(shell pkg-config --libs raylib)

ifeq ($(UNAME_S), Darwin)
# macOS configuration
CXXFLAGS += -arch arm64 $(RAYLIB_CFLAGS)
LIBS = $(RAYLIB_LIBS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
EXEC = ./$(TARGET)

else ifneq (,$(findstring MINGW,$(UNAME_S)))
# Windows configuration
CXXFLAGS += -IC:/raylib/include
LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
TARGET := $(TARGET).exe
EXEC = ./$(TARGET)

else
# Linux/WSL fallback
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXEC = ./$(TARGET)
endif

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean rule
clean:
	@if [ -f "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -f "$(TARGET).exe" ]; then rm -f $(TARGET).exe; fi

# Run rule
run: $(TARGET)
	$(EXEC)