#include <cuda_runtime.h>
#include <stdio.h>
__global__ void cuda_hello() {
    printf("Hello World from GPU!\n");
}

int main() {
    printf("Hello from CPU!\n");
    cuda_hello << <1, 1 >> > ();
    cudaDeviceSynchronize();
    cudaDeviceReset();
    return 0;
}

//int main() {
//    int* a;
//    cudaMalloc(&a, 100);
//    cudaFree(a);
//
//    return 0;
//}