#!/bin/bash

cwd=${PWD}

sudo apt-get install build-essential -y  
sudo apt-get install cmake -y  
sudo apt-get install curl -y  
sudo apt-get install gfortran -y  
sudo apt-get install git -y  
sudo apt-get install libatlas-dev -y  
sudo apt-get install libavcodec-dev -y  
sudo apt-get install libavformat-dev -y  
sudo apt-get install libboost-all-dev -y  
sudo apt-get install libgtk2.0-dev -y  
sudo apt-get install libjpeg-dev -y  
sudo apt-get install liblapack-dev -y  
sudo apt-get install libswscale-dev -y  
sudo apt-get install pkg-config -y  
sudo apt-get install python-dev -y  
sudo apt-get install python-pip -y  
sudo apt-get install wget -y  
sudo apt-get install zip –y  


#install necessary libs
pip2 install numpy  
pip2 install scipy
pip2 install pandas
pip2 install scikit-learn
pip2 install scikit-image --timeout 100
pip install txaio
pip install twisted
pip install autobahn imagehash

#include Torch 
curl -s https://raw.githubusercontent.com/torch/ezinstall/master/install-deps

git clone https://github.com/torch/distro.git ~/torch --recursive  
cd ~/torch && ./install.sh  

chmod 755 ~/torch/install/bin/luarocks 
~/torch/install/bin/luarocks install dpnn  
~/torch/install/bin/luarocks install nn  
~/torch/install/bin/luarocks install optim  
~/torch/install/bin/luarocks install csvigo  
~/torch/install/bin/luarocks install cunn  
~/torch/install/bin/luarocks install fblualib  
~/torch/install/bin/luarocks install torchx  


#include opencv2

cd ~ && mkdir  -p src
cd src
curl  -L https://github.com/Itseez/opencv/archive/2.4.11.zip -o ocv.zip
unzip  ocv.zip
cd  opencv-2.4.11
mkdir  release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make  -j8
sudo make  install


#install dlib

cd $cwd
sudo cp dlib-* ~/src
cd ~/src tar xf dlib-18.16.tar.bz2
cd dlib-18.16/python_examples
mkdir build
cd build
cmake ../../tools/python
cmake --build . --config Release
sudo cp dlib.so /usr/local/lib/python2.7/dist-packages

#get openface code
cd $cwd
cd ~/src
git clone https://github.com/cmusatyalab/openface.git
cd openface
git submodule init
git submodule update
sudo python2 setup.py install
./models/get-models.sh

