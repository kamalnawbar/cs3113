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

# Add screens
ifeq ($(wildcard screens/MenuScene.cpp),screens/MenuScene.cpp)
SRCS += screens/MenuScene.cpp
endif

ifeq ($(wildcard screens/Level1IntroScreen.cpp),screens/Level1IntroScreen.cpp)
SRCS += screens/Level1IntroScreen.cpp
endif

ifeq ($(wildcard screens/Level2IntroScreen.cpp),screens/Level2IntroScreen.cpp)
SRCS += screens/Level2IntroScreen.cpp
endif

ifeq ($(wildcard screens/Level3IntroScreen.cpp),screens/Level3IntroScreen.cpp)
SRCS += screens/Level3IntroScreen.cpp
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

ifeq ($(wildcard screens/WinScreen.cpp),screens/WinScreen.cpp)
SRCS += screens/WinScreen.cpp
endif

ifeq ($(wildcard screens/GameOverScreen.cpp),screens/GameOverScreen.cpp)
SRCS += screens/GameOverScreen.cpp
endif

# Add Effects if it exists
ifeq ($(wildcard CS3113/Effects.cpp),CS3113/Effects.cpp)
SRCS += CS3113/Effects.cpp
endif

# Add ShaderProgram if it exists
ifeq ($(wildcard CS3113/ShaderProgram.cpp),CS3113/ShaderProgram.cpp)
SRCS += CS3113/ShaderProgram.cpp
endif

# Default values
CXX = g++
CXXFLAGS = -std=c++11 -ICS3113

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