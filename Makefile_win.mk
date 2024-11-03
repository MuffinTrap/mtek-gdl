# Build a Windows static library
include Makefile_pc.mk

# Windows specific settings
CXX = g++
LIBDIR	:= lib/win
CXXFLAGS += -DMGDL_PLATFORM_WINDOWS

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

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
