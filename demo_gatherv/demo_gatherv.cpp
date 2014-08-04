#include "mpi.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;


void main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(0 == rank)
	{
		cout << "Running on " << size << " nodes." << endl;
		cout << "This program shows how to use MPI_Gatherv.\n"
				"Each processor sends a random amount of data to the root processor.\n"
				"MPI_Gather is used first to tell the root how much data is going to be sent.\n"
			 << endl;
	}

	int *recvcounts=0, *offsets=0;
	char *recvbuf=0;

	// create the place for storage of digits received from nodes
	if(rank == 0)
	{
		recvcounts = new int[size];
		offsets = new int[size];
	}

	// each node generates 1 random number from [1,9] range
	srand((int)time(0)+rank);
	int num = (rand() % 9) + 1;

	// then each node sends its number to root node
	MPI_Gather(&num, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) // root node prints the counts it has received and allocates space
	{
		int total_count = 0;
		cout << "Received numbers:";
		for(int i=0; i<size; ++i)
		{
			cout << " " << recvcounts[i];
			offsets[i] = total_count;
			total_count += recvcounts[i];
		}
		cout << endl;

		recvbuf = new char[total_count];
	}

	//each node generates a random data
	char *sendbuf = new char[num];
	for(int i=0; i<num; ++i)
		sendbuf[i] = 'A' + (rand()%20);

	//then each node sends its data to root node
	MPI_Gatherv(sendbuf, num, MPI_CHAR, recvbuf, recvcounts, offsets, MPI_CHAR, 0, MPI_COMM_WORLD);

	//then each node frees memory allocated for its data
	delete sendbuf;

	if(rank == 0) //root node prints the data it received from each node
	{
		for(int i=0; i<size; ++i)
		{
			cout << "From node #" << i << ":";
			char *start = recvbuf + offsets[i];
			for(int j=0; j<recvcounts[i]; ++j)
			{
				cout << " " << start[j];
			}
			cout << endl;
		}
	}

	if(rank == 0) //only node #0 should delete, because only it has created that arrays
	{
		delete recvcounts;
		delete offsets;
		delete recvbuf;
	}

	MPI_Finalize();
}