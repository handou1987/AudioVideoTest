#include "widgetbody.h"
#include "videodisplaywidget.h"

CWidgetBody::CWidgetBody():m_pLayout(Q_NULLPTR),
    m_pVideo(Q_NULLPTR)
{

}

CWidgetBody::~CWidgetBody()
{
    UnInit();
}

bool CWidgetBody::Init()
{
    setStyleSheet("background-color:#636363;");

    m_pVideo = new CVideoDisplayWidget();
    if(Q_NULLPTR == m_pVideo || false == m_pVideo->Init())
    {
        return false;
    }

    m_pLayout = new QHBoxLayout();
    if(Q_NULLPTR == m_pLayout)
    {
        return false;
    }

    m_pLayout->addWidget(m_pVideo);
    setLayout(m_pLayout);

    return true;
}

void CWidgetBody::UnInit()
{
    if(Q_NULLPTR != m_pVideo)
    {
        delete m_pVideo;
        m_pLayout = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pLayout)
    {
        delete m_pLayout;
        m_pLayout = Q_NULLPTR;
    }
}
