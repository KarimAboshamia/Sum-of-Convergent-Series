#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include "mpi.h"


int main(int argc, char *argv[]) {
    int numprocs, rank, n;
    double start, end;
    int s = 3;
    double i = 1.0;
    int rootStart;
    int limit;
    int tmpi;
    int size;
    double zeta = 0.0;
    double myCalc = 0.0;
    double t = 0.0;
    double cpu_time;
    double error;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
    	printf("Enter n size\n");
        scanf("%d", &n);
        start = MPI_Wtime();
        size = n / numprocs;
        rootStart = n - (size * (numprocs - 1));
        if(numprocs == 1)
        	rootStart = 1;
        MPI_Bcast(&n, 1, MPI_INT, 0,MPI_COMM_WORLD);
        MPI_Bcast(&size, 1, MPI_INT, 0,MPI_COMM_WORLD);

        for(i = rootStart; i <= n; i++)
	{
		myCalc +=  1 / (i * i * i);
	}
        MPI_Reduce(&myCalc, &zeta, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
        
        //Check
        printf("Zeta = %f\n", zeta);
      
  	printf("Error = %f\n", fabs(1.202056903159594 - zeta));

        end = MPI_Wtime();
        cpu_time = ((double) (end-start));
        printf("Run Time = %f\n", cpu_time);
    }
    else {
        MPI_Bcast(&n, 1, MPI_INT, 0,MPI_COMM_WORLD);
        MPI_Bcast(&size, 1, MPI_INT, 0,MPI_COMM_WORLD);  
        limit = size * rank;
        i = (rank - 1) * size;
        if(i == 0) i = i+1;
        for(; i < limit; i++)
	{
		myCalc +=  1 / (i * i * i);
	} 
	MPI_Reduce(&myCalc, &zeta, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
    }
    MPI_Finalize();
    return 0;
}
