// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include<iostream>

// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    for (int i = 1; i < data.size(); i++){
      int key = data[i];
      int j = i-1;
      while (j>=0 && data[j]>key){
          data[j+1] = data[j];
          j--;
      }
      data[j+1] = key;
    }
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data){
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high) {
    if (low<high){
        int q = Partition(data,low,high);
        QuickSortSubVector(data,low,q-1);
        QuickSortSubVector(data,q+1,high);}
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
}

int SortTool::Partition(vector<int>& data, int low, int high) {
    int pivot = data[high];
    int i = low-1;
    for (int j = low; j<high ; j++){
        if (data[j] <= pivot){
            i=i+1;
            int temp = data[j]; data[j] = data[i]; data[i] = temp;
            //swap(data[j],data[i]);
        }
    }
    data[high] = data[i+1]; data[i+1] = pivot;
    //swap(data[high],data[i+1]);

    return i+1;
    // Function : Partition the vector 
    // TODO : Please complete the function
    // Hint : Textbook page 171
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {
    if (low<high){
        int q = (low+high)/2;
        MergeSortSubVector(data,low,q);
        MergeSortSubVector(data,q+1,high);
        Merge(data,low,q,q+1,high);
    }
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    int n1 = middle1-low+1 ; int n2 = high-middle2+1;
    vector<int> l1; vector<int> l2;
    for (int i = 0 ; i<n1;i++){l1.push_back(data[low+i]);}
    for (int i = 0 ; i<n2;i++){l2.push_back(data[middle2+i]);}
    int i = 0; int j = 0;
    for (int k = low; k<=high; k++){
        if (i<n1 && j<n2){
          if (l1[i]<=l2[j]){
              data[k]=l1[i];
              i++;
          }
          else{
              data[k] = l2[j];
              j++;
        }}
        else{
          if (i<n1){
              data[k]=l1[i];
              i++;
          }
          if (j<n2){
              data[k] = l2[j];
              j++;
        }}
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
}}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    for (int i = data.size() - 1; i >= 1; i--) {
        int temp = data[0]; data[0] = data[i]; data[i] = temp;
        heapSize--;
        MaxHeapify(data,0);
    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root) {
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int l = (root+1)*2-1; int r = (root+1)*2; int largest;
    if (l < heapSize && data[l]>data[root]){
        largest = l;
    }
    else{largest = root;}
    if (r < heapSize && data[r]>data[largest]){
        largest = r;
    }
    if (largest != root){
        int temp = data[root] ; data[root] = data[largest]; data[largest] = temp;
        MaxHeapify(data,largest);
    }
}

//Build max heap
void SortTool::BuildMaxHeap(vector<int>& data) {
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for (int i = (heapSize/2-1) ; i>=0; i--){
        MaxHeapify(data,i);
    }
}
