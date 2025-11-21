# Centralized Linker Scripts

This directory contains centralized linker scripts for all TafcoPigstore projects.

## Structure

Linker scripts are organized by their memory mapping, following the typical bootloader layout:

```
ldscripts/
├── 0-preboot/          # Pre-bootloader (4KB @ 0x08000000)
│   ├── AT32F403AxG_FLASH.ld
│   └── AT32F407xG_FLASH.ld
├── 400-boot/           # Bootloader (32KB @ 0x08001000)
│   ├── AT32F403AxG_FLASH.ld
│   └── AT32F407xG_FLASH.ld
├── 1000-main-app/      # Main Application (256KB @ 0x08009000)
│   ├── AT32F403AxG_FLASH.ld
│   └── AT32F407xG_FLASH.ld
└── README.md
```

## Usage

Each project's CMake configuration references these scripts:

- **prebootload**: Uses `0-preboot/` scripts
- **bootloader0**: Uses `400-boot/` scripts  
- **P-block**: Uses `1000-main-app/` scripts

## Memory Layout

### Pre-bootloader (0-preboot)
- **FLASH**: 0x08000000 - 4KB (0x08001000)
- **RAM**: 0x20000000 - 96KB

### Bootloader (400-boot)
- **FLASH**: 0x08001000 - 32KB (0x08009000)
- **RAM**: 0x20000000 - 96KB

### Main Application (1000-main-app)
- **FLASH**: 0x08009000 - 256KB (0x08400000)
- **RAM**: 0x20000000 - 96KB

## MCU Support

Both script sets support:
- **AT32F407VGT7**: 1000KB Flash, 96KB RAM
- **AT32F403ACGT7**: 1000KB Flash, 96KB RAM (default)
- **AT32F403ACGU7**: Development board variant

