#ifndef ACOM_H
#define ACOM_H

#include "winnt.h"
#include <process.h>
#include "string.h"
//#include "nodelist.h"

/***************
串口功能函数:
打开opencomm
读取ReadData
写入WriteData
关闭CloseComm
***************/

HANDLE OpenComm(CString port, DWORD baud);

int ReadData(HANDLE hCom,unsigned char* pBuffer, unsigned short ulen);

int WriteData(HANDLE hCom, unsigned char *pBuffer, unsigned short uLen);	

void CloseComm(HANDLE hCom);

#endif