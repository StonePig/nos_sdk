#ifdef __ECU_HW_TUNNING_ENABLE__
#include "ecu_hw_tunning_app.h"

static uint32_t tick, tick_cmd_timeout;
static bool user_app_init_done = false;
static CAN_DI_ADC_SEND_DATA_T *recv_data;
static cmd_type_t cmd_type = CMD_GET_INFO_FROM_DEV;
static cmd_status_t cmd_status = CMD_STATUS_DONE;
static bool is_nomal_mode = true;
static bool is_data_recv_done = false;

static const_t uint8_t *p_first_line_str = \
"time,EVCRUN,IGN,HIN,ECL,ELIN,EL,BREAK,ETC2_FF0_1,ETC2_FF0_2,ETC3_FF1_1,ETC3_FF1_2,\
UR,UA,ETC,FETC,VBATP,VSENSOR,MAP,MAT,TP1,TP2,FTP1,FTP2,CTS,O2,GAS PRESS,GAS TEMP,OIL PRESS,TURBO_PRESS,TURB0 PRESS,BINDV,OIL TEMP,TURB TEMP1,TURB TEMP2,AIN BAK1,AIN BAK2\r\n";

static uint8_t record_file_name[50];
static nos_tm_t info;

static const_t uint8_t *remider_msg[] = {
    "切换失败！",
    "切换已完成！",
    "正在切换...",
    "正在切换...",
    "切换超时！",
};

static void show_cmd_reminder(cmd_status_t cmd_status)
{
    uint32_t color;
    if (cmd_status == CMD_STATUS_FAIL || cmd_status == CMD_STATUS_TIMEOUT)
        color = 0x0000FF;
    else
        color = 0xFF0000;

    ui_show_reminder(remider_msg[cmd_status], color);
}


static void set_control_visable(uint8_t visible)
{
    uint8_t i;

    for (i = 0; i < 14; i++)
    {
        ui_control_set_visible(GROUP_DO, i, visible);
        ui_control_set_value(GROUP_DO, i, 0);
    }
    for (i = 0; i < 9; i++)
    {
        ui_control_set_visible(GROUP_PWM, i, visible);
        ui_control_set_value(GROUP_PWM, i, 0);
    }
}

void can_recv_cb(uint8_t can_num, uint32_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t i;

    PRINT_LOG("can num = %d, addr = 0x%x\r\n", can_num, addr);
    PRINT_LOG("len = %ld\r\n", len);
    if (addr != CAN_HW_DEBUG_MODE_ID)
        return;

    if (memcmp(buf, "KO", 2) == 0 && len == 2)
    {
        cmd_status = CMD_STATUS_DONE;
        show_cmd_reminder(cmd_status);
        if (cmd_type == CMD_SWITCH_2_DEBUG_MODE)
        {
            is_nomal_mode = false;
            ui_control_set_text(GROUP_CMD, 0, "切换到正常模式");
            set_control_visable(1);
        }
        else if (cmd_type == CMD_SWITCH_2_NOMAL_MODE)
        {
            is_nomal_mode = true;
            ui_control_set_text(GROUP_CMD, 0, "切换到调试模式");
            set_control_visable(0);

            for (i = 0; i < 11; i++)
            {
                ui_control_set_value(GROUP_DI, i, 0);
            }
            for (i = 0; i < 25; i++)
            {
                ui_control_set_value(GROUP_ADC, i, 0);
            }
        }
        is_data_recv_done = true;
        return;
    }

    recv_data = (CAN_DI_ADC_SEND_DATA_T *)buf;
    if (recv_data->type == 0x3412 && len == sizeof(CAN_DI_ADC_SEND_DATA_T))
    {
        //cmd_status = CMD_STATUS_DONE;
        for (i = 0; i < 11; i++)
        {
            uint16_t di;
            di = (recv_data->di << 8) & 0xFF00;
            di += (recv_data->di >> 8) & 0x00FF;
            ui_control_set_value(GROUP_DI, i, (di >> i) & 0x01);
        }
        for (i = 0; i < 25; i++)
        {
            uint16_t adc;
            adc = (recv_data->adc[i] << 8) & 0xFF00;
            adc += (recv_data->adc[i] >> 8) & 0x00FF;
            ui_control_set_value(GROUP_ADC, i, adc);
        }
    }
}
static uint8_t sendbuf[8];

void control_event_handler(uint8_t group_id, uint8_t value_num, uint32_t value)
{
    if (group_id == GROUP_CMD)
    {
        cmd_status = CMD_STATUS_START;
        show_cmd_reminder(cmd_status);
        nos_set_current_tick(&tick);
        if (is_nomal_mode)
        {
            cmd_type = CMD_SWITCH_2_DEBUG_MODE;
        }
        else
        {
            cmd_type = CMD_SWITCH_2_NOMAL_MODE;
        }
    }

    if (cmd_status != CMD_STATUS_DONE)
        return;

    if (group_id == GROUP_DO)
    {
        nos_set_current_tick(&tick);
        cmd_type = CMD_SET_DO;
        cmd_status = CMD_STATUS_START;
        sendbuf[0] = '1';
        sendbuf[1] = '1';
        sendbuf[2] = value_num;
        sendbuf[3] = value & 0xFF;
    }
    if (group_id == GROUP_PWM)
    {
        nos_set_current_tick(&tick);
        cmd_type = CMD_SET_PWM;
        cmd_status = CMD_STATUS_START;
        sendbuf[0] = '2';
        sendbuf[1] = '2';
        sendbuf[2] = value_num;
        value = (value * 255  + 50)/ 100;
        sendbuf[3] = value & 0xFF;
        PRINT_LOG("value = %d\r\n", value);
    }
}

void user_app_task(void)
{
    static uint32_t count = 0;

    if (user_app_init_done == false)
    {
        PRINT_LOG("user_app_task init\r\n");
        win32_show_title("ECU HW TUNNING V1.0.0");

        nos_can_recv_regist_cb(0, can_recv_cb);

        ui_control_set_callback(0xFF, 0, control_event_handler);

        set_control_visable(0);

        nos_gmtime(&info, nos_get_cur_sys_second());
        sprintf(record_file_name, "record_data_%04d_%02d_%02d.csv", info.tm_year + 1900, info.tm_mon + 1, info.tm_mday);
        if (ui_file_read(record_file_name, NULL) == 0)
        {
            ui_file_write(record_file_name, p_first_line_str, (uint32_t)strlen(p_first_line_str), 0);
        }

        user_app_init_done = true;
    }

    if (nos_get_lapse_tick(tick) >= 200)
    {
        nos_set_current_tick(&tick);
        // ui_control_set_value(GROUP_ADC, 1, tick);
        if (cmd_status == CMD_STATUS_START)
        {
            nos_set_current_tick(&tick_cmd_timeout);
            cmd_status = CMD_STATUS_DOING;
            if (cmd_type == CMD_SWITCH_2_DEBUG_MODE)
            {
                show_cmd_reminder(cmd_status);
                nos_can_send_data(0, CAN_HW_DEBUG_MODE_ID, "66573861", 8);
            }
            else if (cmd_type == CMD_SWITCH_2_NOMAL_MODE)
            {
                show_cmd_reminder(cmd_status);
                nos_can_send_data(0, CAN_HW_DEBUG_MODE_ID, "66573862", 8);
            }
            else if (cmd_type == CMD_SET_DO || cmd_type == CMD_SET_PWM)
            {
                nos_can_send_data(0, CAN_HW_DEBUG_MODE_ID, sendbuf, 4);
                cmd_status = CMD_STATUS_DONE;
            }
        }
        else if (cmd_status == CMD_STATUS_DOING)
        {
            // 命令超时
            if (nos_get_lapse_tick(tick_cmd_timeout) > 3000)
            {
                cmd_status = CMD_STATUS_TIMEOUT;
                if(cmd_type != CMD_GET_INFO_FROM_DEV)
                {
                    show_cmd_reminder(cmd_status);
                }
            }
        }
        else if (cmd_status == CMD_STATUS_DONE)
        {
            // nos_set_current_tick(&tick_cmd_timeout);
            // cmd_status = CMD_STATUS_DONE;
            nos_can_send_data(0, CAN_HW_DEBUG_MODE_ID, "66573863", 8);
        }
        if(is_data_recv_done)
        {
            uint8_t file_buf[500];
            uint32_t cur_tick;
            nos_gmtime(&info, (nos_time_t)nos_get_cur_sys_second());
            nos_set_current_tick(&cur_tick);

            sprintf(file_buf, "%02d:%02d:%02d:%03d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", info.tm_hour, info.tm_min, info.tm_sec, cur_tick % 1000, \
                ui_control_get_value(GROUP_DI, 0),
                ui_control_get_value(GROUP_DI, 1),
                ui_control_get_value(GROUP_DI, 2),
                ui_control_get_value(GROUP_DI, 3),
                ui_control_get_value(GROUP_DI, 4),
                ui_control_get_value(GROUP_DI, 5),
                ui_control_get_value(GROUP_DI, 6),
                ui_control_get_value(GROUP_DI, 7),
                ui_control_get_value(GROUP_DI, 8),
                ui_control_get_value(GROUP_DI, 9),
                ui_control_get_value(GROUP_DI, 10),
                ui_control_get_value(GROUP_ADC, 0),
                ui_control_get_value(GROUP_ADC, 1),
                ui_control_get_value(GROUP_ADC, 2),
                ui_control_get_value(GROUP_ADC, 3),
                ui_control_get_value(GROUP_ADC, 4),
                ui_control_get_value(GROUP_ADC, 5),
                ui_control_get_value(GROUP_ADC, 6),
                ui_control_get_value(GROUP_ADC, 7),
                ui_control_get_value(GROUP_ADC, 8),
                ui_control_get_value(GROUP_ADC, 9),
                ui_control_get_value(GROUP_ADC, 10),
                ui_control_get_value(GROUP_ADC, 11),
                ui_control_get_value(GROUP_ADC, 12),
                ui_control_get_value(GROUP_ADC, 13),
                ui_control_get_value(GROUP_ADC, 14),
                ui_control_get_value(GROUP_ADC, 15),
                ui_control_get_value(GROUP_ADC, 16),
                ui_control_get_value(GROUP_ADC, 17),
                ui_control_get_value(GROUP_ADC, 18),
                ui_control_get_value(GROUP_ADC, 19),
                ui_control_get_value(GROUP_ADC, 20),
                ui_control_get_value(GROUP_ADC, 21),
                ui_control_get_value(GROUP_ADC, 22),
                ui_control_get_value(GROUP_ADC, 23),
                ui_control_get_value(GROUP_ADC, 24)
            );
            ui_file_write(record_file_name, file_buf, (uint32_t)strlen(file_buf), 1);
            is_data_recv_done = false;
        }
    }
}
#endif
