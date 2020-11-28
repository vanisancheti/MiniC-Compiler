void main()
{
    int L;
    int i;
    L = read();
    char filename[L];
    
    for(i=0;i<L;i=i+1)
    {
        filename[i] = read();
    }

    char data[100000];
    data = readfile(filename); 

    int count[26];
    i = 0;

    for(i=0;i<26;i=i+1)
    {
        count[i] = count[i] + 1;
    }

    while(data[i]!= EOF)
    {
        if(data[i] >= 'a' AND data[i] <= 'z')
        {
            count[data[i] - 'a'] = count[data[i] - 'a'] + 1; 
        }
        if(data[i] >= 'A' AND data[i] <= 'Z')
        {
            count[data[i] - 'A'] = count[data[i] - 'Z'] + 1; 
        }        
    }

    for(i=0;i<26;i=i+1)
    {
        print(count[i]);
        print("\n");
    }

    return;
}