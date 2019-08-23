#include "NeuralNet.h"
#include <fstream>

using namespace std;

NeuralNet::NeuralNet(const int inputs, const int outputs, const int nLayers, const int neurons) :
	numberOfLayers(nLayers), numberOfInputs(inputs), numberOfOutputs(outputs), neuronsPerLayer(neurons), outputLayer(nullptr, neurons, outputs)
{	
	layers.reserve(numberOfLayers);
	createLayers();
	outputLayer.inputs = layers.back().outputs;
}

void NeuralNet::createLayers()
{
	layers.emplace_back(nullptr, numberOfInputs, neuronsPerLayer);
	for (int i = 1; i < numberOfLayers; i++) {		
		layers.emplace_back(layers.back().outputs, neuronsPerLayer, neuronsPerLayer);
	}
}

double* NeuralNet::processInput(double* input)
{
	layers[0].inputs = input;
	for (int i = 0; i < numberOfLayers; i++)
	{
		layers[i].processInput();
	}
	outputLayer.processInput();
	return outputLayer.outputs;
}

void NeuralNet::saveWeights(const std::string& fileName)
{
	vector<double> weights;
	for_each(begin(layers), end(layers),
	[&](HiddenLayer& layer){
		for (int neuron = 0; neuron < layer.numberOfNeurons; neuron++)
		{
			for (int weight = 0; weight < layer.numberOfInputs + 1; weight++)
			{
				weights.push_back(layer.weightMatrix[neuron*(layer.numberOfInputs + 1) + weight]);
			}
		}
	});
	for (int neuron = 0; neuron < outputLayer.numberOfNeurons; neuron++)
	{
		for (int weight = 0; weight < outputLayer.numberOfInputs + 1; weight++)
		{
			weights.push_back(outputLayer.weightMatrix[neuron*(outputLayer.numberOfInputs + 1) + weight]);
		}
	}
	std::ofstream f(fileName);
	for_each( weights.begin(), weights.end(),
		[&](double x){ f << x << "\n";});
}

void NeuralNet::readWeights(const std::string& fileName)
{
	string str;
	vector<double> input;
	ifstream f(fileName);
	if(f.is_open()){
		while(getline(f, str))
			input.push_back(stod(str));
		for_each(layers.begin(), layers.end(),
		[&](HiddenLayer& layer){
			for (int neuron = 0; neuron < layer.numberOfNeurons; neuron++)
			{
				for (int weight = 0; weight < layer.numberOfInputs + 1; weight++)
				{
					layer.weightMatrix[neuron*(layer.numberOfInputs + 1) + weight] = input[0];
					input.erase(input.begin());
				}
			}
		});
		for (int neuron = 0; neuron < outputLayer.numberOfNeurons; neuron++)
		{
			for (int weight = 0; weight < outputLayer.numberOfInputs + 1; weight++)
			{
				outputLayer.weightMatrix[neuron*(outputLayer.numberOfInputs + 1) + weight] = input[0];
				input.erase(input.begin());
			}
		}
	} else{
		cout << "Could not read weights from file." << endl;
	}
}


