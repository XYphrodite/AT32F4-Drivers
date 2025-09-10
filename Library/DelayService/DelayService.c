#include "DelayService.h"

uint32_t cpu_speed = 240000000;

void __attribute__((optimize("O0"))) delay_ticks(uint64_t ticks) {
    while (ticks--);
}

void __attribute__((optimize("O0"))) delay_us(uint64_t us) {
    uint32_t ticksPerUs = cpu_speed / 1000 / 1000;
    uint64_t ticks = ticksPerUs * us / 17.06666666666667;
    if (ticks > 50) ticks -= 50;
    delay_ticks(ticks);
}

void __attribute__((optimize("O0"))) delay_ms(uint64_t ms) {
    uint32_t ticksPerMs = cpu_speed / 1000;
    uint64_t ticks = ticksPerMs * ms / 17.06666666666667;
    if (ticks > 50) ticks -= 50;
    delay_ticks(ticks);
}