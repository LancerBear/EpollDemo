echo "===================build start==================="

mkdir ./build
cd build
cmake ..
make
cd ..
mkdir bin
cp build/server/src/server bin