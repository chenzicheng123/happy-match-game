#include "gamewidget.h"

GameWidget::GameWidget(QWidget* parent) : QWidget(parent), score(0), steps(30), firstSelected(-1, -1) {
        setFixedSize(COLS * CELL_SIZE, ROWS * CELL_SIZE + 40);
        setWindowTitle("开心消消乐");
        loadicons();
        initMap();
        this->setFixedSize(COLS * CELL_SIZE, ROWS * CELL_SIZE + 40);
        this->setWindowTitle("开心消消乐");
    }

    void GameWidget:: paintEvent(QPaintEvent*) {
        QPainter p(this);
        p.setPen(Qt::black);
        p.setFont(QFont("Microsoft YaHei", 12));
        p.drawText(10, 25, QString("Steps Left: %1   Score: %2").arg(QString::number(steps)).arg(QString::number(score)));
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int x = j * CELL_SIZE;
                int y = i * CELL_SIZE + 40;
                p.drawRect(x, y, CELL_SIZE, CELL_SIZE);
                int idx = map[i][j];
                if (idx >= 1 && idx <= ICON_COUNT && !icons[idx].isNull()) {
                    p.drawPixmap(x + 2, y + 2, icons[idx]);
                }
                else {
                    p.drawText(x + 20, y + 35, QString::number(idx));
                }
            }
        }
    }

    // 已修复Qt 6的鼠标事件API
    void GameWidget::mousePressEvent(QMouseEvent* e) {
        if (steps <= 0) return;
        int col = e->position().x() / CELL_SIZE;
        int row = (e->position().y() - 40) / CELL_SIZE;

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;

        if (firstSelected.x() == -1) {
            firstSelected = { col, row };
        }
        else {
            int x1 = firstSelected.x();
            int y1 = firstSelected.y();
            if (isAdjacent(x1, y1, col, row)) {
                swap(x1, y1, col, row);
                if (!checkAndClear()) {
                    swap(x1, y1, col, row);
                }
                else {
                    steps--;
                    update();
                    QTimer::singleShot(300, this, &GameWidget::dropDown);
                    QTimer::singleShot(600, this, &GameWidget::fillEmpty);
                }
            }
            firstSelected = { -1, -1 };
        }
        update();
    }
    void GameWidget::loadicons() {
        icons.resize(ICON_COUNT + 1);
        for (int i = 1; i <= ICON_COUNT; i++) {
            QString path = QString("photos/%1.jpg").arg(i);
            QPixmap pix(path);
            if (!pix.isNull())
            {
                icons[i] = pix.scaled(CELL_SIZE - 4, CELL_SIZE - 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            else {
                icons[i] = QPixmap();
            }
        }
    }
    void GameWidget::initMap() {
        map.resize(ROWS, QVector<int>(COLS));
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                map[i][j] = rand() % ICON_COUNT + 1;
            }
        }
        while (checkAndClear()) {
            dropDown();
            fillEmpty();
        }
    }

    bool GameWidget:: isAdjacent(int x1, int y1, int x2, int y2) {
        int dx = abs(x1 - x2);
        int dy = abs(y1 - y2);
        return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
    }

    void GameWidget::swap(int x1, int y1, int x2, int y2) {
        int t = map[y1][x1];
        map[y1][x1] = map[y2][x2];
        map[y2][x2] = t;
    }

    bool GameWidget:: checkAndClear() {
        bool cleared = false;
        QVector<QVector<bool>> toClear(ROWS, QVector<bool>(COLS, false));

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS - 2; j++) {
                int v = map[i][j];
                if (v && v == map[i][j + 1] && v == map[i][j + 2]) {
                    toClear[i][j] = toClear[i][j + 1] = toClear[i][j + 2] = true;
                    cleared = true;
                }
            }
        }

        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS - 2; i++) {
                int v = map[i][j];
                if (v && v == map[i + 1][j] && v == map[i + 2][j]) {
                    toClear[i][j] = toClear[i + 1][j] = toClear[i + 2][j] = true;
                    cleared = true;
                }
            }
        }

        if (cleared) {
            for (int i = 0; i < ROWS; i++)
                for (int j = 0; j < COLS; j++)
                    if (toClear[i][j]) { map[i][j] = 0; score += 10; }
        }
        return cleared;
    }

    void GameWidget::dropDown() {
        for (int j = 0; j < COLS; j++) {
            int pos = ROWS - 1;
            for (int i = ROWS - 1; i >= 0; i--) {
                if (map[i][j] != 0) {
                    if (pos != i) {
                        map[pos][j] = map[i][j];
                        map[i][j] = 0;
                    }
                    pos--;
                }
            }
        }
        update();
    }
    void GameWidget::fillEmpty()
    {
        // ===================== 关键1：一进函数先检查游戏是否结束 =====================
        // 只要步数用完了，立刻弹窗+重置+return，不跑后面任何代码！
        if (steps <= 0)
        {
            // 用延迟弹窗，让函数先退出，主线程不被阻塞，弹窗再弹出来
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::information(this, "Game Over", "Final Score: " + QString::number(score));
                initMap();
                score = 0;
                steps = 30;
                });
            return; // 直接退出，后面的循环、填充代码一行都不跑！
        }

        // ===================== 你原来的填充格子代码，完全不动 =====================
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (map[i][j] == 0)
                {
                    map[i][j] = rand() % 6 + 1;
                }
            }
        }

        // ===================== 关键2：消除循环加三重保险，绝对不会无限循环 =====================
        bool hasCleared;
        int maxLoopCount = 20; // 最多连锁消除20次，超过就强制停止，防止死循环
        do
        {
            hasCleared = checkAndClear();
            if (hasCleared)
            {
                dropDown();
            }
            maxLoopCount--; // 每次循环次数减1
        } while (
            hasCleared       // 条件1：还有可消除的匹配
            && steps > 0     // 条件2：步数还没用完
            && maxLoopCount > 0 // 条件3：没超过最大连锁次数
            );

        // ===================== 关键3：循环结束后再检查一次，防止漏掉 =====================
        if (steps <= 0)
        {
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::information(this, "Game Over", "Final Score: " + QString::number(score));
                initMap();
                score = 0;
                steps = 30;
                });
            return;
        }

        // ===================== 只刷新一次界面 =====================
        update();
    }
    
    /* taskkill / f / im QTcodes.exe */