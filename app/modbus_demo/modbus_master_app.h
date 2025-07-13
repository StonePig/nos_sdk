#ifndef _MODBUS_MASTER_APP_H_
#define _MODBUS_MASTER_APP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MODBUS_MASTER_UART_NUM 0

#define DEV1_SALVE_ADDR 0x01 // 设备地址1
#define DEV2_SALVE_ADDR 0x02 // 设备地址2

#define SLAVE_DEV_NUM (2) // 设备数量

#define CMD_TIMEOUT_MS 500

    typedef enum
    {
        MODBUS_CMD_NONE,
        MODBUS_CMD_READ,
        MODBUS_CMD_WRITE,
        // MODBUS_CMD_WRITE_READ,
    } modbus_cmd_type_t;

    typedef enum
    {
        MODBUS_CMD_STATUS_FAIL,
        MODBUS_CMD_STATUS_DONE,
        MODBUS_CMD_STATUS_START,
        MODBUS_CMD_STATUS_DOING,
        MODBUS_CMD_STATUS_TIMEOUT,
    } modbus_cmd_status_t;

    typedef struct
    {
        uint8_t slave_addr;
        modbus_cmd_type_t cmd_type;
        uint16_t base_reg_addr;
        void *p_data;
        uint8_t size;
        modbus_cmd_status_t cmd_status;
    } modbus_app_data_info_t;

    typedef struct
    {
        uint16_t data1;
        uint16_t data2;
        uint16_t data3;
        uint16_t data4;
        uint16_t data5;
        uint16_t data6;
        uint16_t data7;
        uint16_t data8;
    } modbus_app_reg_00_0F_t;

    typedef struct
    {
        uint16_t data1;
        uint16_t data2;
        uint16_t data3;
        uint16_t data4;
        uint16_t data5;
        uint16_t data6;
        uint16_t data7;
        uint16_t data8;
    } modbus_app_reg_10_1F_t;

    typedef enum
    {
        MODBUS_CMD_ID0 = 0,
        MODBUS_CMD_ID1,

        MODBUS_CMD_ID_MAX,
    } modbus_cmd_id_t;

    typedef struct
    {
        cQueue queue;
        void *p_data[MODBUS_CMD_ID_MAX];
    } init_queue_t;

    void WriteRegister2Slave(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerList);
    void ReadRegister2Slave(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity, uint16_t *registerList);
    void WriteMultipleRegisterRespond(uint8_t salveAddress, uint16_t startAddress, uint16_t quantity);

#ifdef __cplusplus
}
#endif

#endif
