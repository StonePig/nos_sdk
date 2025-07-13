/**
 * @file drv_can.h
 * @brief CAN驱动模块头文件
 * @details 定义CAN驱动模块的接口函数，提供CAN总线通信功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__

EXTERN void drv_can_init(uint8_t can_num, uint8_t *recv_buf, uint16_t recv_buf_len, uint8_t send_buf_len);
EXTERN void drv_can_send_data(uint8_t can_num, uint32_t addr, uint8_t *buf, uint8_t len);
EXTERN uint8_t *drv_can_get_recv_buf(uint8_t can_num, uint32_t *addr);
EXTERN void drv_can_clear_recv_buf(uint8_t can_num);
EXTERN uint16_t drv_can_get_recv_len(uint8_t can_num);

#endif /* __DRV_CAN_H__ */
