#ifndef __STRIP_H
#define __STRIP_H

#include <FastLED.h>

#include "config.h"
#include "marco.h"

class Strip {
   public:
    /**
     * @brief 灯带构造函数
     * @note 本灯带类并不代表单条灯条, 代表DBMAL的整个灯条系统(可能为多个)
     */
    explicit Strip();

    /**
     * @brief 灯带颜色更新函数(无极)
     */
    void UpdateFade();

    /**
     * @brief 灯带颜色更新函数(立即)
     */
    void Update();

    /**
     * @brief 关灯
     */
    void Close();

    /**
     * @brief 常亮
     * @note 台灯照明模式
     */
    void Solid();

    /**
     * @brief 幻彩模式(非音乐随变)
     */
    void Mirage();

    /**
     * @brief 音乐随变模式
     */
    void Music();

    /**
     * @brief 选择幻彩方案
     * @param mirage_choice 幻彩方案编号
     */
    void SetMirageChoice(int mirage_choice);

    /**
     * @brief 选择音乐方案
     * @param music_choice 音乐方案编号
     */
    void SetMusicChoice(int music_choice);

   private:
    int m_step;           // 颜色渐变步长
    int m_db;             // 声强传感器虚拟值
    int m_mirage_choice;  // 幻彩方案选择变量
    int m_music_choice;   // 音乐方案选择变量

    CRGB m_leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];  // 三排灯珠RGB颜色数组
    CRGB m_leds_target[NUM_STRIPS]
                      [NUM_LEDS_PER_STRIP];  // 三排灯珠RGB变换目标颜色数组

    /**
     * @brief 黄昏模式
     * @note 灯珠在橙色与红色色域间随机变换
     */
    void MM_Dusk() {
        static int H_Max = 48;  // 橙色色域
        static int H_Min = 0;   // 红色色域

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

    /**
     * @brief 蹦迪模式
     * @note 灯珠在红蓝紫随变切换
     */
    void MM_Disco() {
        m_step = 4;

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

    /**
     * @brief 梦幻音乐随变模式
     * @note 声强大小决定颜色渐变速率
     * @note 声强大小决定颜色集群数量
     * @note 声强大小决定颜色丰富程序(色域宽度)
     */
    void MM_Fantasy() {
        float color_rate = MAX(10.0f, ((float)m_db - (float)VOICE_DOWN_DB)) /
                           (float)VOICE_WIDTH_DB;

        m_step = color_rate * 3.0f;

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
};

#endif
