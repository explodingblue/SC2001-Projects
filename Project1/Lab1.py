import random

# Global counter
key_comparisons = 0

# Insertion Sort
def swap(A, i, j):
    A[i], A[j] = A[j], A[i] # swap elements at index i and j

def insertion_sort(A):
    global key_comparisons
    for i in range(1, len(A)):
        for j in range(i, 0, -1): # move the current element left in the sorted portion
            key_comparisons += 1  # count comparison
            if A[j] < A[j - 1]: # compare the current element with its left neighbor
                swap(A, j, j - 1) # swap if it is smaller
            else: # if in the correct position
                break
    return A

# Merge Sort
def merge(list1, list2):
    global key_comparisons
    if list1 == [] or list2 == []: # if either list is empty
        return list1 + list2 # just return the other list

    sorted_list = []
    while list1 != [] and list2 != []: # while both lists are non-empty
        key_comparisons += 1  # count comparison
        head1, head2 = list1[0], list2[0] # get the first element of each list
        if head1 <= head2: # compare the elements
            sorted_list.append(head1) # add the smaller (or equal) element
            list1 = list1[1:] # remove it from list1
        else:
            sorted_list.append(head2) # add the smaller element
            list2 = list2[1:] # remove it from list2

    return sorted_list + list1 + list2 # append any remaining elements

def merge_sort(A):
    if len(A) <= 1:
        return A
    mid = len(A) // 2 # find the midpoint of the list
    B1 = merge_sort(A[:mid]) # recursively sort the left half
    B2 = merge_sort(A[mid:]) # recursively sort the right half
    return merge(B1, B2) # merge the sorted halves

# Hybrid Merge Sort
def hybrid_merge_sort(A, S):
    if len(A) <= 1:
        return A
    if len(A) <= S: # if list size is below threshold S
        return insertion_sort(A)

    mid = len(A) // 2 # find the midpoint
    B1 = hybrid_merge_sort(A[:mid], S) # recursively sort left half
    B2 = hybrid_merge_sort(A[mid:], S) # recursively sort right half
    return merge(B1, B2) # merge the sorted halves

# Generate a dataset
def generate_dataset(n):
    return [random.randint(1, n) for _ in range(n)] # list of n random integers from 1 to n

# Example usage
if __name__ == "__main__":
    n = 1000       # dataset size
    S = random.randint(5, 100)  # random threshold
    data = generate_dataset(n) # generate the dataset

    # Insertion Sort
    key_comparisons = 0 # reset the counter
    insertion_sort(data.copy()) # sort a copy of the dataset
    insertion_count = key_comparisons # store the number of comparisons

    # Merge Sort
    key_comparisons = 0 # reset the counter
    merge_sort(data.copy()) # sort a copy of the dataset
    merge_count = key_comparisons # store the number of comparisons

    # Hybrid Merge Sort
    key_comparisons = 0 # reset the counter
    hybrid_merge_sort(data.copy(), S) # sort a copy of the dataset
    hybrid_count = key_comparisons # store the number of comparisons

    # Print results
    print(f"Input size: {n}, Threshold S: {S}")
    print(f"Insertion Sort comparisons: {insertion_count}")
    print(f"Merge Sort comparisons: {merge_count}")
    print(f"Hybrid Merge Sort comparisons: {hybrid_count}")