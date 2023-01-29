#include<bits/stdc++.h>

using namespace std;

int main()
{
    int a[6][6],b,c,b1,c1;
    for(int i=1;i<=5;i++)
    {
       for(int j=1;j<=5;j++)
       {
           cin>>a[i][j];
           if (a[i][j]==1)
           {
               b=i;
               c=j;
               //if(b!=3 || c!=3)
                   b1=abs(b-3);
                   c1=abs(c-3);
           }
       }
    }
    cout<<b1+c1<<endl;

}
