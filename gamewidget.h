#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QTimer>
#include <QMessageBox>
#include<QPixmap>

#define ROWS 8
#define COLS 8
#define CELL_SIZE 90
#define ICON_COUNT 6

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
    QVector<QVector<int>> map;
    QPoint firstSelected;
    int score;
    int steps;
    QVector<QPixmap>icons;
    void loadicons();
    void initMap();

    bool isAdjacent(int x1, int y1, int x2, int y2);

    void swap(int x1, int y1, int x2, int y2);

    bool checkAndClear();

    void dropDown();

    void fillEmpty();
};
#endif