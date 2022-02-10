#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <QImage>

enum PieceType:int{
    BlackKingType,
    WhiteKingType,
    BlackQueenType,
    WhiteQueenType,
    BlackBishopType,
    WhiteBishopType,
    BlackKnightType,
    WhiteKnightType,
    BlackRookType,
    WhiteRookType,
    BlackPawnType,
    WhitePawnType,
    NoneType
};

struct GameState;

struct GridPoint{
    int row;
    int col;
    GridPoint();
    GridPoint(int row, int col);
    GridPoint(const GridPoint&) = default;
    bool operator ==(const GridPoint& other) const;
    operator QString() const;
};

extern QImage emptyBoard;
extern QImage blueBox;
extern QImage redBox;
extern QImage blackBox;
extern QImage whiteBox;
extern std::vector<QImage> pieceSprites;

class ChessPiece
{
public:
    int row;
    int col;
    const PieceType type;
    const QImage sprite;
    ChessPiece(int row, int col, const PieceType type, const QImage sprite);
    virtual ~ChessPiece();
    virtual std::vector<GridPoint> getAvailableRawMoves(const GameState& state) = 0;
    virtual std::vector<GridPoint> getAvailableMoves(const GameState& state) = 0;
    virtual ChessPiece* clone() = 0;
};

struct BlackKing : public ChessPiece{
    BlackKing(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackKing* clone() override;
    ~BlackKing() override;
};
struct WhiteKing : public ChessPiece{
    WhiteKing(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhiteKing* clone() override;
    ~WhiteKing() override;
};
struct BlackQueen : public ChessPiece{
    BlackQueen(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackQueen* clone() override;
    ~BlackQueen() override;
};
struct WhiteQueen : public ChessPiece{
    WhiteQueen(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhiteQueen* clone() override;
    ~WhiteQueen() override;
};
struct BlackBishop : public ChessPiece{
    BlackBishop(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackBishop* clone() override;
    ~BlackBishop() override;
};
struct WhiteBishop : public ChessPiece{
    WhiteBishop(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhiteBishop* clone() override;
    ~WhiteBishop() override;
};
struct BlackKnight : public ChessPiece{
    BlackKnight(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackKnight* clone() override;
    ~BlackKnight() override;
};
struct WhiteKnight : public ChessPiece{
    WhiteKnight(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhiteKnight* clone() override;
    ~WhiteKnight() override;
};
struct BlackRook : public ChessPiece{
    BlackRook(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackRook* clone() override;
    ~BlackRook() override;
};
struct WhiteRook : public ChessPiece{
    WhiteRook(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhiteRook* clone() override;
    ~WhiteRook() override;
};
struct BlackPawn : public ChessPiece{
    BlackPawn(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    BlackPawn* clone() override;
    ~BlackPawn() override;
};
struct WhitePawn : public ChessPiece{
    WhitePawn(int row, int col);
    std::vector<GridPoint> getAvailableRawMoves(const GameState& state) override;
    std::vector<GridPoint> getAvailableMoves(const GameState& state) override;
    WhitePawn* clone() override;
    ~WhitePawn() override;
};

#endif // CHESSPIECE_H
