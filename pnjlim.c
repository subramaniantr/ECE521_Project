    /* pnjlim(vnew,vold,vt,vcrit,icheck)
     * limit the per-iteration change of PN junction  voltages 
     * (From Spice3)
     * vcrit=Vt*log(Vt/(sqrt(2)*Is))
     * icheck is a flag that is set to 1 if limiting is performed else
     * it is set to 0
     */

#include "macros.h"
#include "math.h"

double
pnjlim(vnew,vold,vt,vcrit,icheck)

    double vnew;
    double vold;
    double vt;
    double vcrit;
    int *icheck;

{
    double arg;
    int temp;

    if((vnew > vcrit) && (ABS(vnew - vold) > (vt + vt))) {
        if(vold > 0) {
            arg = 1 + (vnew - vold) / vt;
            if(arg > 0) {
                vnew = vold + vt * log(arg);
            } else {
                vnew = vcrit;
            }
        } else {
            vnew = vt *log(vnew/vt);
        }
        *icheck = 1;
    } else {
        *icheck = 0;
    }
    return(vnew);
}
