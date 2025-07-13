#pragma once
#include <afxwin.h>

class CSkinButton : public CButton
{
    DECLARE_DYNAMIC(CSkinButton);

public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};