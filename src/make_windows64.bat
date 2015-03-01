cd ..
rm -rf _build
mkdir _build
cd _build

cmake ../src -G"Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Debug

cd ../src