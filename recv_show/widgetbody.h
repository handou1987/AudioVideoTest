#ifndef _WIDGET_BODY_H
#define _WIDGET_BODY_H

#include <QWidget>
#include <QHBoxLayout>
#include "videodisplaywidget.h"

class CWidgetBody:public QWidget
{
public:
    CWidgetBody();
    ~CWidgetBody();

    bool Init();

private:
    void UnInit();

private:
    QHBoxLayout* m_pLayout;
    CVideoDisplayWidget* m_pVideo;
};

#endif
