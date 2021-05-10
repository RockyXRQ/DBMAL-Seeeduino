#ifndef __STATER_H
#define __STATER_H

enum SYS_STATE { CLOSE = 0, SOLID, MIRAGE, MUSIC };  // 全局状态枚举量

class Stater {
   private:
    int m_pin;
    int m_counter_limit;

    int m_switch_state_counter;
    enum SWITCH_STATE { RELEASE = 0, TOUCH } m_switch_state;  // 开关状态枚举量
    enum SYS_STATE m_system_state;

   public:
    explicit Stater(int pin, int counter_limit = 200);
    void StateUpdate();  // 检测按键状态并更新台灯状态
    int GetSysState();
};
#endif