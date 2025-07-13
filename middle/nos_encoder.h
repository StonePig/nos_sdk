/**
 * @file nos_encoder.h
 * @brief 中间层编码器模块头文件
 * @details 定义中间层编码器模块的接口函数，封装底层编码器驱动，提供编码器数据处理服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_ENCODER_H__
#define __NOS_ENCODER_H__

#ifdef __cplusplus
extern "C"
{
#endif
    
    int32_t nos_encode_get_speed(uint8_t encoder_num);
    int32_t nos_encode_get_pos(uint8_t encoder_num);
    bool nos_encoder_is_pressed(uint8_t encoder_num);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_ENCODER_H__ */
