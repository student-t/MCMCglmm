#include "cs_inv.h"

/* matrix inversion through Gauss-Jordan elimination: modified from numerical recipes: outputs the determinant */

double cs_invR(const cs *C, const cs *A){  

    int n, i, icol,irow,j,k,l,ll;
    double big,dum,pivinv,temp, det;

    n = C->n;
    det=1.0;
	
    int indxc[10], indxr[10],ipiv[10]; 

	for(i = 0; i<(n*n); i++){
          A->x[i] = C->x[i];
        }

         for (j=0;j<n;j++) ipiv[j]=0;
         for (i=0;i<n;i++) {
                 big=0.0;
                 for (j=0;j<n;j++)
                         if (ipiv[j] != 1)
                                 for (k=0;k<n;k++) {
                                         if (ipiv[k] == 0) {
                                                 if (fabs(A->x[A->i[j]+A->p[k]]) >= big) {
                                                         big=fabs(A->x[A->i[j]+A->p[k]]);
                                                         irow=j;
                                                         icol=k;
                                                 }
                                         } else if (ipiv[k] > 1) error("Singular G/R structure: use proper priors\n"); // R_CheckUserInterrupt(); //exit(1);        
								 }
                 ++(ipiv[icol]);
                 if (irow != icol) {
                         for (l=0;l<n;l++){
                             temp = A->x[A->i[irow]+A->p[l]];
                             A->x[A->i[irow]+A->p[l]] = A->x[A->i[icol]+A->p[l]];
                             A->x[A->i[icol]+A->p[l]]= temp;
                          }
                 }
                 indxr[i]=irow;
                 indxc[i]=icol;
    			 det *= A->x[A->i[icol]+A->p[icol]];
				 pivinv=1.0/A->x[A->i[icol]+A->p[icol]];
                 A->x[A->i[icol]+A->p[icol]]=1.0;
                 for (l=0;l<n;l++) A->x[A->i[icol]+A->p[l]] *= pivinv;
                 for (ll=0;ll<n;ll++)
                         if (ll != icol) {
                                 dum=A->x[A->i[ll]+A->p[icol]];
                                 A->x[A->i[ll]+A->p[icol]]=0.0;
                                 for (l=0;l<n;l++) A->x[A->i[ll]+A->p[l]] -= A->x[A->i[icol]+A->p[l]]*dum;
                         }
         }
         for (l=(n-1);l>=0;l--) {
                 if (indxr[l] != indxc[l]){
                         for (k=0;k<n;k++){
                             temp = A->x[A->i[k]+A->p[indxr[l]]];
                             A->x[A->i[k]+A->p[indxr[l]]] = A->x[A->i[k]+A->p[indxc[l]]];
                             A->x[A->i[k]+A->p[indxc[l]]] = temp;
                         }
                 }
       }
	
	  if (det  < 0.0){
		error("non-positive definte G/R structure: use proper priors\n");
	  }
        return (det) ;	/* success; free workspace, return C */

}

