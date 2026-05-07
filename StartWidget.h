#pragma once
#include<QEvent>
#include <QWidget>
#include "ui_StartWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWidgetClass; };
QT_END_NAMESPACE

class StartWidget : public QWidget
{
	Q_OBJECT

public:
	StartWidget(QWidget *parent = nullptr);
	~StartWidget();
protected:
	bool eventFilter(QObject* obj, QEvent* event)override;
private:
	Ui::StartWidgetClass *ui;
};

