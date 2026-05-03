.PHONY: wii linux msys2 mac installwii installlinux installmac cleanwii cleanlinux cleanmsys2 cleanmac


wii:
	$(MAKE) -f Makefile_wii.mk
wii_install:
	$(MAKE) -f Makefile_wii.mk install
wii_clean:
	$(MAKE) -f Makefile_wii.mk clean

linux:
	$(MAKE) -f Makefile_linux.mk
linux_install:
	$(MAKE) -f Makefile_linux.mk install
linux_clean:
	$(MAKE) -f Makefile_linux.mk clean

msys2:
	$(MAKE) -f Makefile_msys2.mk
msys2_install:
	$(MAKE) -f Makefile_msys2.mk install
msys2_clean:
	$(MAKE) -f Makefile_msys2.mk clean

mac:
	$(MAKE) -f Makefile_mac.mk
mac_install:
	$(MAKE) -f Makefile_mac.mk install
mac_clean:
	$(MAKE) -f Makefile_mac.mk clean
