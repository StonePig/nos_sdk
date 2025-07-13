/**
 * @file drv_common.h
 * @brief 驱动层公共头文件
 * @details 包含所有驱动层模块的头文件引用，提供统一的驱动层接口
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#include <string.h>
#include "../../config.h"
#include "../../data_type.h"

#include "drv_io.h"
#include "drv_sys_tick.h"
#include "drv_debug.h"
#include "drv_led.h"
#include "drv_misc.h"
#include "drv_adc.h"
#include "drv_pwm.h"
#include "drv_key.h"
#include "drv_lcd.h"
#include "drv_uart.h"
#include "drv_can.h"
#include "drv_nvram.h"
#include "drv_encoder.h"
#include "drv_touchpad.h"
#include "drv_ui.h"

#ifdef _WINDOWS
#include "win32.h"
#endif

#endif /* __DRV_COMMON_H__ */
