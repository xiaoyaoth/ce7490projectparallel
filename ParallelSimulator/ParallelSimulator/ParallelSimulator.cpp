#define _USE_MATH_DEFINES /*the #define must come before the #include.*/
#define SAMPLENO 3000

#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include <ppl.h>
#include <random>
#include <time.h>
#include <iostream>

#include "Process.h"

using namespace Concurrency;
using namespace std;

void initializeEnv(MPI_Datatype &t);

void main(int argc, char* argv[]){
	int procsAmount, myRank;
	MPI_Status status;
	MPI_Datatype mpiType;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procsAmount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	initializeEnv(mpiType);

	Process p(procsAmount, myRank, mpiType);
	p.run();

	MPI_Type_free(&mpiType);
	MPI_Finalize();
}

void initializeEnv(MPI_Datatype &t)
{
	int blocklen[2] = {3,4};
	MPI_Datatype types[2]={MPI_INT, MPI_FLOAT};
	MPI_Aint disp[2];//A probably means address, this shows the address of the 
	MPI_Aint intex; //A probably means address, this shows the displament in the array
	MPI_Type_extent(MPI_INT, &intex);

	disp[0] = (MPI_Aint)0;
	disp[1] = intex+intex+intex;

	MPI_Type_struct(2, blocklen, disp, types, &t);
	MPI_Type_commit(&t);
	return;
}

