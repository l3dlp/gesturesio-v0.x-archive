#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QLabel>
#include <QPixmap>

namespace Ui
{
    class ImageDisplay;
}

class ImageDisplay : public QLabel
{
    Q_OBJECT
    
public:
    explicit ImageDisplay(QWidget *parent = 0, QString filename = "");
    ~ImageDisplay();
    
private:
    Ui::ImageDisplay *ui;
    QImage      *mDisplayQt;        // 4x8U
};

#endif // IMAGEDISPLAY_H
