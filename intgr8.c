#include<stdio.h>
#include "defs.h"


void intgr8(x,xdot,h,alpha,beta)
  double x[];
  double xdot;
  double h;
  double *alpha, *beta;
{ 
int m,i,j;
float t[7];
float C[7];

  /* TR method */
 // *alpha = 2.0/h;
 // *beta = (-2.0/h * x[1] - xdot);
 // /* BE method */
  /*
  *alpha = 1.0/h;
  *beta = -1.0/h * x;
  */
/////////////TIME POINTS//////////////////
                for(i=0;i<=6;i++)
         {         t[i]=time_points[i];//(1-i)*h;//time_points[i]; //current + m previous points
//                printf("Corr  :t = %9.2g, time_points = %9.2g, i = %d \n",t[i],time_points[i],i);
        }
        
//////////INITIALIZING COEFF VECTOR C BASED ON ORDER/////////////// 
             for(i=1;i<7;i++)
              {  
                if(i<=order)
                  C[i]=1;
                else
                  C[i]=0;
               }
        
//////////CALCULATING FIRST COEFFICIENT OF BDF OF ORDER K///////   
                  C[0]=0;
             for(i=1;i<=order;i++) 
                C[0] = C[0]+(t[0]-t[1])/(t[0]-t[i]);
        
        //CALCULATING ALPHA         
                *alpha = C[0]/h;
        
//////////CALCULATING REMAINING COEFFICIENT OF BDF OF ORDER K////////////////

              for(i=1;i<=order;i++)  //go through all o+1 
                {
                   for(j=1;j<=order;j++)  //go through all o+1 
                      {
                       if(i!=j)           // BUT points where i and j don't cancel
                          {
                             C[i]=C[i]*((t[0]-t[j])/(t[i]-t[j]));  //(curr - j)/(i-j)
                          }
                       }
                    C[i]=C[i]*(t[1]-t[0])/(t[0]-t[i]); // hn/(tn-tn-i)
                 }
        for(i=0;i<7;i++)
        printf("ALPHA_BDF[%d]  = %f \n",i,C[i]);
        
        //CALCULATING BETA
        
        for(i=1,*beta=0;i<7;i++)
        {
        *beta += C[i]*x[i]/h;
  //      printf("beta[%d]  = %f \n",i,*beta);
        }

        printf("ORDER  = %d\n", order);


 }
        


