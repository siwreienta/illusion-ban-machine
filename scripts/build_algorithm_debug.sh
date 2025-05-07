BUILD_DIR="build"
TARGET_NAME="apotheosis_algorithm"

echo "Remove previous building..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR


echo "Start building..."
cd $BUILD_DIR
cmake -DBUILD_ALGORITHM=ON -DBUILD_SERVER=OFF -DBUILD_DEBUG=ON ..
make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "Error, you're banned!"
    exit 1
fi
