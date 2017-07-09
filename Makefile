# make help http://unicorn.ejudge.ru/instr.pdf

CXX = g++
CXXFLAGS = -O2 -g -Wall -std=c++11

# Strict compiler options
CXXFLAGS += -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
		-Wfloat-equal -Wshadow -Wpointer-arith \
		-Wtype-limits -Wempty-body -Wlogical-op \
		-Wmissing-field-initializers -Wctor-dtor-privacy \
		-Wnon-virtual-dtor -Wstrict-null-sentinel \
		-Woverloaded-virtual -Wextra -pedantic

# Flag for spdloglib
CXXFLAGS += -pthread

# Directories with source code
SRC_DIR = src
INCLUDE_DIR = include

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Add headers dirs to gcc search path
CXXFLAGS += -I $(INCLUDE_DIR)

# Helper macros
# subst is sensitive to leading spaces in arguments.
make_path = $(addsuffix $(1), $(basename $(subst $(2), $(3), $(4))))
# Takes path list with source files and returns pathes to related objects.
src_to_obj = $(call make_path,.o, $(SRC_DIR), $(OBJ_DIR), $(1))

# All source files in our project that must be built into movable object code.
CXXFILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJFILES := $(call src_to_obj, $(CXXFILES))

# Default target (make without specified target).
.DEFAULT_GOAL := all

# Making build directories
mkdirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) 

# Rules for compiling targets
$(BIN_DIR)/solution: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(filter %.o, $^) -o $@

# Pattern for compiling object files (*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $(call src_to_obj, $<) $<

# Alias to make all targets.
.PHONY: all clean
all: mkdirs $(BIN_DIR)/solution 

# Alias to clean all 
clean: 
	rm -rf $(BUILD_DIR)

# Suppress makefile rebuilding.
Makefile: ;
