#include "NeuralLayer.h"
#include <random>

using namespace std;
random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
uniform_real_distribution<> dis(0.0, 1.0);

NeuralLayer::NeuralLayer() :
	numberOfInputs(0), numberOfNeurons(0)
{}

NeuralLayer::NeuralLayer(const int inputs, const int neurons) : 
	numberOfInputs(inputs), numberOfNeurons(neurons)
{
	inputVector.resize(inputs + 1);
	inputVector[0] = 1;
	outputVector.resize(neurons);
	weightMatrix.resize(neurons, inputs + 1);
	dydx.resize(neurons, inputs);
	dydw.resize(neurons, neurons, inputs + 1);
	generateRandomWeights();
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
			dydx(output, input) = outputVector[output] * weightMatrix(output, input+1) * (1-outputVector[output]);
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
					dydw(output, weightOutput, weightInput) = outputVector[output] * inputVector[weightInput] * (1 - outputVector[weightOutput]);
				}else{
					dydw(output, weightOutput, weightInput) = 0;
				}			                       
			}
		}
	}
}

HiddenLayer::HiddenLayer(){};
HiddenLayer::HiddenLayer(const int inputs, const int neurons) : NeuralLayer(inputs, neurons){};

double HiddenLayer::sigmoid(double x)
{
	double y = 1 / (1 + exp(-x));
	return y;
}

void HiddenLayer::processInput()
{
	for (int i = 0; i < numberOfNeurons; i++)
	{
		outputVector[i] = 0;
		for(int j = 0; j < numberOfInputs + 1; j++)
		{
			outputVector[i] += weightMatrix(i, j)*inputVector[j];
		}
		outputVector[i] = sigmoid(outputVector[i]);
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
					dydx(output, input) += weightMatrix(neuron, input+1) * (1 - outputVector[neuron]);
				}
				else
				{
					dydx(output, input) -= weightMatrix(neuron, input+1) * (outputVector[neuron]);
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
					dydw(output, weightOutput, weightInput) += inputVector[weightInput] * (1 - outputVector[weightOutput]);
				}
				else
				{
					dydw(output, weightOutput, weightInput) -= inputVector[weightInput] * (outputVector[weightOutput]);
				}
			}
		}
	}
}

void OutputLayer::processInput()
{
	for (int neuron = 0; neuron < numberOfNeurons; neuron++)
	{
		outputVector[neuron] = 0;
		for (int input = 0; input < numberOfInputs + 1; input++)
		{
			outputVector[neuron] += weightMatrix(neuron, input) * inputVector[input];
		}                
	}
	vectorSigmoid(outputVector);
	calcDlnyDw();
	calcDlnyDx();
}

OutputLayer::OutputLayer(){};
OutputLayer::OutputLayer(const int numberOfInputs, const int numberOfNeurons) : NeuralLayer(numberOfInputs, numberOfNeurons)
{
}

void OutputLayer::vectorSigmoid(vector<double>& input)
        {           
            double sum = 0;
            double invSum;
            for(int i = 0; i < input.size(); i++)
                input[i] = exp(input[i]);
            for(int i = 0; i < input.size(); i++)
                sum += input[i];
            invSum = 1 / sum;
            for (int i = 0; i < numberOfNeurons; i++)
                input[i] = invSum * input[i];
        }