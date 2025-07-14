include Makefile_pc.mk

# Linux specific settings
CXX = clang++
LIBDIR	:= lib/lnx
CXXFLAGS += -DMGDL_PLATFORM_LINUX
CXXFLAGS += -DMGDL_PLATFORM=\"Linux\"

# Common part

.PHONY: all clean install

all : $(ARC)

$(ARC): $(OFILES)
	@mkdir -p $(LIBDIR)
# Create static library
	@$(AR) rcs $(ARC) $(OFILES)
# Create shared library
	$(CXX) -shared -o $(DLL) $(OFILES)
# Move static library
	@mv $(ARC) $(LIBDIR)
	@mv $(DLL) $(LIBDIR)
	@echo built library $(ARC)

# Installs to /home/user/libmgdl
install: $(ARC)
# Main library
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/$(LIB)
	@mkdir -p $(INSTALL_DIR)/$(LIB)/pc

	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)
	@cp $(LIBDIR)/$(DLL) $(INSTALL_DIR)

	@cp $(LIBHDR) $(INSTALL_DIR)
	@cp $(HDRS_X) $(INSTALL_DIR)/$(LIB)
	@cp $(HDRS_PC) $(INSTALL_DIR)/$(LIB)/pc

# UFBX library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/ufbx
	@cp $(UFBX_HDR) $(INSTALL_DIR)/$(LIB)/ufbx
# ccVector library
	@mkdir -p $(INSTALL_DIR)/$(LIB)/ccVector
	@cp $(CCVEC_HDR) $(INSTALL_DIR)/$(LIB)/ccVector

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
