/** summax
 * Compute the maximum of each element in matrices A and B,
 * then sum the columns of the resulting matrix and output the
 * resulting vector C
 * @param A int** matrix m x k
 * @param B int** matrix m x k
 * @param m int number of rows of matrices A and B
 * @param k int number of columns of matrices A and B
 * @param C int* output vector 1 x k
 * @return 0 if success, 1 if error
 */
int summax(int **A, int **B, int m, int k, int *C);
