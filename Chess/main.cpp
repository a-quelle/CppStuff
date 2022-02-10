#include "mainwindow.h"
#include <QApplication>

QImage emptyBoard;
QImage blueBox;
QImage redBox;
QImage blackBox;
QImage whiteBox;
std::vector<QImage> pieceSprites;

void loadAssets(){
    QImageReader reader("/home/anton/QtCode/Chess/images/board.jpg");
    reader.setAutoTransform(true);
    emptyBoard = reader.read();
    emptyBoard = emptyBoard.mirrored();
    reader.setFileName("/home/anton/QtCode/Chess/images/blackKing.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteKing.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blackQueen.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteQueen.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blackBishop.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteBishop.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blackKnight.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteKnight.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blackRook.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteRook.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blackPawn.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/whitePawn.png");
    pieceSprites.push_back(reader.read());
    reader.setFileName("/home/anton/QtCode/Chess/images/blueBox.png");
    blueBox = reader.read();
    reader.setFileName("/home/anton/QtCode/Chess/images/redBox.png");
    redBox = reader.read();
    reader.setFileName("/home/anton/QtCode/Chess/images/blackBox.png");
    blackBox = reader.read();
    reader.setFileName("/home/anton/QtCode/Chess/images/whiteBox.png");
    whiteBox = reader.read();
}

int main(int argc, char *argv[])
{
    loadAssets();
    QApplication a(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Chess Application"));
    MainWindow w;
    w.show();

    return a.exec();
}
