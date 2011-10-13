#define _USE_MATH_DEFINES /*the #define must come before the #include.*/
#define SAMPLENO 3000

#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include <ppl.h>
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "Base.h"
#include "Event.h"
#include "EventList.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"

using namespace Concurrency;
using namespace std;

void initializeEnv(MPI_Datatype &t);
struct eventStruct toHandoverStruct(int arrivalNo, float duration, float pos, bool prevCallReserved, float speed, float time);
struct eventStruct toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time);
struct eventStruct toTerminationStruct(int arrivalNo, float pos, bool prevCallReserved, float time);
struct eventStruct parseData(string rec);

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
		int bid = myRank*size + i;
		if(bid<20)
			blist[i].setBaseID(bid);
		else
			blist[i].setBaseID(-1);
		cout<<blist[i].getBaseID()<<endl;
	}


	for(int i = 0; i<procsAmount; i++){
		if(myRank == 0){ // each specific process
			ifstream fin;
			fin.open("data.txt");
			ofstream fout("out.txt");
			if(!fin)
				cout<<"file not exist"<<endl;
			string rec; //one record
			getline(fin, rec);
			eventStruct e = parseData(rec);
			if(e.getBaseID()<size)
				blist[e.getBaseID()]
		}
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

struct eventStruct toHandoverStruct(int arrivalNo, float duration, float pos, bool prevCallReserved, float speed, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = duration;
	e.etype = 1;
	e.pos = pos;
	e.rc = (int)prevCallReserved;
	e.speed = speed;
	e.time = time;
	return e;
}


struct eventStruct toTerminationStruct(int arrivalNo, float pos, bool prevCallReserved, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = -1;
	e.etype = 2;
	e.pos = pos;
	e.rc = prevCallReserved;
	e.speed = -1;
	e.time = time;
	return e;
}


struct eventStruct toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = duration;
	e.etype = 0;
	e.pos = pos;
	e.rc = 0;
	e.speed = speed;
	e.time = time;
	return e;
}

struct eventStruct parseData(string rec){
	char * cstr, *p;
	int no, baseID;
	float time, duration, speed, pos;

	cstr = new char[rec.size()+1];
	strcpy_s(cstr, rec.size()+1, rec.c_str());

	p=strtok (cstr,"\t");
	no = atoi(p);
	p=strtok(NULL,"\t");
	time = (float)atof(p);
	p=strtok(NULL,"\t");
	baseID = atoi(p) - 1;
	pos = baseID*2 + 1;
	p=strtok(NULL,"\t");
	duration = (float)atof(p);
	p=strtok(NULL,"\t");
	speed = (float)atof(p);

	return toInitiationStruct(no,duration, pos, speed, time);
}