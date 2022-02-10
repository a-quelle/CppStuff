#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <memory>
#include <chesspiece.h>
#include <transpositiontable.h>

struct StateFlags{
    int enPassantCol: 5;
    bool whiteLeftCastle : 1;
    bool whiteRightCastle : 1;
    bool blackLeftCastle : 1;
    bool blackRightCastle : 1;
    int pieceTaken : 8;
};

struct GameState
{
    uint64_t hash;
    std::vector<std::pair<PointPair, StateFlags>> history;
    std::vector<std::unique_ptr<ChessPiece>> pieces;
    Color turn = WhiteType;
    StateFlags flags;

    GameState& operator=(const GameState& state);
    GameState& operator=(GameState&& state) noexcept;
    GameState(const GameState& state);
    GameState(GameState&& state) noexcept ;
    GameState();
    ChessPiece* pieceAtLocation(const int row, const int col) const;
    std::vector<PointPair> getMoves(const Color color) const;
    bool kingCheckOf(const Color color) const;
    bool locationCheckBy(const int row, const int col, const Color color) const;
    void changeTurn();
    GameState& make(const PointPair& move);
    GameState& undo();
private:
    bool kingAdjacent(const int row, const int col, const Color color) const;
    bool checkByExceptFromKing(const int row, const int col, const Color color) const;
};

#endif // GAMESTATE_H
