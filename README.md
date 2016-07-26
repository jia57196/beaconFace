# beaconFace

BeaconFace is an opensource project for face match & recognition from realtime IP Camera Streaming media source.


##How to Build
mkdir build
cd build && cmake -DUSE_TBB=ON -DUSE_OPENCV=ON -DUNIT_TEST=ON ..
make
