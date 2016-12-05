
#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "lincap.h"



void makeLinCap(LinCap, numLinCap, buf)
lincap *LinCap[];
int numLinCap;
char *buf;
{
    lincap *inst;
    int j, nodeP, nodeN, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeP = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeN = getMappedNode(node);
    j = getNextField(buf, num, j);
    value = atof(num);

    inst = CALLOC(lincap, 1);
    inst->name = (char *)strdup(name);
    inst->pNode = nodeP;
    inst->nNode = nodeN;
    inst->value = value;
    LinCap[numLinCap] = inst;
}

void printLinCap(LinCap, numLinCap)
lincap *LinCap[];
int numLinCap;
{
    int i;
    lincap *inst;

    for(i = 1; i <= numLinCap; i++) {
	inst = LinCap[i];
	printf("%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->pNode], NodeArray[inst->nNode], inst->value);
    }
}




void setupLinCap(Matrix, Rhs, LinCap, numLinCap)
char *Matrix;
double *Rhs;
lincap *LinCap[];
int numLinCap;
{
    int i, p,n;
    lincap *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numLinCap; i++) {
	inst = LinCap[i];
//	inst->branchNum += NumNodes;
	p = inst->pNode;
	n = inst->nNode;
	/* setup matrix and pointers */
	inst->ppp = spGetElement(Matrix, p, p);
	inst->ppn = spGetElement(Matrix, p, n);
	inst->pnp = spGetElement(Matrix, n, p);
	inst->pnn = spGetElement(Matrix, n, n);
        

    }
}


void loadLinCap(Matrix, Rhs, LinCap, numLinCap, Xk,h ,time_step_count,XP0,XP1,XP2,XP3,XP4,XP5,XP6)
char *Matrix;
double *Rhs;
lincap *LinCap[];
int numLinCap;
double* Xk;
double h;
int time_step_count;
double *XP6,*XP5,*XP4,*XP3,*XP2,*XP1,*XP0;
{
  int i, p, n;
  lincap *inst;
  double Gk, Ik;
  double  Vcap,Vp;
  double Capval;
  int na, nb;
  double vpast[7];

  // code template outlining procedure for linear capacitor
  for(i = 1; i <= numLinCap; i++) {
      inst = LinCap[i];
      Capval = inst->value;
      na = inst->pNode;
      nb = inst->nNode;



      // Calculate voltage across capacitor. It is assumed that 
    // at the first iteration of a new timepoint
      // Sol has the solution from the previous timepoint
      vpast[6] = XP6[na]-XP6[nb];
      vpast[5] = XP5[na]-XP5[nb];
      vpast[4] = XP4[na]-XP4[nb];
      vpast[3] = XP3[na]-XP3[nb];
      vpast[2] = XP2[na]-XP2[nb];
      vpast[1] = XP1[na]-XP1[nb];
 
    printf("vpast[6] = %f \n",vpast[6]); 
    printf("vpast[5] = %f \n",vpast[5]); 
    printf("vpast[4] = %f \n",vpast[4]); 
    printf("vpast[3] = %f \n",vpast[3]); 
    printf("vpast[2] = %f \n",vpast[2]); 
    printf("vpast[1] = %f \n",vpast[1]); 




///////////////INITIAL CONDITIONS FOR NEWTON AND LMS/////////////////////
///INTEGRATION METHOD SHOULD BE EXECUTED ONLY ONCE IN A TIME STEP////////
      if(iter_counter == 0) {  
          //SHIFTING OLD VALUES 
         // first iteration of a given timepoint
         if(time_step_count == 0) {
             // first time point
             inst->vdot = 0;
             inst->alpha = 0;
             inst->beta = 0;
             vpast[0] = 0.1;
             order = 1;
         }

////////// SUBSEQUENT LMS ITERATION(time points)///////////////////////
//////// USE THE OLD ALPHA BETA AND NEW V TO GET NEW V'////////////////
         else {
    if(time_step_count<6)
     { 
        order = 1;
        predictor(vpast); 
      }
    else
     { 
        order = 5;
        predictor(vpast); 
      }
             inst->vpred = vpast[0];
    printf("vpast[0] = %f \n",vpast[0]); 
             inst->vdot = (inst->alpha)*vpast[1]+(inst->beta); 

         }
////////////////UPDATE THE NEW ALPHA AND BETA//////////////////////////

         intgr8(vpast,inst->vdot,h,&inst->alpha,&inst->beta);
      }//IF CONDITION ONLY EXECUTED WHEN NEWTON LOOP STARTS 
         //SEPARATING THE NEWTON FROM THE LMS

/////////////////////Gk FOR THE COMPANION NETWORK///////////////////////
///////////////////////////Gk = C*ALPHA/////////////////////////////////

      Gk = (inst->alpha)*Capval;

///////////////////////////Ik calculation///////////////////////////////

      Ik = (inst->beta)*Capval;

////////// stamp matrix
  *(inst->ppp) += Gk;
  *(inst->ppn) -= Gk;
  *(inst->pnp) -= Gk;
  *(inst->pnn) += Gk;
  
  Rhs[na] -= Ik;
  Rhs[nb] += Ik;

              }

}



int LELinCap(LinCap, numLinCap, Xk)
lincap *LinCap[];
int numLinCap;
double* Xk;
{
  int flag,i,na,nb;
  double LE, vcor;
  lincap *inst;

  
     flag=0;
  for(i = 1; i <= numLinCap; i++) {
      inst = LinCap[i];
      na = inst->pNode;
      nb = inst->nNode;
      vcor = Xk[na]-Xk[nb];
      printf("Vcor = %f \n\n", vcor); 
      printf("H = %9.9g \n\n", (time_points[0]-time_points[1])); 
      printf("Vpred = %f \n\n", inst->vpred); 
      printf("time0 = %f \n\n", time_points[0]); 
      printf("timeo = %f \n\n", time_points[order+1]); 
      LE = fabs((inst->vpred - vcor)/(time_points[0]-time_points[order+1])*(time_points[0]-time_points[1]));
      printf("Local Error = %9.9g \n\n", LE); 
     if(LE > 1e-5)
      {
         flag=2;
         break;
      }
    else if(LE < 1e-7)
      {
         flag=1; 
      }
    else
     {flag = 0;      
      break;
     }
    }
   return flag;

}
