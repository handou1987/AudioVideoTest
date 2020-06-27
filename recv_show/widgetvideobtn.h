#ifndef _WIDGET_VIDEO_BTN_H
#define _WIDGET_VIDEO_BTN_H

#include <QImage>

enum CWidgetVideoBtnState
{
    normal = 0,
    hover,
    press,
};

class CWidgetVideoBtn
{
public:
    CWidgetVideoBtn();
    ~CWidgetVideoBtn();

    void SetNormal();
    bool IsNormal();

    void SetHover();
    bool IsHover();

    void SetPress();
    bool IsPress();

    QImage* GetCurrStateImage();

    int GetBtnWidth();
    int GetBtnHeight();

    bool Init(const QString& normalImg,const QString& hoverImg,const QString& pressImg);
    void UnInit();

private:
    CWidgetVideoBtnState m_emState;
    QImage* m_normalImg;
    QImage* m_hoverImg;
    QImage* m_pressImg;
};

#endif
