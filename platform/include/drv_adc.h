/**
 * @file drv_adc.h
 * @brief ADC驱动模块头文件
 * @details 定义ADC驱动模块的接口函数，提供ADC数据读取功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__

EXTERN uint16_t drv_adc_get_value(uint8_t adc_num);

#endif /* __DRV_ADC_H__ */
