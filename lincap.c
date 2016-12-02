
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
  int i, p, n, k;
  lincap *inst;
  double Gk, Ik;
  double  Vcap,Vp;
  double Capval;
  int na, nb;
  double vpast[7];
FILE *fptr;
fptr=fopen("out.csv","a");

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
 
//    printf("%f \n",vpast[6]); 
//    printf("%f \n",vpast[5]); 
//    printf("%f \n",vpast[4]); 
//    printf("%f \n",vpast[3]); 
//    printf("%f \n",vpast[2]); 
//    printf("%f \n",vpast[1]); 
//    printf("%f \n",vpast[0]); 




///////////////INITIAL CONDITIONS FOR NEWTON AND LMS/////////////////////
///INTEGRATION METHOD SHOULD BE EXECUTED ONLY ONCE IN A TIME STEP////////
      if(iter_counter == 0) {  
          //SHIFTING OLD VALUES 
         // first iteration of a given timepoint
         if(time_step_count == 1) {
             // first time point
             inst->vdot = 0;
             inst->alpha = 0;
             inst->beta = 0;
             Vp = 0;
             k = 1;
         }

////////// SUBSEQUENT LMS ITERATION(time points)///////////////////////
//////// USE THE OLD ALPHA BETA AND NEW V TO GET NEW V'////////////////
         else {
    if(time_step_count< 3)
     { 
        k = time_step_count;
        vpast[0] = vpast[1];
      }
    else
     { 
        k = 4;
        predictor(vpast,k); 
      }




          
             inst->vdot = (inst->alpha)*vpast[1]+(inst->beta); 

         }
////////////////UPDATE THE NEW ALPHA AND BETA//////////////////////////

         intgr8(vpast,inst->vdot,h,&inst->alpha,&inst->beta,k);
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

fclose(fptr);
}
