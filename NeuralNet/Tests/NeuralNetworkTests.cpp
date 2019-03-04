#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "..\NeuralNetwork\NeuralLayer.h"
#include "..\NeuralNetwork\NeuralNet.h"

TEST_CASE( "All HiddenLayer stuff", "[HiddenLayer]" ) {
  HiddenLayer hiddenLayer(2,2);

  SECTION( "vectors are correct size" ) {        
        REQUIRE( hiddenLayer.weightMatrix.size() == 6 );
        REQUIRE( hiddenLayer.outputVector.size() == 2 );
        REQUIRE( hiddenLayer.inputVector.size() == 3 );
    }

    hiddenLayer.weightMatrix[0] = 0;
    hiddenLayer.weightMatrix[1] = 1;
    hiddenLayer.weightMatrix[2] = 0;
    hiddenLayer.weightMatrix[3] = 1;
    hiddenLayer.weightMatrix[4] = 0;
    hiddenLayer.weightMatrix[5] = 0;

    SECTION( "affine input processed correctly" ) {        
        hiddenLayer.inputVector = std::vector<double>{1,0,0};
        hiddenLayer.processInput();
        REQUIRE( hiddenLayer.outputVector[0] == 1 / (1 + exp(0)) );
        REQUIRE( hiddenLayer.outputVector[1] == 1 / (1 + exp(-1)) );
    }
    SECTION( "linear input processed correctly" ) {        
        hiddenLayer.inputVector = std::vector<double>{0,1,0};
        hiddenLayer.processInput();
        REQUIRE( hiddenLayer.outputVector[0] == 1 / (1 + exp(-1)) );
        REQUIRE( hiddenLayer.outputVector[1] == 1 / (1 + exp(0)) );
    }
            
    hiddenLayer.inputVector = std::vector<double>{0,1,0};
    hiddenLayer.processInput();

    SECTION( "dy/dx calculated correctly" ) {
        REQUIRE( hiddenLayer.dydx.size() == 4 );
        REQUIRE( hiddenLayer.dydx[0] == exp(-1) / (1 + exp(-1))/ (1 + exp(-1)) );
        REQUIRE( hiddenLayer.dydx[3] == 0 );
        REQUIRE( hiddenLayer.dydx[1] == 0);
        REQUIRE( hiddenLayer.dydx[2] == 0 );
    }
    SECTION( "dy/dw calculated correctly" ) {
        REQUIRE( hiddenLayer.dydw.size() == 12 );
        REQUIRE( hiddenLayer.dydw[0] == 0 );
        REQUIRE( hiddenLayer.dydw[1] == exp(-1) / (1 + exp(-1))/ (1 + exp(-1)) );
        REQUIRE( hiddenLayer.dydw[2] == 0 );
        REQUIRE( hiddenLayer.dydw[3] == 0 );
        REQUIRE( hiddenLayer.dydw[4] == 0 );
        REQUIRE( hiddenLayer.dydw[5] == 0 );
        REQUIRE( hiddenLayer.dydw[6] == 0);
        REQUIRE( hiddenLayer.dydw[7] == 0 );
        REQUIRE( hiddenLayer.dydw[8] == 0 );
        REQUIRE( hiddenLayer.dydw[9] == 0 );
        REQUIRE( hiddenLayer.dydw[10] == exp(0) / (1 + exp(0))/ (1 + exp(0)));
        REQUIRE( hiddenLayer.dydw[11] == 0 );
    }
}

TEST_CASE( "All OutputLayer stuff", "[OutputLayer]" ) {
  OutputLayer outputLayer(2,2);

  SECTION( "vectors are correct size" ) {        
        REQUIRE( outputLayer.weightMatrix.size() == 6 );
        REQUIRE( outputLayer.outputVector.size() == 2 );
        REQUIRE( outputLayer.inputVector.size() == 3 );
    }
 
    outputLayer.weightMatrix[0] = 0;
    outputLayer.weightMatrix[1] = 1;
    outputLayer.weightMatrix[2] = 0;
    outputLayer.weightMatrix[3] = 1;
    outputLayer.weightMatrix[4] = 0;
    outputLayer.weightMatrix[5] = 0;

    SECTION( "affine input processed correctly" ) {        
        outputLayer.inputVector = std::vector<double>{1,0,0};
        outputLayer.processInput();
        //Check that floating point errors are smaller than 10^(-15)
        REQUIRE( abs(outputLayer.outputVector[0] - exp(0) / ( exp(1) + exp(0))) < 1./1000000000000000 );
        REQUIRE( abs(outputLayer.outputVector[1] - exp(1) / ( exp(1) + exp(0))) < 1./1000000000000000 );
    }
    SECTION( "linear input processed correctly" ) {        
        outputLayer.inputVector = std::vector<double>{0,1,0};
        outputLayer.processInput();
        //Check that floating point errors are smaller than 10^(-15)
        REQUIRE( abs(outputLayer.outputVector[0] -  exp(1) / ( exp(1) + exp(0))) < 1./1000000000000000 );
        REQUIRE( abs(outputLayer.outputVector[1] - exp(0) / ( exp(1) + exp(0))) < 1./1000000000000000 );
    }
            
    outputLayer.inputVector = std::vector<double>{0,1,0};
    outputLayer.processInput();

    SECTION( "dy/dx calculated correctly" ) {
        REQUIRE( outputLayer.dydx.size() == 4 );
        REQUIRE( outputLayer.dydx[0] - 1 + exp(1) / ( exp(1) + exp(0)) < 1./1000000000000000 );
        REQUIRE( outputLayer.dydx[3] == 0 );
        REQUIRE( outputLayer.dydx[1] == 0);
        REQUIRE( outputLayer.dydx[2] + exp(1) / ( exp(1) + exp(0)) < 1./1000000000000000 );
    }
    SECTION( "dy/dw calculated correctly" ) {
        REQUIRE( outputLayer.dydw.size() == 12 );
        REQUIRE( outputLayer.dydw[0] == 0 );
        REQUIRE( outputLayer.dydw[1] - 1 + exp(1) / ( exp(1) + exp(0)) < 1./1000000000000000);
        REQUIRE( outputLayer.dydw[2] == 0 );
        REQUIRE( outputLayer.dydw[3] == 0 );
        REQUIRE( outputLayer.dydw[4] + exp(0) / ( exp(1) + exp(0)) < 1./1000000000000000);
        REQUIRE( outputLayer.dydw[5] == 0 );
        REQUIRE( outputLayer.dydw[6] == 0);
        REQUIRE( outputLayer.dydw[7] + exp(1) / ( exp(1) + exp(0))< 1./1000000000000000);
        REQUIRE( outputLayer.dydw[8] == 0 );
        REQUIRE( outputLayer.dydw[9] == 0 );
        REQUIRE( outputLayer.dydw[10] - 1 + exp(0) / ( exp(1) + exp(0))< 1./1000000000000000);
        REQUIRE( outputLayer.dydw[11] == 0 );
    }
}

TEST_CASE( "NeuralNet functions", "[NeuralNet]" ) {
    NeuralNet neuralNet(2,2,2,3);
    SECTION("Network filled with layers correctly"){
        REQUIRE(neuralNet.layers.size() == 2);
        REQUIRE(neuralNet.layers[0].numberOfInputs == 2);
        REQUIRE(neuralNet.layers[0].numberOfNeurons == 3);
        REQUIRE(neuralNet.layers[1].numberOfInputs == 3);
        REQUIRE(neuralNet.layers[1].numberOfNeurons == 3);
        REQUIRE(neuralNet.outputLayer.numberOfInputs == 3);
        REQUIRE(neuralNet.outputLayer.numberOfNeurons == 2);
    }

    neuralNet.layers[0].weightMatrix[0] = 0;
    neuralNet.layers[0].weightMatrix[1] = 1;
    neuralNet.layers[0].weightMatrix[2] = 0;
    neuralNet.layers[0].weightMatrix[3] = 1;
    neuralNet.layers[0].weightMatrix[4] = 0;
    neuralNet.layers[0].weightMatrix[5] = 0;

    neuralNet.outputLayer.weightMatrix[0] = 0;
    neuralNet.outputLayer.weightMatrix[1] = 1;
    neuralNet.outputLayer.weightMatrix[2] = 0;
    neuralNet.outputLayer.weightMatrix[3] = 1;
    neuralNet.outputLayer.weightMatrix[4] = 1;
    neuralNet.outputLayer.weightMatrix[5] = 0;
    neuralNet.outputLayer.weightMatrix[6] = 1;
    neuralNet.outputLayer.weightMatrix[7] = 0;
    neuralNet.processInput(std::vector<double>{1,2});

    SECTION("Network processes input correctly"){
        REQUIRE(neuralNet.layers[0].inputVector[0] == 1);
        REQUIRE(neuralNet.layers[0].inputVector[1] == 1);
        REQUIRE(neuralNet.layers[0].inputVector[2] == 2);
        REQUIRE(neuralNet.layers[1].inputVector[0] == 1);
        REQUIRE(neuralNet.layers[1].inputVector[1] == neuralNet.layers[0].outputVector[0]);
        REQUIRE(neuralNet.layers[1].inputVector[2] == neuralNet.layers[0].outputVector[1]);
        REQUIRE(neuralNet.layers[1].inputVector[3] == neuralNet.layers[0].outputVector[2]);
        REQUIRE(neuralNet.outputLayer.inputVector[0] == 1);
        REQUIRE(neuralNet.outputLayer.inputVector[1] == neuralNet.layers[1].outputVector[0]);
        REQUIRE(neuralNet.outputLayer.inputVector[2] == neuralNet.layers[1].outputVector[1]);
        REQUIRE(neuralNet.outputLayer.inputVector[3] == neuralNet.layers[1].outputVector[2]);
    }
}