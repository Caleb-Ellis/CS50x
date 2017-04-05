/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Set initial max and min values
    int min = 0;
    int max = n - 1;
    
    // While there are still elements in the list
    while (n > 0)
    {
        // Mid-point is determined
        int mid = (max - min) / 2 + min;
        // If value is at midpoint, return true
        if (values[mid] == value)
        {
            return true;
        }
        // Otherwise if it's greater, set new max to one element left of old midpoint
        else if (values[mid] > value)
        {
            max = mid - 1;
        }
        // If lesser, set new min to one element to the right of the old midpoint
        else if (values[mid] < value)
        {
            min = mid + 1;
        }
        // Set new number of elements
        n = max - min + 1;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Selection sort cycles through each element of "values"
    for (int i = 0; i < n - 1; i++)
    {
        // Minimum is the i'th element of "values"
        int min = i;
        // Selection sort compares the i'th element with the remaining elements in order to determine the current lowest value in "values"
        for (int j = i + 1; j < n; j++)
        {
            // If a new minimum is found, store that new value in "min", thereby replacing "i"
            if (values[j] < values[min])
            min = j;
        }
        // Swap values if there is a new minimum
        if (min != i)
        {
            int temp = values[min];
            values[min] = values[i];
            values[i] = temp;
        }
    }
    return;
}
