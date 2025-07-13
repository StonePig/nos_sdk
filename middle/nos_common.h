/**
 * @file nos_common.h
 * @brief 中间层公共头文件
 * @details 包含所有中间层模块的头文件引用，提供统一的中间层接口
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_COMMON_H__
#define __NOS_COMMON_H__

#include "../config.h"
#include "../data_type.h"

#include "../platform/include/drv_common.h"

#include "nos_log.h"
#include "nos_io.h"
#include "nos_sys_tick.h"
#include "nos_led.h"
#include "nos_adc.h"
#include "nos_pwm.h"
#include "nos_key.h"
#include "nos_msg.h"
#include "nos_lcd.h"
#include "nos_uart.h"
#include "nos_can.h"
#include "nos_i2c.h"
#include "nos_time.h"
#include "nos_mmi_mgr.h"
#include "nos_nvram.h"
#include "nos_encoder.h"
#include "nos_touchpad.h"

#endif /* __NOS_COMMON_H__ */
