
# echo all commands
set -x

# Create build directory
mkdir build
cd build
BUILD_CONFIG=Release

# Declare platform specific flags
declare -a CMAKE_PLATFORM_FLAGS
if [[ ${HOST} =~ .*darwin.* ]]; then
  CMAKE_PLATFORM_FLAGS+=(-DCMAKE_OSX_SYSROOT="${CONDA_BUILD_SYSROOT}")
fi
#CMAKE_PLATFORM_FLAGS+=(-DCMAKE_TOOLCHAIN_FILE="${RECIPE_DIR}/cross-linux.cmake")

ls -lh /opt/

# CMake
cmake .. \
	-G "Ninja" \
	-DCMAKE_BUILD_TYPE=$BUILD_CONFIG \
	-DCMAKE_PREFIX_PATH:PATH="${PREFIX}" \
	-DCMAKE_INSTALL_PREFIX:PATH="${PREFIX}" \
	-DCMAKE_INSTALL_RPATH:PATH="${PREFIX}/lib" \
	-DBOOST_ROOT:PATH="${PREFIX}" \
	-DBUILD_SHARED_LIBS:BOOL=ON \
	-DENABLE_TESTING:BOOL=ON \
	${CMAKE_PLATFORM_FLAGS[@]}

# Compile and install
ninja install -v

# Run tests
ctest -V
