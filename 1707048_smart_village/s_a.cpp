#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
#define endl '\n'
#include <iostream>
#define optimize() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

int main()
{
    optimize();
    int t;cin>>t;
    while(t--)
    {
        int n,a,b;
        cin>>n>>a>>b;
        if(n==a && n==b) {cout<<"Yes"<<endl; continue;}
        if((n-a-b)>=2) cout<<"Yes"<<endl;
        else cout<<"No"<<endl;
    }

}
