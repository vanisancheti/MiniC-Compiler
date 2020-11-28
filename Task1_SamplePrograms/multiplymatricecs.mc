void main()
{
    int P;
    int Q;
    int R;

    P = read();
    Q = read();
    R = read();

    int arra[P,Q];
    int arrb[Q,R];

    int arrc[P,R];

    int i;
    int j;
    int k;

    for(i=0;i<P;i=i+1)
    {
        for(j=0;j<Q;j=j+1)
        {
            arra[i,j] = read();
        }
    }

    for(i=0;i<Q;i=i+1)
    {
        for(j=0;j<R;j=j+1)
        {
            arrb[i,j] = read();
        }
    }

    for(i=0;i<P;i=i+1)
    {
        for(j=0;j<R;j=j+1)
        {
            for(k=0;k<Q;k=k+1)
            {
                arrc[i,j] = arrc[i,j] + arra[i,k]*arrb[k,j];
            }
            print(arrc[i,j]);
            print(" ");
        }
        print("\n");
    }

    return;
}