#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_mainLayOut(Q_NULLPTR)
    ,m_mainWidget(Q_NULLPTR)
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

    m_pTitleBar = new CWidgetTitleBar(this);
    if(false == m_pTitleBar->Init())
    {
        return;
    }

    m_pBody = new CWidgetBody();
    if(false == m_pBody->Init())
    {
        return;
    }

    m_mainLayOut = new QVBoxLayout();
    m_mainLayOut->setContentsMargins(0,0,0,0);
    m_mainLayOut->setSpacing(10);
    m_mainLayOut->addWidget(m_pTitleBar);
    m_mainLayOut->addWidget(m_pBody);

    m_mainWidget = new QWidget();
    m_mainWidget->setFixedSize(800,600);
    m_mainWidget->setLayout(m_mainLayOut);
    setCentralWidget(m_mainWidget);
}

void MainWindow::UnInit()
{
    if(Q_NULLPTR != m_mainLayOut)
    {
        delete m_mainLayOut;
        m_mainLayOut = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_mainWidget)
    {
        delete m_mainWidget;
        m_mainWidget = Q_NULLPTR;
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
}
