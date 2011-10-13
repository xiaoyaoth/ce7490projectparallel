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

#include "Base.h"
#include "Event.h"
#include "EventList.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"

using namespace Concurrency;
using namespace std;

struct eventStruct{
	int etype;//event type, 0 is initial, 1 is handover, 2 is termination
	int ano;
	int rc;
	float time;
	float speed;
	float dura;
	float pos;

	void toString(){
		cout<<etype<<" "<<ano<<" "<<dura
			<<" "<<pos<<" "<<rc<<" "<<speed
			<<" "<<time<<endl;
	}
};

void initializeEnv(MPI_Datatype &t);

void main(int argc, char* argv[]){
	int procsAmount, myRank;
	MPI_Status status;
	MPI_Datatype mpiType;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procsAmount);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	initializeEnv(mpiType);

	int size = BASENO/procsAmount+1;
	Base *blist = new Base[size];
	for(int i = 0; i<size; i++){
		int bid = i*procsAmount + myRank;
		if(bid<20)
			blist[i].setBaseID(bid);
		else
			blist[i].setBaseID(-1);
		cout<<blist[i].getBaseID()<<endl;
	}

	if(myRank == 0){
		eventStruct e;
		MPI_Recv(&e, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
		e.toString();
		MPI_Recv(&e, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
		e.toString();
		MPI_Recv(&e, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
		e.toString();
	}else if(myRank == 1){
		eventStruct e1;
		e1.etype = 0; e1.ano = 0; e1.dura = 0; e1.pos = 0; e1.speed = 0; e1.time = 0;
		MPI_Send(&e1, 1, mpiType, 0, 99, MPI_COMM_WORLD);
	}
	else if(myRank == 2){
		eventStruct e2;
		e2.etype = 1; e2.ano = 1; e2.dura = 1; e2.pos = 1; e2.speed = 1; e2.time = 1; e2.rc = 1;
		MPI_Send(&e2, 1, mpiType, 0, 99, MPI_COMM_WORLD);
	}
	else if(myRank == 3){
		eventStruct e3;
		e3.etype = 2; e3.ano = 2; e3.pos = 2; e3.time = 2; e3.rc = 2;
		MPI_Send(&e3, 1, mpiType, 0, 99, MPI_COMM_WORLD);
	}
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
