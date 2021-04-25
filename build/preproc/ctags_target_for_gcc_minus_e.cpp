# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
        
# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
#pragma message "DBML(Arduino-Demo) version 0.001.001"
# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"


# 4 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino" 2
# 19 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
// 返回限流后的值




enum SWITCH_STATE { RELEASE = 0, TOUCH }; // 开关状态枚举量
enum STATE { CLOSE = 0, SOLID, MIRAGE, MUSIC }; // 全局状态枚举量

struct {
    int16_t switch_counter;
    enum SWITCH_STATE switch_state;
    enum STATE state;
} state_controller; // 全局状态控制器

int8_t step = 1; // 颜色渐变步长
int16_t db = 0; // 声强传感器虚拟值
int8_t mirage_choice = 0; // 幻彩方案选择变量
int8_t music_choice = 0; // 音乐方案选择变量

CRGB leds[3 /* 灯条数量*/][40 /* 每排灯条所含灯珠数量*/]; // 三排灯珠RGB颜色数组
CRGB leds_target[3 /* 灯条数量*/]
                [40 /* 每排灯条所含灯珠数量*/]; // 三排灯珠RGB变换目标颜色数组

// 感应开关相关函数
void Switch_Init(); // 感应开关初始化函数
void Switch_State_Update(); // 检测按键状态并更新台灯状态函数

// 灯条相关函数
void Strip_Init(); // 灯条初始化函数
void Strip_Update_Fade(int8_t step); // 灯条颜色更新(渐变)
void Strip_Update(); // 灯条颜色更新(立即)
void Strip_Close(); // 关灯
void Strip_Solid(); // 常亮照明模式
void Strip_Mirage(int8_t mm_choice = 0); // 幻彩氛围模式
void Strip_Music(int8_t mm_choice = 0, int16_t db_step = 103); // 音乐模式

// 幻彩&音乐相关函数
void MM_Dusk(); // 夕阳西下模式
void MM_Disco(int16_t db_step = 103); // 蹦迪模式

void setup() {
    randomSeed(analogRead(0)); // 采集A0口噪声作为随机种子

    // 台灯状态初始化
    state_controller.switch_counter = 0;
    state_controller.switch_state = RELEASE;
    state_controller.state = CLOSE;

    Switch_Init();
    Strip_Init();
}

void loop() {
    db = analogRead(9 /* 声强传感器对应虚拟端*/);
    Switch_State_Update();

    switch (state_controller.state) {
        case CLOSE:
            Strip_Close();
            break;

        case SOLID:
            Strip_Solid();
            break;

        case MIRAGE:
            Strip_Mirage(mirage_choice);
            break;

        case MUSIC:
            Strip_Music(music_choice);
            break;
    }
    Strip_Update_Fade(step);
    // Strip_Update();

    FastLED.show();
}

/*******************************************************************************

 *                                感应开关相关函数 *

 *******************************************************************************/
# 101 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
// 感应开关初始化函数
void Switch_Init() { pinMode(10 /* 感应开关对应数字端*/, (0x0)); }

// 检测按键状态并更新台灯状态函数
void Switch_State_Update() {
    if (digitalRead(10 /* 感应开关对应数字端*/) == 1) {
        state_controller.switch_state = TOUCH;
        state_controller.switch_counter++;
    } else {
        state_controller.switch_state = RELEASE;

        if (state_controller.switch_counter > 0 &&
            state_controller.switch_counter < 250 /* 按住超过2秒视为长按*/) {
            // 此时视为开关短按
            if (state_controller.state == CLOSE) {
                state_controller.state = SOLID;
            } else if (state_controller.state == SOLID) {
                state_controller.state = MIRAGE;
            } else if (state_controller.state == MIRAGE ||
                       state_controller.state == MUSIC) {
                state_controller.state = SOLID;
            }
        }

        if (state_controller.switch_counter >= 250 /* 按住超过2秒视为长按*/) {
            state_controller.state = CLOSE;
        }
        state_controller.switch_counter = 0;
    }
}

/*******************************************************************************

 *                                 灯条相关函数 *

 *******************************************************************************/
# 135 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
// 灯条初始化函数
void Strip_Init() {
    FastLED.addLeds<WS2812, 6 /* 左排灯条对应数字端*/, GRB>(leds[0], 40 /* 每排灯条所含灯珠数量*/);
    FastLED.addLeds<WS2812, 7 /* 中排灯条对应数字端*/, GRB>(leds[1], 40 /* 每排灯条所含灯珠数量*/);
    FastLED.addLeds<WS2812, 8 /* 右排灯条对应数字端*/, GRB>(leds[2], 40 /* 每排灯条所含灯珠数量*/);
    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            leds[i][j] = CRGB::Black; // 保证初始化时所有灯珠全灭
        }
    }
    FastLED.show();
}

// 灯条颜色更新(渐变)
void Strip_Update_Fade(int8_t step) {
    step = (((step) > (0) ? (step) : (0)) < (10) ? ((step) > (0) ? (step) : (0)) : (10)); // 防止step速度为负

    // 灯珠实际颜色渐变为目标颜色
    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            leds[i][j].r +=
                (((leds_target[i][j].r - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].r - leds[i][j].r) : ((-1 * step))) < (step) ? ((leds_target[i][j].r - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].r - leds[i][j].r) : ((-1 * step))) : (step));
            leds[i][j].g +=
                (((leds_target[i][j].g - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].g - leds[i][j].r) : ((-1 * step))) < (step) ? ((leds_target[i][j].g - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].g - leds[i][j].r) : ((-1 * step))) : (step));
            leds[i][j].b +=
                (((leds_target[i][j].b - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].b - leds[i][j].r) : ((-1 * step))) < (step) ? ((leds_target[i][j].b - leds[i][j].r) > ((-1 * step)) ? (leds_target[i][j].b - leds[i][j].r) : ((-1 * step))) : (step));
        }
    }
}

// 灯条颜色更新(立即)
void Strip_Update() {
    // 灯珠实际颜色直接变换为目标颜色, 不经历渐变
    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            leds[i][j] = leds_target[i][j];
        }
    }
}

// 关灯
void Strip_Close() {
    step = 1;

    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            leds_target[i][j] = CRGB::Black; // 保证初始化时所有灯珠全灭
        }
    }
}

// 常亮照明模式
void Strip_Solid() {
    step = 1;

    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            leds_target[i][j] = (CRGB(255, 248, 220)) /* 纯色照明模式下灯珠颜色*/; // 灯珠颜色变为暖白
        }
    }
}

// 幻彩氛围模式
void Strip_Mirage(int8_t mm_choice) {
    switch (mm_choice) {
        case 0:
            MM_Dusk();
            break;

        case 1:
            MM_Disco();
            break;
    }
}

// 音乐模式
void Strip_Music(int8_t mm_choice, int16_t db_step) {
    switch (mm_choice) {
        case 0:
            MM_Disco(db_step);
            break;
    }
}

/*******************************************************************************

 *                               幻彩&音乐相关函数 *

 *******************************************************************************/
# 222 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBML.ino"
// 夕阳西下模式
void MM_Dusk() {
    // 夕阳西下模式下, HSV模式下颜色变换
    static int H_Max = 48;
    static int H_Min = 0;

    // 颜色变化较缓慢
    step = 1;

    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            if (leds[i][j] == leds_target[i][j]) {
                leds_target[i][j] =
                    CHSV(random(H_Min, H_Max + 1), random(240, 255 + 1),
                         random(230, 255 + 1));
            }
        }
    }
}

// 蹦迪模式
void MM_Disco(int16_t db_step) {
    //根据输入分贝的大小决定步长
    step = ((float)db_step / 1024.0f) * 10.0f;

    // 颜色变化较快
    for (int16_t i = 0; i < 3 /* 灯条数量*/; i++) {
        for (int16_t j = 0; j < 40 /* 每排灯条所含灯珠数量*/; j++) {
            if (leds[i][j] == leds_target[i][j]) {
                leds_target[i][j] = CRGB(random(255 + 1), 0, random(255 + 1));
            }
        }
    }
}
