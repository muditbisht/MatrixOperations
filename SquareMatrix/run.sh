g++ SquareMatrixMultiplication.cpp -o squarematrixmultiplication && python createMatrix.py $1 && time cat input.txt | ./squarematrixmultiplication  > output.txt
