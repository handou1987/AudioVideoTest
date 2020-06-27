#ifndef _VIDEO_DISPLAY_WIDGET_H
#define _VIDEO_DISPLAY_WIDGET_H

#include <QImage>
#include <QWidget>
#include <QPaintEvent>

#include "opencv2/core/mat.hpp"
using namespace cv;

#include "widgetvideobtn.h"

class CVideoFrameDrawThread;

class CVideoDisplayWidget:public QWidget
{
    Q_OBJECT
public:
    CVideoDisplayWidget();
    ~CVideoDisplayWidget();

    static void  Mat2QImage(Mat* cvImg,QImage& qImg);

    bool Init();
    void UnInit();
    void UpdateNewMat(Mat* pMat);

signals:
    void sigNewMat(Mat* pMat);

public slots:
    void OnNewMat(Mat* pMat);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *leaveEvent);

private:
    CVideoFrameDrawThread* m_pDrawThread;
    Mat* m_pCurrMat;
    CWidgetVideoBtn* m_pBtnStart;
    QRect  m_rcBtnStart;
    CWidgetVideoBtn* m_pBtnStop;
    QRect m_rcBtnStop;
};

#endif
