/**
 * @file nos_io.h
 * @brief 中间层IO模块头文件
 * @details 定义中间层IO模块的接口函数，封装底层IO驱动，提供GPIO控制服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_IO_H__
#define __NOS_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum {
        NOS_IO_DIR_IN = 0,
        NOS_IO_DIR_OUT
    }nos_io_dir_t;

    void nos_io_set_value(uint8_t io_num, uint8_t value);
    uint8_t nos_io_get_value(uint8_t io_num);
    void nos_io_set_dir(uint8_t io_num, nos_io_dir_t dir);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_IO_H__ */
