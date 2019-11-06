#include "inputwidget.h"

InputWidget::InputWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    initConnect();
}

InputWidget::~InputWidget()
{
    delete mHeightEdit;
    delete mMinLonEdit;
    delete mMaxLonEdit;
    delete mMinLatEdit;
    delete mMaxLatEdit;
    delete mPrecisionEdit;
    delete mFilePathEdit;
    delete mSearchButton;
    delete mSmoothCheckBox;
    delete mGridCheckBox;
    delete mOkButton;
}

void InputWidget::initUI()
{
    QLabel *heightLabel = new QLabel("高度");
    mHeightEdit = new QLineEdit;
    mHeightEdit->setValidator(new QIntValidator);
    mHeightEdit->setText("10000");
    mHeightEdit->setFixedWidth(50);

    QLabel *rangeLabel = new QLabel("范围");
    mMinLonEdit = new QLineEdit;
    mMinLonEdit->setValidator(new QDoubleValidator);
    mMinLonEdit->setPlaceholderText("W");
    mMinLonEdit->setFixedWidth(50);
    mMaxLonEdit = new QLineEdit;
    mMaxLonEdit->setValidator(new QDoubleValidator);
    mMaxLonEdit->setPlaceholderText("E");
    mMaxLonEdit->setFixedWidth(50);
    mMinLatEdit = new QLineEdit;
    mMinLatEdit->setValidator(new QDoubleValidator);
    mMinLatEdit->setPlaceholderText("S");
    mMinLatEdit->setFixedWidth(50);
    mMaxLatEdit = new QLineEdit;
    mMaxLatEdit->setValidator(new QDoubleValidator);
    mMaxLatEdit->setPlaceholderText("N");
    mMaxLatEdit->setFixedWidth(50);

    QLabel *precisionLabel = new QLabel("精度");
    mPrecisionEdit = new QLineEdit;
    mPrecisionEdit->setValidator(new QIntValidator);
    mPrecisionEdit->setFixedWidth(30);
    mPrecisionEdit->setText("1");

    QLabel *layerLabel = new QLabel("图层");
    mFilePathEdit = new QLineEdit;
    mFilePathEdit->setMinimumWidth(400);
    mFilePathEdit->setPlaceholderText("路径");
    mSearchButton = new QPushButton;
    mSearchButton->setIcon(QIcon(":/icon/search.png"));
    mSearchButton->setToolTip("查找");

    mSmoothCheckBox = new QCheckBox("平滑");
    mGridCheckBox = new QCheckBox("网格");

    mOkButton = new QPushButton;
    mOkButton->setText("显示");

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(heightLabel);
    mainLayout->addWidget(mHeightEdit);
    mainLayout->addWidget(rangeLabel);
    mainLayout->addWidget(mMinLonEdit);
    mainLayout->addWidget(mMaxLonEdit);
    mainLayout->addWidget(mMinLatEdit);
    mainLayout->addWidget(mMaxLatEdit);
    mainLayout->addWidget(precisionLabel);
    mainLayout->addWidget(mPrecisionEdit);
    mainLayout->addWidget(layerLabel);
    mainLayout->addWidget(mFilePathEdit);
    mainLayout->addWidget(mSearchButton);
    mainLayout->addWidget(mSmoothCheckBox);
    mainLayout->addWidget(mGridCheckBox);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(mOkButton);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void InputWidget::initConnect()
{
    connect(mSearchButton, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(mOkButton, SIGNAL(clicked()), this, SLOT(onOkClicked()));
}

void InputWidget::onSearchClicked()
{
    QSettings setting(QApplication::applicationDirPath() + "/setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("layer").toString();
    QString currentPath = mFilePathEdit->text().isEmpty() ? lastPath : mFilePathEdit->text();
    QString path = QFileDialog::getOpenFileName(this, "图层", currentPath);
    if(!path.isEmpty())
    {
        setting.setValue("layer", path);
        mFilePathEdit->setText(path);
    }
}

void InputWidget::onOkClicked()
{
    if(mHeightEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "高度不能为空");
        return;
    }
    if(mMinLonEdit->text().isEmpty()
            || mMaxLonEdit->text().isEmpty()
            || mMinLatEdit->text().isEmpty()
            || mMaxLatEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "范围不能为空");
        return;
    }
    if(mPrecisionEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "精度不能为空");
        return;
    }
    int height = mHeightEdit->text().toInt();
    float minLon = mMinLonEdit->text().toFloat();
    float maxLon = mMaxLonEdit->text().toFloat();
    float minLat = mMinLatEdit->text().toFloat();
    float maxLat = mMaxLatEdit->text().toFloat();
    int precision = mPrecisionEdit->text().toInt();
    if(height <= 0)
    {
        QMessageBox::warning(this, "警告", "高度必须大于0");
        return;
    }
    if(minLon >= maxLon)
    {
        QMessageBox::warning(this, "警告", "经度范围错误");
        return;
    }
    if(minLat >= maxLat)
    {
        QMessageBox::warning(this, "警告", "纬度范围错误");
        return;
    }
    if(precision <= 0)
    {
        QMessageBox::warning(this, "警告", "精度必须大于0");
        return;
    }

    QString path = mFilePathEdit->text().trimmed();
    bool isSmooth = mSmoothCheckBox->isChecked();
    bool isGrid = mGridCheckBox->isChecked();

    emit okClicked(height, minLon, maxLon, minLat, maxLat, precision, path, isSmooth, isGrid);
}
