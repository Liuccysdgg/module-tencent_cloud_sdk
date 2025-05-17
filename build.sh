#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# install tencent cloud sdk
mkdir 3rdparty -p
cd 3rdparty
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libssl-dev
git clone https://github.com/TencentCloud/tencentcloud-sdk-cpp.git
cd tencentcloud-sdk-cpp
./build.sh build -DBUILD_MODULES="lcic"
./build.sh install
cd ../..
mkdir target -p
mkdir build -p
cd build
cmake ..
make

cp -f libtencent_cloud_sdk.so ../target
