# Build a Windows static library
include Makefile_pc.mk

# Windows specific settings
CXX = clang++
LIBDIR	:= lib/win
CXXFLAGS += -DMGDL_PLATFORM_MSYS2
CXXFLAGS += -DMGDL_PLATFORM=\"Windows\ MSYS2\"

# Common part

.PHONY: all clean install

all : $(ARC)

$(ARC) : $(OFILES)
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
# rocket library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/rocket
	@cp $(ROCKET_CODE) $(INSTALL_DIR)/include/$(LIB)/rocket
# random generation library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/wflcg
	@cp $(RANDOM_HDR) $(INSTALL_DIR)/include/$(LIB)/wflcg

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
