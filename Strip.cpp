#include "Strip.h"

#include "marco.h"

/**
 * @brief 灯带构造函数
 * @note 本灯带类并不代表单条灯条, 代表DBMAL的整个灯条系统(可能为多个)
 */
Strip::Strip() {
    m_step = 1;
    m_db = 0;

    m_mirage_choice = 0;  // 幻彩方案选择变量
    m_music_choice = 1;   // 音乐方案选择变量

    FastLED.addLeds<WS2812, LED_PIN_LEFT, GRB>(m_leds[0], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, LED_PIN_MID, GRB>(m_leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<WS2812, LED_PIN_RIGHT, GRB>(m_leds[2], NUM_LEDS_PER_STRIP);

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

/**
 * @brief 灯带颜色更新函数(无极)
 */
void Strip::UpdateFade() {
    m_db = analogRead(VOLUME_PIN);  // 获取声强传感器值

    m_step = LIMIT(m_step, 0, 51);  // 防止step速度为负

    // 灯珠实际颜色渐变为目标颜色
    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j].r +=
                LIMIT((int)((int)m_leds_target[i][j].r - (int)m_leds[i][j].r),
                      (-1 * m_step), m_step);
            m_leds[i][j].g +=
                LIMIT((int)((int)m_leds_target[i][j].g - (int)m_leds[i][j].g),
                      (-1 * m_step), m_step);
            m_leds[i][j].b +=
                LIMIT((int)((int)m_leds_target[i][j].b - (int)m_leds[i][j].b),
                      (-1 * m_step), m_step);
        }
    }
}

/**
 * @brief 灯带颜色更新函数(立即)
 */
void Strip::Update() {
    m_db = analogRead(VOLUME_PIN);  // 获取声强传感器值

    // 灯珠实际颜色直接变换为目标颜色, 不经历渐变
    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds[i][j] = m_leds_target[i][j];
        }
    }
}

/**
 * @brief 关灯
 */
void Strip::Close() {
    m_step = 1;

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds_target[i][j] = CRGB::Black;  // 保证初始化时所有灯珠全灭
        }
    }
}

/**
 * @brief 常亮
 * @note 台灯照明模式
 */
void Strip::Solid() {
    m_step = 1;

    for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            m_leds_target[i][j] = WARM_WHITE;  // 灯珠颜色变为暖白
        }
    }
}

/**
 * @brief 幻彩模式(非音乐随变)
 */
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

        default:
            break;
    }
}

/**
 * @brief 音乐随变模式
 */
void Strip::Music() {
    switch (m_music_choice) {
        case 0:
            MM_Disco();
            break;

        case 1:
            MM_Fantasy();
            break;

        default:
            break;
    }
}

/**
 * @brief 选择幻彩方案
 * @param mirage_choice 幻彩方案编号
 */
void Strip::SetMirageChoice(int mirage_choice) {
    m_mirage_choice = mirage_choice;
}

/**
 * @brief 选择音乐方案
 * @param music_choice 音乐方案编号
 */
void Strip::SetMirageChoice(int music_choice) { m_music_choice = music_choice; }