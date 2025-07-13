#ifdef __IEM_TUNNING_ENABLE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../config.h"

#include "../../middle/nos_common.h"

typedef enum
{
    MSG_FROM_HOST_QUERY_VERSION = 0x81, // 获取后视镜软件版本
    MSG_FROM_HOST_QUERY_UID = 0x82,     // 获取后视镜唯一ID
    MSG_FROM_HOST_ONOFF = 0x84,         // 后视镜开关设定
    MSG_FROM_HOST_SET_BACKLIGHT = 0x85, // 亮度设定指令
    MSG_FROM_HOST_SET_VIEW = 0x86,      // 亮度设定指令
    MSG_FROM_HOST_SET_SCALE = 0x87,     // 倍数设定指令
    MSG_FROM_HOST_SET_DISP_MODE = 0x88, // 显示模式
    MSG_FROM_HOST_QUERY_PARM = 0x8A,    // 参数查询指令
    MSG_TO_HOST_SET_PARM = 0x8B,        // 参数更新指令
    MSG_TO_HOST_HEART_BEAT = 0x8C,      // 心跳指令
} send_msg_from_host_t;

typedef enum
{
    RESPONSE_CORRECT = 0,
    RESPONSE_ERROR = 255
} response_data_t;

static uint16_t counter;
static uint32_t tick, tick_cmd_timeout;
static bool user_app_init_done = false;
static uint8_t checksum;
static uint8_t send_buf[256];
static uint16_t send_buf_len = 0;
static uint8_t iem_status[10];
static uint8_t app_status[10];

static void PutByte2SendBuf(uint8_t byte)
{
    checksum ^= byte;
    send_buf[send_buf_len++] = byte;
}

void SendMsg2Host(uint8_t uart_num, uint8_t msg_type, uint8_t *buf, uint16_t len)
{
    uint8_t i;

    // add msg to send buf
    checksum = 0;
    send_buf_len = 0;

    // 开始符
    PutByte2SendBuf(0xA5);

    // 长度
    PutByte2SendBuf((len + 1) >> 8);
    PutByte2SendBuf((len + 1) & 0xFF);

    // 内容
    PutByte2SendBuf(msg_type);
    for (i = 0; i < len; i++)
        PutByte2SendBuf(buf[i]);

    // 是否需要应答
    // PutByte2SendBuf(0);

    // checksum
    PutByte2SendBuf(checksum);

    // 结束符
    PutByte2SendBuf(0x5A);

    nos_uart_send_data(uart_num, send_buf, send_buf_len);
}

void ResMsg2Host(uint8_t uart_num, uint8_t msg_type, uint8_t ack)
{
    SendMsg2Host(uart_num, msg_type, &ack, 1);
}

static void uart_recv_process(uint8_t uart_num, uint8_t *recv_buf, uint16_t recv_len)
{
    uint16_t i, len;
    uint8_t msg_type;

    if (uart_num != 0)
        return;

    nos_set_current_tick(&tick);

    if (recv_len > 0)
    {
        len = recv_buf[1] * 256 + recv_buf[2];
        msg_type = recv_buf[3];
        // ack_flag = recv_buf[recv_len - 3];

        if (recv_buf[0] != 0xA5 || recv_buf[recv_len - 1] != 0x5A)
        {
            return;
        }

        // checksum
        checksum = 0;
        for (i = 0; i < recv_len - 2; i++)
        {
            checksum ^= recv_buf[i];
        }
        if (checksum != recv_buf[recv_len - 2])
        {
            ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            return;
        }

        switch (msg_type)
        {
        case MSG_FROM_HOST_QUERY_VERSION:
            SendMsg2Host(0, msg_type, "123456789", 9); // 发送版本号
            break;
        case MSG_FROM_HOST_QUERY_UID:
            {
                uint8_t uid[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
                SendMsg2Host(0, msg_type, uid, sizeof(uid)); // 发送唯一ID
            }
            break;
        case MSG_FROM_HOST_ONOFF:
            if (recv_buf[4] == 0x01) // 开启
            {
                // 执行开启操作
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[0] = 1;
                ui_control_set_value(0, 0, 1);
            }
            else if (recv_buf[4] == 0x00) // 关闭
            {
                // 执行关闭操作
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[0] = 0;
                ui_control_set_value(0, 0, 0);
            }
            else
            {
                ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            }
            break;
        case MSG_FROM_HOST_SET_BACKLIGHT:
            if (recv_buf[4] <= 14) // 
            {
                // 设置亮度
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[1] = recv_buf[4];
                ui_control_set_value(0, 1, recv_buf[4]);
            }
            else
            {
                ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            }
            break;
        case MSG_FROM_HOST_SET_VIEW:
            if (recv_buf[4] <= 6) // 
            {
                // 设置视图模式
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[2] = recv_buf[4];
                ui_control_set_value(0, 2, recv_buf[4]);
            }
            else
            {
                ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            }
            break;
        case MSG_FROM_HOST_SET_SCALE:
            if (recv_buf[4] <= 2) 
            {
                // 设置缩放
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[3] = recv_buf[4];
                ui_control_set_value(0, 3, recv_buf[4]);
            }
            else
            {
                ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            }
            break;
        case MSG_FROM_HOST_SET_DISP_MODE:
            if (recv_buf[4] <= 1) 
            {
                // 设置显示模式
                SendMsg2Host(0, msg_type, &recv_buf[4], 2);
                iem_status[4] = recv_buf[4];
                ui_control_set_value(0, 4, recv_buf[4]);
                
            }
            else
            {
                ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            }
            break;
		case MSG_FROM_HOST_QUERY_PARM:
            SendMsg2Host(0, msg_type, iem_status, 6);
			break;
        }
    }
}
static void uart_recv_process_app(uint8_t uart_num, uint8_t *recv_buf, uint16_t recv_len)
{
    uint16_t i, len;
    uint8_t msg_type;

    // 处理来自应用的UART数据
    if (uart_num != 1)
        return;

    // 这里可以添加处理应用数据的逻辑


    if (recv_len > 0)
    {
        len = recv_buf[1] * 256 + recv_buf[2];
        msg_type = recv_buf[3];
        // ack_flag = recv_buf[recv_len - 3];

        if (recv_buf[0] != 0xA5 || recv_buf[recv_len - 1] != 0x5A)
        {
            return;
        }

        // checksum
        checksum = 0;
        for (i = 0; i < recv_len - 2; i++)
        {
            checksum ^= recv_buf[i];
        }
        if (checksum != recv_buf[recv_len - 2])
        {
            ResMsg2Host(0, msg_type, RESPONSE_ERROR);
            return;
        }

        switch (msg_type)
        {
            case MSG_TO_HOST_SET_PARM:
            case MSG_FROM_HOST_QUERY_PARM:
                if(len == 7)
                {
                    memcpy(iem_status, &recv_buf[4], 5);
                    for (i = 0; i < 5; i++)
                    {
                        ui_control_set_value(1, i, iem_status[i]);
                    }
                    SendMsg2Host(0, msg_type, NULL, 0); // 应答
                }
                break;
        }
    }
}



static void control_event_handler(uint8_t group_id, uint8_t value_num, uint32_t value)
{
    if (group_id == 0)
    {
        if (value_num == 0)
        {
            iem_status[0] = (value & 0x01) ? 1 : 0; // 开关状态
            SendMsg2Host(0, 0x8B, iem_status, 6);
            nos_set_current_tick(&tick);
        }
    }
    // app simu
    if (group_id == 1 && value_num < 5)
    {
        uint8_t temp[2];
        if(app_status[value_num] == value)
        {
            return; // 如果状态没有变化，则不发送
        }
        app_status[value_num] = value;
        temp[0] = value & 0xFF; // 确保value_num在0-255范围内
        temp[1] = 0x00; // 预留字节
        SendMsg2Host(1, MSG_FROM_HOST_ONOFF + value_num, &temp[0], 2);
    }
}

void user_app_task(void)
{

    if (user_app_init_done == false)
    {
        PRINT_LOG("user_app_task init\r\n");

        win32_show_title("IEM SIMU TOOL V1.0.0");

        user_app_init_done = true;

        nos_uart_recv_regist_cb(0, uart_recv_process);
        nos_uart_recv_regist_cb(1, uart_recv_process_app);

        // ui_control_set_text(0, 0, "打开");
        ui_control_set_value(0, 0, 1);
        ui_control_set_value(0, 1, 1);
        ui_control_set_value(0, 2, 1);
        ui_control_set_value(0, 3, 1);
        ui_control_set_value(0, 4, 1);

        ui_control_set_callback(0xFF, 0, control_event_handler);
        ui_control_set_callback(0xFF, 1, control_event_handler);
    }

    if (nos_tick_timeout(&tick, 5000))
    {
        static uint8_t is_uart_opened = 1;

        if (win32_is_uart_opend(0) == is_uart_opened)
        {
            SendMsg2Host(0, MSG_TO_HOST_HEART_BEAT, NULL, 0); // UART状态变化通知
        }
    }
}
#endif
