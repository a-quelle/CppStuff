#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "..\Matrix\Matrix.h"

TEST_CASE( "Matrix creation and dereference", "[Matrix]" ) {
  Matrix<double> matrix(2,2);
  matrix(0,0) = 1;
  matrix(0,1) = 2;
  matrix(1,0) = 3;
  matrix(1,1) = 4;
  
  SECTION("Dereference done correctly"){
    REQUIRE( matrix.size() == 4 );
    REQUIRE( matrix[0] == 1);
    REQUIRE( matrix[1] == 2);
    REQUIRE( matrix[2] == 3);
    REQUIRE( matrix[3] == 4); 
  }

  matrix.clear();

  SECTION("Clear workds correctly"){
    REQUIRE( matrix.size() == 0 );
  }

}

TEST_CASE( "Tensor creation and dereference", "[Tensor]" ) {
  Tensor<double> tensor(2,2,2);
  tensor(0,0,0) = 1;
  tensor(0,0,1) = 2;
  tensor(0,1,0) = 3;
  tensor(0,1,1) = 4;
  tensor(1,0,0) = 5;
  tensor(1,0,1) = 6;
  tensor(1,1,0) = 7;
  tensor(1,1,1) = 8;
  
  SECTION("Dereference done correctly"){
    REQUIRE( tensor.size() == 8 );
    REQUIRE( tensor[0] == 1);
    REQUIRE( tensor[1] == 2);
    REQUIRE( tensor[2] == 3);
    REQUIRE( tensor[3] == 4);
    REQUIRE( tensor[4] == 5);
    REQUIRE( tensor[5] == 6);
    REQUIRE( tensor[6] == 7);
    REQUIRE( tensor[7] == 8);
  }

  tensor.clear();

  SECTION("Clear workds correctly"){
    REQUIRE( tensor.size() == 0 );
  }

}