#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <cstdlib>
#include <chesspiece.h>
#include <random>
#include <limits>
#define TT_SIZE (1 << 26)

struct GameState;
enum Color:char{ BlackType, WhiteType };
enum NodeFlag:char{ValidType, UBoundType, LBoundType};

typedef std::pair<GridPoint, GridPoint> PointPair;
Q_DECLARE_METATYPE(PointPair)


struct Transposition{
    uint64_t hash;
    int score;
    unsigned short move;
    NodeFlag flag;
    unsigned char depth;

    Transposition& operator=(const Transposition& other);
    Transposition();
    Transposition(const Transposition& other);
};

class TranspositionTable
{
    Transposition table[TT_SIZE];
    uint64_t zobristKeys[12*64];
    int getIndex(uint64_t hash);
    TranspositionTable();
public:
    Transposition& get(uint64_t hash);
    Transposition& put(Transposition& transposition);
    static TranspositionTable& getTable();
    uint64_t calculateHash(GameState& state);
    uint64_t togglePiece(PieceType type, GridPoint loc);
    uint64_t toggleLeftCastle(Color color);
    uint64_t toggleRightCastle(Color color);
    uint64_t toggleEnPassant(Color color, int col);
    TranspositionTable(TranspositionTable& other) = delete;
    TranspositionTable& operator=(TranspositionTable& other) = delete;
};

#endif // TRANSPOSITIONTABLE_H
