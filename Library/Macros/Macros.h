#define BYTE_SHIFT(a, b, r) r == 0 ? a << (b * 8) : a >> (b * 8)
#define CUT_OFF(value, max) value &(max - 1)
#define BOARD_ARRAY_SIZE(array) ((int)((sizeof(array) / sizeof((array)[0]))))
