#include "StartWidget.h"
#include"ui_StartWidget.h"
#include "gamewidget.h"
StartWidget::StartWidget(QWidget *parent): QWidget(parent), ui(new Ui::StartWidgetClass())
{
	ui->setupUi(this);
	ui->lab_play->installEventFilter(this);
}
bool StartWidget::eventFilter(QObject* obj, QEvent* event) {
	if (obj == ui->lab_play && event->type() == QEvent::MouseButtonPress) {
		GameWidget* game = new GameWidget();
		game->show();
		this->close();
		return true;
	}
	return QWidget::eventFilter(obj, event);
}
StartWidget::~StartWidget()
{
	delete ui;
}

