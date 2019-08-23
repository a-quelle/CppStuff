#include "NeuralLayer.h"
#include <random>

using namespace std;
random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
uniform_real_distribution<> dis(0.0, 1.0);

NeuralLayer::NeuralLayer() :
	numberOfInputs(0), numberOfNeurons(0)
{}

NeuralLayer::NeuralLayer(double* inputPtr, const int nInputs, const int nNeurons) : 
	numberOfInputs(nInputs), numberOfNeurons(nNeurons)
{
	inputs = inputPtr;
	outputs = new double[nNeurons+1]();
	outputs[0] = 1;
	weightMatrix.resize(nNeurons, nInputs + 1);
	dydx.resize(nNeurons, nInputs);
	dydw.resize(nNeurons, nNeurons, nInputs + 1);
	generateRandomWeights();
}

NeuralLayer::NeuralLayer(NeuralLayer&& other): numberOfInputs(other.numberOfInputs), numberOfNeurons(other.numberOfNeurons), weightMatrix(std::move(other.weightMatrix)),
	dydx(std::move(other.dydx)), dydw(std::move(other.dydw))
{
	inputs = other.inputs;
	outputs = other.outputs;
	other.outputs = nullptr;		
}

NeuralLayer::~NeuralLayer(void)
{
	delete[] outputs;
}



void NeuralLayer::generateRandomWeights()
{
	for ( int i = 0; i < numberOfNeurons; i++)
	{
		for (int j = 0; j < numberOfInputs + 1; j++) {
			weightMatrix(i,j) = dis(gen) - dis(gen);
		}
	}
}

void HiddenLayer::calcDyDx()
{            
	for(int output = 0; output < numberOfNeurons; output++)
	{
		for (int input = 0; input < numberOfInputs; input++)
		{
			dydx(output, input) = outputs[output+1] * weightMatrix(output, input+1) * (1-outputs[output+1]);
		}   
	}
}

void HiddenLayer::calcDyDw()
{
	for (int output = 0; output < numberOfNeurons; output++)
	{
		for (int weightOutput = 0; weightOutput < numberOfNeurons; weightOutput++)
		{
			for (int weightInput = 0; weightInput < numberOfInputs+1; weightInput++)
			{				                        
				if (weightOutput == output)
				{
					dydw(output, weightOutput, weightInput) = outputs[output+1] * inputs[weightInput] * (1 - outputs[weightOutput+1]);
				}			                       
			}
		}
	}
}

HiddenLayer::HiddenLayer(){};
HiddenLayer::HiddenLayer(double* inputPtr, const int inputs, const int neurons) : NeuralLayer(inputPtr, inputs, neurons){};

double HiddenLayer::sigmoid(double x)
{
	double y = 1 / (1 + exp(-x));
	return y;
}

void HiddenLayer::processInput()
{
	for (int i = 0; i < numberOfNeurons; i++)
	{
		outputs[i + 1] = 0;
		for(int j = 0; j < numberOfInputs+1; j++)
		{
			outputs[i + 1] += weightMatrix(i, j)*inputs[j];
		}
		outputs[i + 1] = sigmoid(outputs[i + 1]);
	}
	calcDyDx();
	calcDyDw();
}

void OutputLayer::calcDlnyDx()
{
	for (int output = 0; output < numberOfNeurons; output++)
	{
		for (int input = 0; input < numberOfInputs; input++)
		{
			dydx(output, input) = 0;
			for (int neuron = 0; neuron < numberOfNeurons; neuron++)
			{
				if (neuron == output)
				{
					dydx(output, input) += weightMatrix(neuron, input+1) * (1 - outputs[neuron + 1]);
				}
				else
				{
					dydx(output, input) -= weightMatrix(neuron, input+1) * (outputs[neuron + 1]);
				}
			}
		}
	}
}

void OutputLayer::calcDlnyDw()
{
	for (int output = 0; output < numberOfNeurons; output++)
	{
		for (int weightOutput = 0; weightOutput < numberOfNeurons; weightOutput++)
		{
			for (int weightInput = 0; weightInput < numberOfInputs+1; weightInput++)
			{
				dydw(output, weightOutput, weightInput) = 0;
				if (weightOutput == output)
				{
					dydw(output, weightOutput, weightInput) += inputs[weightInput] * (1 - outputs[weightOutput + 1]);
				}
				else
				{
					dydw(output, weightOutput, weightInput) -= inputs[weightInput] * (outputs[weightOutput + 1]);
				}
			}
		}
	}
}

void OutputLayer::processInput()
{
	for (int neuron = 0; neuron < numberOfNeurons; neuron++)
	{
		outputs[neuron + 1] = 0;
		for (int input = 0; input < numberOfInputs + 1; input++)
		{
			outputs[neuron + 1] += weightMatrix(neuron, input) * inputs[input];
		}                
	}
	vectorSigmoid(outputs + 1, numberOfNeurons);
	calcDlnyDw();
	calcDlnyDx();
}

OutputLayer::OutputLayer(){};
OutputLayer::OutputLayer(double* inputPtr, const int numberOfInputs, const int numberOfNeurons) : NeuralLayer(inputPtr, numberOfInputs, numberOfNeurons)
{
}

void OutputLayer::vectorSigmoid(double* input, int size)
{           
	double sum = 0;
	double invSum;
	for(int i = 0; i < size; i++)
		input[i] = exp(input[i]);
	for(int i = 0; i < size; i++)
		sum += input[i];
	invSum = 1 / sum;
	for (int i = 0; i < size; i++)
		input[i] = invSum * input[i];
}