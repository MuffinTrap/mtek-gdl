include Makefile_pc.mk

# MacOS specific settings
CXX = clang++
LIBDIR	:= lib/mac
CXXFLAGS += -DMGDL_PLATFORM_MAC
CXXFLAGS += -DMGDL_PLATFORM=\"MacOS\"

# Do not complain about OpenAL
CXXFLAGS += -Wno-deprecated-declarations
# Add MacPorts include path
CXXFLAGS += -I/opt/local/include

# Intel MacOS homebrew location
CXXFLAGS += -I/usr/local/include

# M1 MacOs homebrew location
CXXFLAGS += -I/opt/homebrew/include

# Exact locations but should not be needed
# CXX_FLAGS += -I/opt/homebrew/Cellar/libsndfile/1.2.2/include -I/opt/homebrew/Cellar/openal-soft/1.23.1/include -I/opt/homebrew/Cellar/glm/1.0.1/include -I/opt/homebrew/Cellar/libpng/1.6.43/include

# Common part

.PHONY: all clean install

all : $(ARC)

$(ARC): $(OFILES)
	@mkdir -p $(LIBDIR)
# Create static library
	@$(AR) rcs $(ARC) $(OFILES)
# Move static library
	@mv $(ARC) $(LIBDIR)
	@echo built library $(ARC)

# Installs to /home/user/libmgdl
install: $(ARC)
# Main library
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/lib
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/pc

	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)/lib
	@cp $(LIBDIR)/$(DLL) $(INSTALL_DIR)/lib

	@cp $(LIBHDR) $(INSTALL_DIR)/include
	@cp $(HDRS_X) $(INSTALL_DIR)/include/$(LIB)
	@cp $(HDRS_PC) $(INSTALL_DIR)/include/$(LIB)/pc

# UFBX library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/fbx
	@cp $(UFBX_HDR) $(INSTALL_DIR)/include/$(LIB)/ufbx
# ccVector library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/ccVector
	@cp $(CCVEC_HDR) $(INSTALL_DIR)/include/$(LIB)/ccVector
# random generation library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/wflcg
	@cp $(RANDOM_HDR) $(INSTALL_DIR)/include/$(LIB)/wflcg

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
