#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTcodes.h"

class QTcodes : public QMainWindow
{
    Q_OBJECT

public:
    QTcodes(QWidget *parent = nullptr);
    ~QTcodes();

private:
    Ui::QTcodesClass ui;
};

