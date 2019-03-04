#pragma once
#include <iostream>
#include <vector>
#include "..\Matrix\Matrix.h"

struct NeuralLayer
{
	friend class NeuralNet;

	NeuralLayer();
	NeuralLayer(const int inputs, const int neurons);
	std::vector<double> inputVector;
	std::vector<double> outputVector;	
	
	const int numberOfInputs;
	const int numberOfNeurons;
	Matrix<double> weightMatrix;
	Matrix<double> dydx;
	Tensor<double> dydw;

	void generateRandomWeights();
};

struct HiddenLayer : NeuralLayer
{
	HiddenLayer();
	HiddenLayer(const int inputs, const int neurons);
	void calcDyDx();
	void calcDyDw();
	void processInput();

private:
	double sigmoid(double x);

};

struct OutputLayer : NeuralLayer
{
	OutputLayer();
	OutputLayer(const int inputs, const int neurons);
	void calcDlnyDx();
	void calcDlnyDw();
	void processInput();

private:
	void vectorSigmoid(std::vector<double>& input);
};

