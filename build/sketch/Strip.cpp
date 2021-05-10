#include "Strip.h"

#include "marco.h"
Strip::Strip() {
    m_step = 1;
    m_db = 0;

    m_mirage_choice = 0;  // 幻彩方案选择变量
    m_music_choice = 0;   // 音乐方案选择变量

    FastLED.addLeds<WS2812, LED_PIN_LEFT, GRB>(m_leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, LED_PIN_MID, GRB>(m_leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, LED_PIN_RIGHT, GRB>(m_leds[2], NUM_LEDS_PER_STRIP);

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

void Strip::UpdateFade() {
    m_db = analogRead(VOLUME_PIN);  // 获取声强传感器值

    m_step = LIMIT(m_step, 0, 51);  // 防止step速度为负

    // 灯珠实际颜色渐变为目标颜色
    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j].r += LIMIT((int)((int)m_leds_target[i][j].r -
                                              (int)m_leds[i][j].r),
                                    (-1 * m_step), m_step);
            m_leds[i][j].g += LIMIT((int)((int)m_leds_target[i][j].g -
                                              (int)m_leds[i][j].g),
                                    (-1 * m_step), m_step);
            m_leds[i][j].b += LIMIT((int)((int)m_leds_target[i][j].b -
                                              (int)m_leds[i][j].b),
                                    (-1 * m_step), m_step);
        }
    }
}

// 灯条颜色更新(立即)
void Strip::Update() {
    m_db = analogRead(VOLUME_PIN);  // 获取声强传感器值

    // 灯珠实际颜色直接变换为目标颜色, 不经历渐变
    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j] = m_leds_target[i][j];
        }
    }
}

// 关灯
void Strip::Close() {
    m_step = 1;

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds_target[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

// 常亮照明模式
void Strip::Solid() {
    m_step = 1;

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds_target[i][j] = WARM_WHITE;  // 灯珠颜色变为暖白
        }
    }
}

// 幻彩氛围模式
void Strip::Mirage() {
    switch (m_mirage_choice) {
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
void Strip::Music() {
    switch (m_music_choice) {
        case 0:
            MM_Disco();
            break;

        case 1:
            MM_Fantasy();
            break;
    }
}