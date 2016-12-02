/* information used to describe a single instance */

typedef struct isource{
    char *name;  /* pointer to character string naming this isource */
    int pNode; /* number of positive node of isource */
    int nNode; /* number of negative node of isource */

    double current;  /* value of isource */
    
    double *prhsn1;     /*pointer to sparse-matrix location (pNode)*/
    double *prhsn2;     /*pointer to sparse-matrix location (nNode)*/
} isource ;

