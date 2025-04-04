BUILD_DIR="build"
TARGET_NAME="apotheosis_server"
CONFIG_PATH="server/configs/config.yaml"


echo "Remove previous building..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR


echo "Start building..."
cd $BUILD_DIR
cmake -DBUILD_ALGORITHM=OFF -DBUILD_SERVER=ON ..
make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "Error, you're banned!"
    exit 1
fi

# Запуск сервера
echo "Run server..."
./$TARGET_NAME --config ../$CONFIG_PATH