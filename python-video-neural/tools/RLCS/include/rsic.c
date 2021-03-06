#include "rsic.h"


DoubleMatrixPTR create_DoubleMatrixPTR() {return (DoubleMatrixPTR) malloc(sizeof(DoubleMatrix));}

void destroy_DoubleMatrixPTR(DoubleMatrixPTR temp) {free(temp);}

void alloc_members_DoubleMatrixPTR(DoubleMatrixPTR mat_ptr, const int max_size, const int dim) {
  mat_ptr->mat = (double**) malloc(sizeof(double*) * max_size);

  int i;  

  for (i = 0; i< max_size; i++) 
    mat_ptr->mat[i] = (double*) malloc(sizeof(double) * dim);

  mat_ptr->dim = dim;
  mat_ptr->size = 0;
  mat_ptr->max_size = max_size;
}

void realloc_members_DoubleMatrixPTR(DoubleMatrixPTR mat_ptr, const int max_size) {

  assert(max_size > mat_ptr->max_size); 

  mat_ptr->mat = (double**) realloc(mat_ptr->mat, sizeof(double*) * max_size);
  
  int i;
  for (i = mat_ptr->max_size; i< max_size; i++) 
    mat_ptr->mat[i] = (double*) malloc(sizeof(double) * mat_ptr->dim);

  mat_ptr->max_size = max_size;
  mat_ptr->size = mat_ptr->size;
  mat_ptr->dim = mat_ptr->dim;
}



void free_members_DoubleMatrixPTR(DoubleMatrixPTR  mat_ptr) {
  
  int i;
  for (i = 0; i< mat_ptr->max_size; i++)
    free(mat_ptr->mat[i]);

  free(mat_ptr->mat);

  mat_ptr->dim = 0;
  mat_ptr->size = 0;
  mat_ptr->max_size = 0;
}


void printf_DoubleMatrixPTR(FILEPTR fout, DoubleMatrixPTR mat){
  fprintf(fout, "%d %d\n", mat->size, mat->dim);
  int i,j;
  for (i = 0; i < mat->size; i++){
    for (j = 0; j < mat->dim; j++)
      fprintf(fout, "%lf ", mat->mat[i][j]);
    fprintf(fout, "\n");
  }
}

void copy_DoubleMatrixPTR(DoubleMatrixPTR  dest_ptr, const DoubleMatrixPTR  src_ptr) {
  copy_DoubleMatrixPTR_region(dest_ptr, src_ptr, 0, src_ptr->size - 1);
}

// TODO: make copy region version for all the types.
void copy_DoubleMatrixPTR_region(DoubleMatrixPTR  dest_ptr, const DoubleMatrixPTR  src_ptr, const int start, const int end) {
  int i, j;
  for (i = 0; i < end-start+1; i++)
    for (j = 0; j < src_ptr->dim; j++) {     
      dest_ptr -> mat[i][j] = src_ptr -> mat[start+i][j];
    }

  dest_ptr -> size = end-start+1;
}


void fill_DoubleMatrixPTR_from_file(String file_path, DoubleMatrixPTR feat_mat_ptr) {

  int i, j;
  char * buffer;

  // do not forget to free buffer
  buffer = read_file_into_buffer(file_path);
  int bi = 0;

  int size, dim;
  int offset; // to store the offset.
  

  // getting teh row and col of feat_mat;
  sscanf(&buffer[bi], "%d%d%n", &size, &dim, &offset);
  bi += offset;
  //TODO: change it to free and reallocate
  alloc_members_DoubleMatrixPTR(feat_mat_ptr, size, dim);

  feat_mat_ptr->size = feat_mat_ptr->max_size;

  for (i = 0; i < size; i++)
    for (j = 0; j < dim; j++){
      sscanf(&buffer[bi], "%lf%n", &(feat_mat_ptr->mat[i][j]), &offset);
      bi += offset;
    }


  // freeing buffer.
  free(buffer);
}



void empty_DoubleMatrixPTR(DoubleMatrixPTR mat_ptr) {
  free_members_DoubleMatrixPTR(mat_ptr);
}


String read_file_into_buffer(String file_path) { 
  
  printf("\nReading file: %s\n", file_path);
  
  charPTR buffer; 

  FILEPTR fin = fopen(file_path, "rb");  
  assert(fin != NULL);

  // getting the size of the file in bytes
  fseek(fin, 0L, SEEK_END);
  long sz = ftell(fin);
  fseek(fin, 0L, SEEK_SET);
  
  
  printf("Size of the file in bytes is %ld\n", sz);

  // allocating  buffer with a size to hold the file contents.
  buffer = (char*) malloc(sizeof(char) * sz);

  // reading the file in the buffer
  fread(buffer, 1, sz, fin);
  fclose(fin);

    return buffer;
}



