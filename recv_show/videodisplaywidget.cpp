#include <QPainter>
#include "videodisplaywidget.h"
#include "videoframedrawthread.h"
#include "videomatqueue.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
using namespace cv;

CVideoDisplayWidget::CVideoDisplayWidget():m_pDrawThread(Q_NULLPTR),
    m_pCurrMat(Q_NULLPTR),
    m_pBtnStart(Q_NULLPTR),
    m_pBtnStop(Q_NULLPTR)
{

}

CVideoDisplayWidget::~CVideoDisplayWidget()
{
    UnInit();
}

bool CVideoDisplayWidget::Init()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:#ffff00;");
    setFixedSize(640,480);

    QString strSkinPath("F:\\test\\AvChat\\capture_send\\res\\");
    m_pBtnStart = new CWidgetVideoBtn();
    if(Q_NULLPTR == m_pBtnStart || false == m_pBtnStart->Init(strSkinPath + QString("btn_start_01.png"),
                                                              strSkinPath + QString("btn_start_02.png"),
                                                              strSkinPath + QString("btn_start_03.png")))
    {
        return false;
    }

    m_pBtnStop = new CWidgetVideoBtn();
    if(Q_NULLPTR == m_pBtnStop || false == m_pBtnStop->Init(strSkinPath + QString("btn_stop_01.png"),
                                                            strSkinPath + QString("btn_stop_02.png"),
                                                            strSkinPath + QString("btn_stop_03.png")))
    {
        return false;
    }

    QRect rc = frameGeometry();
    m_rcBtnStart = QRect(rc.right()-m_pBtnStart->GetCurrStateImage()->width(),
                         rc.top()+10,
                         m_pBtnStart->GetCurrStateImage()->width(),
                         m_pBtnStart->GetCurrStateImage()->height());

    m_rcBtnStop = QRect(m_rcBtnStart.left(),
                        m_rcBtnStart.bottom() + 10,
                        m_pBtnStop->GetCurrStateImage()->width(),
                        m_pBtnStop->GetCurrStateImage()->height());

    m_pDrawThread = new CVideoFrameDrawThread();
    if(false == m_pDrawThread->Init(this))
    {
        return false;
    }

    QObject::connect(this,CVideoDisplayWidget::sigNewMat,this,CVideoDisplayWidget::OnNewMat);
    return true;
}

void CVideoDisplayWidget::UnInit()
{
    if(Q_NULLPTR != m_pDrawThread)
    {
        m_pDrawThread->UnInit();
        delete m_pDrawThread;
        m_pDrawThread = Q_NULLPTR;
    }

    m_pCurrMat = Q_NULLPTR;

    if(Q_NULLPTR != m_pBtnStart)
    {
        delete m_pBtnStart;
        m_pBtnStart = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pBtnStop)
    {
        delete m_pBtnStop;
        m_pBtnStop = Q_NULLPTR;
    }
}

void CVideoDisplayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(Q_NULLPTR != m_pCurrMat)
    {
        QImage img;
        CVideoDisplayWidget::Mat2QImage(m_pCurrMat,img);
        {
            QRect rc = this->frameGeometry();
            QPainter p;
            p.begin(&img);
            p.drawImage(m_rcBtnStart,*(m_pBtnStart->GetCurrStateImage()));
            p.drawImage(m_rcBtnStop,*(m_pBtnStop->GetCurrStateImage()));
            p.end();
        }

        QPainter painter(this);
        painter.drawImage(0,0,img);
    }
}

void  CVideoDisplayWidget::Mat2QImage(Mat* cvImg,QImage& qImg)
{    
    if(cvImg->channels()==3)                             //3 channels color image
    {
        cv::cvtColor(*cvImg,*cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg->data),
                     cvImg->cols, cvImg->rows,
                     cvImg->cols*cvImg->channels(),
                     QImage::Format_RGB888);
    }
    else if(cvImg->channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg->data),
                     cvImg->cols,cvImg->rows,
                     cvImg->cols*cvImg->channels(),
                     QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg->data),
                     cvImg->cols,cvImg->rows,
                     cvImg->cols*cvImg->channels(),
                     QImage::Format_RGB888);
    }
}

void CVideoDisplayWidget::OnNewMat(Mat* frame)
{    
    if(Q_NULLPTR != frame)
    {
        if(Q_NULLPTR != m_pCurrMat)
        {
            CVideoMatQueue::GetInstance()->GiveBackFree(m_pCurrMat);
        }
        m_pCurrMat = frame;
        update();
    }
}

void CVideoDisplayWidget::UpdateNewMat(Mat* pMat)
{
    emit sigNewMat(pMat);
}

void CVideoDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint m_pMouseStart = event->pos();
    if(m_rcBtnStart.contains(m_pMouseStart))
    {
        if(false == m_pBtnStart->IsPress())
        {
            m_pBtnStart->SetPress();
            update();
        }
    }
}

void CVideoDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint m_pMouseStart = event->pos();
    if(m_rcBtnStart.contains(m_pMouseStart))
    {
        if (event->buttons() & Qt::LeftButton)
        {
            if(false == m_pBtnStart->IsPress())
            {
                m_pBtnStart->SetPress();
                update();
            }
        }
        else
        {
            if(false == m_pBtnStart->IsHover())
            {
                m_pBtnStart->SetHover();
                update();
            }
        }
    }
    else
    {
        if(false == m_pBtnStart->IsNormal())
        {
            m_pBtnStart->SetNormal();
            update();
        }
    }
}

void CVideoDisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint m_pMouseStart = event->pos();
    if(m_rcBtnStart.contains(m_pMouseStart))
    {
        if(false == m_pBtnStart->IsHover())
        {
            m_pBtnStart->SetHover();
            update();
        }
    }
    else if(false == m_pBtnStart->IsNormal())
    {
        m_pBtnStart->SetNormal();
        update();
    }
}

void CVideoDisplayWidget::leaveEvent(QEvent *leaveEvent)
{
    if(false == m_pBtnStart->IsNormal())
    {
        m_pBtnStart->SetNormal();
        update();
    }
}
