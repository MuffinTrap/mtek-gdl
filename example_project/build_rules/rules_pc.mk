
# PC RULES

MGDL_DIR	=$(HOME)/libmgdl
MGDL_INCLUDE	= -I$(MGDL_DIR)/include
INCLUDES += $(MGDL_INCLUDE)


# Optional: Use AngelScript either as script or compiled as c++
ifdef ANGEL
# Read from $(ANGELSCRIPT_SDK) environment value directory
INCLUDES += -I$(ANGELSCRIPT_SDK)/angelscript/include
INCLUDES += -I$(ANGELSCRIPT_SDK)/add_on/
# Add AngelScript library directory
LDFLAGS += -L$(ANGELSCRIPT_SDK)/angelscript/lib/linux

ifeq ($(ANGEL), script)
CXXFLAGS += -DUSE_ANGEL_AS_SCRIPT
else ifeq ($(ANGEL), cpp)
CXXFLAGS += -DUSE_ANGEL_AS_CPP
else
$(error Variable ANGEL must be defined either as 'script' or 'cpp' e.g. ANGEL=script)
endif # ifeq ANGEL
endif # ifdef ANGEL
