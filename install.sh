#! /bin/bash

clear
echo "Install Mtek and libraries"

if [! -v DEVKITPRO]; then
	echo "Enviroment variable DEVKITPRO is not set. Set it and/or use command sudo -E ./install.sh to use user's enviroment variables"
	exit 1
fi

if [! -v DEVKITPPC]; then
	echo "Enviroment variable DEVKITPPC is not set. Set it and/or use command sudo -E ./install.sh to use user's enviroment variables"
	exit 1
fi

cd 3rdparty
cd libs-wii
make 
make install
make clean

cd ../..

cd source/wii
make
make install
make clean

echo "Done"
exit 0
