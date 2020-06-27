#ifndef _WIDGET_VIDEO_DISPLAY_H
#define _WIDGET_VIDEO_DISPLAY_H

#include <QWidget>
#include <QImage>

#include "opencv2/core/mat.hpp"
using namespace cv;

#include "widgetvideobtn.h"

class CVideoFrameDrawThread;

class CWidgetVideoDisplay:public QWidget
{
    Q_OBJECT

public:
    CWidgetVideoDisplay();
    ~CWidgetVideoDisplay();

    bool Init();
    void UnInit();

    static void  Mat2QImage(Mat* pMat,QImage& qImg);
    void UpdateNewDrawMat(Mat* pNewMat);

signals:
    void sigNewDrawMat(Mat* pNewMat);

public slots:
    void OnNewDrawMat(Mat* pNewMat);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *leaveEvent);

private:
    CVideoFrameDrawThread* m_pDrawThread;
    Mat* m_pDrawMat;

    CWidgetVideoBtn* m_pStartBtn;
    QRect  m_rcStartBtn;

    CWidgetVideoBtn* m_pStopBtn;
    QRect m_rcStopBtn;
};
#endif
