#pragma once
#include <iostream>
#include <vector>
#include "..\Matrix\Matrix.h"

struct NeuralLayer
{
	friend class NeuralNet;

	NeuralLayer();
	NeuralLayer(double* inputPtr, const int inputs, const int neurons);
	NeuralLayer(NeuralLayer&  other) = delete;
	NeuralLayer(NeuralLayer&&  other);
	~NeuralLayer();
	double* inputs = nullptr;
	double* outputs = nullptr;	
	
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
	HiddenLayer(double* inputPtr, const int inputs, const int neurons);
	void calcDyDx();
	void calcDyDw();
	void processInput();

private:
	double sigmoid(double x);

};

struct OutputLayer : NeuralLayer
{
	OutputLayer();
	OutputLayer(double* inputPtr, const int inputs, const int neurons);
	void calcDlnyDx();
	void calcDlnyDw();
	void processInput();

private:
	void vectorSigmoid(double* input, int size);
};

