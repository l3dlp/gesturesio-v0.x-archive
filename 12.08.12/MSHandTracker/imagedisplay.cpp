#include "imagedisplay.h"
#include "ui_imagedisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent, QString filename) :
    QLabel(parent),
    ui(new Ui::ImageDisplay)
{
    ui->setupUi(this);
    mDisplayQt = new QImage(filename);

    if(mDisplayQt)
    {
        this->setPixmap(QPixmap::fromImage(*mDisplayQt));
    }
    else
    {
        QString text = "the image file " + filename + " either doesn't exist or is not an image file.";
        this->setText(text);
    }

}

ImageDisplay::~ImageDisplay()
{


    if(mDisplayQt)
    {
        delete mDisplayQt;
    }

    delete ui;
}
