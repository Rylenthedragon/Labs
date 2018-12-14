# Lab 4
This lab was taken from option 2 for my final project for Parallel Programming, since the assignment said it could be substituted as such. 
It obviously has heavy reproduction of code from another gitHub user as noted/cited within, though I specifically re-typed any code that I used so that I could understand how it worked and comment about what happens at each step. That said, there were still modifications I made to make all sections of the code run properly. The following is what I submitted along with the final project for clarity:

/* The original code here is from rosetta code and a github user with the username “eduardlopez”. The links for the code in both cases are provided before the sections in which they are found. I personally re-typed all of the code that I’ve utilized from “eduardlopez” to help me understand how the quicksort is parallelized, which is why I was able to add comments about each step. I made minor modifications to some parts of the code to ensure its functionality. Please note that I have 4 cores on my machine, and that happened to line up with what “eduardlopez” formatted his code for, so I heavily relied on his code to know how the format worked.

There are 4 quicksorts represented here, 3 serial and 1 parallel.
The stdlib.h’s method is called qsort, and it is used to verify the results of the other sorts.
The rosetta code sort is a serialized quicksort found using the link provided in the assignment sheet.
The github user “eduardlopez” had his own serial and parallel versions of the quicksort (quicksort(…) and quicksortParrallel(…)/quicksortParallel_Internal(…)).

I decided to compare the sorting times for all four algorithims just to see which serial sort was the best and how much of an improvement the parallelized version made.
After having run the program several times, I’ve found the rosetta sort and serial sort from “eduardlopez” to be about 5 seconds faster than the qsort at sorting 1 GB’s worth of int values. The parallel sort was consistently 5 times faster than either of these sorts.

The parallel section divides the workload into 2 threads per core and rationalizes that a cutoff is useful to stop unneeded parallelization at a point where the sort is manageable without it.
*/
