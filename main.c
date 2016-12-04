#include <stdio.h>
#include <math.h>
#include "macros.h"
#include "defs.h"
#include "res.h"
#include "isrc.h"
#include "vsrc.h"
#include "vcvs.h"
#include "cccs.h"
#include "vccs.h"
#include "ccvs.h"
#include "opamp.h"
#include "tf.h"
#include "gyro.h"
#include "dio.h"
#include "mosfet.h"
#include "bjt.h"
#include "lincap.h"
#include "nonlincap.h"
#include "linind.h"
#include "sparse/spMatrix.h"

int NumNodes = 0;
int NumBranches = 0;
int iter_counter;
char **NodeArray;
char **BranchArray;
double time_points[7];
int order;

double maximum(double a, double b)
{
   if(a > b)
   return a;
   else 
   return b;

}



main(ac, av)
char **av;
{
    char *inFile;
    FILE *fopen(), *fpIn;
    char buf[MAXLINE];
    resistor 	*Res[MAXELEM];
    isource	*Isrc[MAXELEM];
    vsource	*Vsrc[MAXELEM];
    vcvs	*Esrc[MAXELEM];
    cccs	*Fsrc[MAXELEM];
    vccs	*Gsrc[MAXELEM];
    ccvs	*Hsrc[MAXELEM];
    diode    	*Dio[MAXELEM]; 
    transformer  *Tf[MAXELEM]; 
    gyrator    *Gyro[MAXELEM]; 
    opamp      *Op[MAXELEM]; 
    mosfet      *Mosfet[MAXELEM]; 
    bjt      *Bjt[MAXELEM]; 
    lincap   *LinCap[MAXELEM];
    nonlincap   *NonLinCap[MAXELEM];
    linind   *LinInd[MAXELEM];
    
    int i = 0;
    int j = 0;
    int numRes = 0;
    int numIsrc = 0;
    int numVsrc = 0;
    int numEsrc = 0;
    int numFsrc = 0;
    int numGsrc = 0;
    int numHsrc = 0;
    int numDio = 0;
    int numTf = 0;
    int numGyro = 0;
    int numOp = 0;
    int numMosfet = 0;
    int numBjt = 0;
    int numLinCap = 0;
    int numNonLinCap = 0;
    int numLinInd = 0;
    int numEqns;
    char *cktMatrix;
    double *Rhs, *Sol, *Sol_old;
    BOOLEAN foundError();
    int error, icheck;
    double norm_dx, norm_Sol_old, norm_Sol, Ea, Er;
    double *XP6,*XP5,*XP4,*XP3,*XP2,*XP1,*XP0;
    double timeval, tstart, tstop, h;
    int tcount, tcountmax,conv_counter,conv_flag;
    int hflag,Fillins;
    switch (ac) {
        case 2:
            inFile = av[1];
            break;
        default:
            printf( "\n File Name Required \n");
            exit(-1);
    }

    /* initialization */
    NodeArray = CALLOC(char *, MAXNODE);
    BranchArray = CALLOC(char *, MAXBRANCH);
    for(i = 0; i < MAXNODE; i++) {
	NodeArray[i] = CALLOC(char, MAXFIELD);
    }
    for(i = 0; i < MAXBRANCH; i++ ) {
	BranchArray[i] = CALLOC(char, MAXFIELD) ;
    }
    /* fill in ground node */
    strcpy(NodeArray[0], (char *)"0");

    
    fpIn = fopen( inFile, "r" );
    while (fgets( buf, MAXLINE, fpIn ) != NULL) {
	if(tolower(buf[0]) == 'r') 
	{
	    /* resistor */
	    numRes++;
	    makeRes(Res, numRes, buf);
	} 
	else if(tolower(buf[0]) == 'i') 
	{
	    /* isource */
	    numIsrc++;
	    makeIsrc(Isrc, numIsrc, buf);
	}
	else if(tolower(buf[0]) == 'v') 
	{
	    /* vsource */
	    numVsrc++;
	    makeVsrc(Vsrc, numVsrc, buf);
	}
	else if(tolower(buf[0]) == 'e') 
	{
	    /* vcvs */
	    numEsrc++;
	    makeEsrc(Esrc, numEsrc, buf);
	}
	else if(tolower(buf[0]) == 'f') 
	{
	    /* cccs */
	    numFsrc++;
	    makeFsrc(Fsrc, numFsrc, buf);
	}
	else if(tolower(buf[0]) == 'g') 
	{
	    /* vccs */
	    numGsrc++;
	    makeGsrc(Gsrc, numGsrc, buf);
	}
	else if(tolower(buf[0]) == 'h') 
	{
	    /* ccvs */
	    numHsrc++;
	    makeHsrc(Hsrc, numHsrc, buf);
	}
	else if(tolower(buf[0]) == 'd') 
	{
	    /* diode */
	numDio++;
	    makeDio(Dio, numDio, buf);
	}
	else if(tolower(buf[0]) == 'n') 
	{
	    /* transformer */
	numTf++;
	    makeTf(Tf, numTf, buf);
	}
	else if(tolower(buf[0]) == 't') 
	{
	    /* gyrator */
	numGyro++;
	    makeGyro(Gyro, numGyro, buf);
	}
	else if(tolower(buf[0]) == 'o') 
	{
	    /* opamp */
	numOp++;
	    makeOp(Op, numOp, buf);
	}
	else if(tolower(buf[0]) == 'm') 
	{
	    /* mosfet */
	numMosfet++;
	    makeMosfet(Mosfet, numMosfet, buf);
	}
	else if(tolower(buf[0]) == 'q') 
	{
	    /* bjt */
	numBjt++;
	    makeBjt(Bjt, numBjt, buf);
	}
	else if(tolower(buf[0]) == 'c') 
	{
	    /* LinCap */
	numLinCap++;
	    makeLinCap(LinCap, numLinCap, buf);
	}
	else if(tolower(buf[0]) == 'u') 
	{
	    /* NonLinCap */
	numNonLinCap++;
	    makeNonLinCap(NonLinCap, numNonLinCap, buf);
	}
	else if(tolower(buf[0]) == 'l') 
	{
	    /* LinInd */
	numLinInd++;
	    makeLinInd(LinInd, numLinInd, buf);
	}
    }
    fclose( fpIn );

    /* print circuit elements */
    printRes(Res, numRes);
    printIsrc(Isrc, numIsrc); 
    printVsrc(Vsrc, numVsrc); 
    printEsrc(Esrc, numEsrc); 
    printFsrc(Fsrc, numFsrc); 
    printGsrc(Gsrc, numGsrc); 
    printHsrc(Hsrc, numHsrc); 
    printDio(Dio, numDio);
    printTf(Tf, numTf);
    printGyro(Gyro, numGyro);
    printOp(Op, numOp);
    printMosfet(Mosfet, numMosfet);
    printBjt(Bjt, numBjt);
    printLinCap(LinCap, numLinCap);
    printNonLinCap(NonLinCap, numNonLinCap);
    printLinInd(LinInd, numLinInd);

    /* setup circuit matrix */
    numEqns = NumNodes+NumBranches;
    cktMatrix = spCreate( numEqns, 0, &error );
    if( error IS spNO_MEMORY ) {
	printf( "\n: --- NO MEMORY ---" );
	exit( -1 );
    }
    /* Allocate RHS and Solution vectors */
    Rhs = CALLOC(double, numEqns+1);
    Sol = CALLOC(double, numEqns+1);
    Sol_old = CALLOC(double, numEqns+1);
    XP6    = CALLOC(double, numEqns+1);
    XP5    = CALLOC(double, numEqns+1);
    XP4    = CALLOC(double, numEqns+1);
    XP3    = CALLOC(double, numEqns+1);
    XP2    = CALLOC(double, numEqns+1);
    XP1    = CALLOC(double, numEqns+1);
    XP0    = CALLOC(double, numEqns+1);

    /* do any preprocessing */
    setupRes(cktMatrix, Res, numRes);
    setupIsrc(cktMatrix,Rhs, Isrc, numIsrc);
    setupVsrc(cktMatrix,Rhs, Vsrc, numVsrc);
    setupEsrc(cktMatrix, Esrc, numEsrc);
    setupFsrc(cktMatrix, Fsrc, numFsrc);
    setupGsrc(cktMatrix, Gsrc, numGsrc);
    setupHsrc(cktMatrix, Hsrc, numHsrc);
    setupTf(cktMatrix, Tf, numTf);
    setupGyro(cktMatrix, Gyro, numGyro);
    setupOp(cktMatrix, Op, numOp);
    setupMosfet(cktMatrix, Rhs, Mosfet, numMosfet);
    setupDio(cktMatrix, Rhs, Dio, numDio);
    setupBjt(cktMatrix, Rhs, Bjt, numBjt);
    setupLinCap(cktMatrix, Rhs, LinCap, numLinCap);
    setupNonLinCap(cktMatrix, Rhs, NonLinCap, numNonLinCap);
    setupLinInd(cktMatrix, Rhs, LinInd, numLinInd);





///////////////////////DEFINING TIME STEPS AND MAX COUNTS//////////////////////////////////////////////////////
//  printf("Please enter the time step : ");
//  scanf("%f",&h );
//  printf("Please enter the stop time : ");
//  scanf("%f",&tstop );
h = 5e-7;
tstart = 0.0;
tstop  = 5e-6;
FILE *fptr;
fptr=fopen("out.csv","a");


tcount = 1;
////////////////////TRANSIENT START////////////////////
for(timeval=tstart;timeval<tstop;timeval=timeval+h)
{
///////////////////////////////////////////NEWTON ALGORITHM/////////////////////////////////////////////////////////////////////////
 
//Shift the solutions through the queue   
      {
          time_points[6] = time_points[5];
          time_points[5] = time_points[4];
          time_points[4] = time_points[3];
          time_points[3] = time_points[2];
          time_points[2] = time_points[1];
          time_points[1] = time_points[0]; 
          time_points[0] = timeval;

          for(i=0;i<6;i++)
          printf("time_point[%d]  = %9.9g \n",i,time_points[i]);
      }
   for(i=1;i<=numEqns;i++)
      {
          XP6[i] = XP5[i];
          XP5[i] = XP4[i];
          XP4[i] = XP3[i];
          XP3[i] = XP2[i];
          XP2[i] = XP1[i];
          XP1[i] = Sol[i]; 
       }

// Declaration and Initializing Norm variables as zero
   norm_dx = 100;
   norm_Sol_old  = 0;
   norm_Sol = 0;
   Ea = 1e-6;
   Er = 1e-3;
   iter_counter = 0;
   icheck = 0;
   conv_flag =0;  

while(conv_flag==0 || icheck==1){


//Clearing the norm values
   norm_dx = 0;
   norm_Sol_old  = 0;
   norm_Sol = 0;

// Clearing the Matrix
 
    spClear(cktMatrix);


  // Clearing of Rhs
 
   for(i=1;i<=numEqns;i++)
        Rhs[i]=0;
    

 




   /* load circuit matrix */
    loadRes(cktMatrix, Rhs, Res, numRes);
    loadIsrc(cktMatrix, Rhs, Isrc, numIsrc);
    loadVsrc(cktMatrix, Rhs, Vsrc, numVsrc);
    loadEsrc(cktMatrix, Rhs, Esrc, numEsrc);
    loadFsrc(cktMatrix, Rhs, Fsrc, numFsrc);
    loadGsrc(cktMatrix, Rhs, Gsrc, numGsrc);
    loadHsrc(cktMatrix, Rhs, Hsrc, numHsrc);
    loadTf(cktMatrix, Rhs, Tf, numTf);
    loadGyro(cktMatrix, Rhs, Gyro, numGyro);
    loadOp(cktMatrix, Rhs, Op, numOp);
    loadMosfet(cktMatrix, Rhs, Mosfet, numMosfet,Sol,tcount);
    loadDio(cktMatrix, Rhs, Dio, numDio,Sol, &icheck,tcount);
    loadBjt(cktMatrix, Rhs, Bjt, numBjt,Sol, &icheck,tcount);
    loadLinCap(cktMatrix, Rhs, LinCap, numLinCap,Sol,h,tcount,XP0,XP1,XP2,XP3,XP4,XP5,XP6);
    loadNonLinCap(cktMatrix, Rhs, NonLinCap, numNonLinCap,Sol,h,tcount);
    loadLinInd(cktMatrix, Rhs, LinInd, numLinInd,Sol,h,tcount);

// Assigning Current solution to the Old solution
 
   for(i=1;i<=numEqns;i++)
        Sol_old[i]=Sol[i];
 
    /* print circuit matrix */
    printf("\n");
    spPrint(cktMatrix, 0, 1, 0);

    /* print Rhs vector */
    printf("\nRHS\n");
    for(i = 1; i <=numEqns; i++) {
        printf(" %9.3g\n",Rhs[i]);
    }
    /* compute DC solution */
    /* first Factor the matrix and then Forward/Back solve */
    error = spFactor( cktMatrix );
    if( foundError( error ) ) {
	exit( -1 );
    }
    spSolve( cktMatrix, Rhs, Sol );

   iter_counter++;
//Retrieving the number of Fillins in the current 
    Fillins = spFillinCount(cktMatrix);
    printf(" Total Number of Fillins in the iteration no %3d is  = %3d, \n",iter_counter, Fillins);
    
    

//Calculating Norms of Old, Current and Delta Solutions

   conv_counter = 0;
   for(i=1;i<=numEqns;i++)
      {
        norm_Sol_old =  fabs(Sol_old[i]);
        norm_Sol     =  fabs(Sol[i]);
        norm_dx      =  fabs(Sol_old[i]-Sol[i]);

     if(norm_dx < Ea+Er*maximum(norm_Sol_old,norm_Sol))
       conv_flag = 1;
     else
       {conv_flag = 0;
         break; 
         }
       }
   

// Break if iter_counter exceeds 100

   if(iter_counter>100)
     break;

}
///////////////////////////////////////////END NEWTON LOOP/////////////////////////////////////////////////////////////////////////





//////////////////////LOCAL  ERROR CALCULATION ////////////////////////


hflag = LELinCap(LinCap, numLinCap, Sol);
printf("HFLAG = %d\n",hflag);


///////////PRINT WARNING : ACCURACY OF NUMBERS WILL AFFECT THE PLOTS///////////////

fprintf(fptr,"%9.9g,",timeval);
for(i = 1; i<= numEqns; i++) {
    fprintf(fptr,"%9.9g, ",Sol[i]);
    }
fprintf(fptr,"\n");


tcount++;

} //END OF TIME

printf(" Total Number of Iterations= %3d, \n", iter_counter);
fclose(fptr);

}
