#include "Regression.h"

void updateNetworkWeights(NeuralNet& neuralNet);
void calcScaledTotalGrad(NeuralNet& neuralNet);
void calcGradFromDatum(const Datum& input, NeuralNet& neuralNet);
void gradFromOutputLayer(const Datum& input, std::vector<double>& singleGradient, NeuralNet& neuralNet);
void gradFromSecondHidden(const Datum& input, std::vector<double>& singleGradient, NeuralNet& neuralNet);
void gradFromFirstHidden(Datum input, std::vector<double>& singleGradient, NeuralNet& neuralNet);
std::vector<double> multiplyMatrices(std::vector<double>& leftMatrix, std::vector<double>& rightMatrix, NeuralNet& neuralNet);

using namespace std;

Datum::Datum(double lDist, double fDist, double rDist, Direction direction) :
    fDist(fDist), lDist(lDist), rDist(rDist), direction(direction)
    {}

bool running = true;
static double scale = 1;
vector<Datum> dataVector;
static vector<double> weights;
static vector<double> gradient;
static vector<double> singleGradient;

void gradientDescentLoop(NeuralNet& neuralNet)
{

    while (running)
    {
        cout <<"Continuing, not converged yet..." << endl;
        for (int iteration = 1; iteration <= 100; iteration++)
        {
            getNetworkWeights(neuralNet);
            scale = 1.0/50;
            calcScaledTotalGrad(neuralNet);
            for (int i = 0; i < weights.size(); i++)
                weights[i] += gradient[i];
            updateNetworkWeights(neuralNet);
        }
        cout << "Saving Weights..." << endl;
        neuralNet.saveWeights("weights.dat");
    }
    cout << "Regression has stopped." << endl;
}

void generateData()
{
    dataVector.reserve(75*75*75);
    
    for (int lDist = 1; lDist <= 150; lDist += 4)
    {
        for (int rDist = 1; rDist <= 150; rDist += 4)
        {
            for (int fDist = 1; fDist <= 150; fDist += 4)
            {
                if (fDist > 100)
                    dataVector.push_back(Datum(lDist, fDist, rDist, Direction::forward));
                if (fDist < 50 && rDist > lDist)
                    dataVector.push_back(Datum(lDist, fDist, rDist, Direction::right));
                if (fDist < 50 && lDist > rDist)
                    dataVector.push_back(Datum(lDist, fDist, rDist, Direction::left));
            }
        }
    }
}

void getNetworkWeights(NeuralNet& neuralNet)
{
    weights.clear();
    for_each(begin(neuralNet.layers), end(neuralNet.layers),
    [](HiddenLayer& layer){
        for (int neuron = 0; neuron < layer.numberOfNeurons; neuron++)
        {
            for (int weight = 0; weight < layer.numberOfInputs + 1; weight++)
            {
                weights.push_back(layer.weightMatrix(neuron, weight));
            }
        }
    });
    for (int neuron = 0; neuron < neuralNet.outputLayer.numberOfNeurons; neuron++)
    {
        for (int weight = 0; weight < neuralNet.outputLayer.numberOfInputs + 1; weight++)
        {
            weights.push_back(neuralNet.outputLayer.weightMatrix(neuron, weight));
        }
    }
}

void updateNetworkWeights(NeuralNet& neuralNet)
{
    for_each(begin(neuralNet.layers), end(neuralNet.layers),
    [](HiddenLayer& layer){
        for (int neuron = 0; neuron < layer.numberOfNeurons; neuron++)
        {
            for (int weight = 0; weight < layer.numberOfInputs + 1; weight++)
            {
                layer.weightMatrix(neuron, weight) = weights[0];
                weights.erase(weights.begin());
            }
        }
    });
    for (int neuron = 0; neuron < neuralNet.outputLayer.numberOfNeurons; neuron++)
    {
        for (int weight = 0; weight < neuralNet.outputLayer.numberOfInputs + 1; weight++)
        {
            neuralNet.outputLayer.weightMatrix(neuron, weight) = weights[0];
            weights.erase(weights.begin());
        }
    }
}

void calcScaledTotalGrad(NeuralNet& neuralNet)
{
    gradient = vector<double>(weights.size());
    neuralNet.lock.lock();          
    for_each(begin(dataVector), end(dataVector),
     [&](Datum& datum){
        calcGradFromDatum(datum, neuralNet);
        for (int i = 0; i < singleGradient.size(); i++){
            gradient[i] += singleGradient[i];}
    });
    neuralNet.lock.unlock();
    double normalisation = 0;
    for_each(gradient.begin(), gradient.end(),
    [&](double x){
        normalisation += x * x;
    });
    cout << "Norm squared is now " << normalisation << endl;;
    if (normalisation < 0.01)
        running = false;
    normalisation = 1 / sqrt(normalisation);
    for (int i = 0; i < gradient.size(); i++)
    {                
        gradient[i] *= scale * normalisation;
    }
}        

//Only works for exactly 2 hidden layers.
void calcGradFromDatum(const Datum& input, NeuralNet& neuralNet)
{    
    singleGradient.clear();
    vector<double> layerGrad;
    neuralNet.processInput(vector<double> { 0.01 * input.lDist, 0.01 * input.fDist, 0.01 * input.rDist});

    gradFromFirstHidden(input, singleGradient, neuralNet);
    gradFromSecondHidden(input, singleGradient, neuralNet);    
    gradFromOutputLayer(input, singleGradient, neuralNet);
}

void gradFromOutputLayer(const Datum& input, vector<double>& singleGradient, NeuralNet& neuralNet)
{
    vector<double> layerGrad(neuralNet.numberOfOutputs * (neuralNet.neuronsPerLayer + 1));

    for (int i = 0; i < neuralNet.numberOfOutputs; i++)
    {
        for (int j = 0; j < neuralNet.neuronsPerLayer + 1; j++)
        {
            layerGrad[i*(neuralNet.neuronsPerLayer + 1) + j] = neuralNet.outputLayer.dydw
                (input.direction, i, j);
        }
    }
    for_each(layerGrad.begin(), layerGrad.end(), 
    [&](double x){
        singleGradient.push_back(x);});    
}

void gradFromSecondHidden(const Datum& input,  vector<double>& singleGradient, NeuralNet& neuralNet)
{
    vector<double> layerGrad(neuralNet.neuronsPerLayer * (neuralNet.neuronsPerLayer + 1));
    for (int i = 0; i < neuralNet.neuronsPerLayer; i++)
    {
        for (int j = 0; j < neuralNet.neuronsPerLayer + 1; j++)
        {
            for (int k = 0; k < neuralNet.neuronsPerLayer; k++)
                layerGrad[i*(neuralNet.neuronsPerLayer + 1) + j] += neuralNet.outputLayer.dydx(input.direction, k) 
                    * neuralNet.layers[1].dydw(k, i, j);
        }
    }
    
    for_each(layerGrad.begin(), layerGrad.end(), 
    [&](double x){
        singleGradient.push_back(x);});
}

void gradFromFirstHidden(Datum input, vector<double>& singleGradient, NeuralNet& neuralNet)
{
    vector<double> layerGrad(neuralNet.neuronsPerLayer * (neuralNet.numberOfInputs + 1));
    for (int i = 0; i < neuralNet.neuronsPerLayer; i++)
    {
        for (int j = 0; j < neuralNet.numberOfInputs + 1; j++)
        {
            for (int k = 0; k < neuralNet.neuronsPerLayer; k++)
                for(int l = 0; l < neuralNet.neuronsPerLayer; l++){
                    layerGrad[i*(neuralNet.numberOfInputs + 1) + j] += 
                    neuralNet.outputLayer.dydx(input.direction, k) * neuralNet.layers[1].dydx(k, l) * neuralNet.layers[0].dydw(l, i, j);
                }
        }
    }
    
    for_each(layerGrad.begin(), layerGrad.end(), 
    [&](double x){
        singleGradient.push_back(x);}); 
}
