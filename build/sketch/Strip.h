#ifndef __STRIP_H
#define __STRIP_H

#include <FastLED.h>

#include "config.h"
#include "marco.h"

class Strip {
   private:
    int m_step;          // 颜色渐变步长
    int m_db;            // 声强传感器虚拟值
    int m_mirage_choice;  // 幻彩方案选择变量
    int m_music_choice;   // 音乐方案选择变量

    CRGB m_leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];  // 三排灯珠RGB颜色数组
    CRGB m_leds_target[NUM_STRIPS]
                      [NUM_LEDS_PER_STRIP];  // 三排灯珠RGB变换目标颜色数组

    // 夕阳西下模式
    void MM_Dusk() {
        // 夕阳西下模式下, HSV模式下颜色变换
        static int H_Max = 48;
        static int H_Min = 0;

        // 颜色变化较缓慢
        m_step = 1;

        for (int i = 0; i < NUM_STRIPS; i++) {
            for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
                if (m_leds[i][j] == m_leds_target[i][j]) {
                    m_leds_target[i][j] =
                        CHSV(random(H_Min, H_Max + 1), random(240, 255 + 1),
                             random(200, 255 + 1));
                }
            }
        }
    }

    // 蹦迪模式
    void MM_Disco() {
        m_step = 10;

        // 颜色变化较快
        for (int i = 0; i < NUM_STRIPS; i++) {
            for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
                if (m_leds[i][j] == m_leds_target[i][j]) {
                    m_leds_target[i][j] =
                        CRGB(random(255 + 1), random(255 + 1), random(255 + 1));
                }
            }
        }
    }

    // 梦幻模式
    void MM_Fantasy() {
        float color_rate = MAX(10.0f, ((float)m_db - (float)VOICE_DOWN_DB)) /
                           (float)VOICE_WIDTH_DB;

        m_step = color_rate * 6.0f;

        //根据输入分贝的大小决定颜色群数量
        int group_num = color_rate * (float)TOP_COLOR_GROUP_NUM;

        group_num = MAX(1, group_num);

        int group_width = (float)NUM_LEDS_PER_STRIP / (float)group_num;

        int color_down_bias = 255.0f - (color_rate * 155.0f);

        CRGB color_group[4] = {
            {random(color_down_bias, 255 + 1), random(color_down_bias, 255 + 1),
             0},
            {random(color_down_bias, 255 + 1), random(color_down_bias, 255 + 1),
             random(color_down_bias, 255 + 1)},
            {random(color_down_bias, 255 + 1), 0,
             random(color_down_bias, 255 + 1)},
            {0, random(color_down_bias, 255 + 1),
             random(color_down_bias, 255 + 1)}};

        // 颜色变化较快
        for (int i = 0; i < NUM_STRIPS; i++) {
            for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
                if (m_leds[i][j] == m_leds_target[i][j]) {
                    m_leds_target[i][j] = color_group[j / group_width % 4];
                }
            }
        }
    }

   public:
    Strip();

    void UpdateFade();  // 灯条颜色更新(渐变)
    void Update();      // 灯条颜色更新(立即)
    void Close();       // 关灯
    void Solid();       // 常亮照明模式
    void Mirage();      // 幻彩氛围模式
    void Music();       // 音乐模式
};

#endif
