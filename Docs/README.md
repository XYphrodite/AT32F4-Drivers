# AT32F4-Drivers — Documentation Index

[English](README.md) | [Русский](README_ru.md)

## Quick navigation

- **Getting Started** — toolchain, WSL, first build: [English](Getting_Started.md) | [Русский](Getting_Started_ru.md)
- **Library overview** — `../README.md` / `../README_ru.md`
- **Project context & architecture** — `../ProjectContext.md` / `../ProjectContext_ru.md`
- **Packet protocols** — `../Library/Terminal/PROTOCOLS.md` / `PROTOCOLS_ru.md` (CayenneLPP / Tiny / ExtraSmall)
- **Changelog** — `../CHANGELOG.md`
- **License** — `../LICENSE` (MIT)

## Where to start?

1. Install `arm-none-eabi-gcc` (C11 / C++20)
2. Read `Getting_Started.md`
3. Set `SHARED_LIB_PATH` in your firmware `CMakeLists.txt`:
   ```cmake
   set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
   ```
4. Pick examples: `Library/ArteryCore/Periphery/AdcDriver_Examples.md`, `Lora` / `Uart`

## Doxygen (optional)

Source is Doxygen-ready (`@file`, `@brief`). To generate locally:

```bash
doxygen -g Doxyfile && doxygen Doxyfile
# or: doxygen Docs/Doxyfile
```

A `Doxyfile` template will be added later; for now use `Library/*/xxx.h` as reference.

---

For questions, open an Issue on GitHub.
