TARGET = $(notdir $(CURDIR))
BUILDDIR = $(abspath $(CURDIR)/build)

OPTIONS =

INCLUDES = -Isrc -Ideps/glad/include/ -Ideps/glfw/include/ -Ideps/cglm/include/
LINKS = -Ldeps/glfw/build/src/ -Ldeps/glad/src/ -Ldeps/cglm/build

CPPFLAGS = -Wall -std=c++11 -O2 $(OPTIONS) $(LINKS) $(INCLUDES)
CFLAGS = -Wall -std=c99 -O2 $(OPTIONS) $(LINKS) $(INCLUDES)

LDFLAGS = -O2 -Wl,-Ldeps/glfw/build/src/

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lstdc++
endif
ifeq ($(UNAME_S),Darwin)
	LIBS = -lm -lglfw -framework OpenGL -lpthread -ldl -lstdc++
	CFLAGS += -Wno-unused-command-line-argument
	CPPFLAGS += -Wno-unused-command-line-argument -Wno-mismatched-tags
endif

LD_LIBRARY_PATH = deps/glfw/build/src/

CC = gcc
CXX = g++

C_FILES := $(wildcard src/*.c) deps/glad/src/glad.c
CPP_FILES := $(wildcard src/*.cpp)

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: build

build: $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CURDIR)/$(TARGET)

$(BUILDDIR)/%.o: %.c
	@echo -e "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(L_INC) -o "$@" -c "$<"

$(TARGET).o: $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

$(TARGET): $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).o"
