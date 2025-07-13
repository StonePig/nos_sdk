/**
 * @file drv_misc.h
 * @brief 杂项驱动模块头文件
 * @details 定义杂项驱动模块的接口函数，提供系统中断、复位、休眠和看门狗功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_MISC_H__
#define __DRV_MISC_H__

EXTERN void drv_sys_int(void);

EXTERN void drv_sys_reset(void);
EXTERN void drv_sys_halt(void);
EXTERN void drv_sys_sleep(uint32_t ms);
EXTERN void drv_feed_watchdog(void);

#endif /* __DRV_MISC_H__ */
