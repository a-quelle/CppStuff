#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <vector>
#include "..\NeuralNetwork\NeuralLayer.h"
#include "..\NeuralNetwork\NeuralNet.h"
#include "..\Regression\Regression.h"

extern std::vector<Datum> dataVector;

TEST_CASE( "GenerateData", "[Regression]" ) {
    generateData();
    REQUIRE( dataVector[12].direction == Direction::forward );
    REQUIRE( dataVector[13].direction == Direction::right );
    REQUIRE( dataVector[26].direction == Direction::forward );
}

TEST_CASE("RegressionLoop", "[Regression]")
{
    NeuralNet neuralNet(3, 3, 2, 2); // 23 weights
    neuralNet.layers[0].weightMatrix[0] = 0;
    neuralNet.layers[0].weightMatrix[1] = 1;
    neuralNet.layers[0].weightMatrix[2] = 0;
    neuralNet.layers[0].weightMatrix[3] = 1;
    neuralNet.layers[0].weightMatrix[4] = 1;
    neuralNet.layers[0].weightMatrix[5] = 0;
    neuralNet.layers[0].weightMatrix[6] = 1;
    neuralNet.layers[0].weightMatrix[7] = 0;

    neuralNet.layers[1].weightMatrix[0] = 1;
    neuralNet.layers[1].weightMatrix[1] = 0;
    neuralNet.layers[1].weightMatrix[2] = 1;
    neuralNet.layers[1].weightMatrix[3] = 0;
    neuralNet.layers[1].weightMatrix[4] = 1;
    neuralNet.layers[1].weightMatrix[5] = 0;

    neuralNet.outputLayer.weightMatrix[0] = 0;
    neuralNet.outputLayer.weightMatrix[1] = 1;
    neuralNet.outputLayer.weightMatrix[2] = 0;
    neuralNet.outputLayer.weightMatrix[3] = 1;
    neuralNet.outputLayer.weightMatrix[4] = 1;
    neuralNet.outputLayer.weightMatrix[5] = 0;
    neuralNet.outputLayer.weightMatrix[6] = 1;
    neuralNet.outputLayer.weightMatrix[7] = 0;
    neuralNet.outputLayer.weightMatrix[8] = 1;

    setDataVector(std::vector<Datum>({Datum(1, 0, 0, Direction::forward)}));

    gradientDescentLoop(neuralNet, 1);
}