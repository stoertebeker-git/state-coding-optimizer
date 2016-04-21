rm main

echo "Running CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug ..
echo "lelle coppied so hard..."
make

echo "Cleanup..."
rm -r CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
./main
