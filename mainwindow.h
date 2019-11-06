#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inputwidget.h"
#include "graph.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initUI();
    void initConnect();

private:
    InputWidget *mInputWidget;
    Graph *mGraph;
};

#endif // MAINWINDOW_H
