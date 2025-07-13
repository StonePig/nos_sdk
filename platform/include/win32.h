/**
 * @file win32.h
 * @brief 驱动层Win32模块头文件
 * @details 定义驱动层Win32模块的接口函数，提供Windows平台下的系统资源管理服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __WIN32_H__
#define __WIN32_H__

#include <assert.h>

#define GROUP_MAX_NUM 20
#define GROUP_CONTROL_MAX_NUM 200
#define UART_MAX_NUM    20
#define CAN_MAX_NUM    5

typedef struct
{
    uint8_t io_input_value[IO_INPUT_NUM + 1];
    uint8_t io_output_value[IO_OUTPUT_NUM + 1];
    uint8_t io_output_chart_display[IO_OUTPUT_NUM + 1]; // 0: disable, 1: enable, 2: pause
    uint16_t adc_value[ADC_NUM + 1];
    uint16_t pwm_value[PWM_NUM + 1];
    uint8_t is_key_pressed[KEY_NUM + 1];

    uint8_t *recv_buf[UART_MAX_NUM];
    uint16_t recv_buf_len[UART_MAX_NUM];
    uint16_t recv_cur_len[UART_MAX_NUM];
    uint16_t send_buf_len[UART_MAX_NUM];

    uint8_t *can_recv_buf[CAN_MAX_NUM];
    uint16_t can_recv_buf_len[CAN_MAX_NUM];
    uint16_t can_recv_cur_len[CAN_MAX_NUM];
    uint8_t can_send_buf_len[CAN_MAX_NUM]; 
    uint32_t can_addr[CAN_MAX_NUM];   

    int32_t encoder_counter[ENCODER_NUM + 1];
    int32_t encoder_counter_prev[ENCODER_NUM + 1];
    int32_t encoder_speed[ENCODER_NUM + 1];
    bool encoder_pressed[ENCODER_NUM + 1];

    uint16_t touchpad_x;
    uint16_t touchpad_y;
    bool touchpad_pressed;

    control_parm_t control[GROUP_MAX_NUM][GROUP_CONTROL_MAX_NUM];

    uint32_t cur_tick;
    uint32_t cur_sec;

} resource_t;

#ifdef __cplusplus
extern "C"
{
    resource_t res_value = {};
    void win32_update_res_io(uint8_t io_num, uint8_t value);
    void win32_update_res_pwm(uint8_t pwm_num, uint16_t value);
    void win32_update_res_control_value(uint8_t group_id, uint8_t control_id);
    void win32_update_reminder(const uint8_t *buf, uint32_t color);
    void win32_write_file(uint8_t *file_name, uint8_t *file_buf, uint32_t length, uint8_t is_append);
    uint32_t win32_read_file(uint8_t *file_name, uint8_t *file_buf);
    uint8_t win32_is_uart_opend(uint8_t uart_num);
    void win32_update_res_led_buff(uint8_t *buf);
    void win32_update_res_lcd_buff(uint8_t *buf, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth);
    void win32_get_lcd_info(uint16_t *width, uint16_t *height, uint8_t *depth);
    void win32_uart_write2buff(uint8_t uart_num, uint8_t *buff, uint16_t len);
    void win32_can_send_data(uint8_t can_num, uint32_t addr, uint8_t *send_data, uint8_t send_data_len);
    void win32_update_uart_recv_buf(uint8_t uart_num, uint8_t *buf, uint16_t len);
    void win32_sys_sleep(uint32_t ms);
    void win32_output_log(const char *fmt, ...);
    void win32_file_write(uint32_t address, uint8_t *buf, uint16_t len);
    uint8_t win32_file_read_one_byte(uint32_t address);
    void win32_show_title(uint8_t *title);
    uint8_t win32_i2c_read(uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);
    uint8_t win32_i2c_write(uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);
    void win32_set_us_timer(uint32_t us, timer_cb_t cb);
}
#else
extern resource_t res_value;
extern void win32_update_res_io(uint8_t io_num, uint8_t value);
extern void win32_update_res_pwm(uint8_t pwm_num, uint16_t value);
extern void win32_update_res_control_value(uint8_t group_id, uint8_t control_id);
extern void win32_update_reminder(const uint8_t *buf, uint32_t color);
extern void win32_write_file(uint8_t *file_name, uint8_t *file_buf, uint32_t length, uint8_t is_append);
extern uint32_t win32_read_file(uint8_t *file_name, uint8_t *file_buf);
extern uint8_t win32_is_uart_opend(uint8_t uart_num);
extern void win32_update_res_led_buff(uint8_t *buf);
extern void win32_update_res_lcd_buff(uint8_t *buf, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth);
extern void win32_get_lcd_info(uint16_t *width, uint16_t *height, uint8_t *depth);
extern void win32_uart_write2buff(uint8_t uart_num, uint8_t *buff, uint16_t len);
extern void win32_can_send_data(uint8_t can_num, uint32_t addr, uint8_t *send_data, uint8_t send_data_len);
extern void win32_update_uart_recv_buf(uint8_t uart_num, uint8_t *buf, uint16_t len);
extern void win32_sys_sleep(uint32_t ms);
extern void win32_output_log(const char *fmt, ...);
extern void win32_file_write(uint32_t address, uint8_t *buf, uint16_t len);
extern uint8_t win32_file_read_one_byte(uint32_t address);
extern void win32_show_title(uint8_t *title);
extern uint8_t win32_i2c_read(uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);
extern uint8_t win32_i2c_write(uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);
extern void win32_set_us_timer(uint32_t us, timer_cb_t cb);
#endif

#endif /* __WIN32_H__ */