#ifndef _WIDGET_BODY_H
#define _WIDGET_BODY_H

#include <QWidget>
#include <QHBoxLayout>
#include "widgetvideodisplay.h"

class CWidgetBody:public QWidget
{
public:
    CWidgetBody();
    ~CWidgetBody();

    bool Init();
    void UnInit();

private:
    QHBoxLayout* m_pLayout;
    CWidgetVideoDisplay* m_pVideoWidget;
};

#endif
