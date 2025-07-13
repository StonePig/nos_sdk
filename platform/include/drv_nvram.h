/**
 * @file drv_nvram.h
 * @brief NVRAM驱动模块头文件
 * @details 定义NVRAM驱动模块的接口函数，提供非易失性存储器的读写擦除功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_NVRAM_H__
#define __DRV_NVRAM_H__

EXTERN void drv_flash_erase_block(uint32_t address);
EXTERN void drv_flash_write_one_word(uint32_t address, uint32_t one_word);
EXTERN uint8_t drv_flash_read_one_byte(uint32_t address);

#endif /* __DRV_NVRAM_H__ */
