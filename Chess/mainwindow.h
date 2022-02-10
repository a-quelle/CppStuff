#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QScrollArea>
#include <QLabel>
#include <memory>
#include <chessboard.h>
#include <clickableqlabel.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessBoard chessBoard;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void paintChessboard();
    std::unique_ptr<QScrollArea> scrollArea;
    std::unique_ptr<ClickableQLabel> imageLabel;
    std::unique_ptr<QLabel> whiteLabel;
    std::unique_ptr<QLabel> blackLabel;
    std::unique_ptr<QLabel> turnLabel;
    std::unique_ptr<QPlainTextEdit> logView;
    void createMenus();
    void updateColorLabels();
    void updateTurnLabel();
    void logNewMove();

    QAction* whiteCpuAct;
    QAction* blackCpuAct;
    QActionGroup* difficultyGroup;
    QAction* easyAct;
    QAction* normalAct;
    QAction* hardAct;
    QAction* newGameAct;
    QAction* undoAct;

public slots:
    void mouseClickSlot(QMouseEvent* ev);
    void about();
    void whiteCpu();
    void blackCpu();
    void setEasy();
    void setNormal();
    void setHard();
    void aiCallback(const PointPair move);
    void newGame();
    void undo();
};

#endif // MAINWINDOW_H
