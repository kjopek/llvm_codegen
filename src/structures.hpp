#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

/* This file contains structures for 
   solver */

typedef enum {
	RECTANGULAR = -2,
	LOWER_TRIANGULAR,
	ZERO,
	DENSE_LOWER_TRIANGULAR,
	DENSE_RECTANGULAR
} matrix_type;

typedef struct h_martix {
	matrix_type type; // type of matrix
	int rows, cols; 
	double *D; // pointer to data (dense block)
	h_matrix *H; 
} hyper_matrix;


#endif // STRUCTURES_HPP
