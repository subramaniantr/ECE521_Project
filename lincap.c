
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


void loadLinCap(Matrix, Rhs, LinCap, numLinCap, Xk,h ,time_step_count)
char *Matrix;
double *Rhs;
lincap *LinCap[];
int numLinCap;
double* Xk;
double h;
int time_step_count;
{
  int i, p, n, k;
  lincap *inst;
  double Gk, Ik;
  double  Vcap,Vp;
  double Capval;
  int na, nb;

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
      Vcap = Xk[na]-Xk[nb];
     




///////////////INITIAL CONDITIONS FOR NEWTON AND LMS/////////////////////
///INTEGRATION METHOD SHOULD BE EXECUTED ONLY ONCE IN A TIME STEP////////
      if(iter_counter == 0) {  
          //SHIFTING OLD VALUES 
          inst->vpast[5] = inst->vpast[4];
          inst->vpast[4] = inst->vpast[3];
          inst->vpast[3] = inst->vpast[2];
          inst->vpast[2] = inst->vpast[1];
          inst->vpast[1] = inst->vpast[0];
          inst->vpast[0] = Vcap; 
         // first iteration of a given timepoint
         if(time_step_count == 1) {
             // first time point
             inst->vdot = 0;
             inst->alpha = 0;
             inst->beta = 0;
             Vp = 0;
             k = 1;
     /*    
         if(i ==3)   //RING OSCILLATOR
             Vc=1; 
         if(i ==2)   //MOS COLPITT'S OSCILLATOR
             Vc=-1.5; 
         if(i ==2)   //BJT COLPITT'S OSCILLATOR
             Vc=-0.5; 
     */
         }

////////// SUBSEQUENT LMS ITERATION(time points)///////////////////////
//////// USE THE OLD ALPHA BETA AND NEW V TO GET NEW V'////////////////
         else {
    if(time_step_count>6)
     { 
        k = 2; 
        Vp = 2*inst->vpast[0] - inst->vpast[1];
      }
    else
     { 
        k = 0;
        Vp = inst->vpast[0];
      }




          
             inst->vdot = (inst->alpha)*inst->vpast[0]+(inst->beta); 

         }
////////////////UPDATE THE NEW ALPHA AND BETA//////////////////////////

         intgr8(inst->vpast,inst->vdot,h,&inst->alpha,&inst->beta,k);
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
