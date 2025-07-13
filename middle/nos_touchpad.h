/**
 * @file nos_touchpad.h
 * @brief 中间层触摸板模块头文件
 * @details 定义中间层触摸板模块的接口函数，封装底层触摸板驱动，提供触摸检测服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_TOUCHPAD_H__
#define __NOS_TOUCHPAD_H__

#ifdef __cplusplus
extern "C"
{
#endif

    bool nos_touchpad_is_pressed(void);
    void nos_touchpad_get_xy(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_TOUCHPAD_H__ */
