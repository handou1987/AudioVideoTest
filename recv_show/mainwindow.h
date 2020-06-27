#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

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

private:
    void Init();
    void UnInit();

private:
    Ui::MainWindow *ui;
    QVBoxLayout* m_mainLayOut;
    QWidget* m_mainWidget;
    CWidgetTitleBar* m_pTitleBar;
    CWidgetBody* m_pBody;
};
#endif // MAINWINDOW_H
