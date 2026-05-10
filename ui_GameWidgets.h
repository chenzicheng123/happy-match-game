/********************************************************************************
** Form generated from reading UI file 'GameWidgets.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWIDGETS_H
#define UI_GAMEWIDGETS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWidgets
{
public:
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_7;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *GameWidgets)
    {
        if (GameWidgets->objectName().isEmpty())
            GameWidgets->setObjectName("GameWidgets");
        GameWidgets->resize(1017, 921);
        GameWidgets->setStyleSheet(QString::fromUtf8("BACKGROUND-COLOR:rgb(255,225,238)"));
        label_3 = new QLabel(GameWidgets);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 120, 151, 221));
        label_3->setPixmap(QPixmap(QString::fromUtf8("photos/Z5.jpg")));
        label_3->setScaledContents(true);
        label_5 = new QLabel(GameWidgets);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(0, 340, 151, 201));
        label_5->setPixmap(QPixmap(QString::fromUtf8("photos/Z9.jpg")));
        label_5->setScaledContents(true);
        label_9 = new QLabel(GameWidgets);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(0, 530, 151, 301));
        label_9->setPixmap(QPixmap(QString::fromUtf8("photos/Z8.jpg")));
        label_9->setScaledContents(true);
        label_7 = new QLabel(GameWidgets);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(870, 110, 151, 191));
        label_7->setPixmap(QPixmap(QString::fromUtf8("photos/Z6.jpg")));
        label_7->setScaledContents(true);
        label_6 = new QLabel(GameWidgets);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(870, 270, 151, 231));
        label_6->setPixmap(QPixmap(QString::fromUtf8("photos/Z9.jpg")));
        label_6->setScaledContents(true);
        label_4 = new QLabel(GameWidgets);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(870, 500, 151, 411));
        label_4->setPixmap(QPixmap(QString::fromUtf8("photos/Z4.jpg")));
        label_4->setScaledContents(true);
        label = new QLabel(GameWidgets);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 830, 1021, 101));
        label->setPixmap(QPixmap(QString::fromUtf8("photos/Z1.jpg")));
        label->setScaledContents(true);
        label_2 = new QLabel(GameWidgets);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 30, 1021, 91));
        label_2->setPixmap(QPixmap(QString::fromUtf8("photos/Z1.jpg")));
        label_2->setScaledContents(true);
        label_3->raise();
        label_5->raise();
        label_9->raise();
        label_6->raise();
        label_4->raise();
        label->raise();
        label_7->raise();
        label_2->raise();

        retranslateUi(GameWidgets);

        QMetaObject::connectSlotsByName(GameWidgets);
    } // setupUi

    void retranslateUi(QWidget *GameWidgets)
    {
        GameWidgets->setWindowTitle(QCoreApplication::translate("GameWidgets", "GameWidgets", nullptr));
        label_3->setText(QString());
        label_5->setText(QString());
        label_9->setText(QString());
        label_7->setText(QString());
        label_6->setText(QString());
        label_4->setText(QString());
        label->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GameWidgets: public Ui_GameWidgets {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWIDGETS_H
