#if __MODBUS_DEMO_ENABLE__
#include "../../middle/external/modbus/mbrtuslave.h"
#include "modbus_slave_app.h"
#include "../../middle/nos_common.h"


static uint8_t respond_buf[265];

static void uart_recv_process(uint8_t uart_num, uint8_t xdata_t *buf, uint16_t len)
{
  uint16_t respond_len;

   if(uart_num != MODBUS_SLAVE_UART_NUM)
      return;

  respond_len = ParsingMasterAccessCommand(buf, respond_buf, len, STATION_ADDR);
  if (respond_len)
    nos_uart_send_data(MODBUS_SLAVE_UART_NUM, respond_buf, respond_len);
}

void modbus_slave_app_task_init(void)
{
  static bool is_init = false;
  if (is_init == false)
  {
    is_init = true;
    nos_uart_recv_regist_cb(MODBUS_SLAVE_UART_NUM, uart_recv_process);
  }
}

void modbus_slave_app_task(void)
{
  modbus_slave_app_task_init();
}

/*获取想要读取的Coil量的值*/
void GetCoilStatus(uint16_t startAddress, uint16_t quantity, bool *statusList)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*获取想要读取的InputStatus量的值*/
void GetInputStatus(uint16_t startAddress, uint16_t quantity, bool *statusValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*获取想要读取的保持寄存器的值*/
void GetHoldingRegister(uint16_t startAddress, uint16_t quantity, uint16_t *registerValue)
{
  uint8_t i;

  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
  if (startAddress + 1 == STATION_ADDR)
  {
    for (i = 0; i < quantity; i++)
      *registerValue++ = i;
  }
}

/*获取想要读取的输入寄存器的值*/
void GetInputRegister(uint16_t startAddress, uint16_t quantity, uint16_t *registerValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*设置单个线圈的值*/
void SetSingleCoil(uint16_t coilAddress, bool coilValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*设置单个寄存器的值*/
void SetSingleRegister(uint16_t registerAddress, uint16_t registerValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*设置多个线圈的值*/
void SetMultipleCoil(uint16_t startAddress, uint16_t quantity, bool *statusValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

/*设置多个寄存器的值*/
void SetMultipleRegister(uint16_t startAddress, uint16_t quantity, uint16_t *registerValue)
{
  // 如果需要Modbus TCP Server/RTU Slave应用中实现具体内容
}

#endif
