/**
 * @file nos_pwm.h
 * @brief 中间层PWM模块头文件
 * @details 定义中间层PWM模块的接口函数，封装底层PWM驱动，提供PWM输出控制服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_PWM_H__
#define __NOS_PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif

    void nos_pwm_set_value(uint8_t pwm_num, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_PWM_H__ */
