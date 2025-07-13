/**
 * @file nos_lcd.h
 * @brief 中间层LCD模块头文件
 * @details 定义中间层LCD模块的接口函数，封装底层LCD驱动，提供LCD显示控制服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_LCD_H__
#define __NOS_LCD_H__

#ifdef __cplusplus
extern "C"
{
#endif

    // 刷新显示缓冲区内容
    void nos_lcd_update(uint8_t *buf, uint8_t depth);
    void lcd_disp_show_pixel(uint16_t x, uint16_t y, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_LCD_H__ */
