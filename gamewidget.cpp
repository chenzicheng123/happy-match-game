#include "gamewidget.h"


GameWidget::GameWidget(QWidget* parent) : QWidget(parent), score(0), steps(30), firstSelected(-1, -1) {
        this->setFixedSize(COLS * CELL_SIZE, ROWS * CELL_SIZE + 40);
        this->setWindowTitle("开心消消乐");
        playbgm("D:\\Users\\chenz\\source\\repos\\Qt_project\\QTcodes\\photos\\beijing.wav");
        firstSelected=QPoint{ -1,-1 };
        highlightcell = QPoint{ -1,-1 };
        loadicons();
        initMap();
        QTimer::singleShot(500, this, [=]() {
            QTimer::singleShot(300, this, [=]() {dropDown();
            QTimer::singleShot(300, this, &GameWidget::fillEmpty); });
            });
    }

    void GameWidget:: paintEvent(QPaintEvent*) {
        QPainter p(this);
        p.fillRect(this->rect(), QColor(255, 200, 220));
        p.setPen(Qt::black);
        p.setFont(QFont("Microsoft YaHei", 18));
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
        if (highlightcell.x() != -1) {
            int hx = highlightcell.x() * CELL_SIZE;
            int hy = highlightcell.y() * CELL_SIZE + 40;
            p.setPen(QPen(Qt::red, 4));
            p.drawRect(hx, hy, CELL_SIZE, CELL_SIZE);
       }

    } 
   

    void GameWidget::playbgm(QString path) {

        PlaySound((LPCWSTR)path.utf16(), NULL, SND_ASYNC | SND_LOOP);
    }
   
    // 已修复Qt 6的鼠标事件API
  
    void GameWidget::mousePressEvent(QMouseEvent* e)
    {
       
        if (steps <= 0) return;

        int col = e->position().x() / CELL_SIZE;
        int row = (e->position().y() - 40) / CELL_SIZE;
        QPoint click = { col, row };

        // 越界 → 清空所有
        if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        {
            return;
        }

        // ===================== 【1. 高亮逻辑：只控制红框，和交换无关】 =====================
        // 逻辑：点自己=切换亮/灭；点别人=点亮新的
        if (firstSelected.x() == -1)
        {
            // 没有选中：直接点亮当前
            highlightcell = click;
            firstSelected = click;
        }
        else
        {
            // 已经有选中
            if (firstSelected == click)
            {
                // 点同一个：取消高亮+取消选中
                firstSelected = { -1, -1 };
                highlightcell = { -1, -1 };
            }
            else if (isAdjacent(firstSelected.x(), firstSelected.y(), col, row))
            {
                // ===================== 【2. 相邻格子：执行交换！！】 =====================
                swap(firstSelected.x(), firstSelected.y(), col, row);
                if (checkAndClear()) {
                    /*相邻能换，消除*/
                    steps--;
                    QTimer::singleShot(300, this, [=]() {
                        dropDown();
                        QTimer::singleShot(300, this, &GameWidget::fillEmpty);
                        });
                    firstSelected = { -1,-1 };
                    highlightcell = { -1,-1 };
                }
                else {
                    /*相邻不能换回来，红框切换*/
                    swap(firstSelected.x(), firstSelected.y(), col, row);
                    firstSelected = click;
                    highlightcell = click;

                }
            }
            else {
                /*不相邻，切换红框*/
                firstSelected = click;
                highlightcell = click;
            }
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
        checkAndClear();
        /*while (checkAndClear()) {
            dropDown();
            fillEmpty();
        }*/
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
        update();
        if (checkAndClear())
        {
            update(); // 标记消除后，再刷新一次
            QTimer::singleShot(300, this, [=]() {
                dropDown();
                QTimer::singleShot(300, this, &GameWidget::fillEmpty);
                });
        }
       
        if (steps <= 0 && !isgameover)
        {
            // 立刻锁死，防止重复触发弹窗，只弹1次
            isgameover = true;

            // 【关键】用定时器！不卡界面！不破坏消除动画！！
            QTimer::singleShot(0, this, [=]() {
                // 弹出窗口（中文小写ok，只点1次）
                QMessageBox::information(this, "Game Over", "游戏结束！");

                // 关闭弹窗后 打开猫咪视频
                QString videoPath = "./photos/maomi.kiss.mp4";
                QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));

                // 最后执行游戏重置，和弹窗完全分离，绝对不会影响动画
                score = 0;
                steps = 30;
                initMap();
                dropDown();
                fillEmpty();
                update();

                // 解锁
                isgameover = false;
                });

            return;
        }
    }

        
    
        
   
    
    /* taskkill / f / im QTcodes.exe */