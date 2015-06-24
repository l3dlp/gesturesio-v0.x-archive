/********************************************************************************
** Form generated from reading UI file 'indicator.ui'
**
** Created: Sun 12. Aug 19:41:41 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDICATOR_H
#define UI_INDICATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Indicator
{
public:

    void setupUi(QWidget *Indicator)
    {
        if (Indicator->objectName().isEmpty())
            Indicator->setObjectName(QString::fromUtf8("Indicator"));
        Indicator->resize(400, 300);

        retranslateUi(Indicator);

        QMetaObject::connectSlotsByName(Indicator);
    } // setupUi

    void retranslateUi(QWidget *Indicator)
    {
        Indicator->setWindowTitle(QApplication::translate("Indicator", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Indicator: public Ui_Indicator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDICATOR_H
