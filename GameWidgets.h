#pragma once
#ifndef GAMEWIDGETS_H
#define GAMEWIDGETS_H
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QTimer>
#include <QMessageBox>
#include<QPixmap>
#include<QLabel>
#include <QDesktopServices>
#include <QWidget>
#include "ui_GameWidgets.h"
#include<QUrl>
#pragma comment(lib,"winmm.lib")
#include<windows.h>
#define ROWS 8
#define COLS 8
#define CELL_SIZE 90
#define ICON_COUNT 7
#define GAME_W 720
#define GAME_H 720
#define WIN_W 1020
#define WIN_H 910
#define TITLE 40
namespace Ui {
    class GameWidgets;
}
class GameWidgets : public QWidget {
    Q_OBJECT
public:
    explicit GameWidgets(QWidget* parent = nullptr);
    bool iswujinmoshi;
    int mubiaofenshu;
    int dangqianguanqia;
    int score;
    int steps;

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
    Ui::GameWidgets* ui;
    bool isgameover = false;

    QPoint highlightcell;
    QVector<QVector<int>> map;

    QPoint firstSelected;

    QVector<QPixmap>icons;
    void loadicons();

    void initMap();

    void playbgm(QString path);
    void dadaofenshu();

    void nextlevel();

    void playmaomi();

    bool isAdjacent(int x1, int y1, int x2, int y2);

    void swap(int x1, int y1, int x2, int y2);

    bool checkAndClear();

    void dropDown();

    void fillEmpty();

    void zhadan(int x, int y);
};



#endif
