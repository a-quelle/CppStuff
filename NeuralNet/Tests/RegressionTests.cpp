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