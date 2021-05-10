# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
        
# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
#pragma message "DBMAL(Arduino-Demo) version 2.000.000"
# 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"




# 6 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino" 2
# 7 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino" 2

# 9 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino" 2
# 10 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino" 2


SAMDTimer m_timer_tcc{TIMER_TCC}; // SAMD21定时器句柄
Stater m_stater{10 /* 感应开关对应数字端*/, 250 /* 按住超过2秒视为长按*/};
Strip m_strip;

/**

 * @brief DBMAL定时器执行函数

 * @note 检查更新系统状态 -> 根据系统状态执行对应颜色方案 -> 更新灯珠颜色

 */
# 20 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
static void DBMAL_Event() {
    m_stater.StateUpdate();

    switch (m_stater.GetSysState()) {
        case CLOSE:
            m_strip.Close();
            break;

        case SOLID:
            m_strip.Solid();
            break;

        case MIRAGE:
            m_strip.Mirage();
            break;

        case MUSIC:
            m_strip.Music();
            break;
    }
    m_strip.UpdateFade();
    // m_strip.Update();

    FastLED.show();
}

void setup() {
    // 增加定时器中断执行函数, 每8ms执行一次
    m_timer_tcc.attachInterruptInterval(8 /* 系统频率在125hz左右*/ * 1000, DBMAL_Event);




}

void loop() {



}
