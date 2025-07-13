/**
 * @file drv_uart.h
 * @brief UART驱动模块头文件
 * @details 定义UART驱动模块的接口函数，提供串口通信功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

EXTERN void drv_uart_init(uint8_t uart_num, uint8_t *recv_buf, uint16_t recv_buf_len, uint16_t send_buf_len);
EXTERN void drv_uart_send_data(uint8_t uart_num, uint8_t *buf, uint16_t len);
EXTERN uint8_t *drv_uart_get_recv_buf(uint8_t uart_num);
EXTERN void drv_uart_clear_recv_buf(uint8_t uart_num);
EXTERN uint16_t drv_uart_get_recv_len(uint8_t uart_num);

#endif /* __DRV_UART_H__ */
