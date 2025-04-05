BUILD_DIR="build"
TARGET_NAME="apotheosis_server"
CONFIG_PATH="server/configs/config.yaml"

cd $BUILD_DIR

make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "Error, you're banned!"
    exit 1
fi

# Запуск сервера
echo "Run server..."
./$TARGET_NAME --config ../$CONFIG_PATH