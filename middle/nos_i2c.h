/**
 * @file nos_i2c.h
 * @brief 中间层I2C模块头文件
 * @details 定义中间层I2C模块的接口函数，提供I2C通信服务，支持读写操作
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_I2C_H__
#define __NOS_I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

  uint8_t nos_i2c_read(uint8_t ic_num, uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);
  uint8_t nos_i2c_write(uint8_t ic_num, uint8_t slave_addr, uint8_t reg, uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_I2C_H__ */
