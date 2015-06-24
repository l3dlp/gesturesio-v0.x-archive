#include "indicator.h"
#include "ui_indicator.h"

#include <QDialogButtonBox>

Indicator::Indicator(QWidget *parent, QString indicatorName) :
    QWidget(parent),
    ui(new Ui::Indicator)
{
    ui->setupUi(this);

    mHasDevice = false;

    mVLayout = new QVBoxLayout();
    mIndicatorName = new QLabel();
    mIndicatorStatus = new QLabel();

    mIndicatorName->setStyleSheet("QLabel { background-color : white; color : black; }");
    mIndicatorStatus->setStyleSheet("QLabel { background-color : white; color : red; }");

    mIndicatorName->setText(indicatorName);
    mIndicatorStatus->setText("OFF");


    QFont font_status;
    font_status.setPointSize(40);

    QFont font_name;
    font_status.setPointSize(20);

    mIndicatorStatus->setFont(font_status);
    mIndicatorName->setFont(font_name);

    mVLayout->addWidget(mIndicatorName);
    mVLayout->addWidget(mIndicatorStatus);

    this->setLayout(mVLayout);


}

Indicator::~Indicator()
{
    delete mIndicatorName;
    delete mIndicatorStatus;
    delete mVLayout;
    delete ui;
}

void Indicator::changeStatus()
{
    mHasDevice = !mHasDevice;
    if(mHasDevice)
    {
        mIndicatorStatus->setStyleSheet("QLabel { background-color : white; color : green; }");
        mIndicatorStatus->setText("ON");
    }
    else
    {
        mIndicatorStatus->setStyleSheet("QLabel { background-color : white; color : red; }");
        mIndicatorStatus->setText("OFF");
    }
}
