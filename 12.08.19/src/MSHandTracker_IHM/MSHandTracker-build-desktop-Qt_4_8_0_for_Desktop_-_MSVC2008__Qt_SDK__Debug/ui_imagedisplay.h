/********************************************************************************
** Form generated from reading UI file 'imagedisplay.ui'
**
** Created: Sun 12. Aug 19:41:41 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEDISPLAY_H
#define UI_IMAGEDISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageDisplay
{
public:

    void setupUi(QWidget *ImageDisplay)
    {
        if (ImageDisplay->objectName().isEmpty())
            ImageDisplay->setObjectName(QString::fromUtf8("ImageDisplay"));
        ImageDisplay->resize(400, 300);

        retranslateUi(ImageDisplay);

        QMetaObject::connectSlotsByName(ImageDisplay);
    } // setupUi

    void retranslateUi(QWidget *ImageDisplay)
    {
        ImageDisplay->setWindowTitle(QApplication::translate("ImageDisplay", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageDisplay: public Ui_ImageDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEDISPLAY_H
