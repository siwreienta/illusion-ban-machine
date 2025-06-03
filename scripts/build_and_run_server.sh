BUILD_DIR="build"
TARGET_NAME="apotheosis_server"
CONFIG_PATH="server/configs/config.yaml"


echo "Зачищаем следы..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR


echo "Собираем лучший проект..."
cd $BUILD_DIR
cmake -DBUILD_ALGORITHM=OFF -DBUILD_SERVER=ON -DBUILD_DEBUG=OFF ..
make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "О нет, вы забанены"
    exit 1
fi

echo "Запускаем сервер..."
./$TARGET_NAME --config ../$CONFIG_PATH