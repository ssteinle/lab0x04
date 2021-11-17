#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long int busyCount;
long long int countNumber = 10000000;

void doBusyWork()
{
    long long int x = countNumber;

    for(long long int a=0; a<x; a++)
    {
        busyCount++;
    }
}

struct tripletSum
{
    struct tripletSum *nextTriplet;
    int sumArr[3];
};

struct tripletSum startLlist()
{
    struct tripletSum* newSum = (struct tripletSum*) malloc (sizeof(struct tripletSum));

    newSum->nextTriplet = NULL;

    newSum->sumArr[0]=-9999999;

    return *newSum;
}

void freeList(struct tripletSum *head)
{
   struct tripletSum* temp;

   while (head != NULL)
    {
       temp = head;
       head = head->nextTriplet;
       free(temp);
    }
}

void printList(struct tripletSum *head)
{
    int i =0;

    struct tripletSum *tmp = head;

    if(tmp->sumArr[0] == -9999999)
        return;

    printf("\n");

    while (tmp != NULL)
    {
       i++;

       printf("\n%d: %d, %d, %d", i, tmp->sumArr[0], tmp->sumArr[1], tmp->sumArr[2]);

       tmp = tmp->nextTriplet;

    }
}

void addTriplet(struct tripletSum *head, int a, int b, int c)
{

    if(head->sumArr[0] == -9999999)
    {
        head->sumArr[0] = a;
        head->sumArr[1] = b;
        head->sumArr[2] = c;
    }
    else
    {
        struct tripletSum* newSum = (struct tripletSum*) malloc (sizeof(struct tripletSum));

        struct tripletSum *last = head;

        newSum->sumArr[0] = a;
        newSum->sumArr[1] = b;
        newSum->sumArr[2] = c;

        newSum->nextTriplet = NULL;

        while (last->nextTriplet != NULL)
                last = last->nextTriplet;

        last->nextTriplet = newSum;
    }

    doBusyWork();

}

void generateTriSumTestInput(int *arr, int arrSize, int max, int nTriplets)
{
    srand(time(0));

    int tOne;
    int tTwo;
    int tThree;
    int sum;

    for(int i=0; i<arrSize; i++)
    {
        arr[i] = rand() % max;

        if(rand() %2 == 0)
            arr[i] = arr[i]*-1;
    }

    for(int i=0; i<nTriplets; i++)
    {
        tOne = rand() % arrSize;

        do
        tTwo = rand() % arrSize;
        while(tTwo == tOne);

        do
        tThree = rand() % arrSize;
        while(tThree == tOne || tThree == tTwo);

        sum = arr[tOne]+arr[tTwo]+arr[tThree];

        if(sum != 0)
            arr[tOne] -= sum;
    }
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int findDuplicate(struct tripletSum* head, int a, int b, int c)
{
    struct tripletSum *tmp;
    int duplicate = 0;
    tmp = head;

    while (tmp != NULL && tmp->sumArr[0] != -9999999)
    {
        if(tmp->sumArr[0] == a || tmp->sumArr[1] == a || tmp->sumArr[2] == a)
                if(tmp->sumArr[0] == b || tmp->sumArr[1] == b || tmp->sumArr[2] == b)
                        if(tmp->sumArr[0] == c || tmp->sumArr[1] == c || tmp->sumArr[2] == c)
                        {
                            duplicate = 1;
                            break;
                        }

        tmp = tmp->nextTriplet;
    }

    //printf("\n%d", duplicate);

    return duplicate;
}

void bruteForce(int *arr, int arrSize, struct tripletSum* head)
{
for(int i=0;i<arrSize-2;i++)
    for(int j=i+1;j<arrSize-1;j++)
        for(int k=j+1;k<arrSize;k++)
            if (arr[i]+arr[j]+arr[k] == 0)
                addTriplet(head, i, j, k);
}

void fasterForce(int *arr, int arrSize, struct tripletSum* head)
{
    int sum;
    int floor;
    int ceiling;
    int n;
    int m;

    for(int i=0;i<arrSize;i++)
        for(int j=0;j<arrSize;j++)
            if(j != i)
            {
                floor = 0;
                ceiling = arrSize;
                n = arrSize / 2;
                m = arrSize;

                sum = arr[i]+arr[j];

                while(m != -1)
                {
                    m = m/2;

                    if(m==0)
                        m = -1;

                    if(sum + arr[n] > 0)
                    {
                        ceiling = n;
                        n = (ceiling + floor)/2;
                    }
                    else if(sum + arr[n] < 0)
                    {
                        floor = n;
                        n = (ceiling + floor)/2;
                    }

                    if(sum + arr[n] == 0 && n!=i && n!=j)
                    {
                        if(findDuplicate(head, i, j, n) != 1)
                            addTriplet(head, i, j, n);
                        else if((sum + arr[n+1] == 0) && (arr[n+1] == arr[n]))
                            if(findDuplicate(head, i, j, n+1) != 1)
                                addTriplet(head, i, j, n+1);
                        else if((sum + arr[n-1] == 0) && (arr[n-1] == arr[n]))
                            if(findDuplicate(head, i, j, n-1) != 1)
                                addTriplet(head, i, j, n-1);
                    }
                }
            }

}

void fastestForce(int *arr, int arrSize, struct tripletSum* head)
{
    int first;
    int last;
    int d;

    for(int i=0;i<arrSize-2;i++)
    {
        first = i + 1;
        last = arrSize - 1;

        while(first < last)
        {
            if(arr[i]+arr[first]+arr[last] == 0)
                addTriplet(head, i, first, last);
            
            if (arr[i]+arr[first]+arr[last] < 0)
                first++;
            else
                last--;
        }
    }
}

int main()
{
    int tripArrSize;
    int* tripArr;
    int maxValue;
    int nForcedTriplets;

    clock_t start, end;
    double total;

    struct tripletSum mainNode;

    printf("Enter size of list: ");
    scanf("%d", &tripArrSize);

    printf("\nEnter maximum value: ");
    scanf("%d", &maxValue);

    printf("\nEnter number of triplets: ");
    scanf("%d", &nForcedTriplets);

    {
        doBusyWork();

        tripArr = (int *) malloc (tripArrSize * sizeof(int));

        generateTriSumTestInput(tripArr, tripArrSize, maxValue+1, nForcedTriplets);

        printf("\n");

        quickSort(tripArr, 0, tripArrSize-1);

        //for(int i=0; i<tripArrSize; i++)
            //printf("%d: %d ", i, tripArr[i]);

        printf("\n\n==================================================");
        //if(0==1)
        {

            mainNode = startLlist();
            start = clock();
                bruteForce(tripArr, tripArrSize, &mainNode);
            end = clock();

            printf("\n\nN^3: for N=%u", tripArrSize);
            //printf("\n\nTriplets:");
            //printList(&mainNode);
            freeList(&mainNode);

            total = (double)(end - start) / CLOCKS_PER_SEC;
            printf("\n\nExperimental Time Raw: %f", total);
            printf("\n\n==================================================");
        }

        //if(0==1)
        {
            mainNode = startLlist();
            start = clock();
            fasterForce(tripArr, tripArrSize, &mainNode);
            end = clock();


            printf("\n\nlog(N)*N^2: for N=%u", tripArrSize);
            //printf("\n\nTriplets:");
            //printList(&mainNode);
            freeList(&mainNode);

            total = (double)(end - start) / CLOCKS_PER_SEC;
            printf("\n\nExperimental Time Raw: %f", total);
            printf("\n\n==================================================");
        }
        
        //if(0==1)
        {

            mainNode = startLlist();
            start = clock();
            fastestForce(tripArr, tripArrSize, &mainNode);
            end = clock();
            
            printf("\n\nN^2: for N=%u", tripArrSize);
            //printf("\n\nTriplets:");
            //printList(&mainNode);
            freeList(&mainNode);
            total = (double)(end - start) / CLOCKS_PER_SEC;
            printf("\n\nExperimental Time Raw: %f", total);

            printf("\n\n==================================================\n");
        }
        free (tripArr);
    }




    return 0;
}