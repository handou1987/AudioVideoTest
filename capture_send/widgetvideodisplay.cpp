#include "widgetvideodisplay.h"
#include "videoframedrawthread.h"
#include "videoframedrawqueue.h"

#include <QPainter>
#include <QPaintEvent>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
using namespace cv;

CWidgetVideoDisplay::CWidgetVideoDisplay():m_pDrawThread(Q_NULLPTR),
    m_pDrawMat(Q_NULLPTR),
    m_pStartBtn(Q_NULLPTR),
    m_rcStartBtn(0,0,0,0),
    m_pStopBtn(Q_NULLPTR),
    m_rcStopBtn(0,0,0,0)
{

}

CWidgetVideoDisplay::~CWidgetVideoDisplay()
{
    UnInit();
}

bool CWidgetVideoDisplay::Init()
{
    setMouseTracking(true);
    setStyleSheet("background-color:#ffff00;");
    setFixedSize(640,480);

    QString strSkinPath("F:\\test\\AvChat\\capture_send\\res\\");
    m_pStartBtn = new CWidgetVideoBtn();
    if(Q_NULLPTR == m_pStartBtn || false == m_pStartBtn->Init(strSkinPath + QString("btn_start_01.png"),
                                                              strSkinPath + QString("btn_start_02.png"),
                                                              strSkinPath + QString("btn_start_03.png")))
    {
        return false;
    }

    m_pStopBtn= new CWidgetVideoBtn();
    if(Q_NULLPTR == m_pStopBtn || false == m_pStopBtn->Init(strSkinPath + QString("btn_stop_01.png"),
                                                            strSkinPath + QString("btn_stop_02.png"),
                                                            strSkinPath + QString("btn_stop_03.png")))
    {
        return false;
    }

    QRect rc = frameGeometry();
    m_rcStartBtn = QRect(rc.right()-m_pStartBtn->GetCurrStateImage()->width(),
                         rc.top()+10,
                         m_pStartBtn->GetCurrStateImage()->width(),
                         m_pStartBtn->GetCurrStateImage()->height());

    m_rcStopBtn = QRect(m_rcStartBtn.left(),
                        m_rcStartBtn.bottom() + 10,
                        m_pStopBtn->GetCurrStateImage()->width(),
                        m_pStopBtn->GetCurrStateImage()->height());

    m_pDrawThread = new CVideoFrameDrawThread();
    if(false == m_pDrawThread->Init(this))
    {
        return false;
    }

    QObject::connect(this,CWidgetVideoDisplay::sigNewDrawMat,this,CWidgetVideoDisplay::OnNewDrawMat);
    return true;
}

void CWidgetVideoDisplay::UnInit()
{
    if(Q_NULLPTR != m_pDrawThread)
    {
        m_pDrawThread->UnInit();
        delete m_pDrawThread;
        m_pDrawThread = Q_NULLPTR;
    }

    m_pDrawMat = Q_NULLPTR;

    if(Q_NULLPTR != m_pStartBtn)
    {
        delete m_pStartBtn;
        m_pStartBtn = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pStopBtn)
    {
        delete m_pStopBtn;
        m_pStopBtn = Q_NULLPTR;
    }
}

void CWidgetVideoDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(Q_NULLPTR == m_pDrawMat)
    {
        return ;
    }

    QImage img;
    CWidgetVideoDisplay::Mat2QImage(m_pDrawMat,img);
    {
        QPainter p;
        p.begin(&img);
        p.drawImage(m_rcStartBtn,*(m_pStartBtn->GetCurrStateImage()));
        p.drawImage(m_rcStopBtn,*(m_pStopBtn->GetCurrStateImage()));
        p.end();
    }

    QPainter painter(this);
    painter.drawImage(0,0,img);
}

void  CWidgetVideoDisplay::Mat2QImage(Mat* pMat,QImage& qImg)
{    
    if(pMat->channels()==3)                             //3 channels color image
    {
        cv::cvtColor(*pMat,*pMat,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(pMat->data),
                     pMat->cols, pMat->rows,
                     pMat->cols*pMat->channels(),
                     QImage::Format_RGB888);
    }
    else if(pMat->channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(pMat->data),
                     pMat->cols,pMat->rows,
                     pMat->cols*pMat->channels(),
                     QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(pMat->data),
                     pMat->cols,pMat->rows,
                     pMat->cols*pMat->channels(),
                     QImage::Format_RGB888);
    }
}

void CWidgetVideoDisplay::OnNewDrawMat(Mat* pNewMat)
{    
    if(Q_NULLPTR != pNewMat)
    {
        if(Q_NULLPTR != m_pDrawMat)
        {
            CVideoFrameDrawQueue::GetInstance()->GiveBackFree(m_pDrawMat);
        }
        m_pDrawMat = pNewMat;
    }
    this->update();
}

void CWidgetVideoDisplay::UpdateNewDrawMat(Mat* pNewMat)
{
    emit sigNewDrawMat(pNewMat);
}

void CWidgetVideoDisplay::mousePressEvent(QMouseEvent *event)
{
    QPoint ptMouseStart = event->pos();
    if(m_rcStartBtn.contains(ptMouseStart))
    {
        if(false == m_pStartBtn->IsPress())
        {
            m_pStartBtn->SetPress();
            update();
        }
    }
}

void CWidgetVideoDisplay::mouseMoveEvent(QMouseEvent *event)
{
    QPoint ptMouseStart = event->pos();
    if(m_rcStartBtn.contains(ptMouseStart))
    {
        if (event->buttons() & Qt::LeftButton)
        {
            if(false == m_pStartBtn->IsPress())
            {
                m_pStartBtn->SetPress();
                update();
            }
        }
        else
        {
            if(false == m_pStartBtn->IsHover())
            {
                m_pStartBtn->SetHover();
                update();
            }
        }
    }
    else
    {
        if(false == m_pStartBtn->IsNormal())
        {
            m_pStartBtn->SetNormal();
            update();
        }
    }
}

void CWidgetVideoDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint m_ptMouseStart = event->pos();
    if(m_rcStartBtn.contains(m_ptMouseStart))
    {
        if(false == m_pStartBtn->IsHover())
        {
            m_pStartBtn->SetHover();
            update();
        }
    }
    else if(false == m_pStartBtn->IsNormal())
    {
        m_pStartBtn->SetNormal();
        update();
    }
}

void CWidgetVideoDisplay::leaveEvent(QEvent *leaveEvent)
{
    if(false == m_pStartBtn->IsNormal())
    {
        m_pStartBtn->SetNormal();
        update();
    }
}
