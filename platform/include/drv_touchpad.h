/**
 * @file drv_touchpad.h
 * @brief 触摸板驱动模块头文件
 * @details 定义触摸板驱动模块的接口函数，提供触摸检测和坐标获取功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_TOUCHPAD_H__
#define __DRV_TOUCHPAD_H__


EXTERN bool drv_touchpad_is_pressed(void);
EXTERN void drv_touchpad_get_xy(uint16_t *x, uint16_t *y);

#endif /* __DRV_TOUCHPAD_H__ */
