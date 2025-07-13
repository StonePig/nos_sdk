#pragma once
#include <afxwin.h>

class CSkinEdit :
    public CEdit
{
    DECLARE_DYNAMIC(CSkinEdit);

public:
    CSkinEdit();
    ~CSkinEdit();

    void DrawBorder();

private:
    bool m_bTracking;
    bool m_bHover;
    bool m_bFocus;

    HBRUSH m_normalBrush;
    HBRUSH m_hangOnBrush;
    HBRUSH m_focusBrush;
    HBRUSH m_ClientBrush;
    HBRUSH m_hOrderBrush;
    HBRUSH m_HotOrderBrush1;
    HBRUSH m_HotOrderBrush2;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
    afx_msg void OnNcPaint();
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

};