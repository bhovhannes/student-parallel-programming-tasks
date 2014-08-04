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
		cout << "This program shows how to use MPI_Scatter.\n"
				"The node #0 sends parts of 1 long array to all nodes using MPI_Scatter.\n"
				"Each node then prints the part it received.\n"
			 << endl;
	}

	int *source_data = 0;

	// create the data to be sent on the node #0
	if(rank == 0)
	{
		cout << "Source data is: [";
		int sz = ELEMENT_COUNT*size;
		source_data = new int[sz];
		for(int i=0; i<sz; ++i)
		{
			source_data[i] = i;
			cout << " " << source_data[i];
		}
		cout << " ]\n\n";
	}

	// each node will get ELEMENT_COUNT elements from the node #0
	int *node_buf = new int[ELEMENT_COUNT];

	// send different data to each node
	// each node will get ELEMENT_COUNT elements from source_data array
	MPI_Scatter(source_data, ELEMENT_COUNT, MPI_INT, node_buf, ELEMENT_COUNT, MPI_INT, 0, MPI_COMM_WORLD);

	// each node prints what it has received
	printf("Node #%d received elements [%d,%d,%d]\n", rank, node_buf[0], node_buf[1], node_buf[2]);

	delete node_buf; //each node should delete

	if(rank == 0) //only node #0 should delete, because only it has created that array
	{
		delete source_data;
	}
	
	MPI_Finalize();
}