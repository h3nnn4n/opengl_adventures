TARGET = $(notdir $(CURDIR))
BUILDDIR = $(abspath $(CURDIR)/build)

OPTIONS = -DIMGUI_IMPL_API="extern \"C\"" \
					-DIMGUI_IMPL_OPENGL_LOADER_GLAD \
					-DIMGUI_IMPL_OPENGL_LOADER_GLFW

INCLUDES = -Isrc \
					 -Ideps/glad/include/ \
					 -Ideps/glfw/include/ \
					 -Ideps/cglm/include/ \
					 -Ideps/cimgui/ \
					 -Ideps/cimgui/imgui \
					 -Ideps/cimgui/imgui/examples/

LINKS = -Ldeps/glfw/build/src/ \
				-Ldeps/glad/src/

CPPFLAGS = -Wall -std=c++11 -O2 $(OPTIONS) $(LINKS) $(INCLUDES)
CFLAGS = -Wall -std=c99 -O2 $(OPTIONS) $(LINKS) $(INCLUDES)

LDFLAGS = -O2 -Wl,-Ldeps/glfw/build/src/

LIBS = -lm -lglfw -lpthread -ldl -lstdc++

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -lGL
	ECHOFLAGS = -e
endif
ifeq ($(UNAME_S),Darwin)
	LIBS += -framework OpenGL
	CFLAGS += -Wno-unused-command-line-argument
	CPPFLAGS += -Wno-unused-command-line-argument -Wno-mismatched-tags
endif

LD_LIBRARY_PATH = deps/glfw/build/src/

CC = gcc
CXX = g++

C_FILES := $(wildcard src/*.c) deps/glad/src/glad.c
CPP_FILES := $(wildcard src/*.cpp)
IMGUI_FILES := $(wildcard ./deps/cimgui/*.cpp) \
							 $(wildcard ./deps/cimgui/imgui/*.cpp) \
							 $(wildcard ./deps/cimgui/imgui/examples/imgui_impl_glfw.cpp) \
							 $(wildcard ./deps/cimgui/imgui/examples/imgui_impl_opengl3.cpp)

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(IMGUI_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: build

build: $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CURDIR)/$(TARGET)

$(BUILDDIR)/%.o: %.c
	@echo $(ECHOFLAGS) "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) $(L_INC) $(CUSTOM) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo $(ECHOFLAGS) "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(L_INC) $(CUSTOM) -o "$@" -c "$<"

$(TARGET).o: $(OBJS) $(LDSCRIPT)
	@echo $(ECHOFLAGS) "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS) $(CUSTOM)

$(TARGET): $(OBJS) $(LDSCRIPT)
	@echo $(ECHOFLAGS) "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS) $(CUSTOM)

-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).o"
