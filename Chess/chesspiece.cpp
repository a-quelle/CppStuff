#include "chesspiece.h"
#include "gamestate.h"

using namespace std;

bool isWithinBounds(int row, int col){
    return row > -1 && row < 8 && col > -1 && col < 8;
}

ChessPiece::ChessPiece(int row, int col, const PieceType type, const QImage sprite) : row(row), col(col), type(type), sprite(sprite) {}

ChessPiece::~ChessPiece(){}

BlackKing::BlackKing(int row, int col) : ChessPiece(row, col, BlackKingType, pieceSprites[BlackKingType]){}
WhiteKing::WhiteKing(int row, int col) : ChessPiece(row, col, WhiteKingType, pieceSprites[WhiteKingType]){}
BlackQueen::BlackQueen(int row, int col) : ChessPiece(row, col, BlackQueenType, pieceSprites[BlackQueenType]){}
WhiteQueen::WhiteQueen(int row, int col) : ChessPiece(row, col, WhiteQueenType, pieceSprites[WhiteQueenType]){}
BlackBishop::BlackBishop(int row, int col) : ChessPiece(row, col, BlackBishopType, pieceSprites[BlackBishopType]){}
WhiteBishop::WhiteBishop(int row, int col) : ChessPiece(row, col, WhiteBishopType, pieceSprites[WhiteBishopType]){}
BlackKnight::BlackKnight(int row, int col) : ChessPiece(row, col, BlackKnightType, pieceSprites[BlackKnightType]){}
WhiteKnight::WhiteKnight(int row, int col) : ChessPiece(row, col, WhiteKnightType, pieceSprites[WhiteKnightType]){}
BlackRook::BlackRook(int row, int col) : ChessPiece(row, col, BlackRookType, pieceSprites[BlackRookType]){}
WhiteRook::WhiteRook(int row, int col) : ChessPiece(row, col, WhiteRookType, pieceSprites[WhiteRookType]){}
BlackPawn::BlackPawn(int row, int col) : ChessPiece(row, col, BlackPawnType,  pieceSprites[BlackPawnType]){}
WhitePawn::WhitePawn(int row, int col) : ChessPiece(row, col, WhitePawnType, pieceSprites[WhitePawnType]){}

BlackKing::~BlackKing(){}
WhiteKing::~WhiteKing(){}
BlackQueen::~BlackQueen(){}
WhiteQueen::~WhiteQueen(){}
BlackBishop::~BlackBishop(){}
WhiteBishop::~WhiteBishop(){}
BlackKnight::~BlackKnight(){}
WhiteKnight::~WhiteKnight(){}
BlackRook::~BlackRook(){}
WhiteRook::~WhiteRook(){}
BlackPawn::~BlackPawn(){}
WhitePawn::~WhitePawn(){}


vector<GridPoint> BlackKing::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    if(isWithinBounds(row+1, col)){
        target = state.pieceAtLocation(row+1, col);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+1, col);
    }
    if(isWithinBounds(row+1, col+1)){
        target = state.pieceAtLocation(row+1, col+1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+1, col+1);
    }
    if(isWithinBounds(row, col+1)){
        target = state.pieceAtLocation(row, col+1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row, col+1);
    }
    if(isWithinBounds(row-1, col+1)){
        target = state.pieceAtLocation(row-1, col+1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-1, col+1);
    }
    if(isWithinBounds(row-1, col)){
        target = state.pieceAtLocation(row-1, col);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-1, col);
    }
    if(isWithinBounds(row-1, col-1)){
        target = state.pieceAtLocation(row-1, col-1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-1, col-1);
    }
    if(isWithinBounds(row, col-1)){
        target = state.pieceAtLocation(row, col-1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row, col-1);
    }
    if(isWithinBounds(row+1, col-1)){
        target = state.pieceAtLocation(row+1, col-1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+1, col-1);
    }
    //Check for castling
    ChessPiece* tower1 = state.pieceAtLocation(0,0);
    ChessPiece* tower2 = state.pieceAtLocation(0,7);
    if(state.flags.blackRightCastle && tower2 && !state.pieceAtLocation(0,5) && !state.pieceAtLocation(0,6)){
        bool intermediateCheck = false;
        if(state.locationCheckBy(0,4,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(0,5,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(0,6,WhiteType)) intermediateCheck = true;

        if(!intermediateCheck)
            out.emplace_back(0,6);
    }
    if(state.flags.blackLeftCastle && tower1 && !state.pieceAtLocation(0,1)
            && !state.pieceAtLocation(0,2) && ! state.pieceAtLocation(0,3)){
        bool intermediateCheck = false;
        if(state.locationCheckBy(0,1,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(0,2,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(0,3,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(0,4,WhiteType)) intermediateCheck = true;

        if(!intermediateCheck)
            out.emplace_back(0,2);
    }
    return out;
}
vector<GridPoint> WhiteKing::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    if(isWithinBounds(row+1, col)){
        target = state.pieceAtLocation(row+1, col);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+1, col);
    }
    if(isWithinBounds(row+1, col+1)){
        target = state.pieceAtLocation(row+1, col+1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+1, col+1);
    }
    if(isWithinBounds(row, col+1)){
        target = state.pieceAtLocation(row, col+1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row, col+1);
    }
    if(isWithinBounds(row-1, col+1)){
        target = state.pieceAtLocation(row-1, col+1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-1, col+1);
    }
    if(isWithinBounds(row-1, col)){
        target = state.pieceAtLocation(row-1, col);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-1, col);
    }
    if(isWithinBounds(row-1, col-1)){
        target = state.pieceAtLocation(row-1, col-1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-1, col-1);
    }
    if(isWithinBounds(row, col-1)){
        target = state.pieceAtLocation(row, col-1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row, col-1);
    }
    if(isWithinBounds(row+1, col-1)){
        target = state.pieceAtLocation(row+1, col-1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+1, col-1);
    }
    //Check for castling
    ChessPiece* tower1 = state.pieceAtLocation(7,0);
    ChessPiece* tower2 = state.pieceAtLocation(7,7);
    if(state.flags.whiteRightCastle && tower2 && !state.pieceAtLocation(7,5) && !state.pieceAtLocation(7,6)){
        bool intermediateCheck = false;
        if(state.locationCheckBy(7,4,WhiteType)) intermediateCheck = true;
        if(state.locationCheckBy(7,5,BlackType)) intermediateCheck = true;
        if(state.locationCheckBy(7,6,BlackType)) intermediateCheck = true;

        if(!intermediateCheck)
            out.emplace_back(7,6);
    }
    if(state.flags.whiteLeftCastle && tower1 && !state.pieceAtLocation(7,1)
            && !state.pieceAtLocation(7,2) && ! state.pieceAtLocation(7,3)){
        bool intermediateCheck = false;
        if(state.locationCheckBy(7,1,BlackType)) intermediateCheck = true;
        if(state.locationCheckBy(7,2,BlackType)) intermediateCheck = true;
        if(state.locationCheckBy(7,3,BlackType)) intermediateCheck = true;
        if(state.locationCheckBy(7,4,WhiteType)) intermediateCheck = true;

        if(!intermediateCheck)
            out.emplace_back(7,2);
    }
    return out;
}
vector<GridPoint> BlackQueen::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col+dist)){
        target = state.pieceAtLocation(row+dist, col+dist);
        if(!target)
            out.emplace_back(row+dist, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col-dist)){
        target = state.pieceAtLocation(row-dist, col-dist);
        if(!target)
            out.emplace_back(row-dist, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col+dist)){
        target = state.pieceAtLocation(row-dist, col+dist);
        if(!target)
            out.emplace_back(row-dist, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col-dist)){
        target = state.pieceAtLocation(row+dist, col-dist);
        if(!target)
            out.emplace_back(row+dist, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col)){
        target = state.pieceAtLocation(row+dist, col);
        if(!target)
            out.emplace_back(row+dist, col);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col)){
        target = state.pieceAtLocation(row-dist, col);
        if(!target)
            out.emplace_back(row-dist, col);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col+dist)){
        target = state.pieceAtLocation(row, col+dist);
        if(!target)
            out.emplace_back(row, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col-dist)){
        target = state.pieceAtLocation(row, col-dist);
        if(!target)
            out.emplace_back(row, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> WhiteQueen::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col+dist)){
        target = state.pieceAtLocation(row+dist, col+dist);
        if(!target)
            out.emplace_back(row+dist, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col-dist)){
        target = state.pieceAtLocation(row-dist, col-dist);
        if(!target)
            out.emplace_back(row-dist, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col+dist)){
        target = state.pieceAtLocation(row-dist, col+dist);
        if(!target)
            out.emplace_back(row-dist, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col-dist)){
        target = state.pieceAtLocation(row+dist, col-dist);
        if(!target)
            out.emplace_back(row+dist, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col)){
        target = state.pieceAtLocation(row+dist, col);
        if(!target)
            out.emplace_back(row+dist, col);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col)){
        target = state.pieceAtLocation(row-dist, col);
        if(!target)
            out.emplace_back(row-dist, col);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col+dist)){
        target = state.pieceAtLocation(row, col+dist);
        if(!target)
            out.emplace_back(row, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col-dist)){
        target = state.pieceAtLocation(row, col-dist);
        if(!target)
            out.emplace_back(row, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> BlackBishop::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col+dist)){
        target = state.pieceAtLocation(row+dist, col+dist);
        if(!target)
            out.emplace_back(row+dist, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col-dist)){
        target = state.pieceAtLocation(row-dist, col-dist);
        if(!target)
            out.emplace_back(row-dist, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col+dist)){
        target = state.pieceAtLocation(row-dist, col+dist);
        if(!target)
            out.emplace_back(row-dist, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col-dist)){
        target = state.pieceAtLocation(row+dist, col-dist);
        if(!target)
            out.emplace_back(row+dist, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> WhiteBishop::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col+dist)){
        target = state.pieceAtLocation(row+dist, col+dist);
        if(!target)
            out.emplace_back(row+dist, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col-dist)){
        target = state.pieceAtLocation(row-dist, col-dist);
        if(!target)
            out.emplace_back(row-dist, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col-dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col+dist)){
        target = state.pieceAtLocation(row-dist, col+dist);
        if(!target)
            out.emplace_back(row-dist, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row+dist, col-dist)){
        target = state.pieceAtLocation(row+dist, col-dist);
        if(!target)
            out.emplace_back(row+dist, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> BlackKnight::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    if(isWithinBounds(row+1, col-2)){
        target = state.pieceAtLocation(row+1, col-2);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+1, col-2);
    }
    if(isWithinBounds(row+1, col+2)){
        target = state.pieceAtLocation(row+1, col+2);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+1, col+2);
    }
    if(isWithinBounds(row+2, col+1)){
        target = state.pieceAtLocation(row+2, col+1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+2, col+1);
    }
    if(isWithinBounds(row-2, col+1)){
        target = state.pieceAtLocation(row-2, col+1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-2, col+1);
    }
    if(isWithinBounds(row-1, col-2)){
        target = state.pieceAtLocation(row-1, col-2);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-1, col-2);
    }
    if(isWithinBounds(row-1, col+2)){
        target = state.pieceAtLocation(row-1, col+2);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-1, col+2);
    }
    if(isWithinBounds(row+2, col-1)){
        target = state.pieceAtLocation(row+2, col-1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row+2, col-1);
    }
    if(isWithinBounds(row-2, col-1)){
        target = state.pieceAtLocation(row-2, col-1);
        if(!target || (target && (target->type & 0x01)))
            out.emplace_back(row-2, col-1);
    }

    return out;
}
vector<GridPoint> WhiteKnight::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;

    if(isWithinBounds(row+1, col-2)){
        target = state.pieceAtLocation(row+1, col-2);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+1, col-2);
    }
    if(isWithinBounds(row+1, col+2)){
        target = state.pieceAtLocation(row+1, col+2);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+1, col+2);
    }
    if(isWithinBounds(row+2, col+1)){
        target = state.pieceAtLocation(row+2, col+1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+2, col+1);
    }
    if(isWithinBounds(row-2, col+1)){
        target = state.pieceAtLocation(row-2, col+1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-2, col+1);
    }
    if(isWithinBounds(row-1, col-2)){
        target = state.pieceAtLocation(row-1, col-2);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-1, col-2);
    }
    if(isWithinBounds(row-1, col+2)){
        target = state.pieceAtLocation(row-1, col+2);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-1, col+2);
    }
    if(isWithinBounds(row+2, col-1)){
        target = state.pieceAtLocation(row+2, col-1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row+2, col-1);
    }
    if(isWithinBounds(row-2, col-1)){
        target = state.pieceAtLocation(row-2, col-1);
        if(!target || (target && !(target->type & 0x01)))
            out.emplace_back(row-2, col-1);
    }

    return out;
}
vector<GridPoint> BlackRook::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col)){
        target = state.pieceAtLocation(row+dist, col);
        if(!target)
            out.emplace_back(row+dist, col);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row+dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col)){
        target = state.pieceAtLocation(row-dist, col);
        if(!target)
            out.emplace_back(row-dist, col);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row-dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col+dist)){
        target = state.pieceAtLocation(row, col+dist);
        if(!target)
            out.emplace_back(row, col+dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col-dist)){
        target = state.pieceAtLocation(row, col-dist);
        if(!target)
            out.emplace_back(row, col-dist);
        else {
            if(target && (target->type & 0x01))
                out.emplace_back(row, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> WhiteRook::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target;
    int dist = 1;
    while(isWithinBounds(row+dist, col)){
        target = state.pieceAtLocation(row+dist, col);
        if(!target)
            out.emplace_back(row+dist, col);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row+dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row-dist, col)){
        target = state.pieceAtLocation(row-dist, col);
        if(!target)
            out.emplace_back(row-dist, col);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row-dist, col);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col+dist)){
        target = state.pieceAtLocation(row, col+dist);
        if(!target)
            out.emplace_back(row, col+dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row, col+dist);
            break;
        }
        ++dist;
    }
    dist = 1;
    while(isWithinBounds(row, col-dist)){
        target = state.pieceAtLocation(row, col-dist);
        if(!target)
            out.emplace_back(row, col-dist);
        else {
            if(target && !(target->type & 0x01))
                out.emplace_back(row, col-dist);
            break;
        }
        ++dist;
    }
    return out;
}
vector<GridPoint> BlackPawn::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target = state.pieceAtLocation(row+1, col);
    if(!target && isWithinBounds(row+1, col)){
        out.emplace_back(row+1, col);
        if(row == 1){
            target = state.pieceAtLocation(row+2, col);
            if(!target)
                out.emplace_back(row+2, col);
        }
    }
    target = state.pieceAtLocation(row+1, col-1);
    if(target && target->type & 0x01)
        out.emplace_back(row+1, col-1);
    target = state.pieceAtLocation(row+1, col+1);
    if(target && target->type & 0x01)
        out.emplace_back(row+1, col+1);
    //en passant
    if(!state.history.empty()){
        pair<GridPoint, GridPoint> lastMove = state.history.back().first;
        ChessPiece* pawn;
        pawn = state.pieceAtLocation(row, col-1);
        if(pawn && pawn->type == WhitePawnType && lastMove.first.row == row + 2 && lastMove.first.col == col-1)
            out.emplace_back(row+1, col-1);
        pawn = state.pieceAtLocation(row, col+1);
        if(pawn && pawn->type == WhitePawnType  && lastMove.first.row == row + 2 && lastMove.first.col == col+1)
            out.emplace_back(row+1, col+1);
    }

    return out;
}
vector<GridPoint> WhitePawn::getAvailableRawMoves(const GameState& state){
    vector<GridPoint> out;
    ChessPiece* target = state.pieceAtLocation(row-1, col);
    if(!target && isWithinBounds(row-1, col)){
        out.emplace_back(row-1, col);
        if(row == 6){
            target = state.pieceAtLocation(row-2, col);
            if(!target)
                out.emplace_back(row-2, col);
        }
    }
    target = state.pieceAtLocation(row-1, col-1);
    if(target && !(target->type & 0x01))
        out.emplace_back(row-1, col-1);
    target = state.pieceAtLocation(row-1, col+1);
    if(target && !(target->type & 0x01))
        out.emplace_back(row-1, col+1);
    //en passant
    if(!state.history.empty()){
        pair<GridPoint, GridPoint> lastMove = state.history.back().first;
        ChessPiece* pawn;
        pawn = state.pieceAtLocation(row, col-1);
        if(pawn && pawn->type == BlackPawnType && lastMove.first.row == row - 2 && lastMove.first.col == col-1)
            out.emplace_back(row-1, col-1);
        pawn = state.pieceAtLocation(row, col+1);
        if(pawn && pawn->type == BlackPawnType  && lastMove.first.row == row - 2 && lastMove.first.col == col+1)
            out.emplace_back(row-1, col+1);
    }

    return out;
}

vector<GridPoint> BlackKing::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhiteKing::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> BlackQueen::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhiteQueen::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> BlackBishop::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhiteBishop::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> BlackKnight::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhiteKnight::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> BlackRook::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhiteRook::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> BlackPawn::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

vector<GridPoint> WhitePawn::getAvailableMoves(const GameState& state){
    vector<GridPoint> moves = getAvailableRawMoves(state);
    //remove moves that leave the king in check
    moves.erase(remove_if(moves.begin(), moves.end(), [this, &state](GridPoint move){
        GameState tmp = state;
        return tmp.make(PointPair(GridPoint(row, col), move)).kingCheckOf(tmp.turn);
    }), moves.end());
    return moves;
}

BlackKing* BlackKing::clone(){
    BlackKing* piece = new BlackKing(row, col);
    return piece;
}
WhiteKing* WhiteKing::clone(){
    WhiteKing* piece = new WhiteKing(row, col);
    return piece;
}
BlackQueen* BlackQueen::clone(){
    BlackQueen* piece = new BlackQueen(row, col);
    return piece;
}
WhiteQueen* WhiteQueen::clone(){
    WhiteQueen* piece = new WhiteQueen(row, col);
    return piece;
}
BlackBishop* BlackBishop::clone(){
    BlackBishop* piece = new BlackBishop(row, col);
    return piece;
}
WhiteBishop* WhiteBishop::clone(){
    WhiteBishop* piece = new WhiteBishop(row, col);
    return piece;
}
BlackKnight* BlackKnight::clone(){
    BlackKnight* piece = new BlackKnight(row, col);
    return piece;
}
WhiteKnight* WhiteKnight::clone(){
    WhiteKnight* piece = new WhiteKnight(row, col);
    return piece;
}
BlackRook* BlackRook::clone(){
    BlackRook* piece = new BlackRook(row, col);
    return piece;
}
WhiteRook* WhiteRook::clone(){
    WhiteRook* piece = new WhiteRook(row, col);
    return piece;
}
BlackPawn* BlackPawn::clone(){
    BlackPawn* piece = new BlackPawn(row, col);
    return piece;
}
WhitePawn* WhitePawn::clone(){
    WhitePawn* piece = new WhitePawn(row, col);
    return piece;
}
