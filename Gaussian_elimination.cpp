#include <bits/stdc++.h>
using namespace std;
void ge(double* A, double* B, int row, int col);
void out(double* A, double *B, int col, int row);
int main() {
    int N = 5;
    double* matrix = (double*)malloc(N * sizeof(double));
    double* vector = (double*)malloc(N * sizeof(double));
    double* X0 = (double*)malloc(N * sizeof(double));
    for (int i = 0; i < N * N; i++) {
        cin >> matrix[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> vector[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> X0[i];
    }
    ge(matrix, vector, 5, 5);
    for (int i = 0; i < N; i++) {
        cout << X0[i] << endl;
    }
    system("pause");
    return 0;
}
/*
1 1 1 1 1
1 2 2 2 2
1 2 3 3 3
1 2 3 4 4
1 2 3 4 5

1 25 31 -4 5

1.4 -9 23 4 1.9
*/
void ge(double* A, double* B, int row, int col) {
    for (int pivot_row = 0; pivot_row < row; pivot_row++) {
        double scale = A[col * pivot_row + pivot_row];
        for (int pivot_col = 0; pivot_col < col; pivot_col++) {
            A[col * pivot_row + pivot_col] /= scale;
        }
        B[pivot_row] /= scale;
        for (int sub_row = pivot_row + 1; sub_row < row; sub_row++) {
            double elim_scale = A[sub_row * col + pivot_row];
            for (int k = pivot_row; k < col; k++) {
                A[col * sub_row + k] -= A[col * pivot_row + k] * elim_scale;
            }
            B[sub_row] -= B[pivot_row] * elim_scale;
        }
    }
    for (int pivot_row = row - 1; pivot_row > 0; pivot_row--) {
        for (int sub_row = pivot_row - 1; sub_row >= 0; sub_row--) {
            double elim_scale = A[sub_row * col + pivot_row];
            A[sub_row * col + pivot_row]-=elim_scale*A[pivot_row * col + pivot_row];
            B[sub_row] -= B[pivot_row] * elim_scale;
            //out(A,B,5,5);
        }
    }
}

void out(double* A, double *B, int col, int row) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%lf ", A[i * col + j]);
        }
        printf("\n");
    }
    for (int i = 0; i < row; i++) {
        printf("%lf ", B[i]);
    }
    printf("\n");
}