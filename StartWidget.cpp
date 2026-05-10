
#include "StartWidget.h"
#include <QImage>
#include <QPixmap>
#include <QColor>
#include "ui_StartWidget.h"
#include "GameWidgets.h"
#include <windows.h>
#pragma comment(lib,"winmm.lib")



StartWidget::StartWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StartWidgetClass)
{
    ui->setupUi(this);
    this->setFixedSize(WIN_W, WIN_H);
    ui->label_1->installEventFilter(this);
    ui->label_2->installEventFilter(this);

    QPixmap titlePix = ui->label->pixmap();
    QImage titleImg = titlePix.toImage();
    for (int x = 0; x < titleImg.width(); x++)
    {
        for (int y = 0; y < titleImg.height(); y++)
        {
            QColor c = titleImg.pixelColor(x, y);
            int r = qMin(c.red() + 12, 255);
            int g = qMin(c.green() + 12, 255);
            int b = qMin(c.blue() + 12, 255);
            titleImg.setPixelColor(x, y, QColor(r, g, b, c.alpha()));
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(titleImg));

    QPixmap g1 = ui->label_1->pixmap();
    QImage i1 = g1.toImage();
    for (int x = 0; x < i1.width(); x++)
    {
        for (int y = 0; y < i1.height(); y++)
        {
            QColor c = i1.pixelColor(x, y);
            int r = qMin(c.red() + 15, 255);
            int g = qMin(c.green() + 15, 255);
            int b = qMin(c.blue() + 15, 255);
            i1.setPixelColor(x, y, QColor(r, g, b, c.alpha()));
        }
    }
    ui->label_1->setPixmap(QPixmap::fromImage(i1));

    QPixmap g2 = ui->label_2->pixmap();
    QImage i2 = g2.toImage();
    for (int x = 0; x < i2.width(); x++)
    {
        for (int y = 0; y < i2.height(); y++)
        {
            QColor c = i2.pixelColor(x, y);
            int r = qMin(c.red() + 15, 255);
            int g = qMin(c.green() + 15, 255);
            int b = qMin(c.blue() + 15, 255);
            i2.setPixelColor(x, y, QColor(r, g, b, c.alpha()));
        }
    }
    ui->label_2->setPixmap(QPixmap::fromImage(i2));

    PlaySound(L"photos/beijing.wav", NULL, SND_ASYNC | SND_LOOP);
}

bool StartWidget::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::MouseButtonPress) {
		if (obj == ui->label_1) {
			GameWidgets* game = new GameWidgets();
			game->iswujinmoshi = false;
			game->dangqianguanqia = 1;
			game->mubiaofenshu = 1500;
			game->steps = 30;
			game->score = 0;
			game->show();
			this->close();
			return true;
		}
		else if (obj == ui->label_2) {
			GameWidgets* game = new GameWidgets();
			game->iswujinmoshi = true;
			game->dangqianguanqia = 1;
			game->mubiaofenshu = 99999;
			game->steps = 999;
			game->score = 0;
			game->show();
			this->close();
			return true;

		}
		}
	return QWidget::eventFilter(obj, event);
}
StartWidget::~StartWidget()
{
	delete ui;
}

