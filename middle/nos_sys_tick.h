/**
 * @file nos_sys_tick.h
 * @brief 中间层系统时钟模块头文件
 * @details 定义中间层系统时钟模块的接口函数，提供系统时钟管理和超时检测服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_SYS_TICK_H__
#define __NOS_SYS_TICK_H__

#ifdef __cplusplus
extern "C"
{
#endif

    void nos_set_current_tick(uint32_t *p);
    uint32_t nos_get_lapse_tick(uint32_t tick);
    void nos_set_current_tick_16(uint16_t *p);
    uint16_t nos_get_lapse_tick_16(uint16_t tick);
    bool nos_tick_timeout(uint32_t *p, uint32_t period);
    bool nos_tick_timeout_16(uint16_t *p, uint16_t period);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_SYS_TICK_H__ */
