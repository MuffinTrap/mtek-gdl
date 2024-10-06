# Build a linux static library

include Makefile_pc.mk

# Linux specific settings
CXX = clang++
LIBDIR	:= lib/lnx
CXX_FLAGS += -DMGDL_PLATFORM_LINUX
# Different file type for linux object files
OFILES	:= $(CFILES:.cpp=.lo)
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
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(INSTALL_DIR)/mgdl
	@mkdir -p $(INSTALL_DIR)/mgdl/pc
	@cp $(LIBDIR)/$(ARC) $(INSTALL_DIR)
	@cp $(HDRS_X) $(INSTALL_DIR)/mgdl
	@cp $(HDRS_PC) $(INSTALL_DIR)/mgdl/pc
	@cp $(PCHDR) $(INSTALL_DIR)
	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.lo : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
