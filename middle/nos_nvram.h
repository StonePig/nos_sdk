/**
 * @file nos_nvram.h
 * @brief 中间层NVRAM模块头文件
 * @details 定义中间层NVRAM模块的接口函数，封装底层NVRAM驱动，提供非易失性存储服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_NVRAM_H__
#define __NOS_NVRAM_H__

#ifdef __cplusplus
extern "C"
{
#endif

    uint8_t nvram_file_check(uint8_t nvram_id, uint16_t buf_len);
    void nos_nvram_init(uint8_t nvram_id, uint8_t *default_buf, uint16_t buf_len);
    void nos_nvram_write(uint8_t nvram_id, uint8_t *buf, uint16_t buf_len);
    void nos_nvram_read(uint8_t nvram_id, uint8_t *buf, uint16_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_NVRAM_H__ */
