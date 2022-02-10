#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include<QThreadPool>
#include <gamestate.h>
#include "ai.h"
#include <QMessageBox>

class ChessBoard
{
    const QPoint boardOrigin;
    const QSize boardSquare;
    ChessPiece* selected = nullptr;
    std::vector<GridPoint> moves;
    std::vector<QImage> pieceSprites;
    MainWindow& window;

    void drawBlueBox(const int row, const int col);
    void undrawBox(const int row, const int col);
    void drawRedBox(const int row, const int col);
    bool isBlack(const int row, const int col);

public:
    void populateBoard();
    void drawPiece(const ChessPiece* const piece, QPainter& painter);
    void drawAllPieces(QPainter& painter);
    void handleMouseClick(const QPoint location);
    void finishTurn();
    ChessBoard(MainWindow& window) noexcept;

    QImage populatedBoard;
    GameState currentState;
    Difficulty difficulty = normal;
    bool iscpu[2] = {false, false};
};

#endif // CHESSBOARD_H
