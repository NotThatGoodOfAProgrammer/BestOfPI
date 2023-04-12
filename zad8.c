#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
  for (int i=0; i<m; i++) {
    for (int j=0; j<n; j++) {
      double sum = 0;
      for (int k=0; k<p; k++) {
        sum += A[i][k] * B[k][j];
      }
      AB[i][j] = sum;
    }
  }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
  for (int i=0; i<n; i++) {
    if (A[i][i] == 0) {
      return NAN;
    }
    for (int j=i+1; j<n; j++) {
      double multiplicant = -(A[i][j] / A[i][i]);
      for (int k=0; k<n; k++) {
        A[k][j] += multiplicant * A[k][i];
      }
    }
  }

  double res = A[0][0];
  for (int i=1; i<n; i++) {
    res *= A[i][i];
  }


  return res;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
  int row_i[n];
  int sign = 1;
  for (int i=0; i<n; i++) {
    row_i[i] = i;
  }


  for (int col=0; col<n; col++) {
    int max_i = col;
    for (int row=col+1; row<n; row++) {
      if (A[row_i[row]][col] > A[row_i[max_i]][col]) {
        max_i = row;
      }
    }

    if (max_i != col) {
      int tmp = row_i[max_i];
      row_i[max_i] = row_i[col];
      row_i[col] = tmp;
      sign *= -1;
    }
  }


  for (int i=0; i<n; i++) {
    if (fabs(A[row_i[i]][i]) < eps) {
      return 0;
    }
    for (int j=i+1; j<n; j++) {
      double multiplicant = -(A[row_i[j]][i] / A[row_i[i]][i]);
      for (int k=0; k<n; k++) {
        A[row_i[j]][k] += multiplicant * A[row_i[i]][k];
      }

      b[row_i[j]] += multiplicant * b[row_i[i]];
    }
  } 


  double res = A[row_i[0]][0] * sign;
  for (int i=1; i<n; i++) {
    res *= A[row_i[i]][i];
  }


  double tmp_x[n];
  for (int i=n-1; i>=0; i--) {
    for (int j=n-1; j>i; j--) {
      b[row_i[i]] -= A[row_i[i]][j] * tmp_x[row_i[j]];
    }

    tmp_x[row_i[i]] = b[row_i[i]] / A[row_i[i]][i];
  }

  for (int i=0; i<n; i++) {
    x[i] = tmp_x[row_i[i]];
  }
  

  return res;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
void matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      B[i][j] = (i == j);
    }
  }
  


  int row_i[n];
  int sign = 1;
  for (int i=0; i<n; i++) {
    row_i[i] = i;
  }


  for (int col=0; col<n; col++) {
    int max_i = col;
    for (int row=col+1; row<n; row++) {
      if (A[row_i[row]][col] > A[row_i[max_i]][col]) {
        max_i = row;
      }
    }

    if (max_i != col) {
      int tmp = row_i[max_i];
      row_i[max_i] = row_i[col];
      row_i[col] = tmp;
      sign *= -1;
    }
  }


  for (int i=0; i<n; i++) {
    if (fabs(A[row_i[i]][i]) < eps) {
      return 0;
    }
    for (int j=i+1; j<n; j++) {
      double multiplicant = -(A[row_i[j]][i] / A[row_i[i]][i]);
      for (int k=0; k<n; k++) {
        A[row_i[j]][k] += multiplicant * A[row_i[i]][k];

        B[row_i[j]][k] += multiplicant * B[row_i[i]][k];
      }
    }
  }


  for (int i=n-1; i>=0; i--) {
    if (fabs(A[row_i[i]][i]) < eps) {
      return 0;
    }
    for (int j=i-1; j>=0; j--) {
      double multiplicant = -(A[row_i[j]][i] / A[row_i[i]][i]);
      for (int k=0; k<n; k++) {
        A[row_i[j]][k] += multiplicant * A[row_i[i]][k];

        B[row_i[j]][k] += multiplicant * B[row_i[i]][k];
      }
    }
  }



  double det = A[row_i[0]][0] * sign;
  for (int i=1; i<n; i++) {
    det *= A[row_i[i]][i];
  }


  for (int i=0; i<n; i++) {
    double multiplicate = 1 / A[row_i[i]][i];
    for (int j=0; j<n; j++) {
      B[row_i[i]][j] *= multiplicate;
    }
  }



  printf("%.4f\n", det);

	if (det) {
    for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			printf("%.4f ", B[row_i[i]][j]);
		}
		printf("\n");
	}
  }
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			matrix_inv(A, B, n, eps);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

