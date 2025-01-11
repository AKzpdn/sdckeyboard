#define VID 0x28DE
#define PID 0x1205
#define USGP 0xFFFF
#define USG 0x0001
#define BUFLEN 64
#define TIMEOUT_US 500L
//#define AUTOPRESS_TIME_MS 250L
//#define DOUBLE_CLICK_WINDOW_MS 150L
//#define LONG_PRESS_TIME_MS 500
#define AUTOPRESS_COUNT 100
#define DOUBLE_CLICK_WINDOW_COUNT 100
#define LONG_PRESS_TIME_COUNT 200

#define BUT_L1 0x0000000000000800UL
#define BUT_L4 0x0002000000000000UL
#define BUT_L5 0x0000000000800000UL
#define BUT_R1 0x0000000000000400UL
#define BUT_R4 0x0004000000000000UL
#define BUT_R5 0x0000000001000000UL
#define BUT_X 0x0000000000004000UL
#define BUT_Y 0x0000000000001000UL
#define BUT_DOTS 0x0400000000000000UL

#define BUT_SIGNIFICANT (BUT_L1|BUT_L4|BUT_L5|BUT_R1|BUT_R4|BUT_R5)
#define BUT_MODIFIERS (BUT_X|BUT_Y|BUT_DOTS)

struct rumble_t{
    uint16_t cmd;
    uint8_t unRumbleType;
    uint16_t unIntensity;
    uint16_t unLeftMotorSpeed;
    uint16_t unRightMotorSpeed;
    int8_t nLeftGain;
    int8_t nRightGain;
};

struct rumble_t rumble_short = {
    0xEB,
    0,
    0,
    0,
    3000,
    7,
    7
};

struct rumble_t rumble_long = {
    0xEB,
    0,
    0,
    0,
    1000,
    7,
    7
};

struct chords_t {
    int keycode;
    uint64_t chord;
};

#define CHORDS_COUNT 63
#define MODS_COUNT 3

struct chords_t chords[CHORDS_COUNT] = {
    {KEY_SPACE,     BUT_R1},
    {KEY_BACKSPACE, BUT_L1},
    {KEY_E,         BUT_R4},
    {KEY_T,         BUT_L4},
    {KEY_A,         BUT_R5},
    {KEY_O,         BUT_L5},
    {KEY_I,         BUT_L1|BUT_R1},
    {KEY_N,         BUT_R1|BUT_R4},
    {KEY_S,         BUT_L1|BUT_R4},
    {KEY_H,         BUT_L4|BUT_R1},
    {KEY_R,         BUT_L1|BUT_L4},
    {KEY_D,         BUT_L4|BUT_R4},
    {KEY_L,         BUT_R1|BUT_R5},
    {KEY_C,         BUT_L1|BUT_R5},
    {KEY_U,         BUT_R4|BUT_R5},
    {KEY_M,         BUT_L4|BUT_R5},
    {KEY_W,         BUT_L5|BUT_R1},
    {KEY_F,         BUT_L1|BUT_L5},
    {KEY_G,         BUT_L5|BUT_R4},
    {KEY_Y,         BUT_L4|BUT_L5},
    {KEY_P,         BUT_L5|BUT_R5},
    {KEY_B,         BUT_L1|BUT_R1|BUT_R4},
    {KEY_V,         BUT_L1|BUT_L4|BUT_R1},
    {KEY_K,         BUT_L4|BUT_R1|BUT_R4},
    {KEY_J,         BUT_L1|BUT_L4|BUT_R4},
    {KEY_X,         BUT_L1|BUT_R1|BUT_R5},
    {KEY_COMMA,     BUT_R1|BUT_R4|BUT_R5},
    {KEY_SEMICOLON, BUT_L1|BUT_R4|BUT_R5},
    {KEY_Q,         BUT_L4|BUT_R1|BUT_R5},
    {KEY_Z,         BUT_L1|BUT_L4|BUT_R5},
    {KEY_APOSTROPHE,BUT_L4|BUT_R4|BUT_R5},
    {KEY_LEFTBRACE, BUT_L1|BUT_L5|BUT_R1},
    {KEY_RIGHTBRACE,BUT_L5|BUT_R1|BUT_R4},
    {KEY_SLASH,     BUT_L1|BUT_L5|BUT_R4},
    {KEY_BACKSLASH, BUT_L4|BUT_L5|BUT_R1},
    {KEY_MINUS,     BUT_L1|BUT_L4|BUT_L5},
    {KEY_EQUAL,     BUT_L4|BUT_L5|BUT_R4},
    {KEY_HOME,      BUT_L5|BUT_R1|BUT_R5},
    {KEY_END,       BUT_L1|BUT_L5|BUT_R5},
    {KEY_DELETE,    BUT_L5|BUT_R4|BUT_R5},
    {KEY_GRAVE,     BUT_L4|BUT_L5|BUT_R5},
    {KEY_PAGEDOWN,  BUT_L1|BUT_L4|BUT_R1|BUT_R4},
    {KEY_PAGEUP,    BUT_L1|BUT_R1|BUT_R4|BUT_R5},
    {KEY_1,         BUT_L1|BUT_L4|BUT_R1|BUT_R5},
    {KEY_2,         BUT_L4|BUT_R1|BUT_R4|BUT_R5},
    {KEY_3,         BUT_L1|BUT_L4|BUT_R4|BUT_R5},
    {KEY_4,         BUT_L1|BUT_L5|BUT_R1|BUT_R4},
    {KEY_5,         BUT_L1|BUT_L4|BUT_L5|BUT_R1},
    {KEY_6,         BUT_L4|BUT_L5|BUT_R1|BUT_R4},
    {KEY_7,         BUT_L1|BUT_L4|BUT_L5|BUT_R4},
    {KEY_8,         BUT_L1|BUT_L5|BUT_R1|BUT_R5},
    {KEY_9,         BUT_L5|BUT_R1|BUT_R4|BUT_R5},
    {KEY_0,         BUT_L1|BUT_L5|BUT_R4|BUT_R5},
    {KEY_F1,        BUT_L4|BUT_L5|BUT_R1|BUT_R5},
    {KEY_F2,        BUT_L1|BUT_L4|BUT_L5|BUT_R5},
    {KEY_F3,        BUT_L4|BUT_L5|BUT_R4|BUT_R5},
    {KEY_F4,        BUT_L1|BUT_L4|BUT_R1|BUT_R4|BUT_R5},
    {KEY_F5,        BUT_L1|BUT_L4|BUT_L5|BUT_R1|BUT_R4},
    {KEY_F6,        BUT_L1|BUT_L5|BUT_R1|BUT_R4|BUT_R5},
    {KEY_F7,        BUT_L1|BUT_L4|BUT_L5|BUT_R1|BUT_R5},
    {KEY_F8,        BUT_L4|BUT_L5|BUT_R1|BUT_R4|BUT_R5},
    {KEY_LEFTMETA,  BUT_L1|BUT_L4|BUT_L5|BUT_R4|BUT_R5},
    {KEY_DOT,       BUT_L1|BUT_L4|BUT_L5|BUT_R1|BUT_R4|BUT_R5}
};

struct chords_t mods[MODS_COUNT] = {
    {KEY_LEFTCTRL,  BUT_Y},
    {KEY_LEFTSHIFT, BUT_X},
    {KEY_LEFTALT,   BUT_DOTS}
};
