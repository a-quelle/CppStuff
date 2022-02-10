#include "gamestate.h"
#include <cstdlib>

#define EN_PASSANT 0x10
#define CASTLED 0x20
#define PROMOTED 0X40

using namespace std;

GridPoint::GridPoint() : row(-1), col(-1){};

GridPoint::GridPoint(int row, int col) : row(row), col(col){};

bool GridPoint::operator==(const GridPoint &other) const{
    return row == other.row && col == other.col;
}

GridPoint::operator QString() const{
    return QString('A'+col)+QString::number(8-row);
}



GameState::GameState(const GameState& state){
    history = state.history;
    flags = state.flags;
    turn = state.turn;
    pieces.clear();
    for_each(state.pieces.begin(), state.pieces.end(), [this](auto& piece){
        pieces.emplace_back(unique_ptr<ChessPiece>(piece->clone()));
    });
}

GameState::GameState(GameState&& state) noexcept {
    history = move(state.history);
    flags = state.flags;
    turn = state.turn;
    pieces = move(state.pieces);
}

GameState& GameState::operator=(const GameState& state){
    history = state.history;
    flags = state.flags;
    turn = state.turn;
    pieces.clear();
    for_each(state.pieces.begin(), state.pieces.end(), [this](auto& piece){
        pieces.emplace_back(unique_ptr<ChessPiece>(piece->clone()));
    });
    return *this;
}

GameState& GameState::operator=(GameState&& state) noexcept {
    history = move(state.history);
    flags = state.flags;
    turn = state.turn;
    pieces = move(state.pieces);
    return *this;
}

GameState::GameState()
{
    pieces.emplace_back(make_unique<BlackRook>(0,0));
    pieces.emplace_back(make_unique<BlackKnight>(0,1));
    pieces.emplace_back(make_unique<BlackBishop>(0,2));
    pieces.emplace_back(make_unique<BlackQueen>(0,3));
    pieces.emplace_back(make_unique<BlackKing>(0,4));
    pieces.emplace_back(make_unique<BlackBishop>(0,5));
    pieces.emplace_back(make_unique<BlackKnight>(0,6));
    pieces.emplace_back(make_unique<BlackRook>(0,7));
    pieces.emplace_back(make_unique<BlackPawn>(1,0));
    pieces.emplace_back(make_unique<BlackPawn>(1,1));
    pieces.emplace_back(make_unique<BlackPawn>(1,2));
    pieces.emplace_back(make_unique<BlackPawn>(1,3));
    pieces.emplace_back(make_unique<BlackPawn>(1,4));
    pieces.emplace_back(make_unique<BlackPawn>(1,5));
    pieces.emplace_back(make_unique<BlackPawn>(1,6));
    pieces.emplace_back(make_unique<BlackPawn>(1,7));

    pieces.emplace_back(make_unique<WhiteRook>(7,0));
    pieces.emplace_back(make_unique<WhiteKnight>(7,1));
    pieces.emplace_back(make_unique<WhiteBishop>(7,2));
    pieces.emplace_back(make_unique<WhiteQueen>(7,3));
    pieces.emplace_back(make_unique<WhiteKing>(7,4));
    pieces.emplace_back(make_unique<WhiteBishop>(7,5));
    pieces.emplace_back(make_unique<WhiteKnight>(7,6));
    pieces.emplace_back(make_unique<WhiteRook>(7,7));
    pieces.emplace_back(make_unique<WhitePawn>(6,0));
    pieces.emplace_back(make_unique<WhitePawn>(6,1));
    pieces.emplace_back(make_unique<WhitePawn>(6,2));
    pieces.emplace_back(make_unique<WhitePawn>(6,3));
    pieces.emplace_back(make_unique<WhitePawn>(6,4));
    pieces.emplace_back(make_unique<WhitePawn>(6,5));
    pieces.emplace_back(make_unique<WhitePawn>(6,6));
    pieces.emplace_back(make_unique<WhitePawn>(6,7));

    hash = TranspositionTable::getTable().calculateHash(*this);
    flags.pieceTaken = 0;
    flags.enPassantCol = -1;
    flags.blackLeftCastle = true;
    flags.whiteLeftCastle = true;
    flags.blackRightCastle = true;
    flags.whiteRightCastle = true;
}

ChessPiece* GameState::pieceAtLocation(const int row, const int col) const{
    for(auto& piece : pieces)
        if(piece && piece->row == row && piece->col == col)
            return piece.get();
    return nullptr;
}

void GameState::changeTurn(){
    if(turn == WhiteType)
        turn = BlackType;
    else {
        turn = WhiteType;
    }
}

bool GameState::kingAdjacent(const int row, const int col, const Color color) const{
    ChessPiece* piece;
    if(color == WhiteType){
        piece = pieceAtLocation(row+1, col-1);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row+1, col);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row+1, col+1);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row-1, col-1);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row-1, col);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row-1, col+1);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row, col+1);
        if(piece && piece->type == WhiteKingType)
            return true;
        piece = pieceAtLocation(row, col-1);
        if(piece && piece->type == WhiteKingType)
            return true;
    }

    if(color == BlackType){
        piece = pieceAtLocation(row+1, col-1);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row+1, col);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row+1, col+1);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row-1, col-1);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row-1, col);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row-1, col+1);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row, col+1);
        if(piece && piece->type == BlackKingType)
            return true;
        piece = pieceAtLocation(row, col-1);
        if(piece && piece->type == BlackKingType)
            return true;
    }
    return false;
}

bool GameState::checkByExceptFromKing(const int row, const int col, const Color color) const{
    bool check = false;
    if(color == BlackType){
        for_each(pieces.begin(), pieces.end(), [&check, this, row, col](auto& piece){
            if(!(piece->type&0x01) && piece->type != BlackKingType){
                auto availableMoves = piece->getAvailableRawMoves(*this);
                if(availableMoves.end() != find(availableMoves.begin(), availableMoves.end(), GridPoint(row,col)))
                    check = true;
            }
        });}
    if(color == WhiteType){
        for_each(pieces.begin(), pieces.end(), [&check, this, row, col](auto& piece){
            if((piece->type&0x01) && piece->type != WhiteKingType){
                auto availableMoves = piece->getAvailableRawMoves(*this);
                if(availableMoves.end() != find(availableMoves.begin(), availableMoves.end(), GridPoint(row,col)))
                    check = true;
            }
        });}
    return check;
}

bool GameState::locationCheckBy(const int row, const int col, const Color color) const{
    return checkByExceptFromKing(row, col, color) || kingAdjacent(row, col, color);
}

bool GameState::kingCheckOf(const Color color) const{
    ChessPiece* king;
    for_each(pieces.begin(), pieces.end(), [&king, color](auto& piece){
        if((piece->type & 0x01) == color && !(piece->type & 0xfe))
            king = piece.get();
    });
    bool isCheck = locationCheckBy(king->row, king->col, color?BlackType:WhiteType);
    return isCheck;
}

GameState& GameState::make(const PointPair& move){
    int row = move.second.row, col = move.second.col;
    ChessPiece* selected = (*find_if(pieces.begin(), pieces.end(), [&move](auto& piece){
        return piece->row == move.first.row && piece->col == move.first.col;
    })).get();    

    StateFlags historyFlags;

    historyFlags.pieceTaken = 0;
    historyFlags.enPassantCol = flags.enPassantCol;
    historyFlags.blackLeftCastle = flags.blackLeftCastle;
    historyFlags.blackRightCastle = flags.blackRightCastle;
    historyFlags.whiteLeftCastle = flags.whiteLeftCastle;
    historyFlags.whiteRightCastle = flags.whiteRightCastle;


    //remove pawn if en-passant. Do before remove piece at destination
    if((selected->type == BlackPawnType || selected->type == WhitePawnType) && col != selected->col && !pieceAtLocation(row, col)){
        pieces.erase(remove_if(
                               pieces.begin(), pieces.end(),
                               [this, col, selected](unique_ptr<ChessPiece>& piece){
            if(pieceAtLocation(selected->row, col) == piece.get()){
                hash ^= TranspositionTable::getTable().togglePiece(piece->type, GridPoint(selected->row, col));
                return true;
            }
            return false;
        }),
                           pieces.end());
        historyFlags.pieceTaken |= EN_PASSANT;
    }
    //remove possible existing piece at new location
    pieces.erase(remove_if(pieces.begin(), pieces.end(),
                     [this, row, col, &historyFlags](unique_ptr<ChessPiece>& piece){
        if(pieceAtLocation(row, col) == piece.get()){
            historyFlags.pieceTaken |= piece->type;
            hash ^= TranspositionTable::getTable().togglePiece(piece->type, GridPoint(row, col));
            return true;
        } else {
            return false;
        }}), pieces.end());

    //if castling occured, also move rook and update hash
    ChessPiece* rook;
    if(selected->type == WhiteKingType && flags.whiteLeftCastle && col == 2)
    {
        rook = pieceAtLocation(7,0);
        rook->col = 3;
        hash ^= TranspositionTable::getTable().togglePiece(WhiteRookType, GridPoint(7, 0))
                ^TranspositionTable::getTable().togglePiece(WhiteRookType, GridPoint(7, 3));
        historyFlags.pieceTaken |= CASTLED;
    }
    if(selected->type == WhiteKingType && flags.whiteRightCastle && col == 6){
        rook = pieceAtLocation(7,7);
        rook->col = 5;
        hash ^= TranspositionTable::getTable().togglePiece(WhiteRookType, GridPoint(7, 7))
                ^TranspositionTable::getTable().togglePiece(WhiteRookType, GridPoint(7, 5));
        historyFlags.pieceTaken |= CASTLED;
    }
    if(selected->type == BlackKingType && flags.blackLeftCastle && col == 2)
    {
        rook = pieceAtLocation(0,0);
        rook->col = 3;
        hash ^= TranspositionTable::getTable().togglePiece(BlackRookType, GridPoint(0, 0))
                ^TranspositionTable::getTable().togglePiece(BlackRookType, GridPoint(0, 3));
        historyFlags.pieceTaken |= CASTLED;
    }
    if(selected->type == BlackKingType && flags.blackRightCastle && col == 6){
        rook = pieceAtLocation(0,7);
        rook->col = 5;
        hash ^= TranspositionTable::getTable().togglePiece(BlackRookType, GridPoint(0, 7))
                ^TranspositionTable::getTable().togglePiece(BlackRookType, GridPoint(0, 5));
        historyFlags.pieceTaken |= CASTLED;
    }
    //if king or rook moved and castling allowed, set to false and update hash
    if(((selected->row == 0 && selected-> col == 0) || selected->type == BlackKingType ) && flags.blackLeftCastle){
        flags.blackLeftCastle = false;
        hash ^= TranspositionTable::getTable().toggleLeftCastle(BlackType);

    }
    if(((selected->row == 0 && selected-> col == 7) || selected->type == BlackKingType ) && flags.blackRightCastle){
        flags.blackRightCastle = false;
        hash ^= TranspositionTable::getTable().toggleRightCastle(BlackType);
    }
    if(((selected->row == 7 && selected-> col == 0) || selected->type == WhiteKingType ) && flags.whiteLeftCastle){
        flags.whiteLeftCastle = false;
        hash ^= TranspositionTable::getTable().toggleLeftCastle(WhiteType);
    }
    if(((selected->row == 7 && selected-> col == 7) || selected->type == WhiteKingType ) && flags.whiteRightCastle){
        flags.whiteRightCastle = false;
        hash ^= TranspositionTable::getTable().toggleRightCastle(WhiteType);
    }
    //update hash, and update en passant flag
    hash ^= TranspositionTable::getTable().toggleEnPassant(static_cast<Color>(1-turn), flags.enPassantCol);
    if(selected->type == WhitePawnType && selected->row == row + 2){
        flags.enPassantCol = col;
        hash ^= TranspositionTable::getTable().toggleEnPassant(turn, flags.enPassantCol);
    }
    else if(selected->type == BlackPawnType && selected->row == row - 2){
        flags.enPassantCol = col;
        hash ^= TranspositionTable::getTable().toggleEnPassant(turn, flags.enPassantCol);
    }
    else{
        flags.enPassantCol = -1;
    }
    //create move for history; uses old position, do before update position
    pair<GridPoint,GridPoint> newMove = PointPair(GridPoint(selected->row, selected->col), GridPoint(row, col));
    //update position of piece and hash
    hash ^= TranspositionTable::getTable().togglePiece(selected->type, GridPoint(selected->row, selected->col))
            ^ TranspositionTable::getTable().togglePiece(selected->type, GridPoint(row, col));
    selected->row = row;
    selected->col = col;

    //promote pawn to queen and update hash, uses new position of piece
    if(selected->type == WhitePawnType && row == 0){
        pieces.erase(remove_if(pieces.begin(), pieces.end(), [row, col](auto& piece){
            return piece->row == row && piece->col == col;
        }), pieces.end());
        hash ^= TranspositionTable::getTable().togglePiece(WhitePawnType, GridPoint(row, col))
                ^ TranspositionTable::getTable().togglePiece(WhiteQueenType, GridPoint(row, col));
        pieces.emplace_back(make_unique<WhiteQueen>(row, col));
        historyFlags.pieceTaken |= PROMOTED;
    }
    if(selected->type == BlackPawnType && row == 7){
        pieces.erase(remove_if(pieces.begin(), pieces.end(), [row, col](auto& piece){
            return piece->row == row && piece->col == col;
        }), pieces.end());
        hash ^= TranspositionTable::getTable().togglePiece(BlackPawnType, GridPoint(row, col))
                ^ TranspositionTable::getTable().togglePiece(BlackQueenType, GridPoint(row, col));
        pieces.emplace_back(make_unique<BlackQueen>(row, col));
        historyFlags.pieceTaken |= PROMOTED;
    }

    //Update history now all flags are set
    history.emplace_back(newMove, historyFlags);

    changeTurn();
    return *this;
}

GameState& GameState::undo(){
    if(history.empty())
        return *this;
    pair<PointPair, StateFlags> lastMove = history.back();
    //if promotion occured, change back to pawn
    if(lastMove.second.pieceTaken & PROMOTED){
        pieces.erase(remove_if(pieces.begin(), pieces.end(), [&lastMove](auto& piece){
            return piece->row == lastMove.first.second.row && piece->col == lastMove.first.second.col;
        }), pieces.end());
        if(turn){ //turn indicates next player, so change black here
            pieces.emplace_back(make_unique<BlackPawn>(lastMove.first.first.row, lastMove.first.first.col));

            hash ^= TranspositionTable::getTable().togglePiece(BlackPawnType, GridPoint(lastMove.first.first.row, lastMove.first.first.col))
                    ^ TranspositionTable::getTable().togglePiece(BlackQueenType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
        }
        else{
            pieces.emplace_back(make_unique<WhitePawn>(lastMove.first.first.row, lastMove.first.first.col));

            hash ^= TranspositionTable::getTable().togglePiece(WhitePawnType, GridPoint(lastMove.first.first.row, lastMove.first.first.col))
                    ^ TranspositionTable::getTable().togglePiece(WhiteQueenType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
        }

    }else{
        //get moved piece, reset position and hash
        ChessPiece* movedPiece = pieceAtLocation(lastMove.first.second.row, lastMove.first.second.col);
        movedPiece->row = lastMove.first.first.row;
        movedPiece->col = lastMove.first.first.col;
        hash ^= TranspositionTable::getTable().togglePiece(movedPiece->type, GridPoint(lastMove.first.second.row, lastMove.first.second.col))
                ^ TranspositionTable::getTable().togglePiece(movedPiece->type, GridPoint(lastMove.first.first.row, lastMove.first.first.col));

        //if castling occured, return rook and update hash
        if(lastMove.second.pieceTaken & CASTLED){
            ChessPiece* rook = nullptr;
            if(lastMove.first.second.col == 2){
                rook = pieceAtLocation(lastMove.first.second.row, 3);
                rook->col = 0;
                hash ^= TranspositionTable::getTable().togglePiece(rook->type, GridPoint(lastMove.first.second.row, 0))
                        ^TranspositionTable::getTable().togglePiece(rook->type, GridPoint(lastMove.first.second.row, 3));
            }
            if(lastMove.first.second.col == 6){
                rook = pieceAtLocation(lastMove.first.second.row, 5);
                rook->col = 7;
                hash ^= TranspositionTable::getTable().togglePiece(rook->type, GridPoint(lastMove.first.second.row, 5))
                        ^TranspositionTable::getTable().togglePiece(rook->type, GridPoint(lastMove.first.second.row, 7));
            }
        }
        //if en passant, return pawn and update hash
        if(lastMove.second.pieceTaken & EN_PASSANT){
            if(turn){
                pieces.emplace_back(make_unique<WhitePawn>(lastMove.first.first.row, lastMove.first.second.col));//turn is still next player
                hash ^= TranspositionTable::getTable().togglePiece(WhitePawnType, GridPoint(lastMove.first.first.row, lastMove.first.second.col));
            }
            else{
                pieces.emplace_back(make_unique<BlackPawn>(lastMove.first.first.row, lastMove.first.second.col)); //turn is still next player
                hash ^= TranspositionTable::getTable().togglePiece(BlackPawnType, GridPoint(lastMove.first.first.row, lastMove.first.second.col));
            }
        }
    }

    //if a piece was captured normally, replace it
    switch(lastMove.second.pieceTaken & 0x0F){
        case BlackQueenType:
            pieces.emplace_back(make_unique<BlackQueen>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(BlackQueenType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case WhiteQueenType:
            pieces.emplace_back(make_unique<WhiteQueen>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(WhiteQueenType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case BlackBishopType:
            pieces.emplace_back(make_unique<BlackBishop>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(BlackBishopType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case WhiteBishopType:
            pieces.emplace_back(make_unique<WhiteBishop>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(WhiteBishopType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case BlackKnightType:
            pieces.emplace_back(make_unique<BlackKnight>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(BlackKnightType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case WhiteKnightType:
            pieces.emplace_back(make_unique<WhiteKnight>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(WhiteKnightType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case BlackRookType:
            pieces.emplace_back(make_unique<BlackRook>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(BlackRookType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case WhiteRookType:
            pieces.emplace_back(make_unique<WhiteRook>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(WhiteRookType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case BlackPawnType:
            pieces.emplace_back(make_unique<BlackPawn>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(BlackPawnType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
        case WhitePawnType:
            pieces.emplace_back(make_unique<WhitePawn>(lastMove.first.second.row, lastMove.first.second.col));
            hash ^= TranspositionTable::getTable().togglePiece(WhitePawnType, GridPoint(lastMove.first.second.row, lastMove.first.second.col));
            break;
    }
    //update hash for flag changes
    if(lastMove.second.blackLeftCastle != flags.blackLeftCastle){
        hash ^= TranspositionTable::getTable().toggleLeftCastle(BlackType);
    }
    if(lastMove.second.blackRightCastle != flags.blackRightCastle){
        hash ^= TranspositionTable::getTable().toggleRightCastle(BlackType);
    }
    if(lastMove.second.whiteLeftCastle != flags.whiteLeftCastle){
        hash ^= TranspositionTable::getTable().toggleLeftCastle(WhiteType);
    }
    if(lastMove.second.whiteRightCastle != flags.whiteRightCastle){
        hash ^= TranspositionTable::getTable().toggleRightCastle(WhiteType);
    }
    hash ^= TranspositionTable::getTable().toggleEnPassant(turn, lastMove.second.enPassantCol)
            ^ TranspositionTable::getTable().toggleEnPassant(static_cast<Color>(1-turn), flags.enPassantCol);

    history.erase(history.end() - 1);
    flags = lastMove.second;

    changeTurn(); //roll back turn value

    return *this;
}

vector<PointPair> GameState::getMoves(const Color color) const{
    vector<PointPair> moves;
    for_each(pieces.begin(), pieces.end(), [this, &moves, color](auto& piece){
        if((piece->type & 0x01) == color){
            vector<GridPoint> locDest = piece->getAvailableMoves(*this);
            vector<PointPair> locMoves;
            transform(locDest.begin(), locDest.end(), back_inserter(locMoves),[&piece](GridPoint& dest){
                return PointPair(GridPoint(piece->row, piece->col), dest);
            });
            moves.insert(moves.end(), locMoves.begin(), locMoves.end());
        }
    });
    return moves;
}
