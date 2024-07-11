# Makefile to build opengx for Nintendo Wii

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC or sudo -E make  ")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO or sudo -E make")
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii

CFLAGS = -MMD -MP -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float  -DHW_RVL -O2 -ggdb
INCLUDE_FLAGS = -I include -I $(DEVKITPRO)/libogc/include

LIB	:= opengx
CFILES	:= $(wildcard *.c)
CPPFILES := $(wildcard *.cpp)
OFILES	:= $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)
ARC	:= lib$(LIB).a
HDR	:= $(LIB).h
GL_HDR	:= include/GL/gl.h
GLU_HDR	:= include/GL/glu.h
GLEXT_HDR	:= include/GL/glext.h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p $(LIBOGC_LIB) $(LIBOGC_INC)/GL
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/GL/
	cp -f $(GL_HDR) $(LIBOGC_INC)/GL/
	cp -f $(GLU_HDR) $(LIBOGC_INC)/GL/
	cp -f $(GLEXT_HDR) $(LIBOGC_INC)/GL/

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

