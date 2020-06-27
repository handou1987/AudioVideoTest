#include "widgetbody.h"

CWidgetBody::CWidgetBody():m_pLayout(Q_NULLPTR),
    m_pVideoWidget(Q_NULLPTR)
{
}

CWidgetBody::~CWidgetBody()
{
    UnInit();
}

bool CWidgetBody::Init()
{
    setStyleSheet("background-color:#ffffff;");

    m_pVideoWidget = new CWidgetVideoDisplay();
    if(Q_NULLPTR == m_pVideoWidget || false == m_pVideoWidget->Init())
    {
        return false;
    }

    m_pLayout = new QHBoxLayout();
    if(Q_NULLPTR == m_pLayout)
    {
        return false;
    }

    m_pLayout->addWidget(m_pVideoWidget);
    setLayout(m_pLayout);
    return true;
}

void CWidgetBody::UnInit()
{
    if(Q_NULLPTR != m_pLayout)
    {
        delete m_pLayout;
        m_pLayout = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pVideoWidget)
    {
        delete m_pVideoWidget;
        m_pLayout = Q_NULLPTR;
    }
}
