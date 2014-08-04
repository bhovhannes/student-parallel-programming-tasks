#include "mpi.h"
#include <iostream>
using namespace std;

//tag for communication
#define COMM_TAG 99


//slaves code goes here
void slave(int size, int rank)
{
	for(int k=1; k<=2; ++k) //will run 2 times
	{
		int slow_slave_rank;
		MPI_Bcast(&slow_slave_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);

		cout << "Slave #"<<rank<<" received "<<slow_slave_rank<<" from master."<<endl;

		if(slow_slave_rank==rank)
		{
			cout << "Slave #"<<rank<<" will be the slow slave."<<endl;
			cout << "Slave #"<<rank<<". Doing long work..."<<endl;
			for(double d=0; d<=1000; d+=0.00001);
			cout << "Slave #"<<rank<<". Doing long work - ends"<<endl;
		}

		MPI_Send(&rank, 1, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD);
		cout << "Slave #"<<rank<<" completed his work and sent result to master..."<<endl;

		MPI_Barrier(MPI_COMM_WORLD);
	}
}


//master code goes here
void master(int size, int rank)
{
	cout << "Demonstration of asynchronous receive call.\n\n";

	cout << "Input rank of slave to slow down. SLOW_SLAVE_RANK=";
	int slow_slave_rank;
	cin>>slow_slave_rank;

	cout<<"\n\nMaster. Starting asynchronous mode."<<endl;
	MPI_Bcast(&slow_slave_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *results = new int[size-1];
	MPI_Request *requests = new MPI_Request[size-1];
	for(int i=1; i<size; ++i)
	{
		MPI_Irecv(&results[i-1], 1, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD, &(requests[i-1]));
	}
	
	cout<<"Master. Waiting for responses."<<endl;
	int index = -1;
	for(int i=1; i<size; ++i)
	{
		MPI_Status status;
		MPI_Waitany(size-1, requests, &index, &status);
		cout<<"Master. Received response from slave #"<<results[index]<<endl;
	}
	delete results;
	delete requests;
	cout<<"Master. Finished."<<endl;
	MPI_Barrier(MPI_COMM_WORLD);


	cout<<"\n\nMaster. Starting synchronous mode."<<endl;
	MPI_Bcast(&slow_slave_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
	cout<<"Master. Waiting for responses."<<endl;
	for(int i=1; i<size; ++i)
	{
		int result;
		MPI_Status status;
		MPI_Recv(&result, 1, MPI_INT, i, COMM_TAG, MPI_COMM_WORLD, &status);
		cout<<"Master. Received response from slave #"<<result<<endl;
	}
	cout<<"Master. Finished."<<endl;
	MPI_Barrier(MPI_COMM_WORLD);
}


void main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(0 == rank)
	{
		cout << "Running on " << size << " nodes." << endl;
		master(size, rank);
	}
	else 
		slave(size, rank);
	
	MPI_Finalize();
}