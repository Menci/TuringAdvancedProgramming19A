cd ../qsort-checker
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
mv ./bin/qsort-checker /tmp/qsort-checker
cd ..
rm build -rf
cd ../2019201409于倬浩