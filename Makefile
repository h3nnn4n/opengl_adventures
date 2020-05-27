TARGET = $(notdir $(CURDIR))
BUILDDIR = $(abspath $(CURDIR)/build)

OPTIONS =

INCLUDES = -Isrc -Ideps/glad/include/ -Ideps/glfw/include/
LINKS = -Ldeps/glfw/build/src/ -Ldeps/glad/src/

CPPFLAGS = -Wall -O2 $(OPTIONS) $(LINKS) $(INCLUDES)
CFLAGS = -std=c99
LDFLAGS = -O2 -Wl,--gc-sections -Ldeps/glfw/build/src/
LIBS = -lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

LD_LIBRARY_PATH = deps/glfw/build/src/

CC = gcc
CXX = g++

C_FILES := $(wildcard src/*.c) deps/glad/src/glad.c

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: build

build: $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CURDIR)/$(TARGET)

$(BUILDDIR)/%.o: %.c
	@echo -e "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

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
