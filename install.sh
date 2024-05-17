#!/bin/bash

clear
echo "Install Mtek and libraries"

export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=/opt/devkitpro/devkitPPC

pDEVKITPRO=/opt/devkitpro
pDEVKITPPC=/opt/devkitpro/devkitPPC

cd 3rdparty
cd libs-wii
make 
make install DEVKITPRO=$pDEVKITPRO DEVKITPPC=$pDEVKITPPC
make clean

cd ../..

cd source/wii
make
make install DEVKITPRO=$pDEVKITPRO DEVKITPPC=$pDEVKITPPC
make clean

echo "Done"
