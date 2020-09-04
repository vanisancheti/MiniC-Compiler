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

void main()
{
    int N;
    N = read();

    bool ans;
    ans = isPrime(N);

    print("print ");
    print(ans);

    return;
}