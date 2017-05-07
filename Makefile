CC = ../gcc-linaro/bin/arm-linux-gnueabihf-gcc
CFLAGS = -c -Wall -pedantic -mfpu=neon -march=armv7-a -O3
LDFLAGS = -static

DIR_SOURCE = source
DIR_HEADER = include
DIR_OUTPUT = output
DIR_MAKEFILE_INC = $(DIR_OUTPUT)

DIRS_PROJECTS = .
DIRS_SOURCES = $(addsuffix /$(DIR_SOURCE)/,$(DIRS_PROJECTS))
DIRS_HEADERS = $(addsuffix /$(DIR_HEADER)/,$(DIRS_PROJECTS))
DIRS_OUTPUTS = $(addsuffix /$(DIR_OUTPUT)/,$(DIRS_PROJECTS))

VPATH := $(DIRS_SOURCES) $(DIRS_HEADERS)

LOOKUP_C := $(addsuffix *.c,$(DIRS_SOURCES))

OBJECTS := $(notdir $(patsubst %.c,%.o,$(wildcard $(LOOKUP_C))))
OBJECT_FILES := $(addprefix $(DIR_OUTPUT)/,$(OBJECTS))
ASM := $(notdir $(patsubst %.c,%.S,$(wildcard $(LOOKUP_C))))
ASM_FILES := $(addprefix $(DIR_OUTPUT)/,$(ASM))

TARGET = test_chacha20

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECT_FILES) -o $@ $(LDFLAGS)

%.o : %.c
	$(CC) $(addprefix -I,$(DIRS_HEADERS)) $(CFLAGS) $< -o $(DIR_OUTPUT)/$@

asm: $(ASM)

%.S : %.c
	$(CC) -S $(addprefix -I,$(DIRS_HEADERS)) $(CFLAGS) $< -o $(DIR_OUTPUT)/$@

.PHONY: clean
clean:
	rm -rf $(DIR_OUTPUT)/*o $(TARGET) $(DIR_MAKEFILE_INC)/*.d
