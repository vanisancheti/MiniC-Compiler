int arr[100];

void main()
{
    int N;
    N = read();

    int arr[N];
    int i;
    int j;

    for(i=0;i<N;i=i+1)
    {
        arr[i] = read();
    }

    int temp;
    for(i=0;i<N;i=i+1)
    {
        temp = arr[i];
        for(j=i-1;j>=0;j=j-1)
        {
            if(arr[j] > temp)
            {
                arr[j+1] = arr[j];
            }
            else
            {
                arr[j+1] = temp;
            }
        }
    }

    for(i=0;i<N;i=i+1)
    {
        print(arr[i]);
        print(" ");
    }
    print("\n");

    return;
}