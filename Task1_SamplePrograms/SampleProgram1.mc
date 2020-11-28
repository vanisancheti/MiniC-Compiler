int power(int x, int k)
{
    int pow;
    
    if(k == 1)
    {
        pow = x;
    }
    else
    {
        k = k - 1;
        pow = x * power(x, k);
    }

    return pow;
}

int g(int N, int k)
{
    
    int temp;
    int sum;
    sum = 0;

    for(temp=0; temp<=N; temp = temp + 1)
    {
        sum = sum + power(temp, k);
    }

    return sum;
}

void main()
{
    int N;
    int k;
    int ans;

    N = read();
    k = read();

    ans = g(N, k);
    print(ans);

    return;
}
