#include <bits/stdc++.h>
using namespace std;
int Gauss_Seidel_Solve(double* array_A, double* B, double* X_k, int row, int col, double tolerance, int Maximum_Number_of_Iterations);
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
    Gauss_Seidel_Solve(matrix, vector, X0, 5, 5, 0.005, 1000);
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

int Gauss_Seidel_Solve(double* array_A, double* B, double* X_k, int row, int col, double tolerance, int Maximum_Number_of_Iterations) {
    bool Gauss_Seidel_convergence_check = true;
    int i, j, iteration = 1;
    double sum = 0;
    double* X_k1;
    X_k1 = (double*)malloc(col * sizeof(double));

    double tolerance_variable;
    double distance = 0.0, distance_squared = 0.0;
    double temporary_variable_one = 0.0;
    double temporary_variable_two = 0.0;
    double norm_variable_squared = 0.0;
    double norm_variable = 0.0;
    // printf("Prog: Gauss_Seidel_MM_Solution.cpp\n\n\n");
    //  X_k[i] is initialized to 0.0
    for (i = 0; i < row; i++) {
        X_k1[i] = 0;
    }
    //  Begin Gauss-Seidel iteration modification
    for (i = 0; i < row; i++) {
        sum = 0;
        for (j = 0; j < col; j++) {
            if (j < i) {
                sum = sum + array_A[i * row + j] * X_k1[j];
            }
            if (j > i) {
                sum = sum + array_A[i * row + j] * X_k[j];
            }
        }
        X_k1[i] = -sum / array_A[i * row + i] + B[i] / array_A[i * row + i];
    }
    //  End Gauss-Seidel iteration modification

    //  Begin Jacobi iteration modification
    //  printf ("Gauss_Seidel_iteration %d   \n", iteration);
    for (i = 0; i < row; i++) {
        sum = 0;
        for (j = 0; j < col; j++) {
            if (j < i) {
                sum = sum + array_A[i * row + j] * X_k1[j];
            }
            if (j > i) {
                sum = sum + array_A[i * row + j] * X_k[j];
            }
        }
        X_k1[i] = -sum / array_A[i * row + i] + B[i] / array_A[i * row + i];
    }
    //  End Jacobi iteration modification
    while (Gauss_Seidel_convergence_check == true) {
        for (i = 0; i < row; i++) {
            X_k[i] = X_k1[i];
        }
        for (i = 0; i < row; i++) {
            sum = 0;
            for (j = 0; j < col; j++) {
                if (j < i) {
                    sum = sum + array_A[i * row + j] * X_k1[j];
                }
                if (j > i) {
                    sum = sum + array_A[i * row + j] * X_k[j];
                }
            }
            X_k1[i] = -sum / array_A[i * row + i] + B[i] / array_A[i * row + i];
        }
        iteration++;
        // Begin tolerance evaluation function
        if (iteration >= Maximum_Number_of_Iterations) {
            Gauss_Seidel_convergence_check = false;
        }
        for (j = 0; j < col; j++) {
            temporary_variable_one = X_k1[j] - X_k[j];
            distance_squared += temporary_variable_one * temporary_variable_one;
            temporary_variable_two = X_k[j] * X_k[j];
            norm_variable_squared += temporary_variable_two;
        }
        distance = sqrt(distance_squared);
        norm_variable = sqrt(norm_variable_squared);
        tolerance_variable = distance / norm_variable;
        distance_squared = 0.0;
        norm_variable_squared = 0.0;
        if ((tolerance_variable) < tolerance) {
            break;
        }
        if (norm_variable > 100.0) {
            break;
        }
        // End tolerance evaluation function
        if (iteration >= Maximum_Number_of_Iterations) {
            Gauss_Seidel_convergence_check = false;
        }
    }
    // print_results(&array_A[0], &B[0], &X_k1[0], row, col, tolerance, iteration, Maximum_Number_of_Iterations);
    free(X_k1);
    return 0;
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