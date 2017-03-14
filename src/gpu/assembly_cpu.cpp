//assembly_cpu.cpp

#include <vector>
#include "assembly_cpu.h"

/**
 * initializes and array to a value in single thread
 * @param  {[type]} T* array        array
 * @param  {[type]} T  value        initialization value
 * @param  {[type]} int size        number of elements in the array
 */
template <typename T>
void init_host_array_to_value(T *array,
                              T value,
                              int size)
{
 for(int i = 0; i < size; i++) array[i] = value;
 }

 /**
  * Cast a directly assembled matrix into GMM compatible sparse matrix
  * @param  {[type]} T* array        array
  * @param  {[type]} T  value        initialization value
  * @param  {[type]} int size        number of elements in the array
  */
 template <typename T>
  cast_host_array_to_GMM_Sparse(T *array,
                                int * vec_ind,
                                int * cvec_ptr,
                                int ref_size)
 {
  
  return gmm_pointer;
  }


 /**
  * Checks that all values in are between given limits
  * @param  {[type]} T* array        array
  * @param  {[type]} T  upper_limit  upper limit value
  * @param  {[type]} T  lower_limit  lower limit value
  * @param  {[type]} int size        number of elements in the array
  * @param  {[type]} std::string name    Name of the array
  */
 template <typename T>
 void check_host_array_for_limits(T *array,
                                 T upper_limit,
                                 T lower_limit,
                                 int size,
                                 std::string array_name)
 {
   int num_errors = 0;
    for(int i = 0; i < size; i++) {
      if(array[i] < lower_limit) num_errors++;
      else if(array[i] > upper_limit) num_errors++;
    }
    if(num_errors == 0) std::cout << "Correct: Array " << array_name << " is correct between given limits " << lower_limit << " to " << upper_limit << std::endl;
    else std::cout << "Error: Array " << array_name << " has " << num_errors<< " outside given limits " << lower_limit << " to " << upper_limit << std::endl;
  }

/**
 * Makes a direct assembly of the global matrix from map in single thread
 * @param  {[type]} T *global_matrix        global sparse matrix
 * @param  {[type]} int *full_map           Array mapping positions in global matrix
 * @param  {[type]} int num_points          number of gauss points
 * @param  {[type]} int support_node_size   number of nodes support for each gausspoint
 * @param  {[type]} int number_elements     number of elements in the sparse matrix
 */

template <typename T>
bool cpu_assemble_global_matrix(T* global_matrix,
                                int* full_map,
                                int num_cells,
                                int support_node_size,
                                int number_elements)
{
  return true;
}

/**
* @brief Creates a Map for a global sparse matrix in either the Compressed
* Column Storage format (CCS) or Compressed Row Storage(CRS) as explained
* in http://www.netlib.org/utk/people/JackDongarra/etemplates/node373.html
* This function also allocates all the supplementary arrays for the sparse format.
* the allocation of the values array is not performed here as depends the data types and wether atomics will be used.
* @param full_map Reference to the global matrix positions, accesible by [row_id * totcols + col_id ]. lhs.
* @param row_ind. Array with the vec_ind of each element. lhs.
* @param cvec_ptr. Array with the memory position where each cvec starts. lhs.
* @param presence_matrix Reference to the global matrix counter where if an element is nonzero will have a counter greater than zero. rhs.
* @param all_point_nodes Reference to the array where each point keeps the relation of support nodes is using. rhs.
* @param number_rows. Integer with the total number of rows of the matrix. rhs.
* @param number_columns. Integer with the total number of columns of the matrix. rhs.
* @return void. lhs
**/
bool map_global_matrix(std::vector<int> &full_map,
                      std::vector<int> &vec_ind,
                      std::vector<int> &cvec_ptr,
                      int *presence_matrix,
                      int number_rows,
                      int number_columns)
{

build_CCS_sparse_matrix_from_map(full_map,
                                vec_ind,
                                cvec_ptr,
                                presence_matrix,
                                number_rows,
                                number_columns);

/*build_CRS_sparse_matrix_from_map(full_map,
                                vec_ind,
                                cvec_ptr,
                                presence_matrix,
                                number_rows,
                                number_columns);*/


return true;

}

/**
 * @brief Allocates and Creates a Map for a global sparse matrix in the Compressed
 * Row Storage format (CRS) as explained in http://www.netlib.org/utk/people/JackDongarra/etemplates/node373.html
 * This function also allocates all the supplementary arrays for the sdparse format.
 * the allocation of the values array is not performed here as depends the data types and wether atomics will be used.
 * @param full_map Reference to the global matrix positions, accesible by [row_id * totcols + col_id ]. lhs.
 * @param col_ind. Array with the col id of each element. lhs.
 * @param row_ptr. Array with the memory position where each row starts. lhs.
 * @param presence_matrix Reference to the global matrix counter where if an element is nonzero will have a counter greater than zero. rhs.
 * @param number_rows. Integer with the total number of rows of the matrix. rhs.
 * @param number_columns. Integer with the total number of columns of the matrix. rhs.
 * @return void. lhs
 **/
bool build_CRS_sparse_matrix_from_map(std::vector<int> &full_map,
                                                      std::vector<int> &col_ind,
                                                      std::vector<int> &row_ptr,
                                                      int *presence_matrix,
                                                      int number_rows,
                                                      int number_columns)//Compressed Row Storage
{
//calculates total memory needed to allocate memory
  int total_elements = 0;
  for(int i = 1; i < number_rows * number_columns; i++)
      if(presence_matrix[i] > 0) total_elements++;

  col_ind.resize(total_elements);
  full_map.resize(number_rows * number_columns);
  row_ptr.resize(number_rows + 1);

  //prefix sum scan will give us the full map
  full_map[0] = 0;
  for(int i = 1; i < number_rows * number_columns; i++)
          full_map[i] = full_map[i-1] + presence_matrix[i-1];

  for(int i = 1; i < number_rows; i++){
    for(int j = 1; j < number_columns; j++){
      if(presence_matrix[i*number_columns + j] > 0){
        col_ind[full_map[i*number_columns + j]] = j;//col id of every element
      }

    }
    row_ptr[i] = full_map[i*number_columns];//pointers to start of every row
  }

  row_ptr[number_rows] = total_elements;//convention
  return true;
}


  /**
  * @brief Allocates and Creates a Map for a global sparse matrix in the Compressed
  * Column Storage format (CCS) as explained in http://www.netlib.org/utk/people/JackDongarra/etemplates/node373.html
  * This function also allocates all the supplementary arrays for the sparse format.
  * the allocation of the values array is not performed here as depends the data types and wether atomics will be used.
  * @param full_map Reference to the global matrix positions, accesible by [row_id * totcols + col_id ]. lhs.
  * @param row_ind. Array with the col id of each element. lhs.
  * @param col_ptr. Array with the memory position where each row starts. lhs.
  * @param presence_matrix Reference to the global matrix counter where if an element is nonzero will have a counter greater than zero. rhs.
  * @param number_rows. Integer with the total number of rows of the matrix. rhs.
  * @param number_columns. Integer with the total number of columns of the matrix. rhs.
  * @return void. lhs
  **/
    bool build_CCS_sparse_matrix_from_map(std::vector<int> &full_map,
                                          std::vector<int> &row_ind,
                                          std::vector<int> &col_ptr,
                                          int *presence_matrix,
                                          int number_rows,
                                          int number_columns)//Compressed Column Storage
    {
      //calculates total memory needed to allocate memory
        int total_elements = 0;
        for(int i = 1; i < number_rows * number_columns; i++)
            if(presence_matrix[i] > 0) total_elements++;

        row_ind.resize(total_elements);
        full_map.resize(number_rows * number_columns);
        col_ptr.resize(number_rows + 1);

        //prefix sum scan will give us the full map
        full_map[0] = 0;
        for(int i = 1; i < number_rows * number_columns; i++)
                full_map[i] = full_map[i-1] + presence_matrix[i-1];

        for(int j = 1; j < number_rows; j++){
          for(int i = 1; i < number_columns; i++){
            if(presence_matrix[j * number_rows + i] > 0){
              row_ind[full_map[j * number_rows + i]] = i;//row id of every element
            }

          }
          col_ptr[j] = full_map[j * number_rows];//pointers to start of every col
        }

        col_ptr[number_columns] = total_elements;//convention

        return true;
    }


    ///////////////////////////////////////////////////////////////////////////
    //////////////// templas parts ///////////////////////////////////////////
    template void init_host_array_to_value<double>(double *array,
                                                   double value,
                                                   int size);
    template void init_host_array_to_value<float>(float *array,
                                                  float value,
                                                  int size);
    template void init_host_array_to_value<int>(int *array,
                                                int value,
                                                int size);

    template void check_host_array_for_limits<double>(double *array,
                                                      double upper_limit,
                                                      double lower_limit,
                                                      int size,
                                                      std::string array_name);
  //
  template void check_host_array_for_limits<float>(float *array,
                                                   float upper_limit,
                                                   float lower_limit,
                                                   int size,
                                                   std::string array_name);
//
template void check_host_array_for_limits<int>(int *array,
                                               int upper_limit,
                                               int lower_limit,
                                               int size,
                                               std::string array_name);
