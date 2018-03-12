#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// compile with:
// gcc -Wall -O2 -DNROWS=100 matrix2.c -o matrix2

#define NCOLS 10

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main() {
    double *table;
    int i,j;
    double ts,te;

    table = (double *)malloc(NROWS*NCOLS*sizeof(double));
    if (table==NULL) {
        printf("alloc error!\n");
        exit(1);
    }

    // warmup
    for (i=0;i<NCOLS*NROWS;i++){
      table[i]=1.0;
      //printf("%f ", table[i]);
    }

    // get starting time (double, seconds)
    get_walltime(&ts);

    // workload
    for (i=0;i<NCOLS;i++){
        for (j=0;j<NROWS;j++){
            table[NCOLS*j+i]+=j*1.0;
            //printf("%f ", table[NCOLS*j+i]);
        }
        //printf("\n");
    }

    // get ending time
    get_walltime(&te);

    // check results
    for (i=0;i<NCOLS;i++){
        for (j=0;j<NROWS;j++){
            if(table[NCOLS*j+i]!=1.0+j*1.0){
                printf("Error starting in array cell: [%d,%d]\n", i,j);
                return 1;
            }
        }
    }

    for (i=0;i<NCOLS*NROWS;i++){
      //table[i]=1.0;
      printf("%f ", table[i]);
      if(i%NCOLS==NCOLS-1){
          printf("\n");
      }
    }

    // print time elapsed and/or Maccesses/sec
    double time = te - ts ;
    double maccess = (2.0*NROWS*NCOLS)/(time*1e6);
    printf ("Maccess/sec = %lf\n" , maccess) ;

    // free arrays
    free(table);

    return 0;
}
