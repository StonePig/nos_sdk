/**
 * @file nos_key.h
 * @brief 中间层按键模块头文件
 * @details 定义中间层按键模块的接口函数，封装底层按键驱动，提供按键事件处理服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_KEY_H__
#define __NOS_KEY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define DEBOUNCE_TIME 10            // debounce time in milliseconds
#define LONG_PRESS_TIME 3000        // long press time in milliseconds
#define LONG_REPEAT_PRESS_TIME 2000 // debounce time in milliseconds
#define DOUBLE_CLICK_TIME 500       // double click time in milliseconds

    typedef uint8_t key_value_t;

    typedef enum
    {
        EVENT_ID_KEY_PRESSED,
        EVENT_ID_KEY_RELEASED,
        EVENT_ID_KEY_LONG_PRESSED,
        EVENT_ID_KEY_LONG_RELEASED,
        EVENT_ID_KEY_LONG_PRESSED_REPEAT,
        EVENT_ID_KEY_DOUBLE_CLICK,
        EVENT_ID_KEY_CLICK,             // down->up
        EVENT_ID_KEY_COMBINE,
    } key_event_id_t;

    typedef enum
    {
        KEY_STATE_IDLE = 0,
        KEY_STATE_PRESSED,
        KEY_STATE_RELEASED,
        KEY_STATE_LONGPRESS,
        KEY_STATE_DOUBLECLICK
    } key_state_t;

    // 定义按键结构体
    typedef struct
    {
        key_state_t state;        // 当前按键状态
        uint16_t debounce;        // 按键消抖时间间隔（毫秒）
        uint16_t longpress;       // 按键长按时间阈值（毫秒）
        uint16_t long_repeat;     // 按键长按时间阈值（毫秒）
        uint16_t doubleclick;     // 按键双击时间阈值（毫秒）
        uint16_t last_time;       // 上一次按键操作时间戳（毫秒）
        uint16_t last_press_time; // 上一次按键按下的时间戳（毫秒）
        // uint16_t click_count;     // 当前连续按下次数
    } key_info_t;

    typedef struct
    {
        uint16_t key_value;    // 组合键值
        uint16_t pressed_time; // 按键消抖时间间隔（毫秒）
    } key_combine_t;

    typedef void (*key_cb_t)(key_value_t key_num, key_event_id_t event_id);

    void nos_key_init(void);
    uint32_t nos_key_get_tick_from_last_key_released(void);
    uint32_t nos_key_get_tick_from_last_key_pressed(void);
    bool nos_key_have_key_pressed(void);
    // 设置按键参数
    void nos_key_set_parm(key_value_t key_num, uint16_t debounce, uint16_t longpress, uint16_t long_repeat, uint16_t doubleclick);
    // 注册按键回调函数
    void nos_key_regist_cb(key_cb_t key_cb);
    // 设置组合键触发条件
    void nos_key_set_combine_parm(uint8_t num, uint16_t combine_key_value, uint16_t pressed_time);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_KEY_H__ */
