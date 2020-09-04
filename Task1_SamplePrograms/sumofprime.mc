bool isPrime(int N)
{
    bool ans;
    ans = true;
    int temp;
    
    for(temp = 1; temp < N; temp = temp + 1)
    {
        if(N%temp == 0)
        {
            ans = false;
            break;
        }
    }
    
    return ans;
}

int SumPrime(int N)
{
    int temp;
    bool check;
    int sum;
    sum = 0;

    for(temp = 1; temp < N; temp = temp + 1)
    {
        check = isPrime(temp);
        if(check == true)
        {
            sum = sum + temp;
        }
    }
    return sum;
}

void main()
{
    int N;
    N = read();

    int ans;
    ans = SumPrime(N);

    print("print ");
    print(ans);

    return;
}