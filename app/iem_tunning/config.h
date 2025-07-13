#ifndef _CONFIG_H_
#define _CONFIG_H_

// 根据MCU的SDK包含main函数的情况，确定是否打开这个宏
#define HAVE_MAIN_IN_SDK

// 定义为inline或空，用于加快程序执行速度或节省程序空间的选择
#ifdef __C51__ //不支持C51的MCU
#define _INLINE
#else
#define _INLINE inline
#endif

#define APP_SLEEP_MS 1 // ms, 0表示不休眠

#define WATCH_DOG_EN 1 // 是否开启看门狗

#define ASSERT_EN 1 // nos_assert是否有效

#define SYS_TIME_EN 1 // 是否开启时间运行系统

#define PRINT_LOG_EN 1 // 是否开启log打印功能

#define NOS_MMI_NODE_MGR_EN 1      // MMI节点管理框架
#define MMI_NODE_STACK_MAX_SIZE 15 // MMI节点管理框架定义的最大栈深度

// key config
#define KEY_NUM 5 // 按键个数
#if KEY_NUM > 1
#define COMBINE_KEY_NUM 2 // 组合键个数
#else
#define COMBINE_KEY_NUM 0
#endif

#define IO_INPUT_NUM 5  // input gpio number
#define IO_OUTPUT_NUM 5 // output gpio number
#define ADC_NUM 8       // adc number
#define PWM_NUM 2       // pwm number
#define ENCODER_NUM 2   // encoder number

// UART config
#define UART_NUM 2            // uart number: 0 - 3
#define UART_RECV_TIMEOUT 100 // ms
#if UART_NUM > 0
#define UART0_BAUD_RATE 9600
#define UART0_RECV_BUF_LEN 255
#define UART0_SEND_BUF_LEN 255
#endif
#if UART_NUM > 1
#define UART1_BAUD_RATE 9600
#define UART1_RECV_BUF_LEN 255
#define UART1_SEND_BUF_LEN 255
#endif
#if UART_NUM > 2
#define UART2_BAUD_RATE 115200
#define UART2_RECV_BUF_LEN 255
#define UART2_SEND_BUF_LEN 255
#endif

#define CAN_NUM 0
#define CAN_RECV_TIMEOUT 20 // ms
#if CAN_NUM > 0
#define CAN0_RECV_BUF_LEN 255
#define CAN0_SEND_BUF_LEN 255
#endif
#if CAN_NUM > 1
#define CAN1_RECV_BUF_LEN 255
#define CAN1_SEND_BUF_LEN 255
#endif
#if CAN_NUM > 2
#define CAN2_RECV_BUF_LEN 255
#define CAN2_SEND_BUF_LEN 255
#endif

// LED config
#define LED_EN 1
#if LED_EN
#define LED_WINK_EN 1   // 是否开启闪烁
#define LED_NUM 8       // led number
#define LED_DIGIT_NUM 6 // led digit number
#else
#define LED_WINK_EN 0
#define LED_NUM 0
#define LED_DIGIT_NUM 0
#endif

// LCD config
#define LCD_EN 1
#if LCD_EN
#define LCD_PIXEL_WIDTH 800l
#define LCD_PIXEL_HEIGHT 400l
#define LCD_PIXEL_DEPTH 16
#endif

//touchpad config
#define TOUCHPAD_EN 1
#if TOUCHPAD_EN
#if LCD_EN == 0
#error "Please define LCD_EN to 1 when TOUCHPAD_EN is 1"
#endif
#endif

// I2C config
#define I2C_NUM 0
#define I2C_SCL_STRETCH_EN 0 // 延展
#if I2C_NUM > 0
#define I2C0_SCL_IO_NUM 0
#define I2C0_SDA_IO_NUM 1
#define I2C0_SPEED 20 // 值越大，速度越慢，要根据实际情况调整
#endif
#if I2C_NUM > 1
#define I2C1_SCL_IO_NUM 2
#define I2C1_SDA_IO_NUM 3
#define I2C1_SPEED 20 // 值越大，速度越慢，要根据实际情况调整
#endif
#if I2C_NUM > 2
#define I2C2_SCL_IO_NUM 4
#define I2C2_SDA_IO_NUM 5
#define I2C2_SPEED 20 // 值越大，速度越慢，要根据实际情况调整
#endif

// nvram config
#define NVRAM_FILE_NUM 2 // 每个nvram file 占用FLASH_BLOCK_SIZE大小的flash空间，首个file的地址为FLASH_NVRAM_BASE_ADDR，后面的file依次加FLASH_BLOCK_SIZE
#if NVRAM_FILE_NUM > 0
#define FLASH_BLOCK_SIZE 0x2000
#define FLASH_NVRAM_BASE_ADDR 0x20000
#define FLASH_WRITE_ONE_WORD_SIZE 4
#endif


// easylogger config
#ifdef __C51__
#define EASYLOGGER_EN 0 //不支持C51的MCU
#else
#define EASYLOGGER_EN 0
#endif
#define EASYLOGGER_UART_NUM 0 // uart number: 0 - 3
#if EASYLOGGER_EN
#if UART_NUM == 0
#error "Please define UART_NUM when EASYLOGGER_EN is 1"
#endif
#endif


#endif
