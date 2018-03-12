#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// compile with:
// gcc -Wall -O2 -DNROWS=100 matrix2.c -o matrix2
// gcc -Wall -O2 -DNROWS=1000 matrix2.c -o matrix2
// gcc -Wall -O2 -DNROWS=10000 matrix2.c -o matrix2
// gcc -Wall -O2 -DNROWS=100000 matrix2.c -o matrix2

#define NCOLS 100

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main() {
    double *table;
    int i,j,testruns;
    double ts,te;

    FILE *fp;
    fp = fopen("Test_for_COL_TO_COL.csv", "a");
    fprintf (fp,"Matrix 1\tFOR %d ROWS\n",NROWS);
    fprintf (fp,"COL TO COL\tMaccess/sec\tTime elapsed\n");

    table = (double *)malloc(NROWS*NCOLS*sizeof(double));
    if (table==NULL) {
        printf("alloc error!\n");
        exit(1);
    }

    for(testruns=0;testruns<10;testruns++){

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

    /*
    // check cell values in exact order
    for (i=0;i<NCOLS*NROWS;i++){
      //table[i]=1.0;
      printf("%f ", table[i]);
      if(i%NCOLS==NCOLS-1){
          printf("\n");
      }
    }
    */

    // print time elapsed & Maccesses/sec
    double time = te - ts ;
    //printf ("Time elapsed = %lf\n" , time) ;
    double maccess = (2.0*NROWS*NCOLS)/(time*1e6);
    //printf ("Maccess/sec = %lf\n" , maccess);
    fprintf (fp,"Test Run %d\t%lf\t%lf\n" , testruns+1,maccess,time) ;

    }
    
    if(NROWS==100){
        fprintf (fp,"AVERAGE\t=AVERAGE(B3:B12)\t=AVERAGE(C3:C12)\n\n");
    }
    else if(NROWS==1000){
        fprintf (fp,"AVERAGE\t=AVERAGE(B17:B26)\t=AVERAGE(C17:C26)\n\n");
    }
    else if(NROWS==10000){
        fprintf (fp,"AVERAGE\t=AVERAGE(B31:B40)\t=AVERAGE(C31:C40)\n\n");
    }
    else if(NROWS==100000){
        fprintf (fp,"AVERAGE\t=AVERAGE(B45:B54)\t=AVERAGE(C45:C54)\n\n");
    }

    // free arrays
    free(table);
    fclose(fp);

    return 0;
}
