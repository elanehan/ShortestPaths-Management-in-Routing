#include<stdio.h>

int n, W;
int v[501], w[1001];
int s[501][1001]={0};

int max(int a, int b)
{
    return (a > b) ? a : b;
}


int opt(int i, int W)
{
    int j, k;


    for(j=1; j<=i; j++)
    {
        for(k=1; k<=W; k++)
        {
            // it's not loadable for this food
            if( k - w[j] < 0 )
            {
                s[j][k] = s[j-1][k];
            }
            // whether food[j] is inside or not
            else
            {
                s[j][k] = max(s[j-1][k], s[j-1][k-w[j]] + v[j]);
            }
        }
    }
    return s[i][W];

}

int main()
{
    int i;
    int ans=0;

    scanf("%d %d", &n, &W);
    for(i=1; i<=n; i++)
    {
        scanf("%d %d", &v[i], &w[i]);
    }

    ans = opt(n, W);
    printf("%d", ans);


    return 0;
}
