#define _USE_MATH_DEFINES /*the #define must come before the #include.*/
#define SAMPLENO 3000

#include "stdio.h"
#include "stdlib.h"
#include <time.h>
#include <iostream>
#include <list>

#include "Process.h"

using namespace std;

void initializeEnv(MPI_Datatype &t);
void test(int rank);
void test2(int rank, int size);
void test3(int rank, int size);

int main(int argc, char* argv[]){
	clock_t time = clock();
	int procsAmount, myRank;
	MPI_Datatype mpiType;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procsAmount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	initializeEnv(mpiType);

	Process p(procsAmount, myRank, mpiType); p.run();
	//test(myRank);
	//test2(myRank, procsAmount);
	//test3(myRank, procsAmount);
	cout<<myRank<<" "<<clock()-time<<endl;
	MPI_Type_free(&mpiType);
	MPI_Finalize(); 
	return 0;
}

void initializeEnv(MPI_Datatype &t)
{
	int blocklen[2] = {INTNO,FLOATNO};
	MPI_Datatype types[2]={MPI_INT, MPI_FLOAT};
	MPI_Aint disp[2];//A probably means address, this shows the address of the 
	MPI_Aint intex; //A probably means address, this shows the displament in the array
	MPI_Type_extent(MPI_INT, &intex);

	disp[0] = (MPI_Aint)0;
	disp[1] = intex*INTNO;

	MPI_Type_struct(2, blocklen, disp, types, &t);
	MPI_Type_commit(&t);
	return;
}

void test(int rank){
	MPI_Status stat;
	int i = 0;
	int j = 1;
	if(rank == 0){
		cout<<rank<<endl;
		i = 1999;
		j = 2010;
		MPI_Send(&i, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
		MPI_Send(&j, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
	} else if(rank == 1) {
		cout<<rank<<endl;
		cin>>i;
		MPI_Recv(&i, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &stat);
		cout<<"i:"<<i<<" j:"<<j<<endl;
		MPI_Recv(&j, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &stat);
		cout<<"i:"<<i<<" j:"<<j<<endl;
	}
}

void test2(int rank, int size){
	MPI_Status stat;
	int * r = new int[size];
	MPI_Allgather(&rank, 1, MPI_INT, r, 1, MPI_INT, MPI_COMM_WORLD);
	for(int i = 0; i<4; i++)
		cout<<r[i]<<endl;
}

void test3(int rank, int size){
	MPI_Status stat;
	int s = rank;
	if(rank == 0){
		s = 2010;
		MPI_Bcast(&s, 1, MPI_INT, 1, MPI_COMM_WORLD);
	}else
	MPI_Recv(&s, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
	cout<<s<<endl;
}
