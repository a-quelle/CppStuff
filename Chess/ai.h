#ifndef AI_H
#define AI_H

#include "gamestate.h"
#include <QRunnable>

enum Difficulty {easy, normal, hard};

class MainWindow;

class AI: public QObject, public QRunnable{
    Q_OBJECT

    Difficulty difficulty;
    GameState state;

    std::pair<int,std::unique_ptr<GameState>> minMax(int depth, int a = INT_MIN+1, int b = INT_MAX-1) noexcept;
    PointPair generateMove() noexcept;

public:
    AI(MainWindow& window);
    ~AI() override;
    void run() override;

signals:
    void workDone(const PointPair move);
};

template <int n, int m>
struct Matrix{
    int data[n*m];
    int& operator[] (const int x) noexcept {
        return data[x];
    }
    int& operator() (const int x, const int y) noexcept {
        return data[x*m+y];
    }
    Matrix(int (&&arr)[n*m]) noexcept {
        for(size_t i = 0; i < n*m; ++i)
            data[i] = arr[i];
    }
};

#endif // AI_H
