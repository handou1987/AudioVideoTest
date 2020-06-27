#include "widgetvideobtn.h"

CWidgetVideoBtn::CWidgetVideoBtn()
{

}

CWidgetVideoBtn::~CWidgetVideoBtn()
{
    UnInit();
}

void CWidgetVideoBtn::SetNormal()
{
    m_emState = CWidgetVideoBtnState::normal;
}

void CWidgetVideoBtn::SetHover()
{
    m_emState = CWidgetVideoBtnState::hover;
}

bool CWidgetVideoBtn::IsNormal()
{
    return CWidgetVideoBtnState::normal == m_emState;
}

bool CWidgetVideoBtn::IsHover()
{
    return CWidgetVideoBtnState::hover == m_emState;
}

void CWidgetVideoBtn::SetPress()
{
    m_emState = CWidgetVideoBtnState::press;
}

bool CWidgetVideoBtn::IsPress()
{
    return CWidgetVideoBtnState::press == m_emState;
}

QImage* CWidgetVideoBtn::GetCurrStateImage()
{
    if(CWidgetVideoBtnState::normal == m_emState )
    {
        return m_normalImg;
    }
    else if(CWidgetVideoBtnState::hover == m_emState )
    {
        return m_hoverImg;
    }
    else if(CWidgetVideoBtnState::press == m_emState )
    {
        return m_pressImg;
    }
}

bool CWidgetVideoBtn::Init(const QString& normalImg,const QString& hoverImg,const QString& pressImg)
{
    m_normalImg = new QImage(normalImg);
    m_hoverImg = new QImage(hoverImg);
    m_pressImg = new QImage(pressImg);
    if(Q_NULLPTR == m_normalImg || Q_NULLPTR == m_hoverImg || Q_NULLPTR == m_pressImg)
    {
        return false;
    }

    SetNormal();
    return true;
}

void CWidgetVideoBtn::UnInit()
{
    if(Q_NULLPTR != m_normalImg)
    {
        delete m_normalImg;
        m_normalImg = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_hoverImg)
    {
        delete m_hoverImg;
        m_hoverImg = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pressImg)
    {
        delete m_pressImg;
        m_pressImg = Q_NULLPTR;
    }
}

int CWidgetVideoBtn::GetBtnWidth()
{
    if(Q_NULLPTR != m_normalImg)
    {
        return m_normalImg->width();
    }
    return 0;
}

int CWidgetVideoBtn::GetBtnHeight()
{
    if(Q_NULLPTR != m_normalImg)
    {
        return m_normalImg->height();
    }
    return 0;
}

