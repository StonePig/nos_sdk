/**
 * @file nos_led.h
 * @brief 中间层LED模块头文件
 * @details 定义中间层LED模块的接口函数，封装底层LED驱动，提供LED显示控制服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_LED_H__
#define __NOS_LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if LED_WINK_EN

    typedef enum
    {
        WINK_OFF = 0,
        WINK_ON = 1,
    } wink_on_off_t;

    typedef enum
    {
        WINK_MODE_NONE = 0,
        WINK_MODE_ON,
        WINK_MODE_OFF,
        WINK_MODE_WINK_TO_OFF,
        WINK_MODE_WINK_TO_ON,
        WINK_MODE_WINK_FOREVER,
    } wink_mode_t;

    typedef struct
    {
        wink_mode_t mode;             /* 运行模式 */
        uint8_t times; /* 执行次数 */ // 非0执行闪烁，0停止退出，0xff永久
        uint8_t on_time;              /* 点亮时间 单位100ms*/
        uint8_t off_time;             /* 熄灭时间 单位100ms*/
        uint16_t counter;              /* 已执行100ms的次数 */
    } wink_info_t;

    void nos_led_wink(uint16_t led_num, uint8_t on_time, uint8_t off_time, uint8_t times, wink_mode_t mode);
    void nos_led_disp_char_wink(uint8_t ascii, uint8_t pos, uint8_t on_time, uint8_t off_time, uint8_t times, wink_mode_t mode);
    // 设置数码管显示闪烁模式，点亮时间为on_time, 熄灭时间为off_time
    void nos_led_disp_set_wink(uint8_t pos, uint8_t on_time, uint8_t off_time, uint8_t times, wink_mode_t mode);
#endif

    void nos_led_disp_buf_backup(void);
    void nos_led_disp_buf_restore(void);
    
    void nos_led_on_all(void);
    void nos_led_off_all(void);

    void nos_led_on(uint16_t led_num);
    void nos_led_off(uint16_t led_num);

    void nos_led_disp_char(uint8_t ascii, uint8_t pos);
    void nos_led_disp_buf(uint8_t seg_data, uint8_t pos);
    void nos_led_disp_str(uint8_t *str, uint8_t pos, bool is_clean);
    void nos_led_disp_number(uint32_t disp_num, uint16_t divisor, uint8_t pos, bool is_clean);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_LED_H__ */
