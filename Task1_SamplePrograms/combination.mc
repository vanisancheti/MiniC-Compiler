int arr[100];
int temp[100];
bool visited[100];

void printcombination(int k, int N)
{
    int i;
    if(k == N)
    {
        for(i=0;i<N;i=i+1)
        {
            print(arr[i]);
            print(" ");
        }
        print("\n");
    }
    else
    {
        for(i=0;i<N;i=i+1)
        {
            if(visited[i] == false)
            {
                temp[k] = arr[i];
                k = k + 1;
                visited[i] = true;
                printcombination(k, N);
                visited[i] = false;
                break;
            }
        }
    }

    return;    
}

void main()
{
    int N;
    N = read();
    int i;

    for(i=0;i<N;i=i+1)
    {
        arr[i] = read();
        visited[i] = false;
    }

    printcombination(0, N);
    return;
}