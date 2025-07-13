#if __MODBUS_DEMO_ENABLE__

#include "../../middle/external/modbus/mbrtumaster.h"
#include "../../middle/external/general/cqueue.h"
#include "modbus_app.h"
#include "modbus_master_app.h"
#include "modbus_slave_app.h"
#include "../../middle/nos_common.h"

static uint16_t tick;

// 应用配置参数
static modbus_app_config_t modbus_app_config = {
    .master_enable = 1,      // 启用主站功能
    .slave_enable = 1,       // 启用从站功能
    .uart_num = 0,           // 使用UART0
    .baud_rate = 9600,       // 波特率9600
    .master_addr1 = 1,       // 主站设备地址1
    .master_addr2 = 2,       // 主站设备地址2
    .slave_addr = 1,         // 从站地址
    .reg_start_addr = 0,     // 寄存器起始地址
    .reg_count = 8,          // 寄存器数量
    .cmd_timeout = 500,      // 命令超时时间(ms)
    .send_interval = 300,    // 发送间隔(ms)
};

// 应用状态
static modbus_app_status_t modbus_app_status = {
    .master_status = MODBUS_STATUS_OFFLINE,
    .slave_status = MODBUS_STATUS_OFFLINE,
    .error_count = 0,
    .timeout_count = 0,
    .reg_data = {0},         // 寄存器数据
};

// 应用任务句柄
static uint16_t modbus_app_tick = 0;
static uint16_t modbus_app_status_tick = 0;

void control_event_handler(uint8_t group_id, uint8_t value_num, uint32_t value)
{
    if (group_id == GROUP_COMMAND)
    {
        if (win32_is_uart_opend(MODBUS_MASTER_UART_NUM) == false)
        {
            // 根据颜色设置图标类型
            ui_show_reminder("请打开串口！", 0xFFFF00);
        }
    }
}

/**
 * @brief 初始化Modbus应用
 */
void modbus_app_init(void)
{
    static bool is_init = false;
    if (is_init)
        return;
    
    is_init = true;
    
    // 初始化主站功能
    if (modbus_app_config.master_enable)
    {
        modbus_master_app_task_init();
        PRINT_LOG("Modbus Master initialized\r\n");
    }
    
    // 初始化从站功能
    if (modbus_app_config.slave_enable)
    {
        modbus_slave_app_task_init();
        PRINT_LOG("Modbus Slave initialized\r\n");
    }
    
    // 初始化应用状态
    modbus_app_status.master_status = MODBUS_STATUS_ONLINE;
    modbus_app_status.slave_status = MODBUS_STATUS_ONLINE;

    ui_control_set_callback(0xFF, 0, control_event_handler);
    
    PRINT_LOG("Modbus App initialized successfully\r\n");
}

/**
 * @brief 更新应用状态
 */
void modbus_app_update_status(void)
{
    // 更新主站状态
    if (modbus_app_config.master_enable)
    {
        // 这里可以根据实际的通信状态更新
        modbus_app_status.master_status = MODBUS_STATUS_ONLINE;
    }
    
    // 更新从站状态
    if (modbus_app_config.slave_enable)
    {
        // 这里可以根据实际的通信状态更新
        modbus_app_status.slave_status = MODBUS_STATUS_ONLINE;
    }
}

/**
 * @brief 处理寄存器数据
 */
void modbus_app_process_reg_data(void)
{
    // 更新寄存器数据（这里可以从ADC或其他数据源获取）
    for (int i = 0; i < MODBUS_REG_MAX_NUM; i++)
    {
        if (i < 3)
        {
            // 前3个寄存器使用ADC数据
            modbus_app_status.reg_data[i] = nos_adc_get_value(i + 1);
        }
        else
        {
            // 其他寄存器使用递增数据
            modbus_app_status.reg_data[i] = i * 100;
        }
    }
}

/**
 * @brief 发送Modbus命令
 */
// void modbus_app_send_commands(void)
// {
//     if (!modbus_app_config.master_enable)
//         return;
    
//     // 发送写命令
//     if (modbus_app_put_cmd(modbus_app_config.master_addr1, MODBUS_CMD_ID0, MODBUS_CMD_WRITE))
//     {
//         PRINT_LOG("Write command sent to slave %d\r\n", modbus_app_config.master_addr1);
//     }
//     else
//     {
//         modbus_app_status.error_count++;
//         PRINT_LOG("Failed to send write command\r\n");
//     }
    
//     // 发送读命令
//     if (modbus_app_put_cmd(modbus_app_config.master_addr1, MODBUS_CMD_ID1, MODBUS_CMD_READ))
//     {
//         PRINT_LOG("Read command sent to slave %d\r\n", modbus_app_config.master_addr1);
//     }
//     else
//     {
//         modbus_app_status.error_count++;
//         PRINT_LOG("Failed to send read command\r\n");
//     }
// }

// /**
//  * @brief 处理命令队列
//  */
// void modbus_app_process_command_queue(void)
// {
//     if (!modbus_app_config.master_enable)
//         return;
    
//     modbus_app_get_cmd();
// }

/**
 * @brief 获取应用配置
 */
modbus_app_config_t* modbus_app_get_config(void)
{
    return &modbus_app_config;
}

/**
 * @brief 设置应用配置
 */
void modbus_app_set_config(const modbus_app_config_t* config)
{
    if (config != NULL)
    {
        modbus_app_config = *config;
        PRINT_LOG("Modbus app config updated\r\n");
    }
}

/**
 * @brief 获取应用状态
 */
modbus_app_status_t* modbus_app_get_status(void)
{
    return &modbus_app_status;
}

/**
 * @brief 设置寄存器数据
 */
void modbus_app_set_reg_data(uint8_t reg_index, uint16_t value)
{
    if (reg_index < MODBUS_REG_MAX_NUM)
    {
        modbus_app_status.reg_data[reg_index] = value;
    }
}

/**
 * @brief 获取寄存器数据
 */
uint16_t modbus_app_get_reg_data(uint8_t reg_index)
{
    if (reg_index < MODBUS_REG_MAX_NUM)
    {
        return modbus_app_status.reg_data[reg_index];
    }
    return 0;
}

/**
 * @brief 清除错误计数
 */
void modbus_app_clear_errors(void)
{
    modbus_app_status.error_count = 0;
    modbus_app_status.timeout_count = 0;
    PRINT_LOG("Error counters cleared\r\n");
}

/**
 * @brief 重置通信
 */
void modbus_app_reset_communication(void)
{
    // 重新初始化应用
    modbus_app_init();
    modbus_app_clear_errors();
    PRINT_LOG("Communication reset\r\n");
}

/**
 * @brief 用户应用任务主函数
 */
void user_app_task(void)
{
    // 初始化应用
    modbus_app_init();
    
    // 主循环
    if (nos_tick_timeout_16(&tick, 10))
    {
        ui_control_set_value(GROUP_STATUS,3,tick);
        // 处理主站任务
        if (modbus_app_config.master_enable)
        {
            modbus_master_app_task();
        }
        
        // 处理从站任务
        // if (modbus_app_config.slave_enable)
        // {
        //     modbus_slave_app_task();
        // }
        
        // 定期更新状态 (1秒)
        // if (nos_tick_timeout_16(&modbus_app_status_tick, 1000))
        // {
        //     modbus_app_update_status();
        //     modbus_app_process_reg_data();
        // }
        
        // // 定期发送命令 (根据配置的发送间隔)
        // if (nos_tick_timeout_16(&modbus_app_tick, modbus_app_config.send_interval))
        // {
        //     modbus_app_send_commands();
        // }
        
        // // 处理命令队列 (100ms)
        // if (nos_tick_timeout_16(&modbus_app_tick, 100))
        // {
        //     modbus_app_process_command_queue();
        // }
        
        // // 系统延时
        // nos_system_sleep(10);
    }
}

#endif // MODBUS_APP_EN 