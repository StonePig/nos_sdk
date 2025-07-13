/**
 * @file nos_can.h
 * @brief 中间层CAN模块头文件
 * @details 定义中间层CAN模块的接口函数，封装底层CAN驱动，提供CAN总线通信服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_CAN_H__
#define __NOS_CAN_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*can_recv_cb_t)(uint8_t can_num, uint32_t addr, uint8_t *buf, uint16_t len);

    void nos_can_init(void);
    void nos_can_task(void);

    void nos_can_send_data(uint8_t can_num, uint32_t addr, uint8_t *buf, uint8_t len);
    void nos_can_recv_regist_cb(uint8_t can_num, can_recv_cb_t recv_cb);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_CAN_H__ */
