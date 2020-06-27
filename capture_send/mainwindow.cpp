#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_pShadowEffect(Q_NULLPTR)
    ,m_pMainLayOut(Q_NULLPTR)
    ,m_pMainWidget(Q_NULLPTR)
    ,m_pTitleBar(Q_NULLPTR)
    ,m_pBody(Q_NULLPTR)
{       
    Init();
}

MainWindow::~MainWindow()
{
    UnInit();
    delete ui;
}

void MainWindow::Init()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_pShadowEffect = new QGraphicsDropShadowEffect();
    m_pMainLayOut = new QVBoxLayout();
    m_pMainWidget = new QWidget();
    m_pTitleBar = new CWidgetTitleBar(this);
    m_pBody = new CWidgetBody();
    if(Q_NULLPTR == m_pShadowEffect || \
            Q_NULLPTR == m_pMainLayOut || Q_NULLPTR == m_pMainWidget || \
            Q_NULLPTR == m_pTitleBar || Q_NULLPTR == m_pBody
            )
    {
        return;
    }

    if(false == m_pTitleBar->Init() || false == m_pBody->Init())
    {
        return;
    }

    m_pShadowEffect->setOffset(0, 0);
    m_pShadowEffect->setColor(QColor(0x63,0x63,0x63));
    m_pShadowEffect->setBlurRadius(20);

    m_pMainLayOut->setContentsMargins(20,20,20,20);
    m_pMainLayOut->setSpacing(0);
    m_pMainLayOut->addWidget(m_pTitleBar);
    m_pMainLayOut->addWidget(m_pBody);

    m_pMainWidget->setGraphicsEffect(m_pShadowEffect);
    m_pMainWidget->setLayout(m_pMainLayOut);
    m_pMainWidget->setFixedSize(800,600);
    setCentralWidget(m_pMainWidget);
}

void MainWindow::UnInit()
{
    if(Q_NULLPTR != m_pMainLayOut)
    {
        delete m_pMainLayOut;
        m_pMainLayOut = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pMainWidget)
    {
        delete m_pMainWidget;
        m_pMainWidget = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pTitleBar)
    {
        delete m_pTitleBar;
        m_pTitleBar = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pBody)
    {
        delete m_pBody;
        m_pBody = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pShadowEffect)
    {
        delete m_pShadowEffect;
        m_pShadowEffect = Q_NULLPTR;
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    exit(0);
}

