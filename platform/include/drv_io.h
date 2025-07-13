/**
 * @file drv_io.h
 * @brief IO驱动模块头文件
 * @details 定义IO驱动模块的接口函数，提供GPIO控制功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_IO_H__
#define __DRV_IO_H__

typedef enum {
    DRV_IO_DIR_IN = 0,
    DRV_IO_DIR_OUT
}drv_io_dir_t;

// 设置制定IO口的值
EXTERN void drv_io_set_value(uint8_t io_num, uint8_t value);
// 获取制定IO口的值
EXTERN uint8_t drv_io_get_value(uint8_t io_num);
// 设置制定IO口的方向
EXTERN void drv_io_set_dir(uint8_t io_num, drv_io_dir_t dir);

#endif /* __DRV_IO_H__ */
