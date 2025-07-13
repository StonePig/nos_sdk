/**
 * @file nos_adc.h
 * @brief 中间层ADC模块头文件
 * @details 定义中间层ADC模块的接口函数，封装底层ADC驱动，提供安全的ADC访问接口
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_ADC_H__
#define __NOS_ADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

    uint16_t nos_adc_get_value(uint8_t adc_num);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_ADC_H__ */
