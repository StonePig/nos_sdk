/**
 * @file drv_sys_tick.h
 * @brief 系统时钟驱动模块头文件
 * @details 定义系统时钟驱动模块的接口函数，提供系统时间获取和设置功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_SYS_TICK_H__
#define __DRV_SYS_TICK_H__

EXTERN uint32_t drv_get_cur_sys_tick(void);
EXTERN uint32_t drv_get_cur_sys_second(void);
EXTERN void drv_set_cur_sys_second(uint32_t sec);
EXTERN void drv_add_cur_sys_second(void);

#endif /* __DRV_SYS_TICK_H__ */
