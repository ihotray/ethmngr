#!/bin/bash

echo "install dependencies"

pwd

cd /opt/dev
rm -fr easy-soc-libs
git clone https://dev.iopsys.eu/iopsys/easy-soc-libs.git
cd easy-soc-libs
git checkout devel
cd libeasy
make CFLAGS+="-w -I/usr/include/libnl3"
mkdir -p /usr/include/easy
cp *.h /usr/include/easy
cp *.h /usr/include
cp -a libeasy*.so* /usr/lib
cd ../libethernet
make PLATFORM=TEST
cp ethernet.h /usr/include
cp -a libethernet*.so* /usr/lib
sudo ldconfig
