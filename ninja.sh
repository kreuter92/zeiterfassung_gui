mkdir -p build
cd build
 
cmake -G "Ninja" \
   -DCMAKE_PREFIX_PATH=~/cs_lib/lib/cmake/CopperSpice \
   -DCMAKE_BUILD_TYPE=Release \
   -DCMAKE_INSTALL_PREFIX=../deploy \
   ..
 
ninja install