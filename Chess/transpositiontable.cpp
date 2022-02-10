#include "transpositiontable.h"
#include "gamestate.h"

#define WHITE_PASSANT(col) (WhitePawnType*64 + 7*8 + col)
#define BLACK_PASSANT(col) (BlackPawnType*64 + col)
#define BLACK_LEFT_CASTLE (BlackPawnType*64 + 7*8)
#define BLACK_RIGHT_CASTLE (BlackPawnType*64 + 7*8 + 7)
#define WHITE_LEFT_CASTLE (WhitePawnType*64)
#define WHITE_RIGHT_CASTLE (WhitePawnType*64 + 7)

using namespace std;

TranspositionTable::TranspositionTable()
{
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(0, numeric_limits<uint64_t>::max());
    std::srand(1337);
    for(int i = 0; i < 12*64; ++i){
        zobristKeys[i] = dis(gen);
    }
}

TranspositionTable& TranspositionTable::getTable(){
    static TranspositionTable table;
    return table;
}

uint64_t TranspositionTable::calculateHash(GameState &state){
    uint64_t hash = 0;
    for_each(state.pieces.begin(), state.pieces.end(), [&hash, this](auto& piece){
        hash ^= this->togglePiece(piece->type, GridPoint(piece->row, piece->col));
    });
    hash ^= zobristKeys[WHITE_LEFT_CASTLE] ^ zobristKeys[WHITE_RIGHT_CASTLE] ^ zobristKeys[BLACK_LEFT_CASTLE] ^ zobristKeys[BLACK_RIGHT_CASTLE];
    return hash;
}

uint64_t TranspositionTable::togglePiece(PieceType type, GridPoint loc){
    return zobristKeys[type*64 + loc.row*8 + loc.col];
}

uint64_t TranspositionTable::toggleLeftCastle(Color color){
    if(color)
        return zobristKeys[WHITE_LEFT_CASTLE];
    else
        return zobristKeys[BLACK_LEFT_CASTLE];
}
uint64_t TranspositionTable::toggleRightCastle(Color color){
    if(color)
        return zobristKeys[WHITE_RIGHT_CASTLE];
    else
        return zobristKeys[BLACK_RIGHT_CASTLE];
}
uint64_t TranspositionTable::toggleEnPassant(Color color, int col){
    if(col == -1)
        return 0;
    if(color)
        return zobristKeys[WHITE_PASSANT(col)];
    else
        return zobristKeys[BLACK_PASSANT(col)];
}
int TranspositionTable::getIndex(uint64_t hash){
    return hash & (TT_SIZE - 1);
}

Transposition& TranspositionTable::get(uint64_t hash){
    return table[getIndex(hash)];
}

Transposition& TranspositionTable::put(Transposition& transposition){
    Transposition& tableEntry = table[getIndex(transposition.hash)];
    tableEntry = transposition;
    return tableEntry;
}

Transposition::Transposition(){}

Transposition::Transposition(const Transposition& other){
    hash = other.hash;
    score = other.score;
    move = other.move;
    flag = other.flag;
    depth = other.depth;
}

Transposition& Transposition::operator=(const Transposition &other){
    hash = other.hash;
    score = other.score;
    move = other.move;
    flag = other.flag;
    depth = other.depth;
    return *this;
}
