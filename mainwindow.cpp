#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
    initConnect();

}

MainWindow::~MainWindow()
{
    delete mInputWidget;
    delete mGraph;
}

void MainWindow::initUI()
{
    mInputWidget = new InputWidget;

    Q3DSurface *surface = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(surface);
    if (!surface->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }
    container->setMinimumSize(QSize(800, 600));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
    mGraph = new Graph(surface);

    QWidget *widget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mInputWidget);
    mainLayout->addWidget(container, 1);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
}

void MainWindow::initConnect()
{
    connect(mInputWidget,
                     SIGNAL(okClicked(int, float, float, float, float, int, QString, bool, bool)),
                     mGraph,
                     SLOT(onOkClicked(int, float, float, float, float, int, QString, bool, bool)));

}
