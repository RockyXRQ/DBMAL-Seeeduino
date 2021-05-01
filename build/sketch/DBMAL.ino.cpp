#include <Arduino.h>
#line 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
#pragma message "DBMAL(Arduino-Demo) version 1.007.001"

#include <FastLED.h>

#define USE_INTERRUPT
#define DEBUG_MODE 1

#ifdef USE_INTERRUPT
#include <SAMDTimerInterrupt.h>
#endif

#define NUM_STRIPS 2           // 灯条数量
#define NUM_LEDS_PER_STRIP 62  // 每排灯条所含灯珠数量

#define LED_PIN_LEFT 1   // 左排灯条对应数字端
#define LED_PIN_MID 2    // 中排灯条对应数字端
#define LED_PIN_RIGHT 8  // 右排灯条对应数字端
#define VOLUME_PIN 9     // 声强传感器对应虚拟端
#define SWITCH_PIN 10    // 感应开关对应数字端

#define TOP_COLOR_GROUP_NUM 8  // 颜色集群最大数量
#define VOICE_UP_DB 315
#define VOICE_DOWN_DB 250
#define VOICE_WIDTH_DB ((VOICE_UP_DB) - (VOICE_DOWN_DB))

#define SYSTEM_INTERVAL_MS 8            // 系统频率在125hz左右
#define LONG_HOLDING_COUNTER_LIMIT 250  // 按住超过2秒视为长按

#define WARM_WHITE (CRGB(255, 248, 220))  // 纯色照明模式下灯珠颜色

// 返回限流后的值
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

#define LIMIT(val, min, max) MAX((min), MIN(val, max))

enum SWITCH_STATE { RELEASE = 0, TOUCH };        // 开关状态枚举量
enum STATE { CLOSE = 0, SOLID, MIRAGE, MUSIC };  // 全局状态枚举量

struct {
    int16_t switch_counter;
    enum SWITCH_STATE switch_state;
    enum STATE state;
} state_controller;  // 全局状态控制器

#ifdef USE_INTERRUPT
SAMDTimer ITimer(TIMER_TCC);
#endif

int8_t step = 1;           // 颜色渐变步长
int16_t db = 0;            // 声强传感器虚拟值
int8_t mirage_choice = 0;  // 幻彩方案选择变量
int8_t music_choice = 0;   // 音乐方案选择变量

CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];  // 三排灯珠RGB颜色数组
CRGB leds_target[NUM_STRIPS]
                [NUM_LEDS_PER_STRIP];  // 三排灯珠RGB变换目标颜色数组

// 系统主定时中断函数
void DBML_Event();

// 感应开关相关函数
void Switch_Init();          // 感应开关初始化函数
void Switch_State_Update();  // 检测按键状态并更新台灯状态函数

// 灯条相关函数
void Strip_Init();                        // 灯条初始化函数
void Strip_Update_Fade(int8_t step);      // 灯条颜色更新(渐变)
void Strip_Update();                      // 灯条颜色更新(立即)
void Strip_Close();                       // 关灯
void Strip_Solid();                       // 常亮照明模式
void Strip_Mirage(int8_t mm_choice = 0);  // 幻彩氛围模式
void Strip_Music(int8_t mm_choice = 0, int16_t db_step = 1024);  // 音乐模式

// 幻彩&音乐相关函数
void MM_Dusk();                        // 夕阳西下模式
void MM_Disco(int16_t db_step = 103);  // 蹦迪模式
void MM_Fantasy(int16_t db = 1024);    // 梦幻模式

#line 80 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void setup();
#line 108 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void loop();
#line 272 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void Strip_Mirage(int8_t mm_choice);
#line 289 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void Strip_Music(int8_t mm_choice, int16_t db_step);
#line 325 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void MM_Disco(int16_t db_step);
#line 340 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void MM_Fantasy(int16_t db);
#line 80 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBML\\DBMAL.ino"
void setup() {
    step = 1;                     // 颜色渐变步长初始化
    db = analogRead(VOLUME_PIN);  // 声强传感器虚拟值初始化
    mirage_choice = 0;            // 幻彩方案选择变量初始化
    music_choice = 1;             // 音乐方案选择变量初始化

    // 台灯状态初始化
    state_controller.switch_counter = 0;
    state_controller.switch_state = RELEASE;

    randomSeed(analogRead(0));

#if DEBUG_MODE
    state_controller.state = MUSIC;
#else
    state_controller.state = CLOSE;
#endif
    Switch_Init();
    Strip_Init();

#ifdef USE_INTERRUPT
    ITimer.attachInterruptInterval(SYSTEM_INTERVAL_MS * 1000, DBML_Event);
#endif
#if DEBUG_MODE
    Serial.begin(115200);
#endif
}

void loop() {
    db = analogRead(VOLUME_PIN);  // 获取声强传感器值
#ifndef USE_INTERRUPT
    DBML_Event();
    delay(SYSTEM_INTERVAL_MS);
#endif
#if DEBUG_MODE
    Serial.print("Voice: ");
    Serial.println(db);
    Serial.print("Switch_Pressed: ");
    Serial.println(digitalRead(SWITCH_PIN));
    Serial.print("Switch_State: ");
    Serial.println(state_controller.state);
    Serial.print("Switch_Counter: ");
    Serial.println(state_controller.switch_counter);
    Serial.print("Strip_Real_RGB: ");
    Serial.print(leds[0][0].r);
    Serial.print(" ");
    Serial.print(leds[0][0].g);
    Serial.print(" ");
    Serial.println(leds[0][0].b);
    Serial.print("Strip_Target_RGB: ");
    Serial.print(leds_target[0][0].r);
    Serial.print(" ");
    Serial.print(leds_target[0][0].g);
    Serial.print(" ");
    Serial.println(leds_target[0][0].b);
#endif
}

/*******************************************************************************
 *                                系统相关函数 *
 *******************************************************************************/
// 系统主定时函数
void DBML_Event() {
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
            Strip_Music(music_choice, db);
            break;
    }
    Strip_Update_Fade(step);  // 台灯颜色渐变式刷新
    // Strip_Update();

    FastLED.show();
}

/*******************************************************************************
 *                                感应开关相关函数 *
 *******************************************************************************/
// 感应开关初始化函数
void Switch_Init() { pinMode(SWITCH_PIN, INPUT); }

// 检测按键状态并更新台灯状态函数
void Switch_State_Update() {
    if (digitalRead(SWITCH_PIN)) {
        state_controller.switch_state = TOUCH;
        state_controller.switch_counter += 1;
    } else {
        state_controller.switch_state = RELEASE;

        if (state_controller.switch_counter > 0 &&
            state_controller.switch_counter < LONG_HOLDING_COUNTER_LIMIT) {
            // 此时视为开关短按
            if (state_controller.state == CLOSE) {
                state_controller.state = SOLID;
            } else if (state_controller.state == SOLID) {
                state_controller.state = MIRAGE;
            } else if (state_controller.state == MIRAGE) {
                state_controller.state = MUSIC;
            } else if (state_controller.state == MUSIC) {
                state_controller.state = SOLID;
            }
        }

        if (state_controller.switch_counter >= LONG_HOLDING_COUNTER_LIMIT) {
            state_controller.state = CLOSE;
        }

        state_controller.switch_counter = 0;
    }
}

/*******************************************************************************
 *                                 灯条相关函数 *
 *******************************************************************************/
// 灯条初始化函数
void Strip_Init() {
    FastLED.addLeds<WS2812, LED_PIN_LEFT, GRB>(leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, LED_PIN_MID, GRB>(leds[1], NUM_LEDS_PER_STRIP);
    // FastLED.addLeds<WS2812, LED_PIN_RIGHT, GRB>(leds[2], NUM_LEDS_PER_STRIP);
    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

// 灯条颜色更新(渐变)
void Strip_Update_Fade(int8_t step) {
    step = LIMIT(step, 0, 51);  // 防止step速度为负

    // 灯珠实际颜色渐变为目标颜色
    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds[i][j].r += LIMIT(
                (int16_t)((int16_t)leds_target[i][j].r - (int16_t)leds[i][j].r),
                (-1 * step), step);
            leds[i][j].g += LIMIT(
                (int16_t)((int16_t)leds_target[i][j].g - (int16_t)leds[i][j].g),
                (-1 * step), step);
            leds[i][j].b += LIMIT(
                (int16_t)((int16_t)leds_target[i][j].b - (int16_t)leds[i][j].b),
                (-1 * step), step);
        }
    }
}

// 灯条颜色更新(立即)
void Strip_Update() {
    // 灯珠实际颜色直接变换为目标颜色, 不经历渐变
    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds[i][j] = leds_target[i][j];
        }
    }
}

// 关灯
void Strip_Close() {
    step = 1;

    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds_target[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

// 常亮照明模式
void Strip_Solid() {
    step = 1;

    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds_target[i][j] = WARM_WHITE;  // 灯珠颜色变为暖白
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

        case 2:
            MM_Fantasy();
            break;
    }
}

// 音乐模式
void Strip_Music(int8_t mm_choice, int16_t db_step) {
    switch (mm_choice) {
        case 0:
            MM_Disco(db_step);
            break;

        case 1:
            MM_Fantasy(db_step);
            break;
    }
}

/*******************************************************************************
 *                               幻彩&音乐相关函数 *
 *******************************************************************************/
// 夕阳西下模式
void MM_Dusk() {
    // 夕阳西下模式下, HSV模式下颜色变换
    static int H_Max = 48;
    static int H_Min = 0;

    // 颜色变化较缓慢
    step = 1;

    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            if (leds[i][j] == leds_target[i][j]) {
                leds_target[i][j] =
                    CHSV(random(H_Min, H_Max + 1), random(240, 255 + 1),
                         random(200, 255 + 1));
            }
        }
    }
}

// 蹦迪模式
void MM_Disco(int16_t db_step) {
    step = 10;

    // 颜色变化较快
    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            if (leds[i][j] == leds_target[i][j]) {
                leds_target[i][j] =
                    CRGB(random(255 + 1), random(255 + 1), random(255 + 1));
            }
        }
    }
}

// 梦幻模式
void MM_Fantasy(int16_t db) {
    float color_rate =
        MAX(10.0f, ((float)db - (float)VOICE_DOWN_DB)) / (float)VOICE_WIDTH_DB;

    step = color_rate * 6.0f;

    //根据输入分贝的大小决定颜色群数量
    int16_t group_num = color_rate * (float)TOP_COLOR_GROUP_NUM;

    group_num = MAX(1, group_num);

    int16_t group_width = (float)NUM_LEDS_PER_STRIP / (float)group_num;

    int16_t color_down_bias = 255.0f - (color_rate * 255.0f);

    CRGB color_group[4] = {
        {random(color_down_bias, 255 + 1), random(color_down_bias, 255 + 1), 0},
        {random(color_down_bias, 255 + 1), random(color_down_bias, 255 + 1),
         random(color_down_bias, 255 + 1)},
        {random(color_down_bias, 255 + 1), 0, random(color_down_bias, 255 + 1)},
        {0, random(color_down_bias, 255 + 1),
         random(color_down_bias, 255 + 1)}};

    // 颜色变化较快
    for (int16_t i = 0; i < NUM_STRIPS; i++) {
        for (int16_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            if (leds[i][j] == leds_target[i][j]) {
                leds_target[i][j] = color_group[j / group_width % 4];
            }
        }
    }
}

