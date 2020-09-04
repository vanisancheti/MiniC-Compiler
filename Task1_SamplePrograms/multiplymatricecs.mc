void main()
{
    int P;
    int Q;
    int R;

    P = read();
    Q = read();
    R = read();

    int arr1[P,Q];
    int arr2[Q,R];

    int arr3[P,R];

    int i;
    int j;
    int k;

    for(i=0;i<P;i=i+1)
    {
        for(j=0;j<Q;j=j+1)
        {
            arr1[i][j] = read();
        }
    }

    for(i=0;i<Q;i=i+1)
    {
        for(j=0;j<R;j=j+1)
        {
            arr2[i][j] = read();
        }
    }

    for(i=0;i<P;i=i+1)
    {
        for(j=0;j<R;j=j+1)
        {
            for(k=0;k<Q;k=k+1)
            {
                arr3[i,j] = arr3[i,j] + arr1[i,k] + arr2[k,j];
            }
            print(arr3[i,j]);
            print(" ");
        }
        print("\n");
    }

    return;
}