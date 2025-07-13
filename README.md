# NOS SDK

## ��Ŀ���

NOS SDK��һ������Ƕ��ʽ����Ķ๦�ܿ�����ܣ������˶���Ӳ��������ͼ�ν�����Ӧ�ó���ʾ������SDK֧�ֶ��ֿ�����������������MCU��APP��������λ�������ȡ�

## ��Ҫ����

- **��ƽ̨֧��**: ֧��Windowsģ�����Ͷ���MCUƽ̨
- **ͼ�ν���**: ����LVGLͼ�ο⣬֧�ַḻ��UI����
- **ʵʱϵͳ**: ����FreeRTOSʵʱ����ϵͳ
- **Ӳ������**: �ṩͳһ��Ӳ������㣬֧�ֶ�������
- **ģ�黯���**: ����ģ�黯�ܹ���������չ��ά��
- **���濪��**: Windowsģ�����ṩ������MCU���滷����֧����λ����������

## ��Ŀ�ṹ

```
nos_sdk/
������ app/                    # Ӧ�ó���Ŀ¼
��   ������ ecu_hw_tunning/    # ECUӲ��������λ��
��   ������ iem_tunning/       # IEM������λ��
��   ������ led_demo/          # LED��ʾӦ��
��   ������ lvgl_app/          # LVGLͼ��Ӧ��
��   ��   ������ porting/       # LVGL��ֲ��
��   ��   ������ smart_gadget/  # �����豸Ӧ��
��   ������ modbus_demo/       # Modbus��ʾӦ��
������ data_type.h            # �������Ͷ���
������ middle/                # �м����
��   ������ external/          # ��������
��   ��   ������ algorithm/     # �㷨��
��   ��   ������ container/     # ������
��   ��   ������ easylogger/    # ��־��
��   ��   ������ FreeRTOS/      # ʵʱ����ϵͳ
��   ��   ������ general/       # ͨ�ù��߿�
��   ��   ������ lvgl/          # ͼ�ν����
��   ��   ������ modbus/        # ModbusЭ���
��   ��   ������ vstd/          # ��׼����չ
��   ������ *.h                # NOS�м���ӿ�
������ platform/              # ƽ̨��ش���
    ������ include/           # �����ӿڶ���
    ������ win32/             # Windowsƽ̨ʵ��
        ������ nos_simu/      # Windowsģ����
```

## ֧�ֵ�Ӧ�ó���ʾ��

### 1. LED��ʾӦ�� (`led_demo`)
- ���ܣ�LED��ʾ������ʾ
- ���ԣ�
  - �༶�˵�ϵͳ
  - �����¼�����
  - LED������ʾ
  - ���ӹ���
  - ����ģʽ

### 2. ECUӲ��������λ�� (`ecu_hw_tunning`)
- ���ܣ�����ECU��Ӳ������
- ���ԣ�
  - ����MCU��PWM��DOֵ
  - ʵʱ������ʾ����¼

### 3. IEM������λ�� (`iem_tunning`)
- ���ܣ�ģ��IEM��Android app
- ���ԣ�
  - ͨ��ģ����������IEM��APP����߿����͵��Ե�Ч��

### 4. �����豸Ӧ�� (`lvgl_app/smart_gadget`)
- ���ܣ������豸������ʾ
- ���ԣ�
  - ���ֲ��Ž���
  - ��������
  - �������
  - ������
  - ͨ������

### 5. Modbus��ʾӦ�� (`modbus_demo`)
- ���ܣ�Modbusͨ��Э����ʾ
- ���ԣ�
  - Modbus RTUЭ��֧��
  - ���ӻ�ͨ��
  - ���ݶ�д��ʾ
  - ͨ��״̬���

## MCUӲ��֧��

### ����֧��
- **GPIO**: �����������
- **ADC**: ģ��ת��
- **PWM**: �����ȵ���
- **UART**: ����ͨ��
- **CAN**: CANͨ��
- **I2C**: I2C��������
- **LCD**: Һ����ʾ
- **LED**: 7/8�������
- **����**: �û�����
- **������**: ��ת������
- **������**: ��������


## Windowsģ����

### ģ��������

Windowsģ������NOS SDK�ĺ������֮һ��ΪǶ��ʽ�����ṩ��������MCU���滷��������Ҫ���ڣ�

- **MCU���ܷ���**: ģ��MCU�ĸ������蹦�ܣ�����GPIO��ADC��PWM��UART��CAN��I2C��
- **��λ������**: Ϊ��λ����������ṩͳһ�Ľӿں͵��Ի���
- **�㷨��֤**: ��Windows��������֤Ƕ��ʽ�㷨��ҵ���߼�
- **�������**: ͨ�����ӻ�����ʵʱ��غ͵���ϵͳ״̬

### ��������

- **Ӳ�������**: �ṩ����ʵMCUһ�µ�API�ӿ�
- **ʵʱ����**: ֧��ʵʱ���ݲɼ���״̬���
- **���ӻ�����**: ͨ��ui.ini�����ļ������û�����
- **ͨ�ŷ���**: ֧�ִ��ڡ�CAN��ͨ��Э��ķ���
- **���ݼ�¼**: ֧�����ݲɼ���ͼ����ʾ���ļ�����

## ���������

### ����Ҫ��
- Windows 10/11
- Visual Studio 2019����߰汾
- CMake 3.5����߰汾

### ���벽��

1. **��¡��Ŀ**
   ```bash
   git clone <repository-url>
   cd nos_sdk
   ```

2. **����Ӧ�ó���**
   �༭ `platform/win32/nos_simu/CMakeLists.txt` �ļ�������Ҫ�����Ӧ�ó���
   ```cmake
   set(APP "LED_DEMO")  # ��ѡ: LED_DEMO, ECU_HW_TUNNING, IEM_TUNNING, SMART_GADGET, MODBUS_DEMO
   ```

3. **������Ŀ�ļ�**
   ```bash
   cd platform/win32/nos_simu
   cmake .
   ```

4. **������Ŀ**
   ```bash
   cmake --build . --config Debug
   ```

5. **���г���**
   ������ɺ󣬿�ִ���ļ�λ�� `Debug/` Ŀ¼�¡�

## ����˵��

ÿ��Ӧ�ó������Լ��� `config.h` �ļ���λ��Ӧ��Ŀ¼�£��� `app/ecu_hw_tunning/config.h`����

### ȫ������
- `APP_SLEEP_MS`: ϵͳ����ʱ��
- `WATCH_DOG_EN`: ���Ź�ʹ��
- `ASSERT_EN`: ����ʹ��
- `SYS_TIME_EN`: ϵͳʱ��ʹ��
- `PRINT_LOG_EN`: ��־��ӡʹ��
- `NOS_MMI_NODE_MGR_EN`: MMI�ڵ����ʹ��

### ��������
- `KEY_NUM`: ��������
- `LED_NUM`: LED����
- `ADC_NUM`: ADCͨ����
- `PWM_NUM`: PWMͨ����
- `UART_NUM`: UART����
- `CAN_NUM`: CAN��������
- `I2C_NUM`: I2C��������

## ����ָ��

### �����Ӧ��
1. �� `app/` Ŀ¼�´����µ�Ӧ��Ŀ¼
2. ʵ�� `user_app_task()` ����
3. �� `CMakeLists.txt` �����Ӧ������
4. ����Ӧ���ض������ò���

### Ӳ����������
1. �� `platform/include/` �ж��������ӿ�
2. �� `platform/win32/drv/` ��ʵ��Windowsƽ̨����
3. �� `middle/` ���ṩͳһ��API�ӿ�

### UI���濪��
1. ʹ��SquareLine Studio��ƽ���
2. ����UI���뵽 `app/lvgl_app/` ��ӦĿ¼
3. ʵ���¼�������

### ��־ϵͳ
- ֧��EasyLogger��־��
- ��������־����
- ֧���첽��־���

### ģ��������
- **Windowsƽ̨ģ����**: ������MCU���滷��
- **ʵʱ���ݼ��**: ֧��ADC��PWM��GPIO�������ʵʱ���
- **�������Ž���**: �ṩ���ӻ��Ĳ���������ϵͳ�Ż�
- **ͼ����ʾ����**: ֧��ʵʱ����ͼ�����ʷ���ݻط�
- **ͨ��Э�����**: ֧��UART��CAN��ͨ��Э�����������
- **��λ���ӿ�**: Ϊ��λ����������ṩ��׼���Ľӿ�

## ���֤

����Ŀ���� [���֤����] ���֤����� LICENSE �ļ���

## ����ָ��

1. Fork ��Ŀ
2. �������ܷ�֧ (`git checkout -b feature/AmazingFeature`)
3. �ύ���� (`git commit -m 'Add some AmazingFeature'`)
4. ���͵���֧ (`git push origin feature/AmazingFeature`)
5. �� Pull Request

## ��ϵ��ʽ

- ��Ŀά����: [ZYZ]
- ����: [6528005@QQ.COM]

## ������־

### v1.0.0
- ��ʼ�汾����
- ֧�ֻ���Ӳ������
- ����LVGLͼ�ο�
- �ṩ����Ӧ��ʾ��
- ʵ��Windowsģ��������֧��MCU�������λ������

---

**ע��**: ����Ŀ���ڳ��������У����ܿ��ܻ������仯�����ע��Ŀ���¡� 

## Windowsģ����UI���÷���

### 1. �����ļ�����

Windowsģ����ʹ�� `ui.ini` �����ļ��������û����档ÿ��Ӧ�ó������Լ��� `ui.ini` �ļ���λ��Ӧ��Ŀ¼�£��� `app/ecu_hw_tunning/ui.ini`����

### 2. �����ļ��ṹ

#### 2.1 �������ö� `[WINDOW]`
```ini
[WINDOW]
MAX_ENABLE=1              # �Ƿ�������󻯴���
FONT_SIZE=70              # Ĭ�������С
FONT_COLOR=0x000000       # Ĭ��������ɫ����ɫ��
FONT_NAME=΢���ź�        # Ĭ����������
```

#### 2.2 ��ʾ��Ϣ���ö� `[REMINDER]`
```ini
[REMINDER]
DISPLAY_MODE=DIALOG       # ��ʾģʽ��DIALOG(�Ի���) �� ��ֵ(�̶�λ��)
FIXED_POS={820,150}       # �̶�λ������
WIDTH=380                 # �ؼ����
HEIGHT=60                 # �ؼ��߶�
FONT_NAME={����}          # ��������
FONT_SIZE={180}           # �����С
```

#### 2.3 �������� `[GROUP]`
```ini
[GROUP]
GROUP={LCD,UART,GROUP_SWITCH,GROUP_PERIOD,GROUP_FILTER}
```
- ��������UI�������ʾ˳��
- ��������ö��ŷָ����ô����Ű�Χ
- **��Ҫ**�����й�������Ϳؼ�����������ڴ˴�����������ʾ

### 3. Ӳ�������������

#### 3.1 LCD��ʾ���� `[LCD]`
```ini
[LCD]
HIDDEN=0                  # ����״̬ (0:��ʾ, 1:����)
WIDTH=800                 # LCD���
HEIGHT=400                # LCD�߶�
DEPTH=16                  # ��ɫ���
WIDTH_ZOOM=100            # ������ű���
HEIGHT_ZOOM=100           # �߶����ű���
```

#### 3.2 ����ͨ������ `[UART]`
```ini
[UART]
DISP_NAME={ͨѶ����}      # ��ʾ����
WIDTH=200                 # ������
HEIGHT=280                # ����߶�
NEXT_GROUP_POS=2          # ��һ�����λ�� (0:�·�, 1:�ұ�, 2:�����)
HIDDEN=1                  # ����״̬
SHOW_HEX=1                # ��ʾʮ������
BAUD={9600}               # �������б�
```

#### 3.3 LED��ʾ���� `[LED]`
```ini
[LED]
HIDDEN=0                  # ����״̬
DIGIT_WIDTH=60            # ���ֿ��
DIGIT_HEIGHT=100          # ���ָ߶�
RECT_WIDTH=40             # ���ο��
RECT_HEIGHT=10            # ���θ߶�
DIGIT_COLOR=0x00          # ������ɫ
RECT_COLOR=0xFF           # ������ɫ
```

#### 3.4 �������� `[KEY]`
```ini
[KEY]
DISP_NAME=key             # ��ʾ����
WIDTH=100                 # ���
CONTROL_HEIGHT={40}       # �ؼ��߶�
CONTROL_NAME={MENU,BACK,OK,UP,DOWN}  # ��������
COMBINE_KEY_SIMU=0        # ��ϼ����濪��
```

#### 3.5 ��������������� `[DI]/[DO]`
```ini
[DI]
DISP_NAME=input           # ��ʾ����
WIDTH=100                 # ���
CONTROL_HEIGHT={40}       # �ؼ��߶�
CONTROL_NAME={DI0,DI1,DI2,SDA,SCK}  # �ؼ�����
CONTROL_DEFAULT_VALUE={1,0,0,0,0}   # Ĭ��ֵ
CHART_SAMPLE_RATE=10      # ͼ�������
```

#### 3.6 PWM���� `[PWM]`
```ini
[PWM]
DISP_NAME=pwm             # ��ʾ����
WIDTH=120                 # ���
CONTROL_HEIGHT={40}       # �ؼ��߶�
CONTROL_NAME={PWM0,PWM1,PWM2,PWM3,PWM4}  # PWMͨ������
CONTROL_DEFAULT_VALUE={0,0,0,0,0}         # Ĭ��ֵ
```

#### 3.7 ADC���� `[ADC]`
```ini
[ADC]
DISP_NAME=adc             # ��ʾ����
WIDTH=200                 # ���
CONTROL_HEIGHT={40}       # �ؼ��߶�
CONTROL_NAME={ADC0,ADC1,ADC2,ADC3,ADC4}  # ADCͨ������
CONTROL_DEFAULT_VALUE={100,200,300,400,500}  # Ĭ��ֵ
ADC_RANGE={4095,1023,4095,4095,4095}      # ADC����
```

#### 3.8 ���������� `[ENCODER]`
```ini
[ENCODER]
DISP_NAME=encode          # ��ʾ����
WIDTH=150                 # ���
CONTROL_HEIGHT={40}       # �ؼ��߶�
CONTROL_NAME={ENCODER0,ENCODER1}  # ����������
CONTROL_DEFAULT_VALUE={0,0}       # Ĭ��ֵ
ENCODER_RANGE={4000,4000}         # ����������
```

### 4. �Զ���ؼ��������

#### 4.1 �����ṹ
```ini
[GROUP_����]
DISP_NAME=��ʾ����        # �����ʾ����
WIDTH=300                # ������
NEXT_GROUP_POS=0         # ��һ�����λ�� (0:�·�, 1:�ұ�, 2:�����)
MAX_ITEMS_PER_COLUMN=10  # ÿ�����ؼ���
CONTROL_NAME={�ؼ�1,�ؼ�2,�ؼ�3}  # �ؼ������б�
CONTROL_TYPE={SLIDER,EDIT,CHECK,BUTTON,CHART,FILE,COMBO,EDIT_BUTTON,STATIC}  # �ؼ�����
CONTROL_DATA_TYPE={0,1}  # �������� (0:��ֵ, 1:�ַ���)
CONTROL_NAME_WIDTH={200} # �ؼ����ƿ��
CONTROL_HEIGHT={50}      # �ؼ��߶�
CONTROL_DEFAULT_VALUE={0,1,2}  # Ĭ��ֵ�б�
```

#### 4.2 �ؼ��������

**����ؼ� (SLIDER)**
```ini
CONTROL_TYPE={SLIDER}
SLIDER_MIN={0}           # ��Сֵ
SLIDER_MAX={1023}        # ���ֵ
```

**�༭��ؼ� (EDIT)**
```ini
CONTROL_TYPE={EDIT}
CONTROL_DATA_TYPE={0}    # 0:��ֵ, 1:�ַ���
```

**��ѡ��ؼ� (CHECK)**
```ini
CONTROL_TYPE={CHECK}
CONTROL_DATA_TYPE={1}    # ͨ��Ϊ�ַ�������
```

**��ť�ؼ� (BUTTON)**
```ini
CONTROL_TYPE={BUTTON}
```

**������ؼ� (COMBO)**
```ini
CONTROL_TYPE={COMBO}
COMBO_LISTx={ѡ��1,ѡ��2}  # x��ʾ�������ڿؼ��б��е�λ��
```

**�ļ��ؼ� (FILE)**
```ini
CONTROL_TYPE={FILE}
FILE_EXT={bin,txt,ini}   # �ļ���չ���б�
```

**ͼ��ؼ� (CHART)**
```ini
CONTROL_TYPE={CHART}
CHART_SAMPLE_RATE=200    # ˢ���ʣ���λms
CHART_SERIES_NAME0={ϵ��1,ϵ��2}  # ����ϵ������
CHART_SERIES_COLOR0={0x0000FF,0xFF0000}  # ����ϵ����ɫ
```

**��̬�ı��ؼ� (STATIC)**
```ini
CONTROL_TYPE={STATIC}
STATIC_CHART={1}         # ��̬ͼ���� (1:��, 0:�ر�)
```

### 5. ʵ������ʾ��

#### 5.1 ECUӲ��������λ������ʾ��
```ini
[WINDOW]
MAX_ENABLE=1
FONT_SIZE=70
FONT_COLOR=0x000000
FONT_NAME=΢���ź�

[REMINDER]
FIXED_POS={820,150}
WIDTH=380
HEIGHT=60
FONT_NAME={����}
FONT_SIZE={180}

[GROUP]
GROUP={LCD,UART,GROUP_SWITCH,GROUP_PERIOD,GROUP_FILTER,GROUP_PID,GROUP_CALI,GROUP_MISC,GROUP_ALS,GROUP_PLS,GROUP_BAND,GROUP_TEMP,GROUP_CMD}

[GROUP_SWITCH]
DISP_NAME=���ܿ���
WIDTH=220
NEXT_GROUP_POS=0
MAX_ITEMS_PER_COLUMN=10
CONTROL_NAME_WIDTH={120}
CONTROL_HEIGHT={33}
CONTROL_TYPE={CHECK}
CONTROL_DATA_TYPE={1}
CONTROL_NAME={LOG���,�¶Ȳ���,����ģʽ}

[GROUP_PERIOD]
DISP_NAME=������(10ms)
WIDTH=220
NEXT_GROUP_POS=0
MAX_ITEMS_PER_COLUMN=10
CONTROL_NAME_WIDTH={120}
CONTROL_HEIGHT={33}
CONTROL_TYPE={EDIT}
CONTROL_DATA_TYPE={0}
CONTROL_NAME={ѣĿ�����,���������,�����ʿ���}

[LCD]
HIDDEN=0
WIDTH=800
HEIGHT=400
DEPTH=16
WIDTH_ZOOM=100
HEIGHT_ZOOM=100

[UART]
DISP_NAME={ͨѶ����}
WIDTH=200
HEIGHT=280
NEXT_GROUP_POS=2
HIDDEN=1
SHOW_HEX=1
```


### 6. ���ò���

1. **ѡ��Ӧ�ó���**
   �༭ `platform/win32/nos_simu/CMakeLists.txt` �ļ���
   ```cmake
   set(APP "ECU_HW_TUNNING")  # ����Ҫ�����Ӧ�ó���
   ```

2. **�������޸�ui.ini�ļ�**
   �ڶ�Ӧ��Ӧ��Ŀ¼�´������޸� `ui.ini` �ļ�

3. **�������**
   - �� `[GROUP]` ��������������Ҫ�����
   - Ϊÿ�����������Ӧ�����ö�
   - �����������ʾ�����Ϳؼ�����

4. **���������**
   ```bash
   cd platform/win32/nos_simu
   cmake --build . --config Debug
   ```

### 7. ע������

1. **�������**�����й�������Ϳؼ������������ `[GROUP]` ��������������ʾ
2. **������ʽ**���ַ���ֵ�ô����Ű�Χ����ֱֵ��д����
3. **���鳤��**����������ĳ��ȿ������Ӧ����Ŀ������һ��
4. **λ�ü���**��`NEXT_GROUP_POS` Ӱ������Ĳ�������
5. **��������ƥ��**��ȷ�� `CONTROL_DATA_TYPE` ��ؼ�����ƥ��
6. **��ɫ��ʽ**����ɫֵ����ʹ��ʮ�����Ƹ�ʽ
7. **��������**�������С��Χͨ��Ϊ8-72

ͨ���������÷�ʽ�����������ض���Windowsģ�������û����棬���㲻ͬӦ�ó�������� 