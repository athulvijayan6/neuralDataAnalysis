#include "types.h"
/* sorts the spot file and saves it as <file>_sorted in the
 * directory
 */

//declaring functions
int spot_comp_func(const void* a, const void* b);



// my main
int main(int argc, char* argv[]) {
  

  if (argc != 1 +1) {
    printf("!!!!!!Error!!!!!\n");
    printf("Usage: sort_spot_result <result_file>\n");
    exit(-1);
  }

  SpotVectorPTR spot_vec_ptr = create_SpotVectorPTR();
  alloc_members_SpotVectorPTR(spot_vec_ptr, 500);
  
  // not sure whether the read binary mode works.
  FILEPTR fin;
  if ((fin = fopen(argv[1], "rb")) == NULL) {
    printf("ERROR: Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  // reading the result into spot vector
  scanf_SpotVectorPTR(fin, spot_vec_ptr);
  
  // closing the file
  fclose(fin);
  
  // sorting the spot_vector_ptr w.r.t. to best_member score in descending order
  
  qsort(spot_vec_ptr->vec, spot_vec_ptr->size, sizeof(Spot), spot_comp_func);
  
  // testing sorting...
  FILEPTR fout = fopen(strcat(argv[1],"_sorted"), "wb");
  printf_SpotVectorPTR(fout, spot_vec_ptr);
  fclose(fout);
  

  // free and destroy
  free_members_SpotVectorPTR(spot_vec_ptr);
  destroy_SpotVectorPTR(spot_vec_ptr);


  return 0;
}



// define functions
int spot_comp_func(const void* a, const void* b) {
  if (((Spot*) a) -> best_member_ptr -> score == ((Spot*) b) -> best_member_ptr -> score) 
    return 0;
  else if (((Spot*) a) -> best_member_ptr -> score > ((Spot*) b) -> best_member_ptr -> score) 
    return -1; // this is done for desceding order
  else 
    return 1; // this is done for descending order
}
