#include<stdio.h>
void predictor(x,k)
double x[];
int k;
{ 
int m,i,j;
float t[7];
float L[7];


k=6;
m=1+k;       //number of parameters
for(i=0;i<=m;i++)
t[m-i]=i; //current + m previous points

for(i=0;i<7;i++)
printf("T[%d] = %d \n",i,t[i]);

for(i=0;i<7;i++)
if(i<m)
L[i]=1;
else
L[i]=0;

printf("\n\n");

for(i=0;i<7;i++)
printf("L[%d] = %d \n",i,L[i]);
printf("\n\n");

for(i=1;i<=m;i++)  //go through all o+1 excluding current
for(j=1;j<=m;j++)  //go through all o+1 excluding current
{
printf("%d %d %f \n",i,j,L[i-1]);
if(i!=j)           // BUT points where i and j don't cancel
{
printf("%f; %f; %f \n",t[0],t[j],t[i]);
L[i-1]=L[i-1]*((t[0]-t[j])/(t[i]-t[j]));  //(curr - j)/(i-j)
}

}

for(i=0;i<7;i++)
printf("L[%d]= %f \n",i,L[i]);

for(i=1;i<7;i++)
{
x[0] += L[i-1]*x[i];
}

