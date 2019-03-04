#include "Regression\Regression.h"
#include "Plotter\Plotter.h"
#include <thread>
#include "Matrix\Matrix.h"


using namespace std;

bool quit; 
extern bool running;

int main(int argc, char* argv[])
{quit = false;
    NeuralNet neuralNet(3, 3, 2, 6);
    neuralNet.readWeights("weights.dat");    
    generateData();
    thread regressionThread(gradientDescentLoop, ref(neuralNet));
    while(!quit)
    {
        if(Plotter::getInstance().shouldDraw())
            Plotter::getInstance().drawPlot(neuralNet);
        Plotter::getInstance().handleEvents();
    };
    running = false;
    regressionThread.join();
    cout << "Exiting program" << endl;
    return 0;
}