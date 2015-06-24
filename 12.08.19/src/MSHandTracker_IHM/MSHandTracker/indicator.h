#ifndef INDICATOR_H
#define INDICATOR_H

#include <QtGui>

namespace Ui {
class Indicator;
}

class Indicator : public QWidget
{
    Q_OBJECT
    
public:
    explicit Indicator(QWidget *parent = 0, QString indicatorName = "Unknown indicator name");
    ~Indicator();
    
private:
    Ui::Indicator *ui;
    QLabel *mIndicatorName;
    QLabel *mIndicatorStatus;
    QVBoxLayout *mVLayout;
    bool mHasDevice;

public slots:
    void changeStatus();
};

#endif // INDICATOR_H
