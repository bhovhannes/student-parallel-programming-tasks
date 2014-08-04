#include "mpi.h"
#include <iostream>
using namespace std;

#define ELEMENT_COUNT 10
#define ELEMENTS_FOR_NODE_1 3
#define ELEMENTS_FOR_NODE_2 2

void main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(0 == rank)
	{
		cout << "Running on " << size << " nodes." << endl;
		cout << "This program shows how to use MPI_Scatterv.\n"
				"The node #0 sends 3 elements of long array to node #1 and 2 elements to node #2, other nodes get nothing.\n"
				"Each node then prints the part it received.\n"
			 << endl;
	}

	int *source_data = 0;
	int *send_counts = 0;
	int *offsets = 0;

	// create the data to be sent on the node #0
	if(rank == 0)
	{
		cout << "Source data is: [";
		source_data = new int[ELEMENT_COUNT];
		for(int i=0; i<ELEMENT_COUNT; ++i)
		{
			source_data[i] = i;
			cout << " " << source_data[i];
		}
		cout << " ]\n";

		offsets = new int[size];
		for(int i=0; i<size; ++i)
			offsets[i] = 0;
		offsets[1] = 1;
		offsets[2] = 5;

		send_counts = new int[size];
		for(int i=0; i<size; ++i)
			send_counts[i] = 0;
		send_counts[1] = ELEMENTS_FOR_NODE_1;
		send_counts[2] = ELEMENTS_FOR_NODE_2;

		for(int i=0; i<size; ++i)
		{
			cout << "\nNode #0 will send " << send_counts[i] << " elements, starting from position " << offsets[i] << " to node #" << i;
		}
		cout << endl << endl;
	}

	// each node will get ELEMENT_COUNT elements from the node #0
	int *node_buf = 0;
	
	// send different data to each node
	if(rank == 1)
	{
		node_buf = new int[ELEMENTS_FOR_NODE_1];
		MPI_Scatterv(source_data, send_counts, offsets, MPI_INT, node_buf, ELEMENTS_FOR_NODE_1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Node #%d received elements [%d,%d,%d]\n", rank, node_buf[0], node_buf[1], node_buf[2]);
		delete node_buf;
	}
	else if(rank == 2)
	{
		node_buf = new int[ELEMENTS_FOR_NODE_2];
		MPI_Scatterv(source_data, send_counts, offsets, MPI_INT, node_buf, ELEMENTS_FOR_NODE_2, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Node #%d received elements [%d,%d]\n", rank, node_buf[0], node_buf[1]);
		delete node_buf;
	}
	else
	{
		MPI_Scatterv(source_data, send_counts, offsets, MPI_INT, node_buf, 0, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Node #%d received nothing\n", rank);
	}

	if(rank == 0) //only node #0 should delete, because only it has created that array
	{
		delete source_data;
		delete send_counts;
		delete offsets;
	}
	
	MPI_Finalize();
}