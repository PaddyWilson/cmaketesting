
rm build -r
mk build

cmake -B build .
cd build

cmake --build .
cd ..
