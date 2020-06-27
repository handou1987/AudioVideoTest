#ifndef _WIDGET_VIDEO_BTN_H
#define _WIDGET_VIDEO_BTN_H

#include <QImage>

enum WidgetVideoBtnState
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

    bool Init(const QString& normalImg,const QString& hoverImg,const QString& pressImg);
    void UnInit();

    void SetNormal();
    bool IsNormal();

    void SetHover();
    bool IsHover();

    void SetPress();
    bool IsPress();

    QImage* GetCurrStateImage();

    int GetBtnWidth();
    int GetBtnHeight();

private:
    WidgetVideoBtnState m_emState;
    QImage* m_pImgNormal;
    QImage* m_pImgHover;
    QImage* m_pImgPress;
};

#endif
