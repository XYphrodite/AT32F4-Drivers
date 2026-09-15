# AT32F4-Drivers — Указатель документации

[Русский](README_ru.md) | [English](README.md)

## Быстрая навигация

- **Быстрый старт** — тулчейн, WSL, первая сборка: [Русский](Getting_Started_ru.md) | [English](Getting_Started.md)
- **Обзор библиотеки** — `../README_ru.md` / `../README.md`
- **Архитектура и контекст** — `../ProjectContext_ru.md` / `../ProjectContext.md`
- **Протоколы пакетов** — `../Library/Terminal/PROTOCOLS_ru.md` / `PROTOCOLS.md` (CayenneLPP / Tiny / ExtraSmall)
- **История изменений** — `../CHANGELOG.md`
- **Лицензия** — `../LICENSE` (MIT)

## С чего начать?

1. Установи `arm-none-eabi-gcc` (C11 / C++20)
2. Прочитай `Getting_Started_ru.md`
3. Задай `SHARED_LIB_PATH` в `CMakeLists.txt` прошивки:
   ```cmake
   set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
   ```
4. Примеры: `Library/ArteryCore/Periphery/AdcDriver_Examples.md`, `Lora` / `Uart`

## Doxygen (опционально)

Исходники готовы к Doxygen (`@file`, `@brief`). Локально:

```bash
doxygen -g Doxyfile && doxygen Doxyfile
# или: doxygen Docs/Doxyfile
```

Шаблон `Doxyfile` добавим позже; пока ориентируйся на `Library/*/xxx.h`.

---

Вопросы — через Issues на GitHub.
