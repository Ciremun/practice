#include <stdlib.h>
#include <stdio.h>

#define TEST_TARGET 69
#define TEST_ARR_SIZE 69420

int binary_search(int *array, int low, int high, int target)
{
    if (low > high)
    {
        return -1;
    }
    int mid = low + (high - low) / 2;
    if (array[mid] == target)
    {
        return mid;
    }
    if (array[mid] > target)
    {
        return binary_search(array, low, mid - 1, target);
    }
    return binary_search(array, mid + 1, high, target);
}

int main(void)
{
    int *test_arr = malloc(TEST_ARR_SIZE * sizeof(int));
    if (test_arr == NULL)
    {
        fprintf(stderr, "error: memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < TEST_ARR_SIZE; i++)
    {
        test_arr[i] = i + 1;
    }
    int result = binary_search(test_arr, 0, TEST_ARR_SIZE - 1, TEST_TARGET);
    if (result == -1)
    {
        printf("%d was not found in test_arr[%d]\n", TEST_TARGET, TEST_ARR_SIZE);
    }
    else
    {
        printf("pos of %d in test_arr[%d] == %d\n", TEST_TARGET, TEST_ARR_SIZE, result);
    }
    free(test_arr);
    return 0;
}