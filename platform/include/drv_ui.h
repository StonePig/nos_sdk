/**
 * @file drv_ui.h
 * @brief 驱动层UI模块头文件
 * @details 定义驱动层UI模块的接口函数，提供Windows平台下的用户界面控制服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_UI_H__
#define __DRV_UI_H__

#define LINE_SERIE_MAX_NUM 10

typedef void (*click_cb_t)(uint8_t group_id, uint8_t value_num, uint32_t value);

typedef void (*timer_cb_t)(void);

typedef enum
{
    CONTROL_TYPE_STATIC,
    CONTROL_TYPE_EDIT,
    CONTROL_TYPE_SLIDER,
    CONTROL_TYPE_BUTTON,
    CONTROL_TYPE_CHECKBOX,
    CONTROL_TYPE_EDIT_BOTTON,
    CONTROL_TYPE_OPEN_FILE,
    CONTROL_TYPE_COMBOBOX,
    CONTROL_TYPE_CHART,
} control_type_t;

typedef enum
{
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_ALPHA,
} value_type_t;

typedef struct
{
    control_type_t control_type; // 0: static, 1: edit, 2: slider, 3: button, 4: checkbox, 5: edit_button, 6: file_dialog
    value_type_t value_type;     // 0: number, 1: string,  for edit/edit_button
    uint32_t value;              // edit/edit_button/slider/checkbox
    uint32_t id;                // control id
    uint8_t *str_value;          // eidt/edit_button/button
    uint8_t is_visible;          // all
    uint8_t *p_file_buffer;      // file_dialog
    uint8_t *p_file_path_name;   // file_dialog
    uint32_t file_size;          // file_dialog
    uint32_t slider_min_value;   // slider
    uint32_t slider_max_value;   // slider
    uint8_t is_switch_chart_enable; // 0: disable, 1: enable
    uint8_t chart_disp_type;        // 0: text, 1: play chart, 2: stop chart
    uint8_t chart_update_flag;      // 0: no update, 1: update
    double *x_data;
    double *p_chart_data[LINE_SERIE_MAX_NUM];         // chart data
    double chart_data_max;         // chart data max
    double chart_data_min;         // chart data min
    uint32_t chart_data_size;       // chart data size
    uint8_t chart_line_serie_size; // chart line serie size
    uint8_t cur_line_serie_index; // current line serie index
    void *p_chart_line_serie[LINE_SERIE_MAX_NUM];       // chart line serie
    void *p_chart_y_axis;       // chart Y axis
    void *file_ext;
    click_cb_t cb;
    // 字体相关
    uint8_t *font_name;   // 字体名称，如"微软雅黑"
    uint8_t font_size;    // 字体大小
    uint32_t font_color;  // 字体颜色（RGB）
} control_parm_t;

#ifdef _WINDOWS
EXTERN uint32_t ui_control_get_value(uint8_t group_id, uint8_t control_id);
EXTERN uint8_t *ui_control_get_text(uint8_t group_id, uint8_t control_id);
EXTERN void ui_control_set_callback(uint8_t group_id, uint8_t control_id, click_cb_t cb);
EXTERN void ui_control_set_param(uint8_t group_id, uint8_t control_id, control_parm_t *parm);
EXTERN void ui_control_get_param(uint8_t group_id, uint8_t control_id, control_parm_t *parm);
EXTERN void ui_control_set_value(uint8_t group_id, uint8_t control_id, uint32_t value);
EXTERN void ui_control_set_value_with_series(uint8_t group_id, uint8_t control_id, uint32_t value, uint8_t index);
EXTERN void ui_control_set_visible(uint8_t group_id, uint8_t control_id, uint8_t is_visible);
EXTERN void ui_control_set_text(uint8_t group_id, uint8_t control_id, uint8_t *str_value);
EXTERN void ui_file_write(uint8_t *file_name, uint8_t *file_buf, uint32_t length, uint8_t is_append);
EXTERN uint32_t ui_file_read(uint8_t *file_name, uint8_t *file_buf);
EXTERN void ui_timer_set(uint32_t us, timer_cb_t cb);
EXTERN void ui_control_set_font(uint8_t group_id, uint8_t control_id, const char* font_name, uint8_t font_size, uint32_t font_color);
EXTERN void ui_show_reminder(const uint8_t *str, uint32_t color);

#else
#define ui_control_get_value(...)
#define ui_control_get_text(...)
#define ui_control_set_callback(...)
#define ui_control_set_param(...)
#define ui_control_get_param(...)
#define ui_control_set_value(...)
#define ui_control_set_value_with_series(...)
#define ui_control_set_visible(...)
#define ui_control_set_text(...)
#define ui_file_write(...)
#define ui_file_read(...)
#define ui_timer_set(...)
#define ui_control_set_font(...)
#define ui_show_reminder(...)

#endif

#endif /* __DRV_UI_H__ */
