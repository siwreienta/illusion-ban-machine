BUILD_DIR="build"
TARGET_NAME="apotheosis_server"
CONFIG_PATH="server/configs/config.yaml"

cd $BUILD_DIR

make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "О нет, вы забанены"
    exit 1
fi

echo "Запускаем сервер..."
./$TARGET_NAME --config ../$CONFIG_PATH