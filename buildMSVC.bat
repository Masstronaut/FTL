mkdir build
cd build
cmake -G"Visual Studio 14 2015 Win64" ../FTL -DCMAKE_BUILD_TYPE="Debug;Release;RelWithDebInfo" 2>> ../cmakeError.log
cd ../