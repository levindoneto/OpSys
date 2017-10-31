#include <stdio.h>
#include <stdlib.h>

/* Function that calculates the sum of the max elements based on two matrices
 * @Parameters: Matrices: inputA, inputB with a random number of columns and vectors
 * @Return: Array: outputC, with the sum of the max elements per column
 */
int **summax (int size, int **inputA, int **inputB);

/* Function that generates a random size value from MIN to MAX. It's used for
 * defining the number of rows and columns of the input matrixes for sum-max
 * @Parameters: Int: Min value, Max Value
 * @Return: Int: Random value
 */
int generateRandomValue (int min, int max);

/* Create a matrix with random values
 * @Parameters: Int: Size (number of columns and rows of the matrix),
 				minimum and maximum value
 * @Return: **Int: Matrix
 */
int **createRandomMatrix (int size,  int min_element_value, int max_element_value);

/* Free memory for an allocated matrix
 * @Parameters: Int**: Allocated matrix, Int: Number of columns and rows
 * @Return: Void, it just cleans an allocated matrix
 */
void freeMatrix(int **matrix, int size);

/* Free memory for an allocated matrix of output (only one row)
 * @Parameters: Int**: Allocated matrix, Int: Number of columns and rows
 * @Return: Void, it just cleans an allocated matrix
 */
void freeOutput(int **matrix, int size);

/* It returns the biggest element between two elements from two matrices
 * @Parameters: Int: element from the 1st matrix, element from the 2nd matrix
 * @Return: **Int: Biggest element
 */
int maxElement (int elementA, int elementB);

/* It creates a matrix with the maximum elements from two input matrices
 * @Parameters: Int**: Matrix A, Matrix B
 * @Return: Int**: Matriz D
 */
int **createMaxMatrix (int **matrixA, int **matrixB, int size);

/* It create a matrix with one row and with the columns containing the sum of
 * the elements from each column of the maximum elements' matrix
 * @Parameters: Int**: Maximum elements' matrix, Int: Number of rows and columns
 * @Return: Int**: Matrix with the sum of columns' elements from the max matrix
 */
int **createSumMatrix (int **maxMatrix, int size);