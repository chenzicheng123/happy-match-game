#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QTimer>
#include <QMessageBox>
#include<QPixmap>
#include<QLabel>
#include <QDesktopServices>
#include<QUrl>
#pragma comment(lib,"winmm.lib")
#include<windows.h>
#define ROWS 8
#define COLS 8
#define CELL_SIZE 90
#define ICON_COUNT 7

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget* parent = nullptr);
    bool iswujinmoshi;
    int mubiaofenshu;
    int dangqianguanqia;
    int score;
    int steps;

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
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