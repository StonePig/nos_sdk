/**
 * @file drv_pwm.h
 * @brief PWM驱动模块头文件
 * @details 定义PWM驱动模块的接口函数，提供PWM输出控制功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

EXTERN void drv_pwm_set_value(uint8_t pwm_num, uint16_t value);

#endif /* __DRV_PWM_H__ */
