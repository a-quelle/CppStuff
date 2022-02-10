#include "chessboard.h"
#include <algorithm>
#include <iostream>
#include <QPainter>

using namespace std;

ChessBoard::ChessBoard(MainWindow& window)noexcept:boardOrigin(72, 75), boardSquare(60, 60),  window(window), currentState()
{
    populateBoard();
}

void ChessBoard::populateBoard(){
    QImage image(emptyBoard.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0,0, emptyBoard);
    drawAllPieces(painter);
    painter.end();
    populatedBoard = image;
}

void ChessBoard::drawPiece(const ChessPiece* const piece, QPainter& painter){
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(boardOrigin.x() + boardSquare.width()*piece->col, boardOrigin.y() + boardSquare.height()*piece->row,
                      piece->sprite);
}

void ChessBoard::drawAllPieces(QPainter& painter){
    for(size_t i = 0; i < currentState.pieces.size(); ++i){
        drawPiece(currentState.pieces[i].get(), painter);
    }
}

void ChessBoard::handleMouseClick(const QPoint location){
    //calculate selected board square from mouse coordinates
    int row, col;
    col = (location.x() - boardOrigin.x());
    row = (location.y() - boardOrigin.y());
    if(col > 0)
        col /= boardSquare.width();
    if(row > 0)
        row /= boardSquare.height();
    //undraw previous selections
    for_each(moves.begin(), moves.end(), [this](GridPoint square){undrawBox(square.row, square.col);});
    if(selected){
        undrawBox(selected->row, selected->col);
    }
    //if valid move is selected
    if(find(moves.begin(), moves.end(), GridPoint(row,col)) != moves.end()){
        currentState.make(PointPair(GridPoint(selected->row, selected->col), GridPoint(row, col)));
        //close up
        finishTurn();
    }
    //if new piece is selected
    else if((selected = currentState.pieceAtLocation(row, col)) && ((currentState.turn&&(selected->type&0x01)&&!iscpu[currentState.turn]) || !(currentState.turn||(selected->type&0x01||iscpu[currentState.turn])))){
        drawBlueBox(row, col);
        moves = selected->getAvailableMoves(currentState);
        for_each(moves.begin(), moves.end(), [this](GridPoint square){drawRedBox(square.row, square.col);});
    }
    //if nothing is selected
    else{
        moves.clear();
    }
}

void ChessBoard::finishTurn(){
    moves.clear();
    selected = nullptr;
    populateBoard();
    if(currentState.kingCheckOf(currentState.turn)){
        if(!currentState.getMoves(currentState.turn).empty()){
            QMessageBox msgBox;
            msgBox.setText("Check!");
            msgBox.exec();
        }
        else{

            QMessageBox msgBox;
            msgBox.setText("Checkmate!");
            msgBox.exec();
            return;
        }
    }
    if(iscpu[currentState.turn]){
        AI* ai = new AI(window);
        QThreadPool::globalInstance()->start(ai);
    }
}

void ChessBoard::undrawBox(const int row, const int col){
    QPainter painter(&populatedBoard);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    if(isBlack(row, col))
        painter.drawImage(boardOrigin.x() + boardSquare.width()*col, boardOrigin.y() + boardSquare.height()*row, blackBox);
    else
        painter.drawImage(boardOrigin.x() + boardSquare.width()*col, boardOrigin.y() + boardSquare.height()*row, whiteBox);
    painter.end();

}

void ChessBoard::drawBlueBox(const int row, const int col){
    QPainter painter(&populatedBoard);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(boardOrigin.x() + boardSquare.width()*col, boardOrigin.y() + boardSquare.height()*row, blueBox);
    painter.end();
}

void ChessBoard::drawRedBox(const int row, const int col){
    QPainter painter(&populatedBoard);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(boardOrigin.x() + boardSquare.width()*col, boardOrigin.y() + boardSquare.height()*row, redBox);
    painter.end();
}

bool ChessBoard::isBlack(const int row, const int col){
    return (row + col)&0x01;
}
