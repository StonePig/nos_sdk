/**
 * @file nos_mmi_mgr.h
 * @brief 中间层MMI管理模块头文件
 * @details 定义中间层MMI管理模块的接口函数，提供人机界面状态管理和节点切换服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_MMI_MGR_H__
#define __NOS_MMI_MGR_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        uint16_t node_id; // node id
#if KEY_NUM
        key_cb_t node_key_cb;
#endif
        void (*node_enter)(void);     // 入口函数，进入node后执行一次
        void (*node_translate)(void); // 进入node后，循环执行
        void (*node_exit)(void);      // 退出node后执行一次
        uint32_t period;              // 循环周期，单位ms
    } nos_mmi_node_t;

    typedef struct
    {
        uint16_t node_id[MMI_NODE_STACK_MAX_SIZE]; // 存储栈元素的数组
        uint16_t top;                              // 栈顶元素的索引
    } node_stack_t;

#define INVALID_NODE 0xFFFF

    void nos_mmi_mgr_init(nos_mmi_node_t *nos_mmi_node, uint16_t node_size);
    /* Clear current stack and switch to new node */
    bool nos_mmi_mgr_switch_node(uint16_t node_id);
    bool nos_mmi_mgr_push_node(uint16_t node_id);
    bool nos_mmi_mgr_pop_node(void);
    void nos_mmi_mgr_start_top_node(void);
    void nos_mmi_mgr_stop_top_node(void);

    /* Pop n node */
    bool nos_mmi_mgr_pop_n_node(uint16_t cnt);
    bool nos_mmi_mgr_pop_to_root_node(void);
    bool nos_mmi_mgr_is_in_top_node(uint16_t node_id);

#ifdef __cplusplus
}
#endif

#endif /* __NOS_MMI_MGR_H__ */
