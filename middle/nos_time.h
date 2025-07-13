/**
 * @file nos_time.h
 * @brief 中间层时间模块头文件
 * @details 定义中间层时间模块的接口函数，提供时间转换、比较和系统时间管理服务
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_TIME_H__
#define __NOS_TIME_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef uint32_t nos_time_t;

    typedef struct {
        uint8_t tm_sec;     // 秒，范围从 0 到 59    
        uint8_t tm_min;     // 分，范围从 0 到 59   
        uint8_t tm_hour;    // 小时，范围从 0 到 23   \*/*
        uint8_t tm_mday;    // 一月中的第几天，范围从 1 到 31 
        uint8_t tm_mon;     // 月，范围从 0 到 11(注意) 
        uint16_t tm_year;    // 自 1900 年起的年数 
        uint16_t tm_yday;    // 一年中的第几天，范围从 0 到 365
        uint8_t tm_wday;    // 一周中的第几天，范围从 0 到 6
        uint8_t tm_isdst;   // 夏令时  
    }nos_tm_t;/* declarations */


    nos_time_t nos_mktime(nos_tm_t *tm);
    nos_tm_t *nos_gmtime(nos_tm_t *nos_tm, nos_time_t nos_time);
    int32_t nos_time_compare(nos_tm_t cur_time, nos_tm_t prev_time);
    nos_time_t nos_get_cur_sys_second(void);
    void nos_set_cur_sys_second(nos_time_t sec);
#ifdef __cplusplus
}
#endif

#endif /* __NOS_TIME_H__ */
