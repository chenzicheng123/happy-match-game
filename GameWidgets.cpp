#include "GameWidgets.h"

#include"ui_GameWidgets.h"

GameWidgets::GameWidgets(QWidget* parent) : QWidget(parent), ui(new Ui::GameWidgets), score(0), steps(0), firstSelected(-1, -1),
iswujinmoshi(false), mubiaofenshu(0), dangqianguanqia(1) {
   
    this->setFixedSize(WIN_W, WIN_H);

    this->setWindowTitle("开心消消乐");
    playbgm("D:\\Users\\chenz\\source\\repos\\Qt_project\\QTcodes\\photos\\beijing.wav");
    firstSelected = QPoint{ -1,-1 };
    highlightcell = QPoint{ -1,-1 };
    loadicons();
    initMap(); 
    ui->setupUi(this);
    QTimer::singleShot(200, this, [=]() {
        QTimer::singleShot(100, this, [=]() {dropDown();
        QTimer::singleShot(100, this, &GameWidgets::fillEmpty); });
        });
}

void GameWidgets::playbgm(QString path) {

    PlaySound((LPCWSTR)path.utf16(), NULL, SND_ASYNC | SND_LOOP);
}

/*绘制游戏界面，绘制选中的红框*/
void GameWidgets::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::black);
    p.setFont(QFont("Microsoft YaHei", 15));
    if (!iswujinmoshi) {
        p.drawText(10, 25, QString("                                          当前关卡: %1  剩余步数: %2  目标分数: %3   当前分数: %4").arg(QString::number(dangqianguanqia)).arg(QString::number(steps)).arg(QString::number(mubiaofenshu)).arg(QString::number(score)));
    }
    else {
        p.drawText(10, 25, QString("                                                                   无尽模式   当前分数: %1").arg(score));

    }
    const int X = (WIN_W - GAME_W) / 2;
    const int Y = TITLE + ((WIN_H - TITLE) - GAME_H) / 2;
   
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int x = j * CELL_SIZE + X;
            int y = i * CELL_SIZE + Y;
            p.drawRect(x, y, CELL_SIZE, CELL_SIZE);
            int idx = map[i][j];
            if (idx == 8) {
                p.drawPixmap(x + 2, y + 2, icons[8]);
            }
            else if (idx >= 1 && idx <= ICON_COUNT && !icons[idx].isNull()) {
                p.drawPixmap(x + 2, y + 2, icons[idx]);
            }
            else {
                p.drawText(x + 20, y + 35, QString::number(idx));
            }
        }
    }
    if (highlightcell.x() != -1) {
        int hx = highlightcell.x() * CELL_SIZE + X;
        int hy = highlightcell.y() * CELL_SIZE + Y;
        p.setPen(QPen(Qt::red, 4));
        p.drawRect(hx, hy, CELL_SIZE, CELL_SIZE);
    }
}

/*识别点击位置，响应炸弹的点击，触发炸弹爆炸逻辑
并记录选中状态，实现选中红框的切换*/
void GameWidgets::mousePressEvent(QMouseEvent* e)
{
    const int X = (WIN_W - GAME_W) / 2;
    const int Y = TITLE + ((WIN_H - TITLE) - GAME_H) / 2;


    int col = (e->position().x() - X) / CELL_SIZE;
    int row = (e->position().y() - Y) / CELL_SIZE;
    QPoint click = { col, row };

    // 越界 → 清空所有
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    {
        return;
    }
    if (map[row][col] == 8) {
        zhadan(row, col);
        steps--;
        map[row][col] = 0;
        firstSelected = QPoint{ -1,-1 };
        highlightcell = QPoint{ -1,-1 };
        update();
        return;
    }

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
            swap(firstSelected.x(), firstSelected.y(), col, row);
            if (checkAndClear()) {
                /*相邻能换，消除*/
                steps--;


                if (steps <= 0)dadaofenshu();
                QTimer::singleShot(300, this, [=]() {
                    dropDown();
                    QTimer::singleShot(300, this, [=]() {
                        fillEmpty();

                        QTimer::singleShot(300, this, [=]() {
                            if (checkAndClear()) {
                                dropDown();
                                fillEmpty();
                            }
                            });
                        });
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

/*判断是否两个格子相邻*/
bool GameWidgets::isAdjacent(int x1, int y1, int x2, int y2) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
}

/*相邻的才可以进行交换*/
void GameWidgets::swap(int x1, int y1, int x2, int y2) {
    int t = map[y1][x1];
    map[y1][x1] = map[y2][x2];
    map[y2][x2] = t;
}

/*加载图标*/
void GameWidgets::loadicons() {
    icons.resize(10);
    for (int i = 1; i <= 7; i++) {
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
    QPixmap bomb("photos/8.jpg");
    icons[8] = bomb.scaled(CELL_SIZE - 4, CELL_SIZE - 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

/*生成地图*/
void GameWidgets::initMap() {
    map.resize(ROWS, QVector<int>(COLS));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            map[i][j] = rand() % ICON_COUNT + 1;
        }
    }
    checkAndClear();

}

/*实现消除效果*/
bool GameWidgets::checkAndClear() {
    bool cleared = false;
    QVector<QVector<bool>>toClear(ROWS, QVector<bool>(COLS, false));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 2; j++) {
            int v = map[i][j];
            if (v == 0 || v == 8) continue;
            if (v && v == map[i][j + 1] && v == map[i][j + 2]) {
                if (j + 3 < COLS && v == map[i][j + 3]) {
                    map[i][j + 1] = 8;
                    toClear[i][j] = toClear[i][j + 2] = toClear[i][j + 3] = true;
                }
                else {
                    toClear[i][j] = toClear[i][j + 1] = toClear[i][j + 2] = true;
                }
                cleared = true;
            }
        }
    }

    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i < ROWS - 2; i++) {
            int v = map[i][j];
            if (v == 0 || v == 8) continue;
            if (v && v == map[i + 1][j] && v == map[i + 2][j]) {
                if (i + 3 < ROWS && v == map[i + 3][j]) {
                    map[i + 1][j] = 8;
                    toClear[i][j] = toClear[i + 2][j] = toClear[i + 3][j] = true;
                }
                else {
                    toClear[i][j] = toClear[i + 1][j] = toClear[i + 2][j] = true;
                }
                cleared = true;
            }
        }
    }

    if (cleared) {

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (toClear[i][j])
                {
                    if (map[i][j] == 8) {
                        zhadan(i, j);
                    }
                    map[i][j] = 0;
                    if (!isgameover)score += 10;
                }
            }
        }
    }
    return cleared;
}

void GameWidgets::zhadan(int x, int y)
{
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS) return;

    //标记要炸的格子，触发粉色空白
    QVector<QVector<bool>> bombClear(ROWS, QVector<bool>(COLS, false));
    // 标记本行本列
    for (int j = 0; j < COLS; j++) bombClear[x][j] = true;
    for (int i = 0; i < ROWS; i++) bombClear[i][y] = true;
    // 连锁引爆其他炸弹
    for (int j = 0; j < COLS; j++)
    {
        if (map[x][j] == 8 && j != y)
        {
            for (int i = 0; i < ROWS; i++) bombClear[i][j] = true;
        }
    }
    for (int i = 0; i < ROWS; i++)
    {
        if (map[i][y] == 8 && i != x)
        {
            for (int j = 0; j < COLS; j++) bombClear[i][j] = true;
        }
    }

    update(); // 这里立刻画出整行整列粉色
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (bombClear[i][j])
            {
                map[i][j] = 0;
                if (!isgameover) score += 10;
            }
        }
    }
    // 先等200ms下落 → 再等200ms填图标，和初始加载、普通消除的延迟完全一致
    QTimer::singleShot(200, this, [=]() {
        dropDown();
        QTimer::singleShot(200, this, [=]() {
            fillEmpty();
            update();
            dadaofenshu();
            });
        });
}

void GameWidgets::nextlevel()
{
    isgameover = true;

    score = 0;

    // 1. 关卡 +1
    dangqianguanqia++;

    // 2. 自动计算当前关 步数、目标分（无限关通用，不用手动写）
    steps = 30 + (dangqianguanqia - 1) * 5;
    mubiaofenshu = 1500 + (dangqianguanqia - 1) * 500;



    // 3. 重置选中、高亮、地图
    firstSelected = { -1, -1 };
    highlightcell = { -1, -1 };
    initMap();
    dropDown();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
        {
            if (map[i][j] == 0) {
                map[i][j] = rand() % 6 + 1;
            }
        }
    }
    update();

    isgameover = false;
}

/*判定胜利条件*/
void GameWidgets::dadaofenshu() {
    if (iswujinmoshi) return;
    bool fenshudabiao = (score >= mubiaofenshu);
    bool stepsyongwan = (steps <= 0);

    if (!isgameover && fenshudabiao && stepsyongwan) {
        isgameover = true;
        score = 0;
        playmaomi();
    }

    else if (stepsyongwan && !fenshudabiao && !isgameover) {
        isgameover = true;
        QTimer::singleShot(0, this, [=]() {
            int ret = QMessageBox::question(this, "Level Failed!",
                "Do you want to try again?",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes);
            if (ret == QMessageBox::Yes) {
                dangqianguanqia = 1;
                mubiaofenshu = 1500;
                steps = 30;;
                score = 0;
                firstSelected = QPoint{ -1,-1 };
                highlightcell = QPoint{ -1,-1 };
                initMap();
                dropDown();
                fillEmpty();
                update();
            }
            else {
                this->close();
            }
            isgameover = false;
            });

    }
}


void GameWidgets::playmaomi()
{
    isgameover = true;
    // 弹窗：选择 下一关 / 关闭（放猫咪视频）
    int ret = QMessageBox::question(this, "Level Clear",
        "恭喜通关！\n是否进入下一关？",
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        // 选下一关：正常执行下一关逻辑
        nextlevel();
    }

    else
    {
        // 选关闭：播放猫咪视频，然后关闭窗口
        QString videoPath = "./photos/maomi.kiss.mp4";
        QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
        this->close();
    }
}


void GameWidgets::dropDown()
{
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



void GameWidgets::fillEmpty()
{

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
            QTimer::singleShot(300, this, &GameWidgets::fillEmpty);
            });
    }

    return;
}







/* taskkill / f / im QTcodes.exe */

