int arr[100];

void sort(int digit, int N)
{
    int temp[N];
    int count[10];
    int i;

    for(i=0;i<10;i=i+1)
    {
        count[i] = 0;
    }

    for(i=0;i<N;i=i+1)
    {
        count[(arr[i]/digit)%10] = count[(arr[i]/digit)%10] + 1; 
    }

    for(i=1;i<10;i=i+1)
    {
        count[i] = count[i] + count[i-1];
    }

    for(i=N-1;i>=0;i=i-1)
    {
        temp[count[(arr[i]/digit)%10]-1] = arr[i];
        count[(arr[i]/digit)%10] = count[(arr[i]/digit)%10]-1;
    }

    for(i=0;i<N;i=i+1)
    {
        arr[i] = temp[i];
    }

    return;
}

void main()
{
    int N;
    N = read();

    int arr[N];
    int i;

    for(i=0;i<N;i=i+1)
    {
        arr[i] = read();
    }

    int max_digit;
    int max;
    max = arr[0];

    for(i=1;i<N;i=i+1)
    {
        if(arr[i] > max)
        {
            max = arr[i];
        }
    }

    for(i=1;max/i > 0; i = i*10)
    {
        sort(i, N);
    }

    for(i=0;i<N;i=i+1)
    {
        print(arr[i]);
        print(" ");
    }
    print("\n");

    return;
}