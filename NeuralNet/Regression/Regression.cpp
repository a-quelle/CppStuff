#include "Regression.h"
#include <ctime>

int getNumberOfWeights(NeuralNet& neuralNet);
void updateNetworkWeights(NeuralNet& neuralNet);
void calcScaledTotalGrad(NeuralNet& neuralNet);
void calcGradFromDatum(const Datum& input, NeuralNet& neuralNet);
void gradFromOutputLayer(const Datum& input, double*& singleGradient, NeuralNet& neuralNet);
void gradFromSecondHidden(const Datum& input, double*& singleGradient, NeuralNet& neuralNet);
void gradFromFirstHidden(Datum input, double*& singleGradient, NeuralNet& neuralNet);

using namespace std;

Datum::Datum(double lDist, double fDist, double rDist, Direction direction) :
    fDist(fDist), lDist(lDist), rDist(rDist), direction(direction)
    {}

bool running = true;
static double scale = 1./10000;
vector<Datum> dataVector;
int weightsSize = 0;
static double* weights;
static double* gradient;
static double* singleGradient;

void gradientDescentLoop(NeuralNet& neuralNet, int batches)
{
    weightsSize = getNumberOfWeights(neuralNet);
    weights = new double[weightsSize];
    gradient = new double[weightsSize];
    singleGradient = new double[weightsSize];
    getNetworkWeights(neuralNet);
    while (running)
    {
        cout <<"Continuing, not converged yet..." << endl;
        for (int batch = 1; batch <= batches; batch++)
        {
            std::clock_t start;
            double duration;

            start = std::clock();
            calcScaledTotalGrad(neuralNet);
            for (int i = 0; i < weightsSize; i++)
                weights[i] += gradient[i];
            updateNetworkWeights(neuralNet);

            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << "This batch took " << duration << endl;
        }
        cout << "Saving Weights..." << endl;
        neuralNet.saveWeights("weights.dat");
    }
    cout << "Regression has stopped." << endl;
    delete[] weights;
    delete[] gradient;
    delete[] singleGradient;
}

void setDataVector(std::vector<Datum> data)
{
    dataVector = data;
}

void generateData()
{
    dataVector.reserve(50*50*25);
    
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

int getNumberOfWeights(NeuralNet& neuralNet)
{
    int numberOfWeights = 0;
    for_each(begin(neuralNet.layers), end(neuralNet.layers),
    [&](HiddenLayer& layer){
        numberOfWeights += layer.numberOfNeurons * (layer.numberOfInputs+1);
    });
    OutputLayer& outputLayer = neuralNet.outputLayer;
    numberOfWeights += outputLayer.numberOfNeurons * (outputLayer.numberOfInputs+1);
    return numberOfWeights;
}

void getNetworkWeights(NeuralNet& neuralNet)
{
    memset(weights, 0, weightsSize*sizeof(double));
    double* weightsCopy = weights;
    for_each(begin(neuralNet.layers), end(neuralNet.layers),
    [&](HiddenLayer& layer){
        memcpy(weightsCopy, layer.weightMatrix.getData(), layer.weightMatrix.size()*sizeof(double));
        weightsCopy += layer.weightMatrix.size();
    });
    memcpy(weightsCopy, neuralNet.outputLayer.weightMatrix.getData(), neuralNet.outputLayer.weightMatrix.size()*sizeof(double));
}

void updateNetworkWeights(NeuralNet& neuralNet)
{
    double* weightsPtr = weights;
    for_each(begin(neuralNet.layers), end(neuralNet.layers),
    [&weightsPtr](HiddenLayer& layer){
        for (int neuron = 0; neuron < layer.numberOfNeurons; neuron++)
        {
            for (int weight = 0; weight < layer.numberOfInputs + 1; weight++)
            {
                layer.weightMatrix(neuron, weight) = *weightsPtr;
                ++weightsPtr;
            }
        }
    });
    for (int neuron = 0; neuron < neuralNet.outputLayer.numberOfNeurons; neuron++)
    {
        for (int weight = 0; weight < neuralNet.outputLayer.numberOfInputs + 1; weight++)
        {
            neuralNet.outputLayer.weightMatrix(neuron, weight) = *weightsPtr;
            ++weightsPtr;
        }
    }
}

void calcScaledTotalGrad(NeuralNet& neuralNet)
{
    memset(gradient, 0, weightsSize*sizeof(double));
    //neuralNet.lock.lock();
    double logLoss = 0;      
    for_each(begin(dataVector), end(dataVector),
     [&](Datum& datum){
        calcGradFromDatum(datum, neuralNet);
        for (int i = 0; i < weightsSize; i++){
            gradient[i] += singleGradient[i];}
        logLoss -= log(neuralNet.outputLayer.outputs[datum.direction+1]);
    });
    logLoss /= dataVector.size();
    //neuralNet.lock.unlock();
    double normalisation = 0;
    for(int i = 0; i < weightsSize; ++i )
    {
        int x = gradient[i];
        normalisation += x*x;
    }
    cout << "Loss is now " << logLoss << endl;;
    cout << "Norm squared is now " << normalisation << endl;;
    if (normalisation < 0.01)
        running = false;
    normalisation = 1 / sqrt(normalisation);
    for (int i = 0; i < weightsSize; i++)
    {                
        gradient[i] *= scale;
    }
}        

//Only works for exactly 2 hidden layers.
void calcGradFromDatum(const Datum& input, NeuralNet& neuralNet)
{    
    memset(singleGradient, 0, weightsSize*sizeof(double));
    vector<double> layerGrad;
    double toProcess[4] = { 1,  0.01 * input.lDist, 0.01 * input.fDist, 0.01 * input.rDist}; //leading 1 for affine inputs
    neuralNet.processInput(toProcess);

    double* singGradCpy = singleGradient;

    gradFromFirstHidden(input, singGradCpy, neuralNet);
    gradFromSecondHidden(input, singGradCpy, neuralNet);    
    gradFromOutputLayer(input, singGradCpy, neuralNet);
}

void gradFromOutputLayer(const Datum& input, double*& singleGradient, NeuralNet& neuralNet)
{
    int iSize = neuralNet.numberOfOutputs;
    int jSize = neuralNet.neuronsPerLayer + 1;
    double* yw = &neuralNet.outputLayer.dydw(input.direction,0,0);
       
    for (int i = 0; i < iSize*jSize; i++)
    {
        singleGradient[i] = yw[i];  
    }
}

void gradFromSecondHidden(const Datum& input,  double*& singleGradient, NeuralNet& neuralNet)
{
    const int lineSize = 16;
    const int iSize = neuralNet.neuronsPerLayer;
    const int jSize = neuralNet.neuronsPerLayer + 1;
    //Tiled algorithm
    double* outLayer = &neuralNet.outputLayer.dydx(input.direction, 0);
    Tensor<double>& hidLayer = neuralNet.layers[1].dydw;
    for(int jb = 0, jBound = iSize*jSize; jBound > 0; jb += lineSize, jBound -= lineSize)
    { 
        double* res = &singleGradient[jb]; 
        if(jBound >= lineSize)
        {                  
            for (int k = 0; k < iSize; k++)
            {                 
                double yx = outLayer[k], *yw = &hidLayer(k,0,jb);
                for (int j = 0; j < lineSize; j++)
                {
                    res[j] +=  yx * yw[j];
                }                    
            } 
        }  
        else
        {                  
            for (int k = 0; k < iSize; k++)
            {                 
                double yx = outLayer[k], *yw = &hidLayer(k,0,jb);
                for (int j = 0; j < jBound; j++)
                {
                    res[j] +=  yx * yw[j];
                }                    
            }             
        }                 
    }
    

    //Naive algorithm   
    // for (int i = 0; i < iSize; i++)
    // {
    //     for (int j = 0; j < jSize; j++)
    //     {
    //             for (int k = 0; k < iSize; k++)
    //         {
    //             singleGradient[i*jSize + j] += neuralNet.outputLayer.dydx(input.direction, k) 
    //                 * neuralNet.layers[1].dydw(k, i, j);
    //         }
    //     }
    // }

    singleGradient += iSize*jSize; 
}

void gradFromFirstHidden(Datum input, double*& singleGradient, NeuralNet& neuralNet)
{
    const int lineSize = 16;
    const int iSize = neuralNet.neuronsPerLayer;
    const int jSize = neuralNet.numberOfInputs + 1;

    double* outLayer = &neuralNet.outputLayer.dydx(input.direction, 0);
    Matrix<double>& hidLayer1 = neuralNet.layers[1].dydx;
    Tensor<double>& hidLayer0 = neuralNet.layers[0].dydw;
    
    for(int lb = 0, lBound = iSize; lBound > 0; lb+= lineSize, lBound -= lineSize)
    {
        if(lBound >= lineSize)
        {
            double* blockyx = &hidLayer1[lb];
            for (int jb = 0, jBound = iSize*jSize; jBound > 0; jb+=lineSize, jBound -= lineSize)
            {
                double* blockGrad = &singleGradient[jb];
                if(jBound >= lineSize)
                {
                    for(int l = 0; l < lineSize; l++)
                    {
                        double* yw = &hidLayer0(l+lb,0,0)+jb;
                        double outXyx = 0;
                        for (int k = 0; k < iSize; k++)
                        {
                            outXyx += outLayer[k]*blockyx[k*iSize+l];
                        }                      
                        for(int j = 0; j < lineSize; j++)
                        {
                            blockGrad[j] += outXyx * yw[j];
                        }
                    }
                }
                else
                {
                    for(int l = 0; l < lineSize; l++)
                    {
                        double* yw = &hidLayer0(l+lb,0,0)+jb;
                        double outXyx = 0;
                        for (int k = 0; k < iSize; k++)
                        {
                            outXyx += outLayer[k]*blockyx[k*iSize+l];
                        }                      
                        for(int j = 0; j < jBound; j++)
                        {
                            blockGrad[j] += outXyx * yw[j];
                        }
                    }                   
                }
            }
        }
        else
        {
            double* blockyx = &hidLayer1[lb];
            for (int jb = 0, jBound = iSize*jSize; jBound > 0; jb+=lineSize, jBound -= lineSize)
            {
                double* blockGrad = &singleGradient[jb];
                if(jBound >= lineSize)
                {
                    for(int l = 0; l < lBound; l++)
                    {
                        double* yw = &hidLayer0(l+lb,0,0)+jb;
                        double outXyx = 0;
                        for (int k = 0; k < iSize; k++)
                        {
                            outXyx += outLayer[k]*blockyx[k*iSize+l];
                        }                      
                        for(int j = 0; j < lineSize; j++)
                        {
                            blockGrad[j] += outXyx * yw[j];
                        }
                    }
                }
                else
                {
                    for(int l = 0; l < lBound; l++)
                    {
                        double* yw = &hidLayer0(l+lb,0,0)+jb;
                        double outXyx = 0;
                        for (int k = 0; k < iSize; k++)
                        {
                            outXyx += outLayer[k]*blockyx[k*iSize+l];
                        }                      
                        for(int j = 0; j < jBound; j++)
                        {
                            blockGrad[j] += outXyx * yw[j];
                        }
                    }                  
                }
            }
        }        
    }

    // for (int i = 0; i < iSize; i++)
    // {
    //     for (int j = 0; j < jSize; j++)
    //     {
    //         for (int k = 0; k < iSize; k++)
    //         {
    //             for(int l = 0; l < iSize; l++)
    //             {
    //                 singleGradient[i*jSize + j] += 
    //                 neuralNet.outputLayer.dydx(input.direction, k) * neuralNet.layers[1].dydx(k, l) * neuralNet.layers[0].dydw(l, i, j);
    //             }
    //         }            
    //     }
    // }
    singleGradient += iSize*jSize; 
}
