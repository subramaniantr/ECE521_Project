#include<stdio.h>
#include "defs.h"


void predictor(x)
double *x;
{ 
int m,i,j;
float t[7];
float L[7];
x[0]=0; //reset before prediction

m=1+order;       //number of parameters = K+1


/////////////TIME POINTS//////////////////

      for(i=0;i<=6;i++)
        {
        t[i]=time_points[i];//current + m previous points
//        printf("Pred  :t = %9.2g, time_points = %9.2g, i = %d \n",t[i],time_points[i],i);
        }


//////////INITIALIZING COEFF VECTOR L BASED ON ORDER/////////////// 
   for(i=0;i<7;i++)
   if(i<m)
   L[i]=1;
   else
   L[i]=0;
   
//////////CALCULATING LEGRANGE INTERPOLATION COEFFICIENTS OF ORDER K///////   

for(i=1;i<=m;i++)  //go through all o+1 exc
  for(j=1;j<=m;j++)  //go through all o+1 excluding current
    {
       if(i!=j)           // BUT points where i and j don't cancel
        {
          L[i-1]=L[i-1]*((t[0]-t[j])/(t[i]-t[j]));  //(curr - j)/(i-j)
         }

    }

///////////PREDICTING X[0] USING INTERPOLATION SUM///////////
for(i=1;i<7;i++)
{
   x[0] += L[i-1]*x[i];
   printf("GAMMA_LGRNGE[%d] = %f \n",i-1,L[i-1]);
}
   printf("GAMMA_LGRNGE[%d] = %f \n",i-1,L[i-1]);

}
