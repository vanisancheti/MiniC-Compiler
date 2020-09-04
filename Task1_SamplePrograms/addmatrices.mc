void main()
{
    int N;
    int M;

    N = read();
    M = read();

    int arr1[N,M];
    int arr2[N,M];

    int arr3[N,M];

    int i;
    int j;

    for(i=0;i<N;i=i+1)
    {
        for(j=0;j<M;j=j+1)
        {
            arr1[i][j] = read();
            arr2[i][j] = read();
        }
    }

    for(i=0;i<N;i = i+1)
    {
        for(j=0;j<M;j=j+1)
        {
            arr3[i,j] = arr1[i,j] + arr2[i,j];
            print(arr3[i,j]);
            print(" "); 
        }
        print("\n");
    }

    return;
}