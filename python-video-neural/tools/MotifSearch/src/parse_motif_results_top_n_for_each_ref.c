#include "types.h"
/* this will parse the spot vector and prepare some numbers of list
 * equal to the number of unique motifs queried. The file will contain the
 * info of the matches in below given format. This file is to be processed 
 * further to get the desired result.
 * <query_file_name> <ref_file_name> <start_sample> <end_sample> <score>
 */

//declaring functions
int spot_comp_func(const void* a, const void* b);



// my main
int main(int argc, char* argv[]) {
  

  if (argc != 4 +1) {
    printf("!!!!!!Error!!!!!\n");
    printf("Usage: parse_motif_results <result_file> <top-n> <frame-shift> <out-file>");
    exit(-1);
  }
  
  int top_n = atoi(argv[2]);
  int frame_shift = atoi(argv[3]);

  SpotVectorPTR spot_vec_ptr = create_SpotVectorPTR();
  alloc_members_SpotVectorPTR(spot_vec_ptr, 500);
  
  // not sure whether the read binary mode works.
  FILEPTR fin;
  if ((fin = fopen(argv[1], "rb")) == NULL) {
    printf("ERROR: Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  // reading the result into spot vector
  printf("Reading spot_vector...\n");
  scanf_SpotVectorPTR(fin, spot_vec_ptr);
  printf("Reading over, %d spots read\n", spot_vec_ptr->size)
;
  // closing the file
  fclose(fin);

   
  // sorting the spot_vector_ptr w.r.t. to best_member score in descending order
  printf("Sorting spots...\n");
  qsort(spot_vec_ptr->vec, spot_vec_ptr->size, sizeof(Spot), spot_comp_func);
  printf("Sorting over\n");
  
  /*
  // loop over spot vector and get the unique queires.
  printf("Finding unique queries... \n");
  char queries[10][500]; 
  int query_ctr = 0; // query counter
  {
    int i;
    for (i = 0; i < spot_vec_ptr -> size; i++) {
      int j = 0;
      for (j = 0; j < query_ctr; j++)
	if (strcmp(spot_vec_ptr->vec[i].query_path, queries[j]) == 0)
	  break;
      // if current query is not found in queries then add it to the list.
      if (j == query_ctr) {
	strcpy(queries[query_ctr], spot_vec_ptr->vec[i].query_path);
	++query_ctr;
      }
    }
    printf("%d unique queries found namely\n", query_ctr);
    for (i = 0; i <  query_ctr; i++)
      printf("%d)\t%s\n", i, queries[i]);
  }
  */




  // loop over spot vector and get the unique refs.
  printf("Finding unique refs and queries... \n");
  char refs[100][500];
  char queries[100][500];
  int ref_ctr = 0;  // ref counter
  int query_ctr =0; // query counter 
  {
    // for ref
    int i;
    for (i = 0; i < spot_vec_ptr -> size; i++) {
      int j = 0;
      for (j = 0; j < ref_ctr; j++)
	if (strcmp(spot_vec_ptr->vec[i].reference_path, refs[j]) == 0)
	  break;
      // if current ref is not found in queries then add it to the list.
      if (j == ref_ctr) {
	strcpy(refs[ref_ctr], spot_vec_ptr->vec[i].reference_path);
	++ref_ctr;
      }
    }
    printf("%d unique refs found namely\n", ref_ctr);
    char dummy[100];
    strcpy(dummy, argv[1]);
    strcat(dummy, "_ref.map");
    FILEPTR refmap = fopen(dummy, "w");
    for (i = 0; i <  ref_ctr; i++){
      printf("%d)\t%s\n", i, refs[i]);
      fprintf(refmap, "%d %s\n", i, refs[i]);
    }


    // for query 

    for (i = 0; i < spot_vec_ptr -> size; i++) { 
      int j = 0; 
	for (j = 0; j < query_ctr; j++) 
	  if (strcmp(spot_vec_ptr->vec[i].query_path, queries[j]) == 0) 
	    break; 
	  // if current query is not found in queries then add it to the list. */
	  if (j == query_ctr) { 
	    strcpy(queries[query_ctr], spot_vec_ptr->vec[i].query_path); 
	    ++query_ctr; 
	  } 
    } 

    printf("%d unique refs found namely\n", ref_ctr);
    //char dummy[100];
    strcpy(dummy, argv[1]);
    strcat(dummy, "_query.map");
    FILEPTR querymap = fopen(dummy, "w");

 
    for (i = 0; i <  query_ctr; i++){ 
      printf("%d)\t%s\n", i, queries[i]); 
      fprintf(querymap, "%d %s\n", i, queries[i]); 
    }

    fclose(querymap);
    fclose(refmap);
    
  }




  // loop over and do something
  int i;  
  printf("Finding top %d hits for each of %d query motifs...\n", top_n, query_ctr);
  // first creating file pointers for each of the query output.
  FILEPTR fp[query_ctr];
  int fp_ctr[query_ctr][ref_ctr];
  for (i = 0; i < query_ctr; i++){
    char dummy[500]; char dummy2[100];
    strcpy(dummy, argv[4]);
    strcat(dummy, "_motif_");
    sprintf(dummy2, "%d", i+1);
    strcat(dummy, dummy2);
    fp[i] = fopen(dummy, "wb");
    int j;
    for (j=0; j < ref_ctr; j++) 
      fp_ctr[i][j] = 0;
  }
  
  for (i = 0; i < spot_vec_ptr -> size; i++) {
    // getting query index
    int j;
    for (j = 0; j < query_ctr; j++)
      if (strcmp(spot_vec_ptr->vec[i].query_path, queries[j])==0)
	break;
    assert(j < query_ctr);
    
    //getting ref index
    int k;
    for (k = 0; k < ref_ctr; k++)
      if (strcmp(spot_vec_ptr->vec[i].reference_path, refs[k])==0)
	break;
    assert(k < ref_ctr);


    if (fp_ctr[j][k] < top_n) {
      fprintf(fp[j], "%s %s %ld %ld %f\n", 
	      spot_vec_ptr -> vec[i].query_path,
	      spot_vec_ptr -> vec[i].reference_path,
	      spot_vec_ptr -> vec[i].best_member_ptr -> start * (long)frame_shift,
	      spot_vec_ptr -> vec[i].best_member_ptr -> end * (long)frame_shift,
	      spot_vec_ptr -> vec[i].best_member_ptr -> score
	      );
      ++fp_ctr[j][k];
    }
  }

  //destroing all the fps
  for (i = 0; i < query_ctr;  i++)
    fclose(fp[i]);

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
