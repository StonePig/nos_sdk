#ifdef __ECU_HW_TUNNING_ENABLE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../config.h"


// #include "lvgl.h"
#include "../../middle/nos_common.h"

#define CAN_HW_DEBUG_MODE_ID           0x00000111UL   

#define CHART_WIDTH (LCD_PIXEL_WIDTH * 4 / 4)
#define CHART_HEIGHT (LCD_PIXEL_HEIGHT / (6 + 1))

#define LABEL_FONT lv_font_montserrat_14

typedef enum
{
    GROUP_DO,
    GROUP_DI,
    GROUP_PWM,
    GROUP_ADC,
    GROUP_CMD
} group_name_t;

typedef enum
{
    CMD_SWITCH_2_DEBUG_MODE,
    CMD_SWITCH_2_NOMAL_MODE,
    CMD_GET_INFO_FROM_DEV,
    CMD_SET_PWM,
    CMD_SET_DO,
    CMD_MAX,
    CMD_NONE,
} cmd_type_t;

typedef enum
{
    CMD_STATUS_FAIL,
    CMD_STATUS_DONE,
    CMD_STATUS_START,
    CMD_STATUS_DOING,
    CMD_STATUS_TIMEOUT,
} cmd_status_t;

typedef struct
{
    uint16_t type;
    uint16_t di;
    uint16_t adc[25];
}CAN_DI_ADC_SEND_DATA_T;


#endif
