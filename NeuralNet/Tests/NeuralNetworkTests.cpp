#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "..\NeuralNetwork\NeuralLayer.h"
#include "..\NeuralNetwork\NeuralNet.h"

double* e1 = new double[3]{1,0,0};
double* e2 = new double[3]{0,1,0};
double* e3 = new double[3]{0,0,1};

TEST_CASE( "All HiddenLayer stuff", "[HiddenLayer]" ) {
  HiddenLayer hiddenLayer(e1, 2,2);

  SECTION( "vectors are correct size" ) {        
        REQUIRE( hiddenLayer.weightMatrix.size() == 6 );
    }

    hiddenLayer.weightMatrix[0] = 0;
    hiddenLayer.weightMatrix[1] = 1;
    hiddenLayer.weightMatrix[2] = 0;
    hiddenLayer.weightMatrix[3] = 1;
    hiddenLayer.weightMatrix[4] = 0;
    hiddenLayer.weightMatrix[5] = 0;

    SECTION( "affine input processed correctly" ) {    
        hiddenLayer.processInput();
        REQUIRE( hiddenLayer.outputs[0] == 1);
        REQUIRE( hiddenLayer.outputs[1] == 1 / (1 + exp(0)) );
        REQUIRE( hiddenLayer.outputs[2] == 1 / (1 + exp(-1)) );
    }
    SECTION( "linear input processed correctly" ) {        
        hiddenLayer.inputs = e2;
        hiddenLayer.processInput();
        REQUIRE( hiddenLayer.outputs[0] == 1);
        REQUIRE( hiddenLayer.outputs[1] == 1 / (1 + exp(-1)) );
        REQUIRE( hiddenLayer.outputs[2] == 1 / (1 + exp(0)) );
    }
            
    hiddenLayer.inputs = e2;
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
  OutputLayer outputLayer(e1, 2,2);

  SECTION( "vectors are correct size" ) {        
        REQUIRE( outputLayer.weightMatrix.size() == 6 );
    }
 
    outputLayer.weightMatrix[0] = 0;
    outputLayer.weightMatrix[1] = 1;
    outputLayer.weightMatrix[2] = 0;
    outputLayer.weightMatrix[3] = 1;
    outputLayer.weightMatrix[4] = 0;
    outputLayer.weightMatrix[5] = 0;

    SECTION( "affine input processed correctly" ) {   
        outputLayer.processInput();
        //Check that floating point errors are smaller than 10^(-15)
        REQUIRE( outputLayer.outputs[0] == 1);
        REQUIRE( abs(outputLayer.outputs[1] - exp(0) / ( exp(1) + exp(0))) < 1./1000000000000000 );
        REQUIRE( abs(outputLayer.outputs[2] - exp(1) / ( exp(1) + exp(0))) < 1./1000000000000000 );
    }
    SECTION( "linear input processed correctly" ) {        
        outputLayer.inputs = e2;
        outputLayer.processInput();
        //Check that floating point errors are smaller than 10^(-15)
        REQUIRE( outputLayer.outputs[0] == 1);
        REQUIRE( abs(outputLayer.outputs[1] -  exp(1) / ( exp(1) + exp(0))) < 1./1000000000000000 );
        REQUIRE( abs(outputLayer.outputs[2] - exp(0) / ( exp(1) + exp(0))) < 1./1000000000000000 );
    }
            
    outputLayer.inputs = e2;
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
    double* inputs = new double[3]{1,1,2};
    neuralNet.processInput(inputs); //leading 1 for affine inputs

    SECTION("Network processes input correctly"){
        REQUIRE(neuralNet.layers[0].inputs[0] == 1);
        REQUIRE(neuralNet.layers[0].inputs[1] == 1);
        REQUIRE(neuralNet.layers[0].inputs[2] == 2);
        REQUIRE(neuralNet.layers[1].inputs[0] == 1);
        REQUIRE(neuralNet.layers[1].inputs[0] == neuralNet.layers[0].outputs[0]);
        REQUIRE(neuralNet.layers[1].inputs[1] == neuralNet.layers[0].outputs[1]);
        REQUIRE(neuralNet.layers[1].inputs[2] == neuralNet.layers[0].outputs[2]);
        REQUIRE(neuralNet.layers[1].inputs[3] == neuralNet.layers[0].outputs[3]);
        REQUIRE(neuralNet.outputLayer.inputs[0] == 1);
        REQUIRE(neuralNet.outputLayer.inputs[0] == neuralNet.layers[1].outputs[0]);
        REQUIRE(neuralNet.outputLayer.inputs[1] == neuralNet.layers[1].outputs[1]);
        REQUIRE(neuralNet.outputLayer.inputs[2] == neuralNet.layers[1].outputs[2]);
        REQUIRE(neuralNet.outputLayer.inputs[3] == neuralNet.layers[1].outputs[3]);
    }
}