#define LOG_TAG "user_app_task"
#define LOG_LVL ELOG_LVL_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../config.h"
#if EASYLOGGER_EN
#include "../../middle/external/easylogger/elog.h"
#endif
#include "../../middle/external/algorithm/pid.h"
#include "../../middle/nos_common.h"

#ifdef __LED_DEMO_ENABLE__

static uint32_t tick;
static bool user_app_init_done = false;
static bool is_in_sleep_mode = false;
static bool is_in_alarm_reminder_mode = false;
static nos_time_t alarm_time = 0;

static uint16_t counter;
static uint16_t sleep_counter;

static pid_info_t pid;

extern void lvgl_app_init(void);

typedef enum
{
    ROOT_NODE_ID,
    IDLE_NODE_ID,
    MAIN_MENU_NODE_ID,
    SET_ALARM_NODE_ID,
    ALARM_REMINDER_NODE_ID,
} node_id_t;

enum
{
    KEY_MENU,
    KEY_BACK,
    KEY_OK,
    KEY_UP,
    KEY_DOWN,
};

void root_key_process(key_value_t key_num, key_event_id_t event_id)
{
    PRINT_LOG("1key num = %d, event id = %d", (uint16_t)key_num, (uint16_t)event_id);
    switch (key_num)
    {
    case KEY_MENU:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_led_disp_number(key_num, 1, 0, true);
        break;
    case KEY_BACK:
        if (event_id == EVENT_ID_KEY_LONG_PRESSED)
            nos_mmi_mgr_switch_node(IDLE_NODE_ID);
        break;
    case KEY_OK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_push_node(IDLE_NODE_ID);
        break;
    }
}

static void idle_key_process(key_value_t key_num, key_event_id_t event_id)
{
    static uint32_t counter;
    PRINT_LOG("2key num = %d, event id = %d", (uint16_t)key_num, (uint16_t)event_id);
    switch (key_num)
    {
    case KEY_OK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_push_node(MAIN_MENU_NODE_ID);
        if(event_id == EVENT_ID_KEY_LONG_PRESSED)
            nos_mmi_mgr_pop_node();             
        break;
    case KEY_MENU:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_led_disp_number(key_num, 1, 0, true);
        if(event_id == EVENT_ID_KEY_LONG_PRESSED)
            nos_mmi_mgr_pop_node();
        break;
    case KEY_BACK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_pop_node();
        break;
    case KEY_UP:
        if (event_id == EVENT_ID_KEY_CLICK || event_id == EVENT_ID_KEY_LONG_PRESSED || event_id == EVENT_ID_KEY_LONG_PRESSED_REPEAT)
            nos_led_disp_number(++counter, 100, 0, true);
        break;
    case KEY_DOWN:
        if (event_id == EVENT_ID_KEY_CLICK || event_id == EVENT_ID_KEY_LONG_PRESSED || event_id == EVENT_ID_KEY_LONG_PRESSED_REPEAT)
            nos_led_disp_number(--counter, 100, 0, true);
        break;
    }
}
static void main_menu_key_process(key_value_t key_num, key_event_id_t event_id)
{
    PRINT_LOG("3key num = %d, event id = %d", (uint16_t)key_num, (uint16_t)event_id);
    switch (key_num)
    {
    case KEY_BACK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_pop_node();
        break;
    case KEY_OK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_push_node(SET_ALARM_NODE_ID);
        if(event_id == EVENT_ID_KEY_LONG_PRESSED)
            nos_mmi_mgr_pop_node();            
        break;
    case KEY_MENU:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_pop_to_root_node();
        break;
    }
}

static void set_alarm_key_process(key_value_t key_num, key_event_id_t event_id)
{
    static uint16_t counter;
    PRINT_LOG("4key num = %d, event id = %d", (uint16_t)key_num, (uint16_t)event_id);
    switch (key_num)
    {
    case KEY_BACK:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_pop_node();
        break;
    case KEY_OK:
        if (event_id == EVENT_ID_KEY_CLICK)
        {
            alarm_time = nos_get_cur_sys_second() + counter * 60;
        }
        if(event_id == EVENT_ID_KEY_LONG_PRESSED)
            nos_mmi_mgr_pop_node();        
        break;
    case KEY_MENU:
        if (event_id == EVENT_ID_KEY_CLICK)
            nos_mmi_mgr_pop_to_root_node();
        break;
    case KEY_UP:
        if (event_id == EVENT_ID_KEY_CLICK || event_id == EVENT_ID_KEY_LONG_PRESSED || event_id == EVENT_ID_KEY_LONG_PRESSED_REPEAT)
            nos_led_disp_number(++counter, 100, 0, true);
        break;
    case KEY_DOWN:
        if (event_id == EVENT_ID_KEY_CLICK || event_id == EVENT_ID_KEY_LONG_PRESSED || event_id == EVENT_ID_KEY_LONG_PRESSED_REPEAT)
            nos_led_disp_number(--counter, 100, 0, true);
        break;
    }
}

static void alarm_reminder_key_process(key_value_t key_num, key_event_id_t event_id)
{
    uint8_t i;

    PRINT_LOG("5key num = %d, event id = %d", (uint16_t)key_num, (uint16_t)event_id);
    if (event_id == EVENT_ID_KEY_CLICK)
    {
        if (is_in_alarm_reminder_mode == true)
        {
            is_in_alarm_reminder_mode = false;
            nos_led_disp_buf_restore();
            nos_mmi_mgr_start_top_node();
        }
        if (is_in_sleep_mode == true)
        {
            sleep_counter = 0;
            is_in_sleep_mode = false;
            nos_led_disp_buf_restore();
            nos_mmi_mgr_start_top_node();
            for (i = 0; i < KEY_NUM; i++)
                nos_led_on(i);
        }
    }
}
static void root_enter(void)
{
    PRINT_LOG("root_enter");
    // nos_led_disp_set_wink(0x1, 3, 3, 40, WINK_MODE_WINK_TO_ON);
    nos_led_wink(7, 3, 7, 20, WINK_MODE_WINK_FOREVER);
    // nos_led_wink(2, 3, 3, 50, WINK_MODE_WINK_TO_OFF);
}
static void root_translate(void)
{
    nos_time_t raw_time;
    nos_tm_t info;
    uint8_t disp_str[10];

    PRINT_LOG("root_translate");

    raw_time = nos_get_cur_sys_second();
    nos_gmtime(&info, raw_time);

    sprintf(disp_str, "%06d", info.tm_hour * 10000 + info.tm_min * 100 + info.tm_sec);

    nos_led_disp_str(disp_str, 0, true);
}
static void root_exit(void)
{
    PRINT_LOG("%s", "root_exit");
}

static void idle_enter(void)
{
    PRINT_LOG("idle_enter");
    nos_led_disp_number(IDLE_NODE_ID, 1, 0, true);
    nos_key_set_parm(KEY_UP, 0, 1000, 100, 0);
    nos_key_set_parm(KEY_DOWN, 0, 1000, 100, 0);
}
static void idle_translate(void)
{
    static uint16_t pos;
    static const_t uint8_t disp_[] = {
        0, 0x01, 
        1, 0x01,
        2, 0x01,
        3, 0x01,
        3, 0x03,
        3, 0x07,
        3, 0x0F,
        2, 0x09,
        1, 0x09,
        0, 0x09,
        0, 0x19,
        0, 0x39,
        0, 0x38,
        1, 0x08,
        2, 0x08,
        3, 0x0E,
        3, 0x0C,
        3, 0x08,
        3, 0x00,
        2, 0x00,
        1, 0x00,
        0, 0x30,
        0, 0x20,
        0, 0x00,
        };
    nos_led_disp_buf(disp_[(pos % 24 * 2 + 1)], disp_[(pos % 24 * 2 )]);
    pos++;
}
static void idle_exit(void)
{
    PRINT_LOG("idle_exit");
}

static void main_menu_enter(void)
{
    PRINT_LOG("main_menu_enter");
    // nos_led_off_all();
    nos_led_disp_number(MAIN_MENU_NODE_ID, 1, 0, true);
}
static void main_menu_translate(void)
{
    static uint16_t pos;
    static const_t uint8_t *disp_str = "hello world!abcdefghigklmnopqrstuvwxyz";
    PRINT_LOG("main_menu_translate");

    if(pos > strlen((const_t char *)disp_str) - 1)
        pos = 0;
    nos_led_disp_str((uint8_t *)(disp_str + pos), 0, true);
    pos++;
}
static void main_menu_exit(void)
{
    PRINT_LOG("main_menu_exit");
}

static void set_alarm_enter(void)
{
    PRINT_LOG("%s", "set_alarm_enter");
    nos_led_disp_number(SET_ALARM_NODE_ID, 1, 0, true);
}
static void set_alarm_translate(void)
{
    PRINT_LOG("%s", "set_alarm_translate");
}
static void set_alarm_exit(void)
{
    PRINT_LOG("%s", "set_alarm_exit");
}

static void alarm_reminder_enter(void)
{
    PRINT_LOG("%s", "alarm_reminder_enter");
    nos_led_disp_set_wink(0xFF, 3, 3, 100, WINK_MODE_WINK_TO_OFF);
    nos_led_disp_str((uint8_t *)"alarm", 0, true);
}
static void alarm_reminder_translate(void)
{
    PRINT_LOG("%s", "alarm_reminder_translate");
    nos_mmi_mgr_pop_node();
}
static void alarm_reminder_exit(void)
{
    PRINT_LOG("%s", "alarm_reminder_exit");
}

const_t nos_mmi_node_t node_root = {
    ROOT_NODE_ID,
#if KEY_NUM
    root_key_process,
#endif
    root_enter,
    root_translate,
    root_exit,
    300,
};
const_t nos_mmi_node_t node_idle = {
    IDLE_NODE_ID,
#if KEY_NUM
    idle_key_process,
#endif
    idle_enter,
    idle_translate,
    idle_exit,
    100,
};

const_t nos_mmi_node_t node_main_menu = {
    MAIN_MENU_NODE_ID,
#if KEY_NUM
    main_menu_key_process,
#endif
    main_menu_enter,
    main_menu_translate,
    main_menu_exit,
    500,
};

const_t nos_mmi_node_t node_set_alarm = {
    SET_ALARM_NODE_ID,
#if KEY_NUM
    set_alarm_key_process,
#endif
    set_alarm_enter,
    set_alarm_translate,
    set_alarm_exit,
    2000,
};

const_t nos_mmi_node_t node_alarm_reminder = {
    ALARM_REMINDER_NODE_ID,
#if KEY_NUM
    alarm_reminder_key_process,
#endif
    alarm_reminder_enter,
    alarm_reminder_translate,
    alarm_reminder_exit,
    20000,
};

static const_t nos_mmi_node_t *mmi_node[] = {
    &node_root,
    &node_idle,
    &node_main_menu,
    &node_set_alarm,
    &node_alarm_reminder,
};

void user_app_task(void)
{
    uint8_t i;

    if (user_app_init_done == false)
    {
        PRINT_LOG("user_app_task init\r\n");
        user_app_init_done = true;


        // nos_tm_t info;
        // info.tm_year = 124;
        // info.tm_mon = 8;
        // info.tm_mday = 13;
        // info.tm_hour = 9;
        // info.tm_min = 0;
        // info.tm_sec = 0;
        // nos_set_cur_sys_second(nos_mktime(&info));


        /* initialize EasyLogger */
#if EASYLOGGER_EN
        elog_init();
        /* set EasyLogger log format */
        elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
        elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
        elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
        /* start EasyLogger */
        elog_start();
#endif
        // pid_init(&pid);
        // pid.kp = 1000;
        // pid.ki = 30;
        // pid.kd = 0;
        // pid.max_output = 100000;
        // pid.min_output = -100000;
        // pid.max_integral = 100000;
        // pid.point = 80;

        for (i = 0; i < KEY_NUM; i++)
            nos_led_wink(i, 100, 0, 3, WINK_MODE_WINK_TO_OFF);

        nos_mmi_mgr_init((nos_mmi_node_t *)mmi_node, sizeof(mmi_node) / sizeof(nos_mmi_node_t *));
        nos_mmi_mgr_switch_node(ROOT_NODE_ID);
        win32_show_title("LED DEMO V1.0.0");
    }
    // process common event: alarm, sleep, keypad led...
    if (nos_tick_timeout(&tick, 100))
    {
        // pid.process = nos_adc_get_value(7) * 4095 / 42000;

        // elog_a("elog", "(%s) has assert failed at %s:%ld.", "test", __FUNCTION__, __LINE__);
        //log_w("pid.process = %ld", pid.process);
        // nos_assert(pid.process >= 0 && pid.process <= 80);
        // ELOG_ASSERT(pid.process < 80);
        // PRINT_LOG("pid.process = %f", pid.process);
        // PRINT_LOG("nos_adc_get_value(7) = %d", nos_adc_get_value(7));
        // pid_compute(&pid);
        // nos_pwm_set_value(0, (uint16_t)(500 + pid.output / 200));

        if (nos_key_have_key_pressed() == true)
        {
            sleep_counter = 0;
        }
        else if (sleep_counter++ > 300 && is_in_sleep_mode == false)
        {
            is_in_sleep_mode = true;
            nos_mmi_mgr_stop_top_node();
            nos_led_disp_buf_backup();
            nos_led_disp_str((uint8_t *)"sleep", 0, true);
            for (i = 0; i < KEY_NUM; i++)
                nos_led_off(i);

#if KEY_NUM
            // nos_key_init();
            nos_key_regist_cb(alarm_reminder_key_process);
#endif
        }

        if (is_in_alarm_reminder_mode == false && alarm_time != 0 && alarm_time < nos_get_cur_sys_second())
        {
            is_in_alarm_reminder_mode = true;
            nos_mmi_mgr_stop_top_node();
            nos_led_disp_buf_backup();
            counter = 0;
            alarm_time = 0;
#if KEY_NUM
            // nos_key_init();
            nos_key_regist_cb(alarm_reminder_key_process);
#endif
            nos_led_disp_set_wink(0xFF, 3, 3, 100, WINK_MODE_WINK_TO_OFF);
            nos_led_disp_str((uint8_t *)"alarm", 0, true);
        }
        if (is_in_alarm_reminder_mode == true)
        {
            sleep_counter = 0;
            is_in_sleep_mode = false;
            if (counter++ > 200)
            {
                is_in_alarm_reminder_mode = false;
                nos_led_disp_buf_restore();
                nos_mmi_mgr_start_top_node();
            }
        }
    }
}
#endif