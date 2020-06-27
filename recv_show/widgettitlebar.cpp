#include "widgettitlebar.h"

CWidgetTitleBar::CWidgetTitleBar(QWidget* pParent):m_pParent(pParent),
    m_bDrag(false),
    m_pMouseStart(0,0),
    m_ptWindowTopLeft(0,0)
{
}

CWidgetTitleBar::~CWidgetTitleBar()
{
    UnInit();
}

void CWidgetTitleBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && Q_NULLPTR != m_pParent)
    {
        m_bDrag = true;
        m_pMouseStart = event->globalPos();
        m_ptWindowTopLeft = m_pParent->frameGeometry().topLeft();
    }
}

void CWidgetTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag && Q_NULLPTR != m_pParent)
    {
        QPoint distance = event->globalPos() - m_pMouseStart;
        m_pParent->move(m_ptWindowTopLeft + distance);
    }
}

void CWidgetTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

bool CWidgetTitleBar::Init()
{
    setFixedHeight(30);
    setStyleSheet("background-color:#ff0000;");
    return true;
}

void CWidgetTitleBar::UnInit()
{
    m_pParent = Q_NULLPTR;
    m_bDrag = false;
}





