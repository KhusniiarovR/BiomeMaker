#!/bin/bash

BUILD_TYPE=Debug
BUILD_DIR=build
EXECUTABLE=BiomeMaker

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}==> Генерация проекта...${NC}"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DRAYLIB_BUILD_EXAMPLES=OFF

if [ $? -ne 0 ]; then
    echo -e "${RED}Ошибка генерации CMake${NC}"
    exit 1
fi

echo -e "${GREEN}==> Сборка проекта...${NC}"
cmake --build "$BUILD_DIR" -j$(nproc)
if [ $? -ne 0 ]; then
    echo -e "${RED}Ошибка сборки${NC}"
    exit 1
fi

EXEC_PATH="$BUILD_DIR/$EXECUTABLE"
if [ ! -f "$EXEC_PATH" ]; then
    echo -e "${RED}Не найден исполняемый файл: $EXEC_PATH${NC}"
    exit 1
fi

echo -e "${GREEN}==> Запуск $EXEC_PATH...${NC}"
"$EXEC_PATH"