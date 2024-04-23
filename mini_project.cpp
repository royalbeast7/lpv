#include <iostream>
#include <algorithm>
#include <string>
#include <mpi.h>
using namespace std;
// Function to partition the array
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low-1);
    for (int j = low; j <= high-1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            
        }
        
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}
// Function to perform quicksort on the partition
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot-1);
        quicksort(arr, pivot + 1, high);
        
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 100;
    int* arr = new int[n];
    int* recvbuf = new int[n];
    int* sendbuf = new int[n];
    // Fill the array with random values
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100;
            
        }
        
    } 
    cout<<"Unsorted Array: " << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
        
    }
    cout << endl;
    // Measure the execution time of the parallel quicksort algorithm
    double start_time = MPI_Wtime();
    // Divide the array into equal sized partitions for each process
    int sub_arr_size = n / size;
    int* sub_arr = new int[sub_arr_size];
    MPI_Scatter(arr, sub_arr_size, MPI_INT, sub_arr, sub_arr_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Sort the partition using quicksort
    quicksort(sub_arr, 0, sub_arr_size-1);
    // Gather the sorted partitions from each process
    MPI_Gather(sub_arr, sub_arr_size, MPI_INT, recvbuf, sub_arr_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Perform the above steps again
    double end_time = MPI_Wtime();
    // Print the sorted array
    cout<<"Sorted Array: " << endl;
    for (int i = 0; i < n; i++) {
        cout << recvbuf[i] << " ";
        
    }
    cout << endl;
    
    double parallel_execution_time = end_time - start_time;
    // Measure the execution time of the sequential quicksort algorithm
    start_time = MPI_Wtime();
    quicksort(arr, 0, n);
    end_time = MPI_Wtime();
    double sequential_execution_time = end_time - start_time;
    // Calculate speedup and efficiency
    double speedup = sequential_execution_time / parallel_execution_time;
    double efficiency = speedup / size;
    cout << "Sequential execution time: " << sequential_execution_time << endl;
    cout << "Parallel execution time: " << parallel_execution_time << endl;
    cout << "Speedup: " << speedup << endl;
    cout << "Efficiency: " << efficiency << endl;
    MPI_Finalize();
    return 0;
}
