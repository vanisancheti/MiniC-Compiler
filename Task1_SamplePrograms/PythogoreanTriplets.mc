void main()
{
    int x;
    int y;
    int z;
    x=0;
    y=0;
    z=0;

    int max_value;
    max_value = 1000000000;

    int m;
    m=2;

    int count;
    count=0;
    
    while(z < max_value)
    {
        int n;
        for(n=1;n<m; n=n+1)
        {
            x = m*m - n*n;
            y = 2*m*n;
            z = m*m + n*n;

            if(z > max_value)
            {
                break;
            }
            count = count+1;
        }
        m = m+1;
    }

    print(count);
    print("\n");

    return;
}