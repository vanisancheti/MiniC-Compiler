int arr[100];

void merge(int l, int mid, int r)
{
    int temp_arr[r-l];
    int i;
    int j;
    int k;
    i = l;
    j = mid;
    k = 0;

    while(i!=mid AND j!=r)
    {
        if(arr[i] < arr[j])
        {
            temp_arr[k] = arr[i];
            i = i + 1;
            k = k + 1;
        }
        else
        {
            temp_arr[k] = arr[j];
            j = j + 1;
            k = k + 1;
        }
    }

    while(i!=mid)
    {
        temp_arr[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }

    while(j!=r)
    {
        temp_arr[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }

    for(i=0;i<k;i = i+1)
    {
        arr[l+i] = temp_arr[i];
    }

    return;
}

void mergesort(int l, int r)
{
    if(r <= l+1)
    {
        return;
    }
    
    int mid;
    mid = (l+r)/2;

    mergesort(l, mid);
    mergesort(mid, r);
    
    int arr_merge[100];
    merge(l, mid, r);

    return;
}

void main()
{
    int N;
    N = read();

    int arr[N];
    int i;

    for(i=0;i<N;i=i+1)
    {
        arr[i] = read();
    }

    mergesort(0, N);

    for(i=0;i<N;i=i+1)
    {
        print(arr[i]);
        print(" ");
    }
    print("\n");

    return;
}