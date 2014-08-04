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
		cout << "This program shows how to use MPI_Gather.\n"
				"Each processor generates a random number and sends it to the root processor.\n"
			 << endl;
	}

	int *recvcounts=0;

	// create the place for storage of digits received from nodes
	if(rank == 0)
	{
		recvcounts = new int[size];
	}

	// each node generates 1 random number from [1,9] range
	srand((int)time(0)+rank);
	int num = (rand() % 9) + 1;

	// then each node sends its number to root node
	MPI_Gather(&num, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) // root node prints the counts it has received
	{
		cout << "Received numbers:";
		for(int i=0; i<size; ++i)
			cout << " " << recvcounts[i];
		cout << endl;

		//only node #0 should delete, because only it has created that array
		delete recvcounts;
	}

	MPI_Finalize();
}