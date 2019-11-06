#ifndef DEM_H
#define DEM_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QSurfaceDataArray>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
using namespace QtDataVisualization;

class Dem
{
public:
    Dem(QString file);
    ~Dem();
    QImage getImage();
    QList<float> getTifRange();
    QList<float> getImgHeight();
    QSurfaceDataArray *generateImage(double minLon, double maxLon, double minLat, double maxLat, int span);

private:
    void readData();
    QString mFile;
    int mRow;
    int mCol;
    double mStartX;
    double mStartY;
    double mXPrecision;
    double mYPrecision;
    double mNoValue;
    float mMinData;
    float mMaxData;
    float *mData;
    QImage mImage;
};

#endif // DEM_H
