/*
** Members
** BT20CSE144 Anukalp Pandey
** BT20CSE113 Anubhav Govind
** BT20CSE139 Utkarsh Srivastava
*/

#include <iostream>
#include <cmath>
#include <climits>

using namespace std;

#define BLOCK_SIZE 8.0
#define META_SIZE ((int)ceil(sizeof(node) / 8.0)) * 8

struct node
{
    int status;     // Meta data to show the allocation status of the heap blocks
    int size;       // Meta data to show the size of the block 
    node *next;     // Pointer to point next block in the heap
    node *prev;     // Pointer to point previous block in the heap
    long long *num; // Pointer which points to the location where payload is stored
};

node *heap = NULL;          //Pointer that points to the start of the heap
node *forNextFit = heap;    //The pointer that marks the traversal starting point for Next Fit algorithm
node *bestFit = NULL;       //The pointer that points to the current best location for Best Fit algorithm
int diff = INT_MAX;         //Stores the difference between the Actual size of the whole block and payload size for the Best Fit algorithm

node *createNode(int Size)  //Utility Function: pass the size of payload to allocate the block while creation of heap
{
    node *ptr = new node;
    ptr->num = new long long[(int)ceil(Size / BLOCK_SIZE)];
    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->status = 1;
    ptr->size = META_SIZE + 8 * (int)ceil(Size / BLOCK_SIZE);
    return ptr;
}

void coalesce()             //Function to merge two or more consecutive blocks with unallocated status         
{
    node *temp = heap;
    node *temp2 = heap->next;
    int size;
    while (temp && temp2)
    {
        if (temp->status == 0 && temp2->status == 0)
        {
            if(forNextFit==temp)
            {
                if(temp->prev!=NULL)
                {
                    forNextFit = temp->prev;
                }
            }
            size = temp->size + temp2->size - META_SIZE;
            node *tempPrev = temp->prev;
            node *tempNext = temp2->next;
            delete temp;
            delete temp2;
            temp = createNode(size);
            temp->status = 0;
            temp->prev = tempPrev;
            temp->next = tempNext;
            temp2 = temp->next;
            if (tempPrev)
            {
                tempPrev->next = temp;
            }
            else
            {
                heap = temp;
            }
            if (tempNext)
            {
                tempNext->prev = temp2;
            }
            continue;
        }
        temp = temp2;
        temp2 = temp2->next;
    }
    return;
}

node *splitting(node *ptr, int payloadSize) //Function to split the block to fit the Payload without much internal fragmentation
{
    int netSize = META_SIZE + 8 * (int)ceil(payloadSize / BLOCK_SIZE);
    if (ptr->size == netSize || (ptr->size - netSize) < 24)
    {
        return ptr;
    }
    else
    {
        node *tempPrev = ptr->prev;
        node *tempNext = ptr->next;
        int diff = ptr->size - netSize;
        delete ptr;
        ptr = createNode(payloadSize);
        ptr->status = 1;
        ptr->next = createNode(diff - META_SIZE);
        ptr->prev = tempPrev;
        ptr->next->next = tempNext;
        if (tempPrev)
        {
            tempPrev->next = ptr;
        }
        if (tempNext)
        {
            tempNext->prev = ptr->next;
        }
        ptr->next->status = 0;
    }
    forNextFit = ptr;
    return ptr;
}

void freeMem(node *ptr)     //Function to free the allocated block by changing the allocation status to 0
{
    if (ptr)
    {
        ptr->status = 0;
        coalesce();
    }
    else
    {
        cout << "The pointer does not point to a valid location\n";
    }
    return;
}

void createHeap(int arr[], int n)   //Utility Function: Used for creation of heap using an array of payload sizes of different blocks
{
    node *temp = NULL;
    for (int i = 0; i < n; i++)
    {
        if (heap == NULL)
        {
            heap = createNode(arr[i]);
            forNextFit = heap;
            temp = heap;
        }
        else
        {
            node *ptr = createNode(arr[i]);
            temp->next = ptr;
            ptr->prev = temp;
            temp = ptr;
        }
    }
    return;
}

void createHoles(int arr[]) //Utility Function: Used to free the allocation blocks in bulk just after creating a Heap
{
    node *t = forNextFit;
    node *temp = heap;
    int i = 0;
    while (temp)
    {
        if (arr[i] == 0)
        {
            freeMem(temp);
        }
        temp = temp->next;
        i++;
    }
    forNextFit = t;
    return;
}

void displayHeap()          //Utility Function: Used to display the complete current state of the heap 
{
    node *temp = heap;
    int flag = 1;
    cout << "START";
    while (temp)
    {
        cout << "----block_" << flag++ << "==>[" << temp->size << "|" << temp->status << "]";
        temp = temp->next;
    }
    cout << "\n";
    return;
}

node *allocateFirstFit(int Size) //First Fit Algorithm of Heap Management
{
    node *temp = heap;
    while (temp)
    {
        if (temp->status == 0)
        {
            if ((temp->size - META_SIZE) == Size)
            {
                temp->status = 1;
                forNextFit = temp;
                return temp;
            }
            else if ((temp->size - META_SIZE) > Size)
            {
                temp = splitting(temp, Size);
                temp->status = 1;
                forNextFit = temp;
                return temp;
            }
        }
        temp = temp->next;
    }
    cout << "MEMORY CANNOT BE ALLOCATED\n";
    return NULL;
}

node *allocateNextFit(int Size)  // Next Fit algorithm of Heap Management
{
    node *temp = forNextFit;
    do
    {
        if (temp == NULL)
        {
            temp = heap;
            continue;
        }
        if (temp->status == 0)
        {
            if ((temp->size - META_SIZE) == Size)
            {
                temp->status = 1;
                forNextFit = temp;
                return temp;
            }
            else if ((temp->size - META_SIZE) > Size)
            {
                temp = splitting(temp, Size);
                temp->status = 1;
                forNextFit = temp;
                return temp;
            }
        }
        temp = temp->next;
    } while (temp != forNextFit);

    cout << "MEMORY CANNOT BE ALLOCATED\n";
    return NULL;
}

node *allocateBestFit(int Size)  //Best Fit Algorithm of Heap Management
{
    node *temp = heap;
    diff = INT_MAX;
    while (temp)
    {
        if (temp->status == 0)
        {
            if ((temp->size - META_SIZE - Size) >= 0 && (temp->size - META_SIZE - Size) < diff)
            {
                diff = temp->size - META_SIZE - Size;
                bestFit = temp;
            }
        }
        temp = temp->next;
    }
    if (bestFit)
    {
        if ((bestFit->size - META_SIZE) == Size)
        {
            bestFit->status = 1;
            forNextFit = bestFit;
            return bestFit;
        }
        else if ((bestFit->size - META_SIZE) > Size)
        {
            bestFit = splitting(bestFit, Size);
            bestFit->status = 1;
            forNextFit = bestFit;
            return bestFit;
        }
        return bestFit;
    }
    else
    {
        cout << "MEMORY CANNOT BE ALLOCATED\n";
        return NULL;
    }
}

//---------------------------------------------------------------TEST CASES--------------------------------------------------------------------
int main()
{
    int arr[] = {64, 16, 32, 8, 128, 24}; // array representing payload sizes to create the linked list of 6 heap blocks
    int holes[] = {0, 1, 0, 1, 0, 1}; // array representing allocation status bits to create holes in the linked list
    createHeap(arr, 6);  //Creating Heap 
    createHoles(holes);
    cout<<"\nAfter creating a heap with some allocated blocks\n";
    displayHeap();
    node *ptr1 = allocateFirstFit(40);  // 40(payload) + 24(meta) = 64
    cout<<"\nAfter calling First Fit for 1st time\n";
    displayHeap();
    node *ptr2 = allocateNextFit(32);   // 32(payload) + 24(meta) = 56
    cout<<"\nAfter calling Next Fit for the first time\n";
    displayHeap();
    freeMem(ptr1);
    cout<<"\nAfter freeing a memory location\n";
    displayHeap();
    ptr1 = allocateNextFit(20);         // 20 is assigned payload of 24 because of 8 block size => 24(payload) + 24(meta) = 48
    cout<<"\nAfter calling  Next Fit for the second time\n";
    displayHeap();
    ptr2 = allocateBestFit(16);         // 16(payload) + 24 (meta) = 40
    cout<<"\nAfter calling  Best Fit for the first time\n";
    displayHeap();
    cout<<"\nAfter calling First Fit for the second time\n";
    node *ptr3 = allocateFirstFit(40);  // 40(payload) + 24 (meta) = 64
    displayHeap();
    node *ptr4 = allocateBestFit(16);   // 16(payload) + 24 (meta) = 40
    cout<<"\nAfter calling  Best Fit for the second time\n";
    displayHeap();
    return 0;
}

/*
OUTPUT

After creating a heap with some allocated blocks
START----block_1==>[88|0]----block_2==>[40|1]----block_3==>[56|0]----block_4==>[32|1]----block_5==>[152|0]----block_6==>[48|1]

After calling First Fit for 1st time
START----block_1==>[64|1]----block_2==>[24|0]----block_3==>[40|1]----block_4==>[56|0]----block_5==>[32|1]----block_6==>[152|0]----block_7==>[48|1]

After calling Next Fit for the first time
START----block_1==>[64|1]----block_2==>[24|0]----block_3==>[40|1]----block_4==>[56|1]----block_5==>[32|1]----block_6==>[152|0]----block_7==>[48|1]

After freeing a memory location
START----block_1==>[88|0]----block_2==>[40|1]----block_3==>[56|1]----block_4==>[32|1]----block_5==>[152|0]----block_6==>[48|1]

After calling  Next Fit for the second time
START----block_1==>[88|0]----block_2==>[40|1]----block_3==>[56|1]----block_4==>[32|1]----block_5==>[48|1]----block_6==>[104|0]----block_7==>[48|1]

After calling  Best Fit for the first time
START----block_1==>[40|1]----block_2==>[48|0]----block_3==>[40|1]----block_4==>[56|1]----block_5==>[32|1]----block_6==>[48|1]----block_7==>[104|0]----block_8==>[48|1]

After calling First Fit for the second time
START----block_1==>[40|1]----block_2==>[48|0]----block_3==>[40|1]----block_4==>[56|1]----block_5==>[32|1]----block_6==>[48|1]----block_7==>[64|1]----block_8==>[40|0]----block_9==>[48|1]

After calling  Best Fit for the second time
START----block_1==>[40|1]----block_2==>[48|0]----block_3==>[40|1]----block_4==>[56|1]----block_5==>[32|1]----block_6==>[48|1]----block_7==>[64|1]----block_8==>[40|1]----block_9==>[48|1]
 */ 
