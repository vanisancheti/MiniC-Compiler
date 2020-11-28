void main()
{
    int N;
    int M;

    N = read();
    M = read();

    int arrone[N,M];
    int arrtwo[N,M];

    int arrthree[N,M];

    int i;
    int j;

    for(i=0;i<N;i=i+1)
    {
        for(j=0;j<M;j=j+1)
        {
            arrone[i,j] = read();
            arrtwo[i,j] = read();
        }
    }

    for(i=0;i<N;i = i+1)
    {
        for(j=0;j<M;j=j+1)
        {
            arrthree[i,j] = arrone[i,j] + arrtwo[i,j];
            print(arrthree[i,j]);
            print(" "); 
        }
        print("\n");
    }

    return;
}