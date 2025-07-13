/**
 * @file nos_msg.h
 * @brief 中间层消息模块头文件
 * @details 定义中间层消息模块的接口函数，提供消息队列管理服务，支持多源消息处理
 * @author zyz
 * @date 2024-05-26
 * @version 1.0
 */

#ifndef __NOS_MSG_H__
#define __NOS_MSG_H__

#define MSG_FIFO_SIZE 10 /* 消息个数 */

enum MSG_SOURCE
{
	MSG_FROM_KEY = 0,
};

/* 按键FIFO用到变量 */
typedef struct
{
	uint8_t MsgSource; /*消息来源*/
	uint16_t MsgCode;  /* 消息代码 */
	uint32_t MsgParam; /* 消息的数据体, 也可以是指针（强制转化） */
} MSG_T;

/* 按键FIFO用到变量 */
typedef struct
{
	MSG_T Buf[MSG_FIFO_SIZE]; /* 消息缓冲区 */
	uint8_t Read;			  /* 缓冲区读指针1 */
	uint8_t Write;			  /* 缓冲区写指针 */
	uint8_t Read2;			  /* 缓冲区读指针2 */
} MSG_FIFO_T;

/* 供外部调用的函数声明 */
void nos_init_msg(void);
void nos_put_msg(uint8_t _MsgSource, uint16_t _MsgCode, uint32_t _MsgParam);
uint8_t nos_get_msg(MSG_T *_pMsg);
uint8_t nos_get_msg2(MSG_T *_pMsg);
void nos_clear_msg(void);

#endif /* __NOS_MSG_H__ */
