#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

#include "widgettitlebar.h"
#include "widgetbody.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent* e);

private:
    void Init();
    void UnInit();

private:
    Ui::MainWindow *ui;

    QGraphicsDropShadowEffect* m_pShadowEffect;
    QVBoxLayout* m_pMainLayOut;
    QWidget* m_pMainWidget;

    CWidgetTitleBar* m_pTitleBar;
    CWidgetBody* m_pBody;
};
#endif // MAINWINDOW_H
