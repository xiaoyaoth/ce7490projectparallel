// ParallelPI.cpp : Defines the entry point for the MPI application.
//
#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include <ppl.h>
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace Concurrency;
using namespace std;

struct initEventStruct{
	int ano;
	float time;
	float speed;
	float dura;
	float pos;
};

struct handoverEventStruct{
	int ano;
	int rc;
	float time;
	float speed;
	float dura;
	float pos;
};

struct termiEventStruct{
	int ano;
	int rc;
	float time;
	float pos;
};

void initializeEnv(MPI_Datatype &init, MPI_Datatype &hando, MPI_Datatype &termi);

void main(int argc, char* argv[]){
	int procsAmount, myRank;
	MPI_Status status;
	MPI_Datatype initType, handoType, termiType;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procsAmount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	initializeEnv(initType, handoType, termiType);
}

void initializeEnv(
	MPI_Datatype &it, //MPI Initiate type
	MPI_Datatype &ht, //MPI handover type
	MPI_Datatype &tt //MPI termination type
	)
{
	int blocklen[2] = {2,4};
	MPI_Datatype types[2]={MPI_INT, MPI_FLOAT};
	
	MPI_Aint iDisp[2];
	MPI_Aint hDisp[2];
	MPI_Aint tDisp[2];//A probably means address, this shows the address of the 
	
	MPI_Aint intex; //A probably means address, this shows the displament in the array
	MPI_Type_extent(MPI_INT, &intex);
	
	iDisp[0] = (MPI_Aint)0;
	iDisp[1] = intex;

	hDisp[0] = (MPI_Aint)0;
	hDisp[1] = intex+intex;

	tDisp[0] = (MPI_Aint)0;
	tDisp[1] = intex+intex;

	MPI_Type_struct(2, blocklen, iDisp, types, &it);
	MPI_Type_commit(&it);
	MPI_Type_struct(2, blocklen, hDisp, types, &ht);
	MPI_Type_commit(&ht);
	MPI_Type_struct(2, blocklen, tDisp, types, &tt);
	MPI_Type_commit(&tt);

	return;
}

void test(int argc, char* argv[]){
	int numprocs, myrank, i, j, k;
	MPI_Status status;
	char msg[20];

	int blocklen[2] = {2,4};
	MPI_Datatype types[3]={MPI_INT, MPI_FLOAT};
	MPI_Aint disp[2]; //A probably means address, this shows the address of the 
	MPI_Datatype handoverType;
	MPI_Aint intex; //A probably means address, this shows the displament in the array

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	cout<<numprocs<<endl;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	MPI_Type_extent(MPI_INT, &intex);
	disp[0] = (MPI_Aint)0;
	disp[1] = intex+intex;

	MPI_Type_struct(2, blocklen, disp, types, &handoverType);

	MPI_Type_commit(&handoverType);
	handoverEventStruct e1;
	if(myrank == 0){
		e1.ano = 0;
		e1.dura = 123.456;
		e1.pos = 23.45;
		e1.rc = 0;
		e1.speed = 99.88;
		e1.time = 0.01;

		MPI_Send(&e1, 1, handoverType, 1, 99, MPI_COMM_WORLD);
		//strcpy(msg, "hello world");
		//MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
		cout<<myrank<<"hello"<<endl;
	}else if(myrank == 1){
		MPI_Recv(&e1, 1, handoverType, 0, 99, MPI_COMM_WORLD, &status);
		cout<<e1.ano<<" "<<e1.dura<<" "<<e1.pos<<" "<<e1.rc<<" "<<e1.speed<<" "<<e1.time;
		//cout<<msg;
	}
	MPI_Type_free(&handoverType);
	MPI_Finalize();
}