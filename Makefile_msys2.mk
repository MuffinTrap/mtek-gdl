# Build a Windows static library
include rules_all.mk
include rules_pc.mk

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
	#@cp $(LIBDIR)/$(DLL) $(INSTALL_DIR)/lib

	@cp $(LIBHDR) $(INSTALL_DIR)/include
	@cp $(HDRS_X) $(INSTALL_DIR)/include/$(LIB)
	@cp $(HDRS_PC) $(INSTALL_DIR)/include/$(LIB)/pc

# UFBX library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/ufbx
	@cp $(UFBX_HDR) $(INSTALL_DIR)/include/$(LIB)/ufbx
# random generation library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/wflcg
	@cp $(RANDOM_HDR) $(INSTALL_DIR)/include/$(LIB)/wflcg
# file change watching library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/dmon
	@cp $(DMON_HDR) $(INSTALL_DIR)/include/$(LIB)/dmon
# string library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/zstrh
	@cp $(ZSTR_HDR) $(INSTALL_DIR)/include/$(LIB)/zstrh
# raymath library
	@mkdir -p $(INSTALL_DIR)/include/$(LIB)/raymath
	@cp $(RAYMATH_HDR) $(INSTALL_DIR)/include/$(LIB)/raymath

	@echo installed to $(INSTALL_DIR)

clean :
	rm -f $(OFILES) $(ARC)
	rm -r $(LIBDIR)

%.pco : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
