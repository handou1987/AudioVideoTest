#include "widgetvideobtn.h"

CWidgetVideoBtn::CWidgetVideoBtn()
{

}

CWidgetVideoBtn::~CWidgetVideoBtn()
{
    UnInit();
}

bool CWidgetVideoBtn::Init(const QString& normalImg,const QString& hoverImg,const QString& pressImg)
{
    m_pImgNormal = new QImage(normalImg);
    m_pImgHover = new QImage(hoverImg);
    m_pImgPress = new QImage(pressImg);

    if(Q_NULLPTR == m_pImgNormal || Q_NULLPTR == m_pImgHover || Q_NULLPTR == m_pImgPress)
    {
        return false;
    }

    SetNormal();
    return true;
}

void CWidgetVideoBtn::UnInit()
{
    if(Q_NULLPTR != m_pImgNormal)
    {
        delete m_pImgNormal;
        m_pImgNormal = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pImgHover)
    {
        delete m_pImgHover;
        m_pImgHover = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pImgPress)
    {
        delete m_pImgPress;
        m_pImgPress = Q_NULLPTR;
    }
}

void CWidgetVideoBtn::SetNormal()
{
    m_emState = WidgetVideoBtnState::normal;
}

void CWidgetVideoBtn::SetHover()
{
    m_emState = WidgetVideoBtnState::hover;
}

bool CWidgetVideoBtn::IsNormal()
{
    return WidgetVideoBtnState::normal == m_emState;
}

bool CWidgetVideoBtn::IsHover()
{
    return WidgetVideoBtnState::hover == m_emState;
}

void CWidgetVideoBtn::SetPress()
{
    m_emState = WidgetVideoBtnState::press;
}

bool CWidgetVideoBtn::IsPress()
{
    return WidgetVideoBtnState::press == m_emState;
}

QImage* CWidgetVideoBtn::GetCurrStateImage()
{
    if(WidgetVideoBtnState::normal == m_emState )
    {
        return m_pImgNormal;
    }
    else if(WidgetVideoBtnState::hover == m_emState )
    {
        return m_pImgHover;
    }
    else if(WidgetVideoBtnState::press == m_emState )
    {
        return m_pImgPress;
    }
}

int CWidgetVideoBtn::GetBtnWidth()
{
    if(Q_NULLPTR != m_pImgNormal)
    {
        return m_pImgNormal->width();
    }
    return 0;
}

int CWidgetVideoBtn::GetBtnHeight()
{
    if(Q_NULLPTR != m_pImgNormal)
    {
        return m_pImgNormal->height();
    }
    return 0;
}

