#include "mpi.h"
#include <iostream>
using namespace std;

#define ELEMENT_COUNT 3

void main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(0 == rank)
	{
		cout << "Running on " << size << " nodes." << endl;
		cout << "The program demonstrates MPI_Reduce usage.\n";
	}

	int v[ELEMENT_COUNT] = {0};
	switch(rank) //each node fills in the array v with some values
	{
	case 0:
		v[0]=1; v[1]=2; v[2]=3;
		break;
	case 1:
		v[0]=3; v[1]=2; v[2]=1;
		break;
	case 2:
		v[0]=5; v[1]=1; v[2]=-2;
		break;
	default:
		v[0]=1; v[1]=1; v[2]=1;
		break;
	}

	//compute sum of arrays from each node in array result_sum
	int result_sum[ELEMENT_COUNT] = {0};
	MPI_Reduce(v, result_sum, ELEMENT_COUNT, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//compute arrays of maximum elements of arrays from each node in array result_max
	int result_max[ELEMENT_COUNT] = {0};
	MPI_Reduce(v, result_max, ELEMENT_COUNT, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if(0 == rank)
	{
		cout << "\nMPI_Reduce with MPI_SUM returned:" << endl;
		for(int i=0; i<ELEMENT_COUNT; ++i)
			cout << result_sum[i] << " ";
		cout << endl;

		cout << "\nMPI_Reduce with MPI_MAX returned:" << endl;
		for(int i=0; i<ELEMENT_COUNT; ++i)
			cout << result_max[i] << " ";
		cout << endl;
	}

	MPI_Finalize();
}