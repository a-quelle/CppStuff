#include "NeuralNet.h"
#include <fstream>

using namespace std;

NeuralNet::NeuralNet(const int inputs, const int outputs, const int nLayers, const int neurons) :
	numberOfLayers(nLayers), numberOfInputs(inputs), numberOfOutputs(outputs), neuronsPerLayer(neurons), outputLayer(OutputLayer(neurons, outputs))
{	
	layers.reserve(numberOfLayers);
	createLayers();
}

void NeuralNet::createLayers()
{
	layers.push_back(HiddenLayer(numberOfInputs, neuronsPerLayer));
	for (int i = 1; i < numberOfLayers; i++) {
		layers.push_back(HiddenLayer(neuronsPerLayer, neuronsPerLayer));
	}
}

vector<double> NeuralNet::processInput(vector<double>& input)
{
	copy(input.data(), input.data() + numberOfInputs, layers[0].inputVector.data() + 1);
	layers[0].processInput();
	for (int i = 1; i < numberOfLayers; i++)
	{
		copy(layers[i-1].outputVector.data(), layers[i-1].outputVector.data() + neuronsPerLayer, layers[i].inputVector.data() + 1);
		layers[i].processInput();
	}
	copy(layers[numberOfLayers - 1].outputVector.data(), layers[numberOfLayers - 1].outputVector.data() + neuronsPerLayer, outputLayer.inputVector.data() + 1);
	outputLayer.processInput();
	return outputLayer.outputVector;
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


