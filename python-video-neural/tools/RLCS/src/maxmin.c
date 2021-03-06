/* Description:
 Contains routines for max-min operations
 * Creation Date: 9 March 2014
 * Author: Shrey Dutta
 */


#include "maxmin.h"


double max_vector_D(const DoubleVectorPTR vec_ptr) {
  int i;
  double max_num = -DBL_MAX;
  for(i = 0; i < vec_ptr->size; i++) 
    if(max_num <= vec_ptr->vec[i])
      max_num = vec_ptr->vec[i];
  return max_num;
}

int argmax_vector_D(const DoubleVectorPTR vec_ptr) {
  int i,
    max_num_index;
  double  max_num = -DBL_MAX; // some very small number;

  for(i = 0; i < vec_ptr->size; i++) 
    if(max_num <= vec_ptr->vec[i]) {
      max_num = vec_ptr->vec[i];
      max_num_index = i;
    }
  return max_num_index;
}

void max_matrix_D(const DoubleMatrixPTR mat_ptr, doublePTR max_num) {
  int i,j;
  *max_num = -DBL_MAX; // some very small number;

  for(i = 0; i < mat_ptr->size; i++)
    for(j = 0; j < mat_ptr->dim; j++) 
      if(*max_num < mat_ptr->mat[i][j])
	*max_num = mat_ptr->mat[i][j];
}


void argmax_matrix_D(const DoubleMatrixPTR mat_ptr, IntVectorPTR max_num_index_ptr) {
  int i,j;
  double  max_num = -DBL_MAX; // some very small number;

  for(i = 0; i < mat_ptr->size; i++)
    for(j = 0; j < mat_ptr->dim; j++) 
      if(max_num < mat_ptr->mat[i][j]) {
	max_num = mat_ptr->mat[i][j];
	max_num_index_ptr->vec[0] = i;
	max_num_index_ptr->vec[1] = j;
      }
  max_num_index_ptr->size = 2;
}

void argmax_matrix_D_gen(const DoubleMatrixPTR mat_ptr, const int min_row, const int min_col, const int max_row, const int max_col, IntVectorPTR max_num_index_ptr) {
  int i,j;
  double  max_num = -DBL_MAX; // some very small number;

  for(i = min_row; i <= max_row; i++)
    for(j = min_col; j <= max_col; j++) 
      if(max_num < mat_ptr->mat[i][j]) {
	max_num = mat_ptr->mat[i][j];
	max_num_index_ptr->vec[0] = i;
	max_num_index_ptr->vec[1] = j;
      }
  max_num_index_ptr->size = 2;
}

// TODO: change the name of teh function-- it should not be after D as it is changing at or after D.
int arg_find_smallest_num_after_D(const DoubleMatrixPTR mat_ptr, int num) {
  // mat has to be one dimentional;
  assert(mat_ptr->dim == 1);

  int i;
  for (i = 0; i < mat_ptr->size; i++)
    if (mat_ptr->mat[i][0] >= num) 
      break;

  // make sure that there has to be atleast one number which is supposed to
  // be greater than num.
  assert(i != mat_ptr->size);

  return i;
}


int arg_find_largest_num_before_D(const DoubleMatrixPTR mat_ptr, int num) {
  // mat has to be one dimentional;
  assert(mat_ptr->dim == 1);

  int i;
  for (i = 0; i < mat_ptr->size; i++)
    if (mat_ptr->mat[i][0] > num) 
      break;

  // make sure that there has to be atleast one number which is supposed to
  // be lesser than num and one number which is greater than num.

  //printf("%d, %d, %d\n",i, mat_ptr->size, num);
  
  //if (i == mat_ptr->size)
    
  //assert(i != 0);

  // returning the largest number before num.
  return i-1;
}
