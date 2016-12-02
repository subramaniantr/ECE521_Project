#include<stdio.h>
void intgr8(x,xdot,h,alpha,beta,k)
  double x[];
  double xdot;
  double h;
  double *alpha, *beta;
  int k;
{ 
  /* TR method */
 // *alpha = 2.0/h;
 // *beta = (-2.0/h * x[0] - xdot);
  /* BE method */
  /*
  *alpha = 1.0/h;
  *beta = -1.0/h * x;
  */
switch(k)   
{  
    case 0:  
 *alpha = 1.0/h;
 *beta  = *alpha*(-x[0]);
 printf("KKKK = %d",k);
        break;
    case 1:  
 *alpha = 3.0/2.0*(1.0/h);
 *beta  = *alpha*(-4.0/3.0*x[0] + 1.0/3.0*x[1]);
 printf("KKKK = %d",k);
        break;
    case 2:  
 *alpha = 11.0/6.0*(1.0/h);
 *beta  = *alpha*(-18.0/11.0*x[0] +  9.0/11.0*x[1] - 2.0/11.0*x[2]);
 printf("KKKK = %d",k);
        break;
    case 3:  
 *alpha = 25.0/12.0*(1.0/h);
 *beta  = *alpha*(-48.0/25.0*x[0] +  36.0/25.0*x[1] - 16.0/25.0*x[2] + 3.0/25.0*x[3]);
 printf("KKKK = %d",k);
        break;
    default :  
        break;
}  

}



