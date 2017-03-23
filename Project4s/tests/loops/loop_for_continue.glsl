
int main()
{
    int sum;
    sum = 2;
    int i;
    for(i=0; i < 5; i++){
        sum++;
        if(i>1){
            continue;
        } 
    }
    return sum;
}
