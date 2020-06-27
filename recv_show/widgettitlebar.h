#ifndef _WIDGET_TITLE_BAR_H
#define _WIDGET_TITLE_BAR_H

#include <QWidget>
#include <QMouseEvent>

class CWidgetTitleBar:public QWidget
{
public:
    CWidgetTitleBar(QWidget* pParent);
    ~CWidgetTitleBar();

    bool Init();
    void UnInit();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QWidget* m_pParent;
    bool        m_bDrag;
    QPoint      m_pMouseStart;
    QPoint      m_ptWindowTopLeft;
};

#endif
