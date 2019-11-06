#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QtWidgets>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

signals:
    void okClicked(int height, float minLon, float maxLon, float minLat, float maxLat, int precision, QString path, bool isSmooth, bool isGrid);

private:
    void initUI();
    void initConnect();

private slots:
    void onSearchClicked();
    void onOkClicked();

private:
    QLineEdit *mHeightEdit;
    QLineEdit *mMinLonEdit;
    QLineEdit *mMaxLonEdit;
    QLineEdit *mMinLatEdit;
    QLineEdit *mMaxLatEdit;
    QLineEdit *mPrecisionEdit;
    QLineEdit *mFilePathEdit;
    QPushButton *mSearchButton;
    QCheckBox *mSmoothCheckBox;
    QCheckBox *mGridCheckBox;
    QPushButton *mOkButton;
};

#endif // INPUTWIDGET_H
