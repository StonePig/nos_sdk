// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: Smart_Gadget

#include "../ui.h"

void ui_Chat_screen_init(void)
{
    ui_Chat = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Chat, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_Chat, &ui_img_pattern_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_tiled(ui_Chat, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_container = lv_obj_create(ui_Chat);
    lv_obj_set_width(ui_Chat_container, lv_pct(100));
    lv_obj_set_height(ui_Chat_container, lv_pct(100));
    lv_obj_set_align(ui_Chat_container, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Chat_container, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chat_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_date = ui_Small_Label_create(ui_Chat_container);
    lv_obj_set_x(ui_Chat_date, 0);
    lv_obj_set_y(ui_Chat_date, 17);
    lv_obj_set_align(ui_Chat_date, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Chat_date, "Yesterday 07:25 PM");
    lv_obj_set_style_text_color(ui_Chat_date, lv_color_hex(0x000746), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chat_date, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_C1 = lv_obj_create(ui_Chat_container);
    lv_obj_set_height(ui_C1, 100);
    lv_obj_set_width(ui_C1, lv_pct(100));
    lv_obj_set_x(ui_C1, 0);
    lv_obj_set_y(ui_C1, 60);
    lv_obj_clear_flag(ui_C1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_C1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_C1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_C1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_C1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_C1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_C1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Panel1 = lv_obj_create(ui_C1);
    lv_obj_set_width(ui_Chat_Panel1, 169);
    lv_obj_set_height(ui_Chat_Panel1, 81);
    lv_obj_clear_flag(ui_Chat_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Chat_Panel1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Chat_Panel1, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chat_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Chat_Panel1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Chat_Panel1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Chat_Panel1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Chat_Panel1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat1 = ui_Small_Label_create(ui_Chat_Panel1);
    lv_obj_set_width(ui_Chat1, lv_pct(100));
    lv_obj_set_height(ui_Chat1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat1, 0);
    lv_obj_set_y(ui_Chat1, 0);
    lv_obj_set_align(ui_Chat1, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_Chat1, "Let's get some dinner, how about pizza?");
    lv_obj_set_style_text_color(ui_Chat1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chat1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Icon1 = lv_img_create(ui_C1);
    lv_img_set_src(ui_Chat_Icon1, &ui_img_chatbox_png);
    lv_obj_set_width(ui_Chat_Icon1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Chat_Icon1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat_Icon1, 0);
    lv_obj_set_y(ui_Chat_Icon1, 80);
    lv_obj_add_flag(ui_Chat_Icon1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Chat_Icon1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_Chat_Icon1, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_Chat_Icon1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_C2 = lv_obj_create(ui_Chat_container);
    lv_obj_set_height(ui_C2, 100);
    lv_obj_set_width(ui_C2, lv_pct(100));
    lv_obj_set_x(ui_C2, 0);
    lv_obj_set_y(ui_C2, 160);
    lv_obj_clear_flag(ui_C2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_C2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_C2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_C2, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_C2, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_C2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_C2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Panel2 = lv_obj_create(ui_C2);
    lv_obj_set_width(ui_Chat_Panel2, 169);
    lv_obj_set_height(ui_Chat_Panel2, 81);
    lv_obj_set_align(ui_Chat_Panel2, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_Chat_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Chat_Panel2, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Chat_Panel2, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chat_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Chat_Panel2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Chat_Panel2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Chat_Panel2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Chat_Panel2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat2 = ui_Small_Label_create(ui_Chat_Panel2);
    lv_obj_set_width(ui_Chat2, lv_pct(100));
    lv_obj_set_height(ui_Chat2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat2, 0);
    lv_obj_set_y(ui_Chat2, 0);
    lv_obj_set_align(ui_Chat2, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_Chat2, "Sounds good! What about James?");
    lv_obj_set_style_text_color(ui_Chat2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chat2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Icon2 = lv_img_create(ui_C2);
    lv_img_set_src(ui_Chat_Icon2, &ui_img_chatbox2_png);
    lv_obj_set_width(ui_Chat_Icon2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Chat_Icon2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat_Icon2, 0);
    lv_obj_set_y(ui_Chat_Icon2, 80);
    lv_obj_set_align(ui_Chat_Icon2, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_Chat_Icon2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Chat_Icon2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_Chat_Icon2, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_Chat_Icon2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Delifered = ui_Small_Label_create(ui_C2);
    lv_obj_set_x(ui_Delifered, 47);
    lv_obj_set_y(ui_Delifered, 83);
    lv_obj_set_align(ui_Delifered, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_Delifered, "Delivered");
    lv_obj_set_style_text_color(ui_Delifered, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Delifered, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_C3 = lv_obj_create(ui_Chat_container);
    lv_obj_set_height(ui_C3, 100);
    lv_obj_set_width(ui_C3, lv_pct(100));
    lv_obj_set_x(ui_C3, 0);
    lv_obj_set_y(ui_C3, 270);
    lv_obj_clear_flag(ui_C3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_C3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_C3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_C3, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_C3, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_C3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_C3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Panel3 = lv_obj_create(ui_C3);
    lv_obj_set_width(ui_Chat_Panel3, 169);
    lv_obj_set_height(ui_Chat_Panel3, 81);
    lv_obj_clear_flag(ui_Chat_Panel3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Chat_Panel3, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Chat_Panel3, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chat_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Chat_Panel3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Chat_Panel3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Chat_Panel3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Chat_Panel3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat3 = ui_Small_Label_create(ui_Chat_Panel3);
    lv_obj_set_width(ui_Chat3, lv_pct(100));
    lv_obj_set_height(ui_Chat3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat3, 0);
    lv_obj_set_y(ui_Chat3, 0);
    lv_obj_set_align(ui_Chat3, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_Chat3, "He likes it too! Where do we meet?");
    lv_obj_set_style_text_color(ui_Chat3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chat3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chat_Icon3 = lv_img_create(ui_C3);
    lv_img_set_src(ui_Chat_Icon3, &ui_img_chatbox_png);
    lv_obj_set_width(ui_Chat_Icon3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Chat_Icon3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Chat_Icon3, 0);
    lv_obj_set_y(ui_Chat_Icon3, 80);
    lv_obj_add_flag(ui_Chat_Icon3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Chat_Icon3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_Chat_Icon3, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_Chat_Icon3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Scrolldots2 = ui_Scrolldots_create(ui_Chat);
    lv_obj_set_x(ui_Scrolldots2, 0);
    lv_obj_set_y(ui_Scrolldots2, -8);

    lv_obj_set_width(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D1), 4);
    lv_obj_set_height(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D1), 4);

    lv_obj_set_x(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D2), 10);
    lv_obj_set_y(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D2), 0);

    lv_obj_set_width(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), 8);
    lv_obj_set_height(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), 8);
    lv_obj_set_x(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), 21);
    lv_obj_set_y(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), 0);
    lv_obj_set_style_bg_color(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), lv_color_hex(0x101C52),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_comp_get_child(ui_Scrolldots2, UI_COMP_SCROLLDOTS_D3), 255, LV_PART_MAIN | LV_STATE_DEFAULT);





    lv_obj_add_event_cb(ui_Chat, ui_event_Chat, LV_EVENT_ALL, NULL);

}
