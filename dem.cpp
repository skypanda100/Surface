#include "dem.h"
#include <gdal_priv.h>
#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QDebug>
#include <QVector3D>

static double EPSILON = 0.0001;

Dem::Dem(QString file)
    : mFile(file)
    , mMinData(99999)
    , mMaxData(-99999)
    , mData(nullptr)
{
    readData();
}

Dem::~Dem()
{
    if(mData != nullptr)
    {
        delete mData;
    }
}

void Dem::readData()
{
    GDALAllRegister();
    double geoInfo[6];
    uint count = 0;
    GDALDataset *dataset = static_cast<GDALDataset *>(GDALOpen(mFile.toStdString().c_str(), GA_ReadOnly));
    if(dataset != nullptr)
    {
        int success;
        mNoValue = dataset->GetRasterBand(1)->GetNoDataValue(&success);
        qDebug() << "no data value is" << mNoValue;
        mCol = dataset->GetRasterXSize();
        mRow = dataset->GetRasterYSize();
        count = static_cast<uint>(mCol * mRow);
        qDebug() << mCol << mRow;
        // 获取影像转换参数
        dataset->GetGeoTransform(geoInfo);
        mStartX = geoInfo[0];
        mXPrecision = geoInfo[1];
        mStartY = geoInfo[3];
        mYPrecision = geoInfo[5];
        qDebug() << "top left x" << geoInfo[0];
        qDebug() << "w-e pixel resolution" << geoInfo[1];
        qDebug() << "rotation, 0 if image is \"north up\"" << geoInfo[2];
        qDebug() << "top left y" << geoInfo[3];
        qDebug() << "rotation, 0 if image is \"north up\"" << geoInfo[4];
        qDebug() << "n-s pixel resolution" << geoInfo[5];
        // 获取影像波段数
//        int bandNum = dataset->GetRasterCount();
        mData = new float[count];      // 指向存储数据,一个波段
        // 将影像第一个波段的全部值存入pafscan
        dataset->GetRasterBand(1)->RasterIO(
                    GF_Read,
                    0,
                    0,
                    mCol,
                    mRow,
                    mData,
                    mCol,
                    mRow,
                    GDT_Float32,
                    0,
                    0);
    }
    // 最后一定要关闭影像
    GDALClose(dataset);
}

QSurfaceDataArray *Dem::generateImage(double minLon, double maxLon, double minLat, double maxLat, int span)
{
    int startCol = static_cast<int>((minLon - mStartX) / mXPrecision);
    int endCol = static_cast<int>((maxLon - mStartX) / mXPrecision);
    int startRow = static_cast<int>((maxLat - mStartY) / mYPrecision);
    int endRow = static_cast<int>((minLat - mStartY) / mYPrecision);
    if(startCol < 0 || endCol > mCol || startRow < 0 || endRow > mRow)
    {
        return nullptr;
    }
    int col = (endCol - startCol + 1) / span;
    int row = (endRow - startRow + 1) / span;
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(row);
    for(int i = 0;i < row;i++)
    {
        float lat = static_cast<float>((i * span + startRow) * mYPrecision + mStartY);
        QSurfaceDataRow *surfaceDataRow = new QSurfaceDataRow(col);
        for(int j = 0;j < col;j++)
        {
            float lon = static_cast<float>((j * span + startCol) * mXPrecision + mStartX);
            int index = (i * span + startRow) * mCol + j * span + startCol;
            float data = mData[index];
            if(qAbs(static_cast<double>(data) - mNoValue) > EPSILON)
            {
                if(mData[index] > mMaxData)
                {
                    mMaxData = mData[index];
                }
                if(mData[index] < mMinData)
                {
                    mMinData = mData[index];
                }
                (*surfaceDataRow)[j].setPosition(QVector3D(lon, data, lat));
            }
            else
            {
                (*surfaceDataRow)[j].setPosition(QVector3D(lon, 0, lat));
            }
        }
        *dataArray << surfaceDataRow;
    }
    qDebug() << "minimum value is" << mMinData;
    qDebug() << "maximum value is" << mMaxData;
    return dataArray;
}

QImage Dem::getImage()
{
    return mImage;
}

QList<float> Dem::getTifRange()
{
    QList<float> range;
    range.append(static_cast<float>(mStartX));
    range.append(static_cast<float>(mStartX + mXPrecision * mCol));
    range.append(static_cast<float>(mStartY + mYPrecision * mRow));
    range.append(static_cast<float>(mStartY));
    return range;
}

QList<float> Dem::getImgHeight()
{
    QList<float> height;
    height.append(static_cast<float>(mMinData));
    height.append(static_cast<float>(mMaxData));
    return height;
}
