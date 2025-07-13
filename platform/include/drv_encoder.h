/**
 * @file drv_encoder.h
 * @brief 编码器驱动模块头文件
 * @details 定义编码器驱动模块的接口函数，提供编码器速度、位置和按键检测功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_ENCODER_H__
#define __DRV_ENCODER_H__

int32_t drv_encode_get_speed(uint8_t encoder_num);
int32_t drv_encode_get_pos(uint8_t encoder_num);
bool drv_encoder_is_pressed(uint8_t encoder_num);

#endif /* __DRV_ENCODER_H__ */

