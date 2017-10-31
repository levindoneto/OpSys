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
 * @Parameters: Int**: Allocated matrix, Int: Size of the matrix
 * @Return: Void, it just cleans an allocated matrix
 */
void freeMatrix(int **matrix, int size);