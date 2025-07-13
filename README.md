# NOS SDK

## 项目简介

NOS SDK是一个用于嵌入式领域的多功能开发框架，集成了多种硬件驱动、图形界面库和应用程序示例。该SDK支持多种开发场景，包括基于MCU的APP开发、上位机开发等。

## 主要特性

- **多平台支持**: 支持Windows模拟器和多种MCU平台
- **图形界面**: 集成LVGL图形库，支持丰富的UI界面
- **实时系统**: 集成FreeRTOS实时操作系统
- **硬件抽象**: 提供统一的硬件抽象层，支持多种外设
- **模块化设计**: 采用模块化架构，便于扩展和维护
- **仿真开发**: Windows模拟器提供完整的MCU仿真环境，支持上位机开发调试

## 项目结构

```
nos_sdk/
├── app/                    # 应用程序目录
│   ├── ecu_hw_tunning/    # ECU硬件调试上位机
│   ├── iem_tunning/       # IEM调试上位机
│   ├── led_demo/          # LED演示应用
│   ├── lvgl_app/          # LVGL图形应用
│   │   ├── porting/       # LVGL移植层
│   │   └── smart_gadget/  # 智能设备应用
│   └── modbus_demo/       # Modbus演示应用
├── data_type.h            # 数据类型定义
├── middle/                # 中间件层
│   ├── external/          # 第三方库
│   │   ├── algorithm/     # 算法库
│   │   ├── container/     # 容器库
│   │   ├── easylogger/    # 日志库
│   │   ├── FreeRTOS/      # 实时操作系统
│   │   ├── general/       # 通用工具库
│   │   ├── lvgl/          # 图形界面库
│   │   ├── modbus/        # Modbus协议库
│   │   └── vstd/          # 标准库扩展
│   └── *.h                # NOS中间件接口
└── platform/              # 平台相关代码
    ├── include/           # 驱动接口定义
    └── win32/             # Windows平台实现
        └── nos_simu/      # Windows模拟器
```

## 支持的应用程序示例

### 1. LED演示应用 (`led_demo`)
- 功能：LED显示控制演示
- 特性：
  - 多级菜单系统
  - 按键事件处理
  - LED数字显示
  - 闹钟功能
  - 休眠模式

### 2. ECU硬件调试上位机 (`ecu_hw_tunning`)
- 功能：用于ECU的硬件调试
- 特性：
  - 设置MCU的PWM、DO值
  - 实时数据显示及记录

### 3. IEM调试上位机 (`iem_tunning`)
- 功能：模拟IEM及Android app
- 特性：
  - 通过模拟器，调试IEM和APP，提高开发和调试的效率

### 4. 智能设备应用 (`lvgl_app/smart_gadget`)
- 功能：智能设备界面演示
- 特性：
  - 音乐播放界面
  - 闹钟设置
  - 聊天界面
  - 相册浏览
  - 通话界面

### 5. Modbus演示应用 (`modbus_demo`)
- 功能：Modbus通信协议演示
- 特性：
  - Modbus RTU协议支持
  - 主从机通信
  - 数据读写演示
  - 通信状态监控

## MCU硬件支持

### 外设支持
- **GPIO**: 输入输出控制
- **ADC**: 模数转换
- **PWM**: 脉冲宽度调制
- **UART**: 串口通信
- **CAN**: CAN通信
- **I2C**: I2C串行总线
- **LCD**: 液晶显示
- **LED**: 7/8段数码管
- **按键**: 用户输入
- **编码器**: 旋转编码器
- **触摸板**: 触摸输入


## Windows模拟器

### 模拟器概述

Windows模拟器是NOS SDK的核心组件之一，为嵌入式开发提供了完整的MCU仿真环境。它主要用于：

- **MCU功能仿真**: 模拟MCU的各种外设功能，包括GPIO、ADC、PWM、UART、CAN、I2C等
- **上位机开发**: 为上位机软件开发提供统一的接口和调试环境
- **算法验证**: 在Windows环境下验证嵌入式算法和业务逻辑
- **界面调试**: 通过可视化界面实时监控和调试系统状态

### 仿真特性

- **硬件抽象层**: 提供与真实MCU一致的API接口
- **实时仿真**: 支持实时数据采集和状态监控
- **可视化界面**: 通过ui.ini配置文件定制用户界面
- **通信仿真**: 支持串口、CAN等通信协议的仿真
- **数据记录**: 支持数据采集、图表显示和文件导出

## 编译和运行

### 环境要求
- Windows 10/11
- Visual Studio 2019或更高版本
- CMake 3.5或更高版本

### 编译步骤

1. **克隆项目**
   ```bash
   git clone <repository-url>
   cd nos_sdk
   ```

2. **配置应用程序**
   编辑 `platform/win32/nos_simu/CMakeLists.txt` 文件，设置要编译的应用程序：
   ```cmake
   set(APP "LED_DEMO")  # 可选: LED_DEMO, ECU_HW_TUNNING, IEM_TUNNING, SMART_GADGET, MODBUS_DEMO
   ```

3. **生成项目文件**
   ```bash
   cd platform/win32/nos_simu
   cmake .
   ```

4. **编译项目**
   ```bash
   cmake --build . --config Debug
   ```

5. **运行程序**
   编译完成后，可执行文件位于 `Debug/` 目录下。

## 配置说明

每个应用程序都有自己的 `config.h` 文件，位于应用目录下（如 `app/ecu_hw_tunning/config.h`）。

### 全局配置
- `APP_SLEEP_MS`: 系统休眠时间
- `WATCH_DOG_EN`: 看门狗使能
- `ASSERT_EN`: 断言使能
- `SYS_TIME_EN`: 系统时间使能
- `PRINT_LOG_EN`: 日志打印使能
- `NOS_MMI_NODE_MGR_EN`: MMI节点管理使能

### 外设配置
- `KEY_NUM`: 按键数量
- `LED_NUM`: LED数量
- `ADC_NUM`: ADC通道数
- `PWM_NUM`: PWM通道数
- `UART_NUM`: UART数量
- `CAN_NUM`: CAN总线数量
- `I2C_NUM`: I2C总线数量

## 开发指南

### 添加新应用
1. 在 `app/` 目录下创建新的应用目录
2. 实现 `user_app_task()` 函数
3. 在 `CMakeLists.txt` 中添加应用配置
4. 设置应用特定的配置参数

### 硬件驱动开发
1. 在 `platform/include/` 中定义驱动接口
2. 在 `platform/win32/drv/` 中实现Windows平台驱动
3. 在 `middle/` 中提供统一的API接口

### UI界面开发
1. 使用SquareLine Studio设计界面
2. 生成UI代码到 `app/lvgl_app/` 对应目录
3. 实现事件处理函数

### 日志系统
- 支持EasyLogger日志库
- 可配置日志级别
- 支持异步日志输出

### 模拟器功能
- **Windows平台模拟器**: 完整的MCU仿真环境
- **实时数据监控**: 支持ADC、PWM、GPIO等外设的实时监控
- **参数调优界面**: 提供可视化的参数调整和系统优化
- **图表显示功能**: 支持实时数据图表和历史数据回放
- **通信协议仿真**: 支持UART、CAN等通信协议的完整仿真
- **上位机接口**: 为上位机软件开发提供标准化的接口

## 许可证

本项目采用 [许可证类型] 许可证，详见 LICENSE 文件。

## 贡献指南

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 联系方式

- 项目维护者: [ZYZ]
- 邮箱: [6528005@QQ.COM]

## 更新日志

### v1.0.0
- 初始版本发布
- 支持基础硬件驱动
- 集成LVGL图形库
- 提供多种应用示例
- 实现Windows模拟器，可支持MCU仿真和上位机开发

---

**注意**: 本项目仍在持续开发中，功能可能会有所变化。请关注项目更新。 

## Windows模拟器UI配置方法

### 1. 配置文件概述

Windows模拟器使用 `ui.ini` 配置文件来定义用户界面。每个应用程序都有自己的 `ui.ini` 文件，位于应用目录下（如 `app/ecu_hw_tunning/ui.ini`）。

### 2. 配置文件结构

#### 2.1 窗口配置段 `[WINDOW]`
```ini
[WINDOW]
MAX_ENABLE=1              # 是否允许最大化窗口
FONT_SIZE=70              # 默认字体大小
FONT_COLOR=0x000000       # 默认字体颜色（黑色）
FONT_NAME=微软雅黑        # 默认字体名称
```

#### 2.2 提示信息配置段 `[REMINDER]`
```ini
[REMINDER]
DISPLAY_MODE=DIALOG       # 显示模式：DIALOG(对话框) 或 空值(固定位置)
FIXED_POS={820,150}       # 固定位置坐标
WIDTH=380                 # 控件宽度
HEIGHT=60                 # 控件高度
FONT_NAME={黑体}          # 字体名称
FONT_SIZE={180}           # 字体大小
```

#### 2.3 组件定义段 `[GROUP]`
```ini
[GROUP]
GROUP={LCD,UART,GROUP_SWITCH,GROUP_PERIOD,GROUP_FILTER}
```
- 定义所有UI组件的显示顺序
- 组件名称用逗号分隔，用大括号包围
- **重要**：所有功能组件和控件组件都必须在此处声明才能显示

### 3. 硬件仿真组件配置

#### 3.1 LCD显示配置 `[LCD]`
```ini
[LCD]
HIDDEN=0                  # 隐藏状态 (0:显示, 1:隐藏)
WIDTH=800                 # LCD宽度
HEIGHT=400                # LCD高度
DEPTH=16                  # 颜色深度
WIDTH_ZOOM=100            # 宽度缩放比例
HEIGHT_ZOOM=100           # 高度缩放比例
```

#### 3.2 串口通信配置 `[UART]`
```ini
[UART]
DISP_NAME={通讯配置}      # 显示名称
WIDTH=200                 # 组件宽度
HEIGHT=280                # 组件高度
NEXT_GROUP_POS=2          # 下一个组件位置 (0:下方, 1:右边, 2:最左边)
HIDDEN=1                  # 隐藏状态
SHOW_HEX=1                # 显示十六进制
BAUD={9600}               # 波特率列表
```

#### 3.3 LED显示配置 `[LED]`
```ini
[LED]
HIDDEN=0                  # 隐藏状态
DIGIT_WIDTH=60            # 数字宽度
DIGIT_HEIGHT=100          # 数字高度
RECT_WIDTH=40             # 矩形宽度
RECT_HEIGHT=10            # 矩形高度
DIGIT_COLOR=0x00          # 数字颜色
RECT_COLOR=0xFF           # 矩形颜色
```

#### 3.4 按键配置 `[KEY]`
```ini
[KEY]
DISP_NAME=key             # 显示名称
WIDTH=100                 # 宽度
CONTROL_HEIGHT={40}       # 控件高度
CONTROL_NAME={MENU,BACK,OK,UP,DOWN}  # 按键名称
COMBINE_KEY_SIMU=0        # 组合键仿真开关
```

#### 3.5 数字输入输出配置 `[DI]/[DO]`
```ini
[DI]
DISP_NAME=input           # 显示名称
WIDTH=100                 # 宽度
CONTROL_HEIGHT={40}       # 控件高度
CONTROL_NAME={DI0,DI1,DI2,SDA,SCK}  # 控件名称
CONTROL_DEFAULT_VALUE={1,0,0,0,0}   # 默认值
CHART_SAMPLE_RATE=10      # 图表采样率
```

#### 3.6 PWM配置 `[PWM]`
```ini
[PWM]
DISP_NAME=pwm             # 显示名称
WIDTH=120                 # 宽度
CONTROL_HEIGHT={40}       # 控件高度
CONTROL_NAME={PWM0,PWM1,PWM2,PWM3,PWM4}  # PWM通道名称
CONTROL_DEFAULT_VALUE={0,0,0,0,0}         # 默认值
```

#### 3.7 ADC配置 `[ADC]`
```ini
[ADC]
DISP_NAME=adc             # 显示名称
WIDTH=200                 # 宽度
CONTROL_HEIGHT={40}       # 控件高度
CONTROL_NAME={ADC0,ADC1,ADC2,ADC3,ADC4}  # ADC通道名称
CONTROL_DEFAULT_VALUE={100,200,300,400,500}  # 默认值
ADC_RANGE={4095,1023,4095,4095,4095}      # ADC量程
```

#### 3.8 编码器配置 `[ENCODER]`
```ini
[ENCODER]
DISP_NAME=encode          # 显示名称
WIDTH=150                 # 宽度
CONTROL_HEIGHT={40}       # 控件高度
CONTROL_NAME={ENCODER0,ENCODER1}  # 编码器名称
CONTROL_DEFAULT_VALUE={0,0}       # 默认值
ENCODER_RANGE={4000,4000}         # 编码器量程
```

### 4. 自定义控件组件配置

#### 4.1 基本结构
```ini
[GROUP_组名]
DISP_NAME=显示名称        # 组件显示名称
WIDTH=300                # 组件宽度
NEXT_GROUP_POS=0         # 下一个组件位置 (0:下方, 1:右边, 2:最左边)
MAX_ITEMS_PER_COLUMN=10  # 每列最大控件数
CONTROL_NAME={控件1,控件2,控件3}  # 控件名称列表
CONTROL_TYPE={SLIDER,EDIT,CHECK,BUTTON,CHART,FILE,COMBO,EDIT_BUTTON,STATIC}  # 控件类型
CONTROL_DATA_TYPE={0,1}  # 数据类型 (0:数值, 1:字符串)
CONTROL_NAME_WIDTH={200} # 控件名称宽度
CONTROL_HEIGHT={50}      # 控件高度
CONTROL_DEFAULT_VALUE={0,1,2}  # 默认值列表
```

#### 4.2 控件类型详解

**滑块控件 (SLIDER)**
```ini
CONTROL_TYPE={SLIDER}
SLIDER_MIN={0}           # 最小值
SLIDER_MAX={1023}        # 最大值
```

**编辑框控件 (EDIT)**
```ini
CONTROL_TYPE={EDIT}
CONTROL_DATA_TYPE={0}    # 0:数值, 1:字符串
```

**复选框控件 (CHECK)**
```ini
CONTROL_TYPE={CHECK}
CONTROL_DATA_TYPE={1}    # 通常为字符串类型
```

**按钮控件 (BUTTON)**
```ini
CONTROL_TYPE={BUTTON}
```

**下拉框控件 (COMBO)**
```ini
CONTROL_TYPE={COMBO}
COMBO_LISTx={选项1,选项2}  # x表示下拉框在控件列表中的位置
```

**文件控件 (FILE)**
```ini
CONTROL_TYPE={FILE}
FILE_EXT={bin,txt,ini}   # 文件扩展名列表
```

**图表控件 (CHART)**
```ini
CONTROL_TYPE={CHART}
CHART_SAMPLE_RATE=200    # 刷新率，单位ms
CHART_SERIES_NAME0={系列1,系列2}  # 数据系列名称
CHART_SERIES_COLOR0={0x0000FF,0xFF0000}  # 数据系列颜色
```

**静态文本控件 (STATIC)**
```ini
CONTROL_TYPE={STATIC}
STATIC_CHART={1}         # 静态图表开关 (1:打开, 0:关闭)
```

### 5. 实际配置示例

#### 5.1 ECU硬件调试上位机配置示例
```ini
[WINDOW]
MAX_ENABLE=1
FONT_SIZE=70
FONT_COLOR=0x000000
FONT_NAME=微软雅黑

[REMINDER]
FIXED_POS={820,150}
WIDTH=380
HEIGHT=60
FONT_NAME={黑体}
FONT_SIZE={180}

[GROUP]
GROUP={LCD,UART,GROUP_SWITCH,GROUP_PERIOD,GROUP_FILTER,GROUP_PID,GROUP_CALI,GROUP_MISC,GROUP_ALS,GROUP_PLS,GROUP_BAND,GROUP_TEMP,GROUP_CMD}

[GROUP_SWITCH]
DISP_NAME=功能开关
WIDTH=220
NEXT_GROUP_POS=0
MAX_ITEMS_PER_COLUMN=10
CONTROL_NAME_WIDTH={120}
CONTROL_HEIGHT={33}
CONTROL_TYPE={CHECK}
CONTROL_DATA_TYPE={1}
CONTROL_NAME={LOG输出,温度补偿,开环模式}

[GROUP_PERIOD]
DISP_NAME=周期数(10ms)
WIDTH=220
NEXT_GROUP_POS=0
MAX_ITEMS_PER_COLUMN=10
CONTROL_NAME_WIDTH={120}
CONTROL_HEIGHT={33}
CONTROL_TYPE={EDIT}
CONTROL_DATA_TYPE={0}
CONTROL_NAME={眩目光采样,环境光采样,反射率控制}

[LCD]
HIDDEN=0
WIDTH=800
HEIGHT=400
DEPTH=16
WIDTH_ZOOM=100
HEIGHT_ZOOM=100

[UART]
DISP_NAME={通讯配置}
WIDTH=200
HEIGHT=280
NEXT_GROUP_POS=2
HIDDEN=1
SHOW_HEX=1
```


### 6. 配置步骤

1. **选择应用程序**
   编辑 `platform/win32/nos_simu/CMakeLists.txt` 文件：
   ```cmake
   set(APP "ECU_HW_TUNNING")  # 设置要编译的应用程序
   ```

2. **创建或修改ui.ini文件**
   在对应的应用目录下创建或修改 `ui.ini` 文件

3. **配置组件**
   - 在 `[GROUP]` 段中声明所有需要的组件
   - 为每个组件创建对应的配置段
   - 设置组件的显示参数和控件属性

4. **编译和运行**
   ```bash
   cd platform/win32/nos_simu
   cmake --build . --config Debug
   ```

### 7. 注意事项

1. **组件声明**：所有功能组件和控件组件都必须在 `[GROUP]` 段中声明才能显示
2. **参数格式**：字符串值用大括号包围，数值直接写数字
3. **数组长度**：数组参数的长度可以与对应的项目数量不一致
4. **位置计算**：`NEXT_GROUP_POS` 影响组件的布局排列
5. **数据类型匹配**：确保 `CONTROL_DATA_TYPE` 与控件类型匹配
6. **颜色格式**：颜色值必须使用十六进制格式
7. **字体设置**：字体大小范围通常为8-72

通过这种配置方式，您可以灵活地定制Windows模拟器的用户界面，满足不同应用程序的需求。 