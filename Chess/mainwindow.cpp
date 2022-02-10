#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), chessBoard(*this)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    qRegisterMetaType<PointPair>();
    //QLabel for chessboard
    imageLabel = std::make_unique<ClickableQLabel>();
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setMinimumSize(QSize(600,600));
    //QScrollArea for QLabel
    scrollArea = std::make_unique<QScrollArea>();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageLabel.get());
    scrollArea->setBackgroundRole(QPalette::Dark);
    //Text Labels
    whiteLabel = std::make_unique<QLabel>("White: Human");
    blackLabel = std::make_unique<QLabel>("Black: Human");
    turnLabel = std::make_unique<QLabel>("Turn: White");
    logView = std::make_unique<QPlainTextEdit>(this);
    logView->setReadOnly(true);
    logView->appendPlainText("Move 0: Initialised board");
    logView->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    logView->setFixedWidth(200);
    //Create HBoxLayout with text labels for bottom of main window
    //Put GridLayout in MainWindow with scrollArea on top and labels below
    QWidget* layoutWidget = new QWidget;
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(scrollArea.get(), 0, 0, 1, 3);
    mainLayout->addWidget(whiteLabel.get(), 1, 0, 1, 1);
    mainLayout->addWidget(blackLabel.get(), 1, 1, 1, 1);
    mainLayout->addWidget(turnLabel.get(), 1, 2, 1, 1);
    mainLayout->addWidget(logView.get(), 0, 3, 2, 1);
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);
    paintChessboard();
    resize(QSize(850, 675));

    createMenus();

    //connect signals and slots
    connect(imageLabel.get(), SIGNAL(mouseClickSignal(QMouseEvent*)),
                      SLOT(mouseClickSlot(QMouseEvent*)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintChessboard(){
    imageLabel->setPixmap(QPixmap::fromImage(chessBoard.populatedBoard));
    imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scrollArea->setVisible(true);
}

void MainWindow::mouseClickSlot(QMouseEvent* ev){
    QSize sizeDiff = imageLabel->size() - chessBoard.populatedBoard.size();
    QPoint boardCoord(ev->x() - sizeDiff.width()/2, ev->y() - sizeDiff.height()/2);
    chessBoard.handleMouseClick(boardCoord);
    updateTurnLabel();
    updateColorLabels();
    logNewMove();
    imageLabel->setPixmap(QPixmap::fromImage(chessBoard.populatedBoard));
}

void MainWindow::updateColorLabels(){
        whiteLabel->setText(QString("White: ") + (chessBoard.iscpu[WhiteType]? QString("CPU"):QString("Human")));
        blackLabel->setText(QString("Black: ") + (chessBoard.iscpu[BlackType]? QString("CPU"):QString("Human")));
}

void MainWindow::updateTurnLabel(){
    if(chessBoard.currentState.turn)
        turnLabel->setText("Turn: White");
    else
        turnLabel->setText("Turn: Black");
}

void MainWindow::logNewMove(){
    static std::pair<GridPoint, GridPoint> move = std::pair<GridPoint, GridPoint>(GridPoint(-1,-1), GridPoint(-1,-1));
    std::pair<GridPoint, GridPoint> latestMove = !chessBoard.currentState.history.empty()?chessBoard.currentState.history.back().first: move;
    if(!chessBoard.iscpu[chessBoard.currentState.turn] && latestMove != move){
        move = latestMove;
        logView->appendPlainText("Move " + QString::number(chessBoard.currentState.history.size()) + ": "
                                 + move.first + " to " + move.second + (chessBoard.currentState.turn? " (Black)":" (White)"));
    }
}

void MainWindow::createMenus(){
    QMenu* fileMenu = menuBar()->addMenu("&File");
    newGameAct = fileMenu->addAction("&New Game", this, &MainWindow::newGame);

    QMenu* editMenu = menuBar()->addMenu("&Edit");

    undoAct = editMenu->addAction("&Undo", this, &MainWindow::undo);

    whiteCpuAct = editMenu->addAction("&White is cpu", this, &MainWindow::whiteCpu);
    whiteCpuAct->setCheckable(true);

    blackCpuAct = editMenu->addAction("&Black is cpu", this, &MainWindow::blackCpu);
    blackCpuAct->setCheckable(true);

    QMenu* difficultyMenu = editMenu->addMenu("&Difficulty");
    difficultyGroup = new QActionGroup(this);
    easyAct = new QAction("&Easy", difficultyGroup);
    easyAct->setCheckable(true);
    connect(easyAct, SIGNAL(triggered()), this, SLOT(setEasy()));;
    normalAct = new QAction("&Normal", difficultyGroup);
    normalAct->setCheckable(true);
    normalAct->setChecked(true);
    connect(normalAct, SIGNAL(triggered()), this, SLOT(setNormal()));;
    hardAct = new QAction("&Hard", difficultyGroup);
    hardAct->setCheckable(true);
    connect(hardAct, SIGNAL(triggered()), this, SLOT(setHard()));

    difficultyMenu->addAction(easyAct);
    difficultyMenu->addAction(normalAct);
    difficultyMenu->addAction(hardAct);


    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::about);
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About Chess"),
                                            tr("<p>The <b>Chess program</b> can be used to play chess."
                                               "Both black and white players can be set to human or cpu."
                                               "This can be done from the edit menu."
                                               "Win conditions are not programmed. Player loses if no moves available</p>"));
}

void MainWindow::whiteCpu(){
    chessBoard.iscpu[WhiteType] = whiteCpuAct->isChecked();
    updateColorLabels();
    if(chessBoard.currentState.turn == WhiteType && chessBoard.iscpu[WhiteType]){
        AI* ai = new AI(*this);
        QThreadPool::globalInstance()->start(ai);
    }
}

void MainWindow::blackCpu(){
    chessBoard.iscpu[BlackType] = blackCpuAct->isChecked();
    updateColorLabels();
    if(chessBoard.currentState.turn == BlackType && chessBoard.iscpu[BlackType]){
        AI* ai = new AI(*this);
        QThreadPool::globalInstance()->start(ai);
    }
}

void MainWindow::aiCallback(const PointPair move){
    chessBoard.currentState.make(move);
    chessBoard.finishTurn();
    updateTurnLabel();
    logNewMove();
    imageLabel->setPixmap(QPixmap::fromImage(chessBoard.populatedBoard));
}

void MainWindow::setEasy(){
    chessBoard.difficulty = easy;
}

void MainWindow::setNormal(){
    chessBoard.difficulty = normal;
}

void MainWindow::setHard(){
    chessBoard.difficulty = hard;
}

void MainWindow::newGame(){
    chessBoard.currentState = GameState();
    chessBoard.populateBoard();
    imageLabel->setPixmap(QPixmap::fromImage(chessBoard.populatedBoard));
}

void MainWindow::undo(){
    chessBoard.currentState.undo();
    chessBoard.populateBoard();
    imageLabel->setPixmap(QPixmap::fromImage(chessBoard.populatedBoard));
    if(chessBoard.currentState.turn)
        whiteCpu();
    else
        blackCpu();
}
