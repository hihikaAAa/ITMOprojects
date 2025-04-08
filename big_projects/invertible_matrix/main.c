#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double **memory_creator(int R, int C) {
    double **matrix = malloc(R * sizeof(double *));
    if (matrix != NULL) {
        for (int i = 0; i < R; ++i) {
            matrix[i] = malloc(C * sizeof(double));
            if (!matrix[i]) {
                for (int k = 0; k < i; ++k) {
                    free(matrix[k]);
                }
                free(matrix);
                fprintf(stderr, "Memory creation failed.\n");
                return NULL;
            }
        }
        return matrix;
    }
    return NULL;
}

void free_memory(double **matrix, int R) {
    if (matrix != NULL) {
        for (int i = 0; i < R; ++i)
            free(matrix[i]);
        free(matrix);
    }
}

int InvertMatrix(double **matrix, int helper, double ***inv_matrix_ptr) {
    double **matrix_args_helper = memory_creator(helper, 2 * helper);
    if (!matrix_args_helper) {
        return 0;
    }

    for (int i = 0; i < helper; ++i) {
        for (int j = 0; j < helper; ++j) {
            matrix_args_helper[i][j] = matrix[i][j];
            matrix_args_helper[i][j + helper] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int i = 0; i < helper; ++i) {
        int maxIndex = i;
        double max = fabs(matrix_args_helper[i][i]);
        for (int k = i + 1; k < helper; ++k) {
            if (fabs(matrix_args_helper[k][i]) > max) {
                max = fabs(matrix_args_helper[k][i]);
                maxIndex = k;
            }
        }

        if (fabs(max) < 1e-12) {
            free_memory(matrix_args_helper, helper);
            return 0;
        }

        if (maxIndex != i) {
            double *temp_row = matrix_args_helper[i];
            matrix_args_helper[i] = matrix_args_helper[maxIndex];
            matrix_args_helper[maxIndex] = temp_row;
        }

        double pivot_value = matrix_args_helper[i][i];
        for (int j = 0; j < 2 * helper; ++j) {
            matrix_args_helper[i][j] /= pivot_value;
        }

        for (int k = 0; k < helper; ++k) {
            if (k != i) {
                double new_h = matrix_args_helper[k][i];
                for (int j = 0; j < 2 * helper; ++j) {
                    matrix_args_helper[k][j] -= new_h * matrix_args_helper[i][j];
                }
            }
        }
    }

    double **inv_matrix = memory_creator(helper, helper);
    if (!inv_matrix) {
        free_memory(matrix_args_helper, helper);
        return 0;
    }

    for (int i = 0; i < helper; ++i) {
        for (int j = 0; j < helper; ++j) {
            inv_matrix[i][j] = matrix_args_helper[i][j + helper];
        }
    }

    free_memory(matrix_args_helper, helper);

    *inv_matrix_ptr = inv_matrix;
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc == 2) {
        if(strcmp("-h",argv[1])!=0 && strcmp("--help",argv[1])!=0) {
            fprintf(stderr,"Syntax error\n");
        }
        else {
            printf("There is a help message");
        }
        return 0;
    }
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }
    if (argc == 4) {
        printf("First arg: %s program_name\n", argv[0]);
        printf("Second arg: %s input_file_name\n", argv[1]);
        printf("Third arg: %s output_file_name\n", argv[2]);
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Could not open an input file %s.\n", argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        fprintf(stderr, "Could not open an exit file %s.\n", argv[2]);
        fclose(input);
        return 1;
    }

    int R, C;
    if (fscanf(input, "%d %d", &R, &C) != 2) {
        fprintf(stderr, "Could not read a size of a matrix.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    if (R != C) {
        fprintf(output, "no_solution\n");
        fclose(input);
        fclose(output);
        return 0;
    }

    if (R <= 0 || C <= 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    double **matrix = memory_creator(R, C);

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (fscanf(input, "%lf", &matrix[i][j]) != 1) {
                fprintf(stderr, "Could not read an element of a matrix.\n");
                free_memory(matrix, R);
                fclose(input);
                fclose(output);
                return 1;
            }
        }
    }

    fclose(input);

    int helper = R;
    double **inv_matrix = NULL;
    if (InvertMatrix(matrix, helper, &inv_matrix)) {
        fprintf(output, "%d %d\n", helper, helper);
        for (int i = 0; i < helper; ++i) {
            for (int j = 0; j < helper; ++j)
                fprintf(output, "%.8lf ", inv_matrix[i][j]);
            fprintf(output, "\n");
        }
    } else {
        fprintf(output, "no_solution\n");
    }

    free_memory(matrix, helper);
    if (inv_matrix != NULL) {
        free_memory(inv_matrix, helper);
    }
    fclose(output);
    return 0;
}
