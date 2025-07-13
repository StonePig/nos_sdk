/**
 * @file drv_lcd.h
 * @brief LCD驱动模块头文件
 * @details 定义LCD驱动模块的接口函数，提供LCD显示控制功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

EXTERN _INLINE void drv_lcd_init(void);

// 刷新显示缓冲区内容
EXTERN void drv_lcd_update(uint8_t *buf, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth);

// 是否冻结刷新
EXTERN void drv_lcd_update_lock(uint8_t lock_enble);

#endif /* __DRV_LCD_H__ */
