int ifnested()
{
    int x;
    x = 1;
    if (x == 1){
        x++;
        if (x == 2){
            x++;
        }
    }
    return x;
}
