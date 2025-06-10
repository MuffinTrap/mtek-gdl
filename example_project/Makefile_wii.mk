# Slightly modified makefile for MTek-GDL based projects for the Nintendo Wii
# muffintrap: added portlibs directories for version 2.6.1


#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
# prevent deletion of implicit targets
#---------------------------------------------------------------------------------
.SECONDARY:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules
#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a directory which contains binary files intended to be converted to obj files
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	./
DATA		:=	data
TEXTURES	:=	
INCLUDES	:=	include

#-------------------
# Extra modules
#-------------------
# Rocket
SOURCES += rocket/
INCLUDES += rocket/

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	= -g -Werror -Wall -Wextra -DMGDL_PLATFORM_WII -DMGDL_USE_CCVECTOR -std=c++11 $(MACHDEP) $(INCLUDE)
CXXFLAGS	= -Wwrite-strings $(CFLAGS) -DSYNC_PLAYER -DUFBX_REAL_IS_FLOAT
# Wii specific settings
CXXFLAGS += -DMGDL_PLATFORM=\"Wii\"

LDFLAGS	=	$(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project (add your own at the left-most)
#---------------------------------------------------------------------------------
LIBS	:=	-lmgdl -lopengx -lglut -lGLU -lvalloc -lwiiuse -loggplayer -lvorbisidec -logg -lpng -lz -lfat -lbte -lasnd -logc -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= 

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(TEXTURES),$(CURDIR)/$(dir))
					
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SCFFILES	:=	$(foreach dir,$(TEXTURES),$(notdir $(wildcard $(dir)/*.scf)))
TPLFILES	:=	$(SCFFILES:.scf=.tpl)


#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(addsuffix .o,$(TPLFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
PORTLIBS_PPC_INC	:=	$(PORTLIBS_PATH)/ppc/include
PORTLIBS_WII_INC	:=	$(PORTLIBS_PATH)/wii/include
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)) \
					-I$(CURDIR)/$(BUILD) \
					-I$(LIBOGC_INC) \
					-I$(PORTLIBS_PPC_INC) \
					-I$(PORTLIBS_WII_INC)
#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PORTLIBS_PPC_LIB	:=	$(DEVKITPRO)/portlibs/ppc/lib
PORTLIBS_WII_LIB	:=	$(DEVKITPRO)/portlibs/wii/lib
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					-L$(LIBOGC_LIB) \
					-L$(PORTLIBS_PPC_LIB) \
					-L$(PORTLIBS_WII_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)


.PHONY: $(BUILD) clean all

all: $(BUILD)

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile_wii.mk
	@mv $(OUTPUT).dol boot.dol

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol
#---------------------------------------------------------------------------------
run:
	wiiload boot.dol
#---------------------------------------------------------------------------------
emulate:
	Dolphin $(OUTPUT).dol

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

#---------------------------------------------------------------------------------
%.tpl.o	:	%.tpl
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------
# Convert png, ogg and wav files
%.png.o : %.png
	@echo $(notdir $<)
	@$(bin2o)

%.ogg.o : %.ogg
	@echo $(notdir $<)
	@$(bin2o)

%.wav.o : %.wav
	@echo $(notdir $<)
	@$(bin2o)
#---------------------------------------------------------------

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
