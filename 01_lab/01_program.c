#include<stdio.h> 
#include<omp.h> 

int main(){
    printf("HEllo Gagan Welcome to the HPC world \n"  ) ; 
    #pragma omp parallel 
    {
        printf("Hello Gagan Welcome in the  World of Paralell Computing \n");
    }
    printf("HEllo Gagan Welcome to the HPC world - End \n"  ) ;
    // i want to know how many threads are there
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads() ; 
        int tid = omp_get_thread_num() ; 
        printf("Total number of threads are %d and this is thread number %d \n" , nthreads , tid ) ; 
    }


    return 0 ; 


}
