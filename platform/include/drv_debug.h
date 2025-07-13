/**
 * @file drv_debug.h
 * @brief 调试驱动模块头文件
 * @details 定义调试驱动模块的接口函数，提供日志输出和断言功能
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __DRV_DEBUG_H__
#define __DRV_DEBUG_H__

EXTERN void output_log(const char *fmt, ...);
EXTERN void output_log_raw(uint8_t *log, uint16_t size);

#if ASSERT_EN
#ifdef _WINDOWS
#include <assert.h>
#define drv_assert(EXPR)                                                                \
    if (!(EXPR))                                                                        \
    {                                                                                   \
        output_log("(%s) has assert failed at %s:%ld.", #EXPR, __FILE__, __LINE__); \
        assert(EXPR);                                                                   \
    }
#else
#define drv_assert(EXPR)                                                                \
    if (!(EXPR))                                                                        \
    {                                                                                   \
        output_log("(%s) has assert failed at %s:%ld.", #EXPR, __FILE__, __LINE__); \
        while (1)                                                                       \
            ;                                                                           \
    }
#endif
#else
#define drv_assert(EXPR)
#endif


#if defined (__STDC_VERSION__) || defined(_WINDOWS)
#if PRINT_LOG_EN
#define PRINT_LOG(fmt, ...) output_log("\r\n[%d] " fmt, drv_get_cur_sys_tick(), ##__VA_ARGS__)
#else
#define PRINT_LOG(fmt, ...)
#endif
#else
#if PRINT_LOG_EN
#define PRINT_LOG output_log
#else
#define PRINT_LOG
#endif

#endif

#endif /* __DRV_DEBUG_H__ */
