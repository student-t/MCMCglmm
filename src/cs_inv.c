#include "cs_inv.h"

/* matrix inversion through Gauss-Jordan elimination: modified from numerical recipes */

cs *cs_inv(const cs *C){  

    int n, i, icol,irow,j,k,l,ll;
    double big,dum,pivinv,temp, det;

    cs *A;
    n = C->n;
    det=1.0;
    int indxc[10], indxr[10],ipiv[10]; 

    A = cs_spalloc (n, n, n*n, 1, 0);

    if (!A ) return (cs_done (A, NULL, NULL, 0));   
      
      for(i = 0; i<(n*n); i++){
        A->i[i] = C->i[i];
        A->x[i] = C->x[i];
      }
      for(i = 0; i<=n; i++){
        A->p[i] = C->p[i];
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
                                         } else if (ipiv[k] > 1) error("Singular G/R structure: use proper priors\n");// //exit(1);
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

	   if (det < 0.0){
		 error("non-positive definite G/R structure: use proper priors\n");
	   }
	   return (cs_done (A, NULL, NULL, 1)) ;	/* success; free workspace, return C */

}

