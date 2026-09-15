# Быстрый старт — AT32F4-Drivers

## 1. Требования

- **Тулчейн:** `arm-none-eabi-gcc` ≥ 12 (C11, C++20, `-O2` / `-Og`)
- **Сборка:** `cmake` ≥ 3.20, `ninja` или `make`, `python` 3.10+ (для утилит)
- **ОС:** Windows + WSL2 (Ubuntu 22.04) или native Linux. Библиотека разрабатывалась в WSL, пути чувствительны к регистру.

Установка (Ubuntu/WSL):

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi cmake ninja-build python3
arm-none-eabi-gcc --version
```

Подойдёт также MSYS2 или `xPack`.

## 2. Клонирование

```bash
git clone --recursive https://github.com/XYphrodite/AT32F4-Drivers.git
# или если уже склонирован:
git submodule update --init --recursive
```

Сабмодули: `Library/sx126x_driver`, `Library/llcc68_driver`, `Library/externals/CANopenNode`, `Library/externals/lvgl`, `at32f403a-407-docs`, `at32f435-437-docs`.

## 3. Интеграция в прошивку

В `CMakeLists.txt` прошивки:

```cmake
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)

# Пример: подключаем нужные драйверы (полный список — в твоей прошивке)
target_sources(${PROJECT_NAME} PRIVATE
  ${SHARED_LIB_PATH}/AT32F403A/drivers/src/at32f403a_407_gpio.c
  ${SHARED_LIB_PATH}/AT32F403A/drivers/src/at32f403a_407_usart.c
  ${SHARED_LIB_PATH}/Library/Uart/UartDrv.cpp
  ${SHARED_LIB_PATH}/Library/Timer/TimerDrv.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
  ${SHARED_LIB_PATH}/Library
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  -DAT32F403ACGT7
  -DUSE_STDPERIPH_DRIVER
)
```

Линкер-скрипт: возьми из `ldscripts/` или `AT32F403A/ldscript/` (например `AT32F403AxG_FLASH.ld` — 1024K FLASH / 96K RAM / `_estack = 0x20018000`).

## 4. Первая сборка (smoke test)

```bash
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake
cmake --build build -j
```

Нет `toolchain` файла? Можно напрямую:

```bash
cmake -B build -G Ninja \
  -DCMAKE_C_COMPILER=arm-none-eabi-gcc \
  -DCMAKE_CXX_COMPILER=arm-none-eabi-g++
```

## 5. Что дальше

- `README_ru.md` — состав библиотеки
- `ProjectContext_ru.md` — структура каталогов и архитектура
- `Library/Terminal/PROTOCOLS_ru.md` — форматы пакетов (CayenneLPP / Tiny)
- `Library/ArteryCore/Periphery/AdcDriver_Examples.md` — пример ADC
- `CHANGELOG.md` — история версий

## Частые проблемы

- **`index.lock` exists:** другой `git` ещё работает — подожди, не удаляй файл, повтори.
- **`DMA1_CHANNEL1` захардкожен:** `UartService` — только DEBUG `USART1`; для `USART2/3` используй `UartDrv` напрямую.
- **`cmake_shared.cmake` закомментирован:** это шаблон, подключай исходники вручную в прошивке.

## Поддержка

Issues: https://github.com/XYphrodite/AT32F4-Drivers/issues
