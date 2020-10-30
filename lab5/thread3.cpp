#include <mpi.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>

#define MASTER_RANK 0

#define MASTER_TAG 1
#define WORKER_TAG 2

int dim;
int t_num;
unsigned short step_it = 0;
std::vector<double> a, b, c;
long double max_t_time = 0;

#define NOT_ENOUGH_PROCESSES_NUM_ERROR 1

MPI_Status status;

int main(int argc, char *argv[]) {
	int communicator_size;
	int process_rank;
	int process_id;
	int workers_num;
	int message_tag;
	unsigned long it, jt, et;

	dim = std::stol(std::string(argv[1]));
	t_num = std::stol(std::string(argv[2])) - 1;
	a.resize(dim * dim); b.resize(dim * dim); c.resize(dim * dim);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &communicator_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

	if (communicator_size < 2) {
		MPI_Abort(MPI_COMM_WORLD, NOT_ENOUGH_PROCESSES_NUM_ERROR);
	}

	if (process_rank == MASTER_RANK) {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::normal_distribution<double> dist;
		for (auto it = 0; it < dim * dim; ++it) {
			a[it] = dist(rng);
			b[it] = dist(rng);
			c[it] = 0;
		}

		workers_num = communicator_size - 1;

		auto start = std::chrono::system_clock::now();

		message_tag = MASTER_TAG;

		for (process_id = 1; process_id <= workers_num; ++process_id) {
			MPI_Send(&step_it, 1, MPI_UNSIGNED_SHORT, process_id, message_tag, MPI_COMM_WORLD);
			MPI_Send(&dim, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);
			MPI_Send(&t_num, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD);
			MPI_Send(a.data(), a.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD);
			MPI_Send(b.data(), b.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD);
			MPI_Send(c.data(), c.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD);
		}

		for (it = workers_num * dim / t_num; it < dim; ++it)
			for (jt = 0; jt < dim; ++jt)
				for (et = 0; et < dim; ++et)
					c[it * dim + jt] += a[it * dim + et] * b[et * dim + jt];

		message_tag = WORKER_TAG;

		for (process_id = 1; process_id <= workers_num; ++process_id) {
			MPI_Recv(&step_it, 1, MPI_UNSIGNED_SHORT, process_id, message_tag, MPI_COMM_WORLD, &status);
			MPI_Recv(&dim, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD, &status);
			MPI_Recv(&t_num, 1, MPI_INT, process_id, message_tag, MPI_COMM_WORLD, &status);
			MPI_Recv(a.data(), a.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD, &status);
			MPI_Recv(b.data(), b.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD, &status);
			MPI_Recv(c.data(), c.size(), MPI_DOUBLE, process_id, message_tag, MPI_COMM_WORLD, &status);
		}

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<long double> difference = end - start;
		max_t_time = std::max(max_t_time, difference.count());

		std::ofstream out("dat/thread3", std::ios_base::app);
		out << std::fixed << max_t_time << "\t" << t_num << std::endl;
	} 
	
	if (process_rank != MASTER_RANK) {
		message_tag = MASTER_TAG;
		MPI_Recv(&step_it, 1, MPI_UNSIGNED_SHORT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&dim, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&t_num, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(a.data(), a.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(b.data(), b.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(c.data(), c.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD, &status);

		for (it = step_it * dim / t_num; it < (step_it + 1) * dim / t_num; ++it)
			for (jt = 0; jt < dim; ++jt)
				for (et = 0; et < dim; ++et)
					c[it * dim + jt] += a[it * dim + et] * b[et * dim + jt];
		++step_it;

		message_tag = WORKER_TAG;
		MPI_Send(&step_it, 1, MPI_UNSIGNED_SHORT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
		MPI_Send(&dim, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
		MPI_Send(&t_num, 1, MPI_INT, MASTER_RANK, message_tag, MPI_COMM_WORLD);
		MPI_Send(a.data(), a.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD);
		MPI_Send(b.data(), b.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD);
		MPI_Send(c.data(), c.size(), MPI_DOUBLE, MASTER_RANK, message_tag, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}