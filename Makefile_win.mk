# Build a Windows static library
include Makefile_pc.mk

# Windows specific settings
CXX = clang++
LIBDIR	:= lib/win
CXXFLAGS += -DMGDL_PLATFORM_WINDOWS
CXXFLAGS += -DMGDL_PLATFORM=\"Windows\"

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
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/mgdl
	@mkdir -p $(INSTALL_DIR)/mgdl/pc

	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)

	@cp $(LIBHDR) $(INSTALL_DIR)
	@cp $(HDRS_X) $(INSTALL_DIR)/mgdl
	@cp $(HDRS_PC) $(INSTALL_DIR)/mgdl/pc

# UFBX library
	@mkdir -p $(INSTALL_DIR)/mgdl/ufbx
	@cp $(UFBX_HDR) $(INSTALL_DIR)/mgdl/ufbx
# ccVector library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/ccVector
	@cp $(CCVEC_HDR) $(INSTALL_DIR)/$(LIB)/ccVector
# rocket library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/rocket
	@cp $(ROCKET_CODE) $(INSTALL_DIR)/$(LIB)/rocket
# random generation library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/wflcg
	@cp $(RANDOM_HDR) $(INSTALL_DIR)/$(LIB)/wflcg

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
