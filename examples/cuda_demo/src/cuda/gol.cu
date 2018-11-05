#include "gol.h"
#include <stdio.h>
#include <iostream>


#define HANDLE_ERROR(err) (handleCudaError(err, __FILE__, __LINE__))

const char* cudaGetErrorString(cudaError_t error);


const int DEVICE_ID = 0;
int DEVICE_SM = 0;


static void handleCudaError(cudaError_t err, const char *file, int line) {
	if (err != cudaSuccess) {
		printf("CUDA Error %s in %s %d\n", cudaGetErrorString(err), file, line);
		exit(err);
	}
}

__global__ void testp(int N) {
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < N; id += blockDim.x * gridDim.x) {
		printf("Test #%d\n", id);
	}
}

__global__ void GOL_Ghost_GPU(int dim, int *world) {
	int i, id;

	//striding loop
	for (id = blockIdx.x * blockDim.x + threadIdx.x; id <= dim + 1; id += blockDim.x * gridDim.x) {
		//row
		//i = id / (dim + 2);
		//col
		//j = id % (dim + 2);

		i = id;
		if (i >= 1 && i <= dim) {
			//TODO: Copy first real row to bottom ghost row
			world[(dim + 1) * (dim + 2) + i] = world[(dim + 2) + i];
			//TODO: Copy last real row to top ghost row
			world[i] = world[dim * (dim + 2) + i];
		}

		//TODO: Copy first real column to right most ghost column
		world[i * (dim + 2) + (dim + 1)] = world[i * (dim + 2) + 1];
		//TODO: Copy last real column to left most ghost column
		world[i * (dim + 2)] = world[i * (dim + 2) + dim];


	}
}

__global__ void GOL_GPU(int dim, int *world, int *newWorld, uchar4 * texture) {
	int id;

	for (id = blockIdx.x * blockDim.x + threadIdx.x; id < (dim + 2) * (dim + 2); id += blockDim.x * gridDim.x) {
		int x = id % (dim + 2);
		if (x == 0 || x == (dim + 1)) {
			texture[id].x = 100;
			texture[id].y = 210;
			texture[id].z = 100;
			continue;
		}
		int y = floorf((float)id / (float)(dim + 2));
		if(y == 0 || y == (dim + 1)) {
			texture[id].x = 100;
			texture[id].y = 210;
			texture[id].z = 100;
			continue;
		}
		//world point
		int cell = world[id];
		int numNeighbors;

		// Get the number of neighbors for a world point
		numNeighbors = world[id + (dim + 2)]   //TODO: lower
			+ world[id - (dim + 2)] //TODO: upper
			+ world[id + 1]         //TODO: right
			+ world[id - 1]         //TODO: left

			+ world[id + (dim + 3)]  //TODO: diagonal lower right
			+ world[id - (dim + 1)]  //TODO: diagonal upper right
			+ world[id + (dim + 1)]  //TODO: diagonal lower left
			+ world[id - (dim + 3)]; //TODO: diagonal upper leftDivision Mo

// game rules for Conways 23/3-world
// 1) Any live cell with fewer than two live neighbours dies
		if (cell == 1 && numNeighbors < 2) //TODO
			newWorld[id] = 0;

		// 2) Any live cell with two or three live neighbours lives
		else if (cell == 1 && numNeighbors == 2 || numNeighbors == 3) //TODO
			newWorld[id] = 1;

		// 3) Any live cell with more than three live neighbours dies
		else if (cell == 1 && numNeighbors > 3) //TODO
			newWorld[id] = 0;

		// 4) Any dead cell with exactly three live neighbours becomes a live cell
		else if (cell == 0 && numNeighbors == 3) //TODO
			newWorld[id] = 1;

		else
			newWorld[id] = cell;
		int value = newWorld[id];
		texture[id].x = value ? 255 : 0;
		texture[id].y = value ? 255 : 0;
		texture[id].z = value ? 255 : 0;
	}
}

__global__ void GOL_GPU_CPY(int dim, int *world, int *newWorld, uchar4 * texture) {
	int id;

	for (id = blockIdx.x * blockDim.x + threadIdx.x; id < (dim + 2) * (dim + 2); id += blockDim.x * gridDim.x) {
		int x = id % (dim + 2);
		if (x == 0 || x == (dim + 1)) {
			texture[id].x = 100;
			texture[id].y = 210;
			texture[id].z = 100;
			continue;
		}
		int y = floorf((float)id / (float)(dim + 2));
		if (y == 0 || y == (dim + 1)) {
			texture[id].x = 100;
			texture[id].y = 210;
			texture[id].z = 100;
			continue;
		}
		//world point
		int numNeighbors;

		// Get the number of neighbors for a world point
		numNeighbors = world[id + (dim + 2)]   //TODO: lower
			+ world[id - (dim + 2)] //TODO: upper
			+ world[id + 1]         //TODO: right
			+ world[id - 1]         //TODO: left

			+ world[id + (dim + 3)]  //TODO: diagonal lower right
			+ world[id - (dim + 1)]  //TODO: diagonal upper right
			+ world[id + (dim + 1)]  //TODO: diagonal lower left
			+ world[id - (dim + 3)]; //TODO: diagonal upper leftDivision Mo

		int cell = numNeighbors % 2;

		newWorld[id] = cell;

		texture[id].x = cell ? 255 : 0;
		texture[id].y = cell ? 255 : 0;
		texture[id].z = cell ? 255 : 0;
	}
}

void initGlider(int dim , int *world) {
	for(int i = 1; i <= dim; i++) {
	  for(int j = 1; j <= dim; j++) {
		int value = 0;
		if(i == 2 && j == 3) {
		  value = 1;
		}
		if(i == 3 && j == 4) {
		  value = 1;
		}      
		if(i == 4 && j >= 2 && j <= 4) {
		  value = 1;
		}
		world[i * (dim + 2) + j] = value;
	  }
	}
  }
  
  void initOscillator(int dim , int *world) {
	for(int i = 1; i <= dim; i++) {
	  for(int j = 1; j <= dim; j++) {
		int value = 0;
		if(i == 3 && j == 3) {
		  value = 1;
		}
		if(i == 3 && j == 4) {
		  value = 1;
		}      
		if(i == 4 && j == 3) {
		  value = 1;
		}
		if(i == 5 && j == 6) {
		  value = 1;
		}
		if(i == 6 && j == 5) {
		  value = 1;
		}
		if(i == 6 && j == 6) {
		  value = 1;
		}
		world[i * (dim + 2) + j] = value;
	  }
	}
  }
  
  void initStatic(int dim , int *world) {
	for(int i = 1; i <= dim; i++) {
	  for(int j = 1; j <= dim; j++) {
		int value = 0;
		if(i >= 4 && i <= 5) {
		  if(j >= 4 && j <= 5) {
			value = 1;
		  }
		}
		world[i * (dim + 2) + j] = value;
	  }
	}
  }

  void initH(int dim, int *world) {
	  for (int i = 1; i <= dim; i++) {
		  for (int j = 1; j <= dim; j++) {
			  int value = 0;
			  if (i == 512 ||i == 513) {
				  if (j == 512 || j == 513)
				  {
					  value = 1;
				  }
			  }
			  if (i >= 510 && i <= 515) {
				  if (j == 511 || j == 514) {
					  value = 1;
				  }
			  }
			  world[i * (dim + 2) + j] = value;
		  }
	  }
  }

  void initRandom(int dim, int *world)
  {
  
	int i, j;
  
	// Assign initial population randomly
	srand(time(NULL));
	for (i = 1; i <= dim; i++)
	{
	  for (j = 1; j <= dim; j++)
	  {
		world[i * (dim + 2) + j] = rand() % 2;
	  }
	}
  }

__global__ void setTextureData(int dim,  int * world, uchar4 * texture) {
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < (dim + 2) * (dim + 2); id += blockDim.x * gridDim.x) {
		int value = world[id];
		texture[id].x = value ? 255 : 0;
		texture[id].y = value ? 255 : 0;
		texture[id].z = value ? 255 : 0;	
	}
} 


void initGOL(int ** h_world, int ** d_world, int ** d_tmpWorld, int dim, uchar4 * texture) {
	cudaDeviceGetAttribute(&DEVICE_SM, cudaDevAttrMultiProcessorCount, DEVICE_ID);
	printf("SMs: %d\n", DEVICE_SM);

	size_t worldBytes = (dim + 2) * (dim + 2) * sizeof(int);
	*h_world = (int*)malloc(worldBytes);
	HANDLE_ERROR(cudaMalloc(d_world, worldBytes));
	HANDLE_ERROR(cudaMalloc(d_tmpWorld, worldBytes));

	//initGlider(dim, *h_world);
	//initRandom(dim, *h_world);
	initH(dim, *h_world);

	HANDLE_ERROR(cudaMemcpy(*d_world, *h_world, worldBytes, cudaMemcpyHostToDevice));

	setTextureData<<<32*DEVICE_SM, 256>>>(dim, *d_world, texture);
	cudaDeviceSynchronize();
	HANDLE_ERROR(cudaGetLastError());
}

void stepGOL(int * h_world, int * d_world, int * d_tmpWorld, int dim, uchar4 * texture) {
	size_t worldBytes = (dim + 2) * (dim + 2) * sizeof(int);
	GOL_Ghost_GPU<<<32*DEVICE_SM, 256>>>(dim, d_world);
	cudaDeviceSynchronize();
    HANDLE_ERROR(cudaGetLastError());
    
	//main world
	//GOL_GPU<<<32*DEVICE_SM, 256>>>(dim, d_world, d_tmpWorld, texture);	
	GOL_GPU_CPY<<<32*DEVICE_SM, 256>>>(dim, d_world, d_tmpWorld, texture);	
	cudaDeviceSynchronize();
	HANDLE_ERROR(cudaGetLastError());
    //world swap
    HANDLE_ERROR(cudaMemcpy(d_world, d_tmpWorld, worldBytes, cudaMemcpyDeviceToDevice)); 
	HANDLE_ERROR(cudaMemcpy(h_world, d_tmpWorld, worldBytes, cudaMemcpyDeviceToHost)); 
}

void testprint() {

	int devId = 0;
	int gpuSMs;
	cudaDeviceGetAttribute(&gpuSMs, cudaDevAttrMultiProcessorCount, devId);
	printf("SMs: %d\n", gpuSMs);

	testp<<<32 * gpuSMs, 256>>>(10);
	cudaDeviceSynchronize();
	HANDLE_ERROR(cudaGetLastError());
}