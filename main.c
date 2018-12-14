#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "omp.h"

//quicksort-omp.h will be represented here instead

int arrayLength = 262144000;
int numThreads = 8;

//this is the rosetta code version of a quicksort
//it can be found here: http://rosettacode.org/wiki/Sorting_algorithms/Quicksort#C.2B.2B
void quicksortRosetta(int *A, int len) {
    if (len < 2) return;

    int pivot = A[len / 2];

    int i, j;
    for (i = 0, j = len - 1; ; i++, j--) {
        while (A[i] < pivot) i++;
        while (A[j] > pivot) j--;

        if (i >= j) break;

        int temp = A[i];
        A[i]     = A[j];
        A[j]     = temp;
    }

    quicksortRosetta(A, i);
    quicksortRosetta(A + i, len - i);
}


//this is the custom quicksort created by "eduardlopez" which has been re-typed from github
//the original can be found here: https://github.com/eduardlopez/quicksort-parallel/blob/master/quicksort-demo-comparison.c
//left starts as 0; right starts as the array's length - 1
void quicksort(int* arr, int left, int right)
{
    int i = left;                      //variable for finding the number left of the pivot
    int j = right;                     //variable for finding the number right of the pivot
    int temp;                          //a temporary variable for use in moving values within the array
    int pivot = arr[(left + right)/2]; //the value (within the array) at which an array will be separated into its higher and lower values



    while (i <= j)
    {
        //find a number to the left of the pivot that's greater than it
        while (arr[i] < pivot)
            i++;
        //find a number to the right of the pivot that's lesser than it
        while (arr[j] > pivot)
            j--;

        //as long as i <= j, the two values can be swapped
        if (i <= j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }


    if (left < j) {quicksort(arr, left, j);}   //if there are more unsorted numbers to the left of j, sort them
    if (i < right) {quicksort(arr, i, right);} //if there are more unsorted numbers to the right of i, sort them
}


//Comparator for qsort() function from "stdlib.h"
int cmpfunc (const void * a, const void * b)
{
    return (*(int*)a - *(int*)b);
}




//this is the beginning of the parallel quicksort created by "eduardlopez" which has been re-typed from github
//the original can be found here: https://github.com/eduardlopez/quicksort-parallel/blob/master/quicksort-omp.h
//declaration for parallelized internal quicksort
void quicksortParallel_internal(int *arr, int left, int right, int cutoff);
void quicksortParallel(int *arr, int arrLength, int threads){

    int cutoff = 1000; //if we're sorting this many values, there's no need to parallelize the sort further

    omp_set_num_threads(threads); //set to 8 threads
    #pragma omp parallel
    {
        #pragma omp single nowait
            {
                quicksortParallel_internal(arr, 0, arrLength - 1, cutoff);
            }
    }
}


void quicksortParallel_internal(int *arr, int left, int right, int cutoff)
{
    int i = left;                      //variable for finding the number left of the pivot
    int j = right;                     //variable for finding the number right of the pivot
    int temp;                          //a temporary variable for use in moving values within the array
    int pivot = arr[(left + right)/2]; //the value (within the array) at which an array will be separated into its higher and lower values


    while (i <= j)
    {
        //find a number to the left of the pivot that's greater than it
        while (arr[i] < pivot)
            i++;
        //find a number to the right of the pivot that's lesser than it
        while (arr[j] > pivot)
            j--;

        //as long as i <= j, the two values can be swapped
        if (i <= j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

//if there are less than a thousand values left in the array to be sorted, don't parallelize the sort
//otherwise, parallelize the sort by defining further tasks
    if (((right - left) < cutoff))
    {
        if (left < j) { quicksortParallel_internal(arr, left, j, cutoff); }
        if (i < right) { quicksortParallel_internal(arr, i, right, cutoff); }
    }else{
#pragma omp task
        { quicksortParallel_internal(arr, left, j, cutoff);}
#pragma omp task
        { quicksortParallel_internal(arr, i, right, cutoff);}
    }
}
//this is the end of the parallel quicksort from "eduardlopez"




int main () {

    //this is the beginning of the block of code from rosetta code
    //it can be found here: http://rosettacode.org/wiki/Sorting_algorithms/Quicksort#C.2B.2B
    /* int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int n = sizeof a / sizeof a[0];

    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    quicksortRosetta(a, n);

    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");*/
    //this is the end of the block of code from rosetta code


    //this is a part of the main created by "eduardlopez" which has been re-typed from github
    //the original can be found here: https://github.com/eduardlopez/quicksort-parallel/blob/master/quicksort-demo-comparison.c
    //some parts have been modified for clarity and to compensate for the use of 4 quicksort methods
    int minNum = 1;               //minimum value for numbers
    int maxNum = arrayLength;     //maximum value for numbers
    int maxNumbersDisplayed = 30; //defines whether the array is small enough to be shown before the sort
    int i;

    double start, stop; //used for determining start and stop times for each sort

    int *arr1; //for qsort
    int *arr2; //for rosetta quicksort
    int *arr3; //for "eduardlopez's" serial quicksort
    int *arr4; //for "eduardlopez's" parallel quicksort
    arr1 = (int*) malloc(arrayLength*sizeof(int));
    arr2 = (int*) malloc(arrayLength*sizeof(int));
    arr3 = (int*) malloc(arrayLength*sizeof(int));
    arr4 = (int*) malloc(arrayLength*sizeof(int));

    printf("\nSize of the array approximately: %lu MB \n", (arrayLength*sizeof(int)/(1024*1024)));
    printf("Total memory allocated: 3 x array = approximately: %lu MB \n\n", (arrayLength*sizeof(int)/(1024*1024))*3);




    //Initialize array with random numbers
    //int i;
    srand(5);
    printf("Initializing array...\n");
    for (i = 0; i < arrayLength; i++)
    {
        arr1[i] = minNum + (rand() % maxNum); //generates value between minimum and maximum
        arr2[i] = arr1[i];
        arr3[i] = arr1[i];
        arr4[i] = arr1[i];
    }
    printf("Complete.\n");



    //if the array is relatively small, might as well show the values being sorted beforehand
    if (arrayLength <= maxNumbersDisplayed)
    {
        printf("\n\nArray before sort:\n");
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d", arr1[i]);
        }
        printf("\n\n\n");
    }


    printf("\nSorting using stdlib.h's qsort method (serial)..."); fflush(stdout);
    start = clock();
    qsort(arr1, arrayLength, sizeof(int), cmpfunc);
    stop = clock();


    //if the array is relatively small, show the sorted values
    if (arrayLength <= maxNumbersDisplayed)
    {
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d", arr1[i]);
        }
    }
    printf("\nSorted in approximately: %f seconds \n\n", (stop - start)/CLOCKS_PER_SEC);



    printf("\nSorting using rosetta code's quicksort method (serial)..."); fflush(stdout);
    start = clock();
    quicksortRosetta(arr2, arrayLength);
    stop = clock();


    //if the array is relatively small, show the sorted values
    if (arrayLength <= maxNumbersDisplayed)
    {
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d", arr2[i]);
        }
    }
    printf("\nSorted in approximately: %f seconds \n\n", (stop - start)/CLOCKS_PER_SEC);




    printf("\nSorting using eduardlopez's serial quicksort..."); fflush(stdout);
    start = clock();
    quicksort(arr3, 0, arrayLength - 1);
    stop = clock();


    //if the array is relatively small, show the sorted values
    if (arrayLength <= maxNumbersDisplayed)
    {
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d", arr3[i]);
        }
    }
    printf("\nSorted in approximately: %f seconds \n\n", (stop - start)/CLOCKS_PER_SEC);




    printf("\nSorting using eduardlopez's parallel quicksort..."); fflush(stdout);
    start = omp_get_wtime();
    quicksortParallel(arr4, arrayLength, numThreads);
    stop = omp_get_wtime();


    //if the array is relatively small, show the sorted values
    if (arrayLength <= maxNumbersDisplayed)
    {
        for (i = 0; i < arrayLength; i++)
        {
            printf("%d", arr4[i]);
        }
    }
    printf("\nSorted in approximately: %f seconds \n\n", (stop - start));






    printf("Verifying results...\n");
    typedef enum {false, true} bool;
    bool correctResult;


    correctResult = true;
    i = 0;
    while( (correctResult == true) && (i < arrayLength) )
    {
        if (arr1[i] != arr2[i]) {correctResult = false;}
        i++;
    }
    if (correctResult == true)
    {
        printf("rosetta's serial quicksort is correct\n");
    }else{
        printf("rosetta's serial quicksort is incorrect\n");
    }

    correctResult = true;
    i = 0;
    while( (correctResult == true) && (i < arrayLength) )
    {
        if (arr1[i] != arr3[i]) {correctResult = false;}
        i++;
    }
    if (correctResult == true)
    {
        printf("eduardlopez's serial quicksort is correct\n");
    }else{
        printf("eduardlopez's serial quicksort is incorrect\n");
    }

    correctResult = true;
    i = 0;
    while( (correctResult == true) && (i < arrayLength) )
    {
        if (arr1[i] != arr4[i]) {correctResult = false;}
        i++;
    }
    if (correctResult == true)
    {
        printf("eduardlopez's parallel quicksort is correct\n");
    }else{
        printf("eduardlopez's parallel quicksort is incorrect\n");
    }



    free(arr1);
    free(arr2);
    free(arr3);
    free(arr4);

    return 0;
}