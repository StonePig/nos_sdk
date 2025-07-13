#ifndef _MODBUS_APP_H_
#define _MODBUS_APP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../middle/nos_common.h"

// 应用使能宏定义
#define MODBUS_APP_EN 1

// 寄存器最大数量
#define MODBUS_REG_MAX_NUM 8

// Modbus状态枚举
typedef enum
{
    MODBUS_STATUS_OFFLINE = 0,    // 离线
    MODBUS_STATUS_ONLINE,         // 在线
    MODBUS_STATUS_ERROR,          // 错误
    MODBUS_STATUS_TIMEOUT,        // 超时
} modbus_status_t;

// 应用配置结构
typedef struct
{
    uint8_t master_enable;        // 主站使能
    uint8_t slave_enable;         // 从站使能
    uint8_t uart_num;             // 串口号
    uint32_t baud_rate;           // 波特率
    uint8_t master_addr1;         // 主站设备地址1
    uint8_t master_addr2;         // 主站设备地址2
    uint8_t slave_addr;           // 从站地址
    uint16_t reg_start_addr;      // 寄存器起始地址
    uint8_t reg_count;            // 寄存器数量
    uint16_t cmd_timeout;         // 命令超时时间(ms)
    uint16_t send_interval;       // 发送间隔(ms)
} modbus_app_config_t;

// 应用状态结构
typedef struct
{
    modbus_status_t master_status;    // 主站状态
    modbus_status_t slave_status;     // 从站状态
    uint32_t error_count;             // 错误计数
    uint32_t timeout_count;           // 超时计数
    uint16_t reg_data[MODBUS_REG_MAX_NUM];  // 寄存器数据
} modbus_app_status_t;

typedef enum
{
    GROUP_MASTER,
    GROUP_SLAVE,
    GROUP_COMMAND,
    GROUP_STATUS,
    GROUP_REGISTER
} group_name_t;

// 函数声明

/**
 * @brief 初始化Modbus应用
 */
void modbus_app_init(void);

/**
 * @brief 更新应用状态
 */
void modbus_app_update_status(void);

/**
 * @brief 处理寄存器数据
 */
void modbus_app_process_reg_data(void);

/**
 * @brief 发送Modbus命令
 */
void modbus_app_send_commands(void);

/**
 * @brief 处理命令队列
 */
void modbus_app_process_command_queue(void);

/**
 * @brief 获取应用配置
 * @return 应用配置指针
 */
modbus_app_config_t* modbus_app_get_config(void);

/**
 * @brief 设置应用配置
 * @param config 配置参数指针
 */
void modbus_app_set_config(const modbus_app_config_t* config);

/**
 * @brief 获取应用状态
 * @return 应用状态指针
 */
modbus_app_status_t* modbus_app_get_status(void);

/**
 * @brief 设置寄存器数据
 * @param reg_index 寄存器索引
 * @param value 寄存器值
 */
void modbus_app_set_reg_data(uint8_t reg_index, uint16_t value);

/**
 * @brief 获取寄存器数据
 * @param reg_index 寄存器索引
 * @return 寄存器值
 */
uint16_t modbus_app_get_reg_data(uint8_t reg_index);

/**
 * @brief 清除错误计数
 */
void modbus_app_clear_errors(void);

/**
 * @brief 重置通信
 */
void modbus_app_reset_communication(void);

/**
 * @brief 用户应用任务主函数
 */
void user_app_task(void);

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_APP_H_ 