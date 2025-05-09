BUILD_DIR="build"
TARGET_NAME="apotheosis_server"
CONFIG_PATH="server/configs/config.yaml"

cd $BUILD_DIR

make -j$(nproc)

echo "Обновляем список табличек"
sudo -u siwreienta psql -d apotheosis -f ../scripts/create_tables.sql

echo "Запускаем сервер..."
./$TARGET_NAME --config ../$CONFIG_PATH

if [ $? -ne 0 ]; then
    echo "О нет, вы забанены"
    exit 1
fi

echo "
Ура, ничего не взорвалось"