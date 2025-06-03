BUILD_DIR="build"
TARGET_NAME="apotheosis_algorithm"

echo "Зачищаем следы..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR


echo "Собираем лучший проект..."
cd $BUILD_DIR
cmake -DBUILD_ALGORITHM=ON -DBUILD_SERVER=OFF -DBUILD_DEBUG=ON ..
make -j$(nproc)


if [ $? -ne 0 ]; then
    echo "О нет, вы забанены"
    exit 1
fi

echo "Ура, ничего не взорвалось"