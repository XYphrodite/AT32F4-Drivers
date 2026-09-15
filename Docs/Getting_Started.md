# Getting Started — AT32F4-Drivers

## 1. Requirements

- **Toolchain:** `arm-none-eabi-gcc` ≥ 12 (C11, C++20, `-O2` / `-Og`)
- **Build:** `cmake` ≥ 3.20, `ninja` or `make`, `python` 3.10+ (for some utils)
- **OS:** Windows + WSL2 (Ubuntu 22.04) or native Linux. The library is developed in WSL; paths are case-sensitive.

Install (Ubuntu/WSL):

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi cmake ninja-build python3
arm-none-eabi-gcc --version
```

Windows (MSYS2) or `xPack` also works.

## 2. Clone

```bash
git clone --recursive https://github.com/XYphrodite/AT32F4-Drivers.git
# or if already cloned:
git submodule update --init --recursive
```

Submodules: `Library/sx126x_driver`, `Library/llcc68_driver`, `Library/externals/CANopenNode`, `Library/externals/lvgl`, `at32f403a-407-docs`, `at32f435-437-docs`.

## 3. Integrate into your firmware

In your firmware `CMakeLists.txt`:

```cmake
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)

# Example: add drivers you use (see your firmware for full list)
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

Linker script: pick one from `ldscripts/` or `AT32F403A/ldscript/` (e.g. `AT32F403AxG_FLASH.ld` — 1024K FLASH / 96K RAM / `_estack = 0x20018000`).

## 4. First build (smoke test)

```bash
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake
cmake --build build -j
```

No `toolchain` file yet? Use plain:

```bash
cmake -B build -G Ninja \
  -DCMAKE_C_COMPILER=arm-none-eabi-gcc \
  -DCMAKE_CXX_COMPILER=arm-none-eabi-g++
```

## 5. Next steps

- `README.md` — library components
- `ProjectContext.md` — directory layout & architecture
- `Library/Terminal/PROTOCOLS.md` — packet formats (CayenneLPP / Tiny)
- `Library/ArteryCore/Periphery/AdcDriver_Examples.md` — ADC example
- `CHANGELOG.md` — version history

## Troubleshooting

- **`index.lock` exists:** another `git` is running — wait, don't delete; retry.
- **`DMA1_CHANNEL1` hard-coded:** `UartService` is DEBUG `USART1` only; for `USART2/3` use `UartDrv` directly.
- **`cmake_shared.cmake` is commented:** it's a template; add sources manually per firmware.

## Support

Open an Issue at https://github.com/XYphrodite/AT32F4-Drivers/issues
