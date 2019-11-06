#ifndef GRAPH_H
#define GRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QMessageBox>
#include <QtCore/QPropertyAnimation>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
using namespace QtDataVisualization;

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(Q3DSurface *surface);
    ~Graph();

public slots:
    void onOkClicked(int height, float minLon, float maxLon, float minLat, float maxLat, int precision, QString path, bool isSmooth, bool isGrid);

private:
    void handleElementSelected(QAbstract3DGraph::ElementType type);
    void resetSelection();
    void toggleSeeThrough(bool seethrough);
    void toggleShadows(bool shadows);

private:
    QLabel *mTextField;
    Q3DSurface *mSurface;
    QPropertyAnimation *mSelectionAnimation;
    QCustom3DItem *mPreviouslyAnimatedItem;
    QVector3D mPreviousScaling;
};
#endif // GRAPH_H
