#ifndef __STATER_H
#define __STATER_H

enum SYS_STATE { CLOSE = 0, SOLID, MIRAGE, MUSIC };  // 系统状态枚举量

class Stater {
   public:
    /**
     * @brief 触摸开关状态器构造函数
     * @param pin 触摸开关输入引脚
     * @param long_holoding_ms 长按识别时间(毫秒)
     */
    explicit Stater(int pin, int long_holoding_ms = 1500);

    /**
     * @brief 触摸开关状态器更新函数
     * @note 检测按键状态并更新台灯状态
     */
    void StateUpdate();

    /**
     * @brief 获取触摸开关状态器当前系统状态
     * @note 按键状态无法被获取, 只能获取系统状态
     */
    int GetSysState();

   private:
    int m_pin;
    int m_counter_limit;

    int m_switch_state_counter;
    enum SWITCH_STATE { RELEASE = 0, TOUCH } m_switch_state;  // 开关状态枚举量
    enum SYS_STATE m_system_state;
};
#endif