/**
 * @file nos_uart.h
 * @brief 中间层UART模块头文件
 * @details 定义中间层UART模块的接口函数，封装底层UART驱动，提供串口通信服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_UART_H__
#define __NOS_UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*uart_recv_cb_t)(uint8_t uart_num, uint8_t xdata_t *buf, uint16_t len);

    void nos_uart_send_data(uint8_t uart_num, uint8_t *buf, uint16_t len);
    void nos_uart_recv_regist_cb(uint8_t uart_num, uart_recv_cb_t recv_cb);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_UART_H__ */
