#**************************************************************************************************
#   Makefile for Raylib C++ on macOS (Homebrew-installed Raylib 5.5)
#**************************************************************************************************

.PHONY: all clean

# ==== Basic Configuration ====
PROJECT_NAME       ?= game
PLATFORM           ?= PLATFORM_DESKTOP
BUILD_MODE         ?= DEBUG

# Compiler settings
CC = clang++
MAKE = make

# ==== macOS Paths ====
# Homebrew Raylib install paths
RAYLIB_PATH        ?= /opt/homebrew/Cellar/raylib/5.5
RAYLIB_INCLUDE_PATH ?= /opt/homebrew/include
RAYLIB_LIB_PATH     ?= /opt/homebrew/lib

# Include and library flags
INCLUDE_PATHS = -I. -I$(RAYLIB_INCLUDE_PATH)
LDFLAGS = -L$(RAYLIB_LIB_PATH)

# ==== Compiler Flags ====
CFLAGS = -Wall -std=c++14 -Wno-missing-braces -D_DEFAULT_SOURCE

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

# ==== Raylib & macOS Frameworks ====
LDLIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenAL

# ==== Source Files ====
OBJS ?= main.cpp

# ==== Targets ====
all:
	$(CC) $(OBJS) -o $(PROJECT_NAME) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)
	@echo "✅ Build complete: ./$(PROJECT_NAME)"

clean:
	rm -f $(PROJECT_NAME)
	rm -f *.o
