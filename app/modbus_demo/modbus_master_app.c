#if __MODBUS_DEMO_ENABLE__

#include "../../middle/external/modbus/mbrtumaster.h"
#include "../../middle/external/general/cqueue.h"
#include "modbus_master_app.h"
#include "../../middle/nos_common.h"


static init_queue_t init_queue;

static modbus_app_reg_00_0F_t modbus_app_reg_00_0F;
static modbus_app_reg_10_1F_t modbus_app_reg_10_1F;

static modbus_app_data_info_t modbus_app_data_info[] = {
    {DEV1_SALVE_ADDR, MODBUS_CMD_WRITE, 0x00, &modbus_app_reg_00_0F, sizeof(modbus_app_reg_00_0F_t), MODBUS_CMD_STATUS_DONE},
    {DEV2_SALVE_ADDR, MODBUS_CMD_READ, 0x10, &modbus_app_reg_10_1F, sizeof(modbus_app_reg_10_1F_t), MODBUS_CMD_STATUS_DONE},
};

static RTULocalMasterType rtuMaster;
static uint8_t slaveWriteCommand[256];

static void uart_recv_process(uint8_t uart_num, uint8_t *buf, uint16_t len);
static bool modbus_app_put_cmd(uint8_t slave_addr, uint8_t cmd_id, modbus_cmd_type_t cmd_type);
static void modbus_app_get_cmd(void);

void UpdateHoldingRegister(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerValue);
void UpdateCoilStatus(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, bool *stateValue);
void UpdateInputStatus(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, bool *stateValue);
void UpdateInputResgister(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerValue);

static uint16_t tick;

static uint16_t tick_send_cmd;

static void uart_recv_process(uint8_t uart_num, uint8_t xdata_t *buf, uint16_t len)
{
   if(uart_num != MODBUS_MASTER_UART_NUM)
      return;
   ParsingSlaveRespondMessage(&rtuMaster, buf, slaveWriteCommand);
}

void WriteRegister2Slave(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerList)
{
   ObjAccessInfo slaveInfo;
   uint16_t WriteDataLen;

   slaveInfo.functionCode = WriteMultipleRegister;
   slaveInfo.unitID = salveAddress;
   slaveInfo.startingAddress = startAddress;
   slaveInfo.quantity = quantity;

   WriteDataLen = SyntheticReadWriteSlaveCommand(slaveInfo, NULL, registerList, slaveWriteCommand);

   nos_uart_send_data(MODBUS_MASTER_UART_NUM, slaveWriteCommand, WriteDataLen);
}

void ReadRegister2Slave(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerList)
{
   ObjAccessInfo slaveInfo;
   uint16_t WriteDataLen;

   slaveInfo.functionCode = ReadHoldingRegister;
   slaveInfo.unitID = salveAddress;
   slaveInfo.startingAddress = startAddress;
   slaveInfo.quantity = quantity;

   WriteDataLen = SyntheticReadWriteSlaveCommand(slaveInfo, NULL, NULL, slaveWriteCommand);

   nos_uart_send_data(MODBUS_MASTER_UART_NUM, slaveWriteCommand, WriteDataLen);
}

void modbus_master_app_task_init(void)
{
   static bool is_init = false;
   if (is_init == false)
   {
      is_init = true;
      cQueue_init(init_queue);

      nos_uart_recv_regist_cb(MODBUS_MASTER_UART_NUM, uart_recv_process);
      InitializeRTUMasterObject(&rtuMaster, 2, NULL, UpdateCoilStatus, UpdateInputStatus, UpdateHoldingRegister, UpdateInputResgister);
   }
}

void modbus_master_app_task(void)
{
   static uint16_t counter = 0;

   // modbus_master_app_task_init();

   if (nos_tick_timeout_16(&tick, 1000))
   {
      modbus_app_reg_00_0F.data3 = nos_adc_get_value(1);
      modbus_app_reg_00_0F.data4 = nos_adc_get_value(2);
      modbus_app_reg_00_0F.data5 = nos_adc_get_value(4);
      // modbus_app_reg_10_1F.data1 = counter++;
      if (modbus_app_put_cmd(DEV1_SALVE_ADDR, MODBUS_CMD_ID0, MODBUS_CMD_WRITE) == false)
         PRINT_LOG("modbus_app_put_cmd failed!cmd_id = %d\r\n", 0);
      if (modbus_app_put_cmd(DEV1_SALVE_ADDR, MODBUS_CMD_ID1, MODBUS_CMD_READ) == false)
         PRINT_LOG("modbus_app_put_cmd failed!cmd_id = %d\r\n", 1);
   }

   if (nos_tick_timeout_16(&tick_send_cmd, 300))
   {
      modbus_app_get_cmd();
   }
}

bool modbus_app_put_cmd(uint8_t slave_addr, uint8_t cmd_id, modbus_cmd_type_t cmd_type)
{
   if (cmd_id >= sizeof(modbus_app_data_info) / sizeof(modbus_app_data_info[0]))
      return false;

   if (modbus_app_data_info[cmd_id].cmd_status != MODBUS_CMD_STATUS_DONE)
      return false;

   modbus_app_data_info[cmd_id].cmd_status = MODBUS_CMD_STATUS_START;
   modbus_app_data_info[cmd_id].slave_addr = slave_addr;
   modbus_app_data_info[cmd_id].cmd_type = cmd_type;

   cQueue_push(init_queue, (void *)&modbus_app_data_info[cmd_id]);

   return true;
}

void modbus_app_get_cmd(void)
{
   modbus_app_data_info_t *modbus_data = NULL;

   if (cQueue_pop(init_queue, modbus_data) == 0)
      return;

   if (modbus_data == NULL)
      return;

   if (modbus_data->cmd_type == MODBUS_CMD_WRITE)
   {
      WriteRegister2Slave(modbus_data->slave_addr, modbus_data->base_reg_addr, modbus_data->size / 2, modbus_data->p_data);
   }
   else if (modbus_data->cmd_type == MODBUS_CMD_READ)
   {
      ReadRegister2Slave(modbus_data->slave_addr, modbus_data->base_reg_addr, modbus_data->size / 2, modbus_data->p_data);
   }

   modbus_data->cmd_status = MODBUS_CMD_STATUS_DONE;
}

void UpdateCoilStatus(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, bool *stateValue)
{
   // 在客户端（主站）应用中实现
}

/*更新读回来的输入状态值*/
void UpdateInputStatus(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, bool *stateValue)
{
   // 在客户端（主站）应用中实现
}

void UpdateHoldingRegister(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerValue)
{
   // 在客户端（主站）应用中实现
   if (salveAddress == DEV1_SALVE_ADDR && startAddress == modbus_app_data_info[MODBUS_CMD_ID1].base_reg_addr && quantity == modbus_app_data_info[MODBUS_CMD_ID1].size / 2)
   {
      modbus_app_reg_00_0F.data1 = *registerValue++;
      modbus_app_reg_00_0F.data2 = *registerValue++;
   }
}

/*更新读回来的输入寄存器*/
void UpdateInputResgister(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerValue)
{
   // 在客户端（主站）应用中实现
}

#endif
