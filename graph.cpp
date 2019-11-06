#include "graph.h"
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include <QImage>
#include <QApplication>
#include "dem.h"

Graph::Graph(Q3DSurface *surface)
    : mSurface(surface)
    , mPreviouslyAnimatedItem(nullptr)
{
    mSelectionAnimation = new QPropertyAnimation(this);
    mSelectionAnimation->setPropertyName("scaling");
    mSelectionAnimation->setDuration(500);
    mSelectionAnimation->setLoopCount(-1);

    toggleShadows(false);
}

Graph::~Graph()
{
    delete mSurface;
}

void Graph::onOkClicked(int height, float minLon, float maxLon, float minLat, float maxLat, int precision, QString path, bool isSmooth, bool isGrid)
{
    if(mSurface->seriesList().size() > 0)
    {
        QSurface3DSeries *series = mSurface->seriesList().at(0);
        mSurface->removeSeries(series);
    }

    Dem* dem = new Dem(QApplication::applicationDirPath() + "/map.tif");
    QList<float> range;
//    range << 103.9 << 106.0 << 29.5 << 31.1;
    range << minLon << maxLon << minLat << maxLat;
    QSurfaceDataArray *surfaceDataArray = dem->generateImage(static_cast<double>(range[0]),
            static_cast<double>(range[1]),
            static_cast<double>(range[2]),
            static_cast<double>(range[3]),
            precision);
    if(surfaceDataArray == nullptr)
    {
        QList<float> range = dem->getTifRange();
        QString message = QString::asprintf(
                    "经纬度范围错误，地形数据经度范围%.4f~%.4f，纬度范围%.4f~%.4f",
                    static_cast<double>(range[0]),
                    static_cast<double>(range[1]),
                    static_cast<double>(range[2]),
                    static_cast<double>(range[3]));
        QMessageBox::warning(nullptr, "警告", message);
        return;
    }
    QList<float> heightRange = dem->getImgHeight();
    QSurface3DSeries *series = new QSurface3DSeries;
    series->dataProxy()->resetArray(surfaceDataArray);
    series->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));
//    series->dataProxy()->setValueRanges(range[0], range[1], range[2], range[3]);
    series->setDrawMode(isGrid ? QSurface3DSeries::DrawWireframe : QSurface3DSeries::DrawSurface);
    series->setFlatShadingEnabled(isSmooth ? false : true);
    series->setTextureFile(path);

    mSurface->axisX()->setLabelFormat("%.1f E");
    mSurface->axisZ()->setLabelFormat("%.1f N");
    mSurface->axisX()->setRange(range[0], range[1]);
    mSurface->axisY()->setRange(heightRange[0], height);
    mSurface->axisZ()->setRange(range[2], range[3]);
    mSurface->axisX()->setTitle(QStringLiteral("Longitude"));
    mSurface->axisY()->setTitle(QStringLiteral("Height"));
    mSurface->axisZ()->setTitle(QStringLiteral("Latitude"));
    mSurface->addSeries(series);

    QLinearGradient gradient;
    gradient.setColorAt(0.0, Qt::darkGreen);
    gradient.setColorAt(0.1, Qt::darkYellow);
    gradient.setColorAt(0.3, Qt::darkGray);
    gradient.setColorAt(1.0, Qt::white);
    mSurface->seriesList().at(0)->setBaseGradient(gradient);
    mSurface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    mSurface->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

//    connect(mSurface, &QAbstract3DGraph::selectedElementChanged,
//            this, &Graph::handleElementSelected);
}

void Graph::toggleSeeThrough(bool seethrough)
{
    if (seethrough) {
        mSurface->seriesList().at(0)->setDrawMode(QSurface3DSeries::DrawWireframe);
    } else {
        mSurface->seriesList().at(0)->setDrawMode(QSurface3DSeries::DrawSurface);
    }
}

void Graph::toggleShadows(bool shadows)
{
    if (shadows)
        mSurface->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
    else
        mSurface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
}

void Graph::handleElementSelected(QAbstract3DGraph::ElementType type)
{
    resetSelection();
    if (type == QAbstract3DGraph::ElementCustomItem) {
        QCustom3DItem *item = mSurface->selectedCustomItem();
        QString text;
        if (qobject_cast<QCustom3DLabel *>(item) != nullptr) {
            text.append("Custom label: ");
        } else {
            QStringList split = item->meshFile().split("/");
            text.append(split.last());
            text.append(": ");
        }
        int index = mSurface->selectedCustomItemIndex();
        text.append(QString::number(index));
        mTextField->setText(text);
        mPreviouslyAnimatedItem = item;
        mPreviousScaling = item->scaling();
        mSelectionAnimation->setTargetObject(item);
        mSelectionAnimation->setStartValue(item->scaling());
        mSelectionAnimation->setEndValue(item->scaling() * 1.5f);
        mSelectionAnimation->start();
    } else if (type == QAbstract3DGraph::ElementSeries) {
        QString text = "Surface (";
        QSurface3DSeries *series = mSurface->selectedSeries();
        if (series) {
            QPoint point = series->selectedPoint();
            QString posStr;
            posStr.setNum(point.x());
            text.append(posStr);
            text.append(", ");
            posStr.setNum(point.y());
            text.append(posStr);
        }
        text.append(")");
        mTextField->setText(text);
    } else if (type > QAbstract3DGraph::ElementSeries
               && type < QAbstract3DGraph::ElementCustomItem) {
        int index = mSurface->selectedLabelIndex();
        QString text;
        if (type == QAbstract3DGraph::ElementAxisXLabel)
            text.append("Axis X label: ");
        else if (type == QAbstract3DGraph::ElementAxisYLabel)
            text.append("Axis Y label: ");
        else
            text.append("Axis Z label: ");
        text.append(QString::number(index));
        mTextField->setText(text);
    } else {
        mTextField->setText("Nothing");
    }
}

void Graph::resetSelection()
{
    mSelectionAnimation->stop();
    if(mPreviouslyAnimatedItem)
    {
        mPreviouslyAnimatedItem->setScaling(mPreviousScaling);
    }
    mPreviouslyAnimatedItem = nullptr;
}
