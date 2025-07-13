// nos_simuDlg.h: 头文件
//

#pragma once

#define CONFIG_FILE _T(".\\ui.ini")

#define WM_CUSTOM_MESSAGE_UART_BUF_UPDATE (WM_USER + 1)

#define CONTROL_START_ID 9000

#define GROUP_MAX_NUM 20
#define UART_MAX_NUM    20
#define CAN_MAX_NUM    5

// CnossimuDlg 对话框
class CnossimuDlg : public CDialogEx
{
// 构造
public:
	CnossimuDlg(CWnd* pParent = nullptr);	// 标准构造函数

	UINT display_rect_id = 0;
	UINT reminder_id = 0;
	BOOL reminder_dialog_mode = FALSE; // 是否以对话框模式显示reminder
	UINT input_group_id = 0;
	UINT output_group_id = 0;
	UINT adc_group_id = 0;
	UINT pwm_group_id = 0;
	UINT key_group_id = 0;
	UINT encoder_group_id = 0;
	UINT system_group_id[GROUP_MAX_NUM] = {0};
	UINT group_num = 0;
	UINT uart_group_id[UART_MAX_NUM] = {0};
	UINT can_group_id[CAN_MAX_NUM] = {0};
	INT total_uart_num = 0;
	INT total_can_num = 0;
	BOOL is_uart_opend[UART_MAX_NUM] = { 0 };
	BOOL is_can_opend[CAN_MAX_NUM] = { 0 };
	HANDLE uart_handle[UART_MAX_NUM] = { NULL };
	CString uart_str[UART_MAX_NUM];
	CString uart_add_str[UART_MAX_NUM];
	uint16_t cedit_line_num[UART_MAX_NUM] = { 0 };
	DWORD uart_baud[UART_MAX_NUM] = {9600};
	BOOL is_exit = 0;
	BOOL is_real_save = 0;
	BOOL is_uart_data_hex = 0;
	BOOL is_lcd_hide = 0;
	BOOL is_led_hide = 0;
	BOOL is_uart_hide = 0;
	BOOL is_log_enable = 0;
	uint16_t chart_sample_rate = 1; 
	// 定义互斥锁
	HANDLE g_mutex;

	ULONG mIndex;
	BOOL m_open;	//I2C 设备是否插入	
	
	int scale;
	float scale_x = 1.0;
	float scale_y = 1.0;
	CRect init_rect;
	// CRect cur_rect;
	CRect *pRect;
	BOOL is_dialog_init_done = FALSE;
	
	// LCD
	int width_zoom = 100;
	int height_zoom = 100;
	int width = 0;
	int height = 0;
	int depth = 16; // 默认深度为16位
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOS_SIMU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CButton input_group;
	CButton output_group;
	CButton adc_group;
	CButton encoder_group;
	CButton pwm_group;
	CButton system_group;
	CButton key_group;
	CButton uart_group[UART_MAX_NUM];
	CButton can_group[CAN_MAX_NUM];

	BOOL is_combine_key_simu;

	CRect update_rect_calc;
	CRect update_lcd_rect_calc;
	CRect update_chart_rect_calc;
	BOOL led_update_flag = 0;
	BOOL disp_update_flag = 0;
	BOOL chart_update_flag = 0;
	//led screen buf
	unsigned char *disp_buf = NULL;
	//lcd screen buf
	unsigned char *disp_lcd_buf = NULL;
	uint16_t disp_x;
	uint16_t disp_y;
	uint16_t disp_w;
	uint16_t disp_h;
	uint8_t disp_depth;

	int LED_DIGIT_WIDTH;
	int LED_DIGIT_HEIGHT;
	int LED_RECT_WIDTH;
	int LED_RECT_HEIGHT;
	int LED_DIGIT_COLOR;
	int LED_RECT_COLOR;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL EnumSerialPort();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg LRESULT OnCustomMessageUartBufUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CnossimuDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonClick(UINT uID);
	afx_msg void OnSelComChange(UINT uID);
	afx_msg void OnEnChangeEdit(UINT uID);
#if I2C_NUM
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
#endif
	void UpdateScreen(unsigned char *buf);
	void UpdateLcdScreen(uint8_t *buf, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth);
	void UpdateChartScreen(void);
	void UpdateChartData(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	afx_msg void ClearUpdateRectCalcArea(CDC* pDC, const CRect& update_rect_calc, COLORREF hPenBar_CLR);
	afx_msg BOOL CnossimuDlg::GetResControlId(UINT uID, uint16_t *group_id, uint16_t *control_id);
};
