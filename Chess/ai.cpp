#include "mainwindow.h"
#include <QMetaType>
#include <ctime>
#include <iostream>

using namespace std;

static Matrix<8,8> whiteKingTable({-6,-8,-8,-10,-10,-8,-8,-6,
                                                  -6,-8,-8,-10,-10,-8,-8,-6,
                                                   -6,-8,-8,-10,-10,-8,-8,-6,
                                                  -6,-8,-8,-10,-10,-8,-8,-6,
                                                  -4,-6,-6,-8,-8,-6,-6,-4,
                                                  -2,-4,-4,-4,-4,-4,-4,-2,
                                                  4,4,0,0,0,0,4,4,
                                                  4,6,2,0,0,2,6,4});
static Matrix<8,8> blackKingTable({4,6,2,0,0,2,6,4,
                                                   4,4,0,0,0,0,4,4,
                                                   -2,-4,-4,-4,-4,-4,-4,-2,
                                                    -4,-6,-6,-8,-8,-6,-6,-4,
                                                   -6,-8,-8,-10,-10,-8,-8,-6,
                                                   -6,-8,-8,-10,-10,-8,-8,-6,
                                                   -6,-8,-8,-10,-10,-8,-8,-6,
                                                   -6,-8,-8,-10,-10,-8,-8,-6});
static Matrix<8,8> whiteQueenTable({-4,-2,-2,-1,-1,-2,-2,-4,
                                               -2,0,0,0,0,0,0,-2,
                                               -2,0,1,1,1,1,0,-2,
                                               -1,0,1,1,1,1,0,-1,
                                               0,0,1,1,1,1,0,-1,
                                               -2,0,0,0,0,0,0,-2,
                                               -2,0,1,1,1,1,0,-2,
                                               -4,-2,-2,-1,-1,-2,-2,-4});
static Matrix<8,8> blackQueenTable({-4,-2,-2,-1,-1,-2,-2,-4,
                                               -2,0,0,0,0,0,0,-2,
                                               -2,0,1,1,1,1,0,-2,
                                               -1,0,1,1,1,1,0,0,
                                               -1,0,1,1,1,1,0,-1,
                                               -2,0,0,0,0,0,0,-2,
                                               -2,0,1,1,1,1,0,-2,
                                               -4,-2,-2,-1,-1,-2,-2,-4});
static Matrix<8,8> whiteRookTable({0,0,0,0,0,0,0,0,
                                               1,2,2,2,2,2,2,1,
                                               -1,0,0,0,0,0,0,-1,
                                                -1,0,0,0,0,0,0,-1,
                                                -1,0,0,0,0,0,0,-1,
                                                -1,0,0,0,0,0,0,-1,
                                                -1,0,0,0,0,0,0,-1,
                                               0,0,0,1,1,0,0,0});
static Matrix<8,8> blackRookTable({0,0,0,1,1,0,0,0,
                                                   -1,0,0,0,0,0,0,-1,
                                                    -1,0,0,0,0,0,0,-1,
                                                    -1,0,0,0,0,0,0,-1,
                                                    -1,0,0,0,0,0,0,-1,
                                                    -1,0,0,0,0,0,0,-1,
                                                   1,2,2,2,2,2,2,1,
                                                   0,0,0,0,0,0,0,0});
static Matrix<8,8> whiteBishopTable({-4,-2,-2,-2,-2,-2,-2,-4,
                                                   -2,0,0,0,0,0,0,-2,
                                                   -2,0,1,2,2,1,0,-2,
                                                     -2,1,1,2,2,1,1,-2,
                                                     -2,0,2,2,2,2,0,-2,
                                                     -2,2,2,2,2,2,2,-2,
                                                    -2,1,0,0,0,0,1,-2,
                                                   -4,-2,-2,-2,-2,-2,-2,-4});
static Matrix<8,8> blackBishopTable({-4,-2,-2,-2,-2,-2,-2,-4,
                                                   -2,0,0,0,0,0,0,-2,
                                                   -2,0,1,2,2,1,0,-2,
                                                     -2,1,1,2,2,1,1,-2,
                                                     -2,0,2,2,2,2,0,-2,
                                                     -2,2,2,2,2,2,2,-2,
                                                    -2,1,0,0,0,0,1,-2,
                                                   -4,-2,-2,-2,-2,-2,-2,-4});
static Matrix<8,8> whiteKnightTable({-10,-8,-6,-6,-6,-6,-8,-10,
                                                     -8,-4,0,0,0,0,-4,-8,
                                                     -6,0,2,3,3,2,0,-6,
                                                     -6,1,3,4,4,3,1,-3,
                                                     -6,0,3,4,4,3,0,-6,
                                                     -6,1,2,3,3,2,1,-6,
                                                     -8,-4,0,1,1,0,-4,-8,
                                                     -10,-8,-6,-6,-6,-6,-8,-10});
static Matrix<8,8> blackKnightTable({-10,-8,-6,-6,-6,-6,-8,-10,
                                                     -8,-4,0,1,1,0,-4,-8,
                                                     -6,1,2,3,3,2,1,-6,
                                                     -6,0,3,4,4,3,0,-6,
                                                     -6,1,3,4,4,3,1,-3,
                                                     -6,0,2,3,3,2,0,-6,
                                                     -8,-4,0,0,0,0,-4,-8,
                                                     -10,-8,-6,-6,-6,-6,-8,-10});
static Matrix<8,8> whitePawnTable({0,0,0,0,0,0,0,0,
                                                     10,10,10,10,10,10,10,10,
                                                     2,2,4,6,6,4,2,2,
                                                     1,1,2,5,5,2,1,1,
                                                     0,0,0,4,4,0,0,0,
                                                     1,-1,-2,0,0,-2,-1,1,
                                                     1,2,2,-4,-4,2,2,1,
                                                     0,0,0,0,0,0,0,0});
static Matrix<8,8> blackPawnTable({0,0,0,0,0,0,0,0,
                                                     1,2,2,-4,-4,2,2,1,
                                                     1,-1,-2,0,0,-2,-1,1,
                                                     0,0,0,4,4,0,0,0,
                                                     1,1,2,5,5,2,1,1,
                                                     2,2,4,6,6,4,2,2,
                                                     10,10,10,10,10,10,10,10,
                                                     0,0,0,0,0,0,0,0});

AI::AI(MainWindow& window) : difficulty(window.chessBoard.difficulty), state(window.chessBoard.currentState){
    connect(this, SIGNAL(workDone(PointPair)), &window, SLOT(aiCallback(PointPair)));
};

AI::~AI(){};

PointPair AI::generateMove()noexcept{
    clock_t start = clock();
        pair<int,unique_ptr<GameState>> finalState;
    switch(difficulty){
    case easy:
        finalState = minMax(1);
        break;
    case normal:
        finalState = minMax(4);
        break;
    case hard:
        finalState = minMax(5);
        break;
    }
    double duration = (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
    cout << "Move took: " << duration << "seconds."<< endl;
    return finalState.second->history[state.history.size()].first;
}

void AI::run(){
    PointPair move = generateMove();
    emit workDone(move);
}

int calculateScore(const GameState &state)noexcept{
    int whiteScore = 0, blackScore = 0;
    for_each(state.pieces.begin(), state.pieces.end(), [&whiteScore, &blackScore](auto& piece){
        switch(piece->type){
            case WhitePawnType:
                whiteScore += 20 + whitePawnTable(piece->row, piece->col);
                break;
            case BlackPawnType:
                blackScore += 20 + blackPawnTable(piece->row, piece->col);
                break;
            case WhiteRookType:
                whiteScore += 100 + whiteRookTable(piece->row, piece->col);
                break;
            case BlackRookType:
                blackScore += 100 + blackRookTable(piece->row, piece->col);
                break;
            case WhiteKnightType:
                whiteScore += 60 + whiteKnightTable(piece->row, piece->col);
                break;
            case BlackKnightType:
                blackScore += 60 + blackKnightTable(piece->row, piece->col);
                break;
            case WhiteBishopType:
                whiteScore += 60 + whiteBishopTable(piece->row, piece->col);
                break;
            case BlackBishopType:
                blackScore += 60 + blackBishopTable(piece->row, piece->col);
                break;
            case WhiteQueenType:
                whiteScore += 180 + whiteQueenTable(piece->row, piece->col);
                break;
            case BlackQueenType:
                blackScore += 180 + blackQueenTable(piece->row, piece->col);
                break;
            case WhiteKingType:
                whiteScore += 1800 + whiteKingTable(piece->row, piece->col);
                break;
            case BlackKingType:
                blackScore += 1800 + blackKingTable(piece->row, piece->col);
                break;
        }
    });

    if(state.turn)
        return whiteScore - blackScore;
    else
        return blackScore - whiteScore;
}

bool operator<(const pair<int, unique_ptr<GameState>>& lhs, const pair<int, unique_ptr<GameState>>& rhs)noexcept{
    return lhs.first < rhs.first;
}
bool operator>(const pair<int, unique_ptr<GameState>>& lhs, const pair<int, unique_ptr<GameState>>& rhs)noexcept{
    return lhs.first > rhs.first;
}

pair<int,unique_ptr<GameState>> AI::minMax(int depth, int a, int b)noexcept{
    vector<GameState> out;
    Transposition transposition = TranspositionTable::getTable().get(state.hash);
    if(transposition.hash == state.hash && transposition.depth > depth){
        if(transposition.flag == ValidType)
            return pair<int, unique_ptr<GameState>>(transposition.score, make_unique<GameState>(state));
        if(transposition.flag == UBoundType && transposition.score < b)
            b = transposition.score;
        if(transposition.flag == LBoundType && transposition.score > a)
            a = transposition.score;
    }
    if(!depth)
        return pair<int, unique_ptr<GameState>>(calculateScore(state), make_unique<GameState>(state));
    else{
        vector<PointPair> moves = state.getMoves(state.turn);
        if(moves.empty()){
            if(state.kingCheckOf(state.turn))
                return pair<int, unique_ptr<GameState>>(calculateScore(state) - 1800, make_unique<GameState>(state));
            return pair<int, unique_ptr<GameState>>(0, make_unique<GameState>(state));
        }
        pair<int,unique_ptr<GameState>> max;
        unsigned short tmpMove = transposition.move;

        if(transposition.hash == state.hash && transposition.depth > 0){
            state.make(moves[tmpMove]);
            max = minMax(depth-1, -b,-a);
            state.undo();
        }else{
            tmpMove = 0;
            state.make(moves[0]);
            max = minMax(depth-1, -b,-a);
            state.undo();
        }
        max.first *= -1;
        for(unsigned short i = 0; i < moves.size(); ++i){
            if(i != tmpMove){
                if(max.first >= b)
                    break;
                if(max.first > a)
                    a = max.first;
                state.make(moves[i]);
                pair<int,unique_ptr<GameState>> value = minMax(depth-1, -a-1,-a);
                value.first *= -1;
                if(value > max){
                    if(a < value.first && value.first < b && depth > 1){
                        max = minMax(depth-1, -b, -value.first);
                        max.first *= -1;
                    }
                    else {
                        max = move(value);
                    }
                    tmpMove = i;
                }
                state.undo();}
        }

        if(transposition.depth <= depth || transposition.hash != state.hash){
            transposition.hash = state.hash;
            transposition.flag = ValidType;
            if(max.first <= a)
                transposition.flag = UBoundType;
            if(max.first >= b)
                transposition.flag = LBoundType;
            transposition.score = max.first;
            transposition.move = tmpMove;
        }
        return max;
    }
}
