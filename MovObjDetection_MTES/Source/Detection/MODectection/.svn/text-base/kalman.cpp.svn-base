#include "Kalman.h"
static void matrix_multiply(float* A, float* B, int m, int p, int n, float* C)
// Matrix Multiplication Routine
{
    // A = input matrix (m x p)
    // B = input matrix (p x n)
    // m = number of rows in A
    // p = number of columns in A = number of rows in B
    // n = number of columns in B
    // C = output matrix = A*B (m x n)
    int i, j, k;
    for (i=0;i<m;i++)
        for(j=0;j<n;j++)
        {
          C[n*i+j]=0;
          for (k=0;k<p;k++)
            C[n*i+j]= C[n*i+j]+A[p*i+k]*B[n*k+j];
        }
}

static void matrix_addition(float* A, float* B, int m, int n, float* C)
// Matrix Addition Routine
{
    // A = input matrix (m x n)
    // B = input matrix (m x n)
    // m = number of rows in A = number of rows in B
    // n = number of columns in A = number of columns in B
    // C = output matrix = A+B (m x n)
    int i, j;
    for (i=0;i<m;i++)
        for(j=0;j<n;j++)
            C[n*i+j]=A[n*i+j]+B[n*i+j];
}

static void matrix_subtraction(float* A, float* B, int m, int n, float* C)
// Matrix Subtraction Routine
{
    // A = input matrix (m x n)
    // B = input matrix (m x n)
    // m = number of rows in A = number of rows in B
    // n = number of columns in A = number of columns in B
    // C = output matrix = A-B (m x n)
    int i, j;
    for (i=0;i<m;i++)
        for(j=0;j<n;j++)
            C[n*i+j]=A[n*i+j]-B[n*i+j];
}

static void matrix_transpose(float* A, int m, int n, float* C)
// Matrix Transpose Routine
{
    // A = input matrix (m x n)
    // m = number of rows in A
    // n = number of columns in A
    // C = output matrix = the transpose of A (n x m)
    int i, j;
    for (i=0;i<m;i++)
        for(j=0;j<n;j++)
            C[m*j+i]=A[n*i+j];
}


static int matrix_inversion(float* A, int n, float* AInverse)
// Matrix Inversion Routine
{
    // A = input matrix (n x n)
    // n = dimension of A
    // AInverse = inverted matrix (n x n)
    // This function inverts a matrix based on the Gauss Jordan method.
    // The function returns 1 on success, 0 on failure.
    int i, j, iPass, imx, icol, irow;
    float det, temp, pivot, factor;
    float* ac = (float*)calloc(n*n, sizeof(float));
    det = 1;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            AInverse[n*i+j] = 0;
            ac[n*i+j] = A[n*i+j];
        }
        AInverse[n*i+i] = 1;
    }

    // The current pivot row is iPass.
    // For each pass, first find the maximum element in the pivot column.
    for (iPass = 0; iPass < n; iPass++)
    {
        imx = iPass;
        for (irow = iPass; irow < n; irow++)
        {
            if (fabs(A[n*irow+iPass]) > fabs(A[n*imx+iPass])) imx = irow;
        }

        // Interchange the elements of row iPass and row imx in both A and AInverse.
        if (imx != iPass)
        {
            for (icol = 0; icol < n; icol++)
            {
                temp = AInverse[n*iPass+icol];
                AInverse[n*iPass+icol] = AInverse[n*imx+icol];
                AInverse[n*imx+icol] = temp;
                if (icol >= iPass)
                {
                    temp = A[n*iPass+icol];
                    A[n*iPass+icol] = A[n*imx+icol];
                    A[n*imx+icol] = temp;
                }
            }
        }

        // The current pivot is now A[iPass][iPass].
        // The determinant is the product of the pivot elements.
        pivot = A[n*iPass+iPass];
        det = det * pivot;
        if (det == 0)
        {
            free(ac);
            return 0;
        }

        for (icol = 0; icol < n; icol++)
        {
            // Normalize the pivot row by dividing by the pivot element.
            AInverse[n*iPass+icol] = AInverse[n*iPass+icol] / pivot;
            if (icol >= iPass) A[n*iPass+icol] = A[n*iPass+icol] / pivot;
        }

        for (irow = 0; irow < n; irow++)
        {
            // Add a multiple of the pivot row to each row.  The multiple factor
            // is chosen so that the element of A on the pivot column is 0.
            if (irow != iPass) factor = A[n*irow+iPass];
            for (icol = 0; icol < n; icol++)
            {
                if (irow != iPass)
                {
                    AInverse[n*irow+icol] -= factor * AInverse[n*iPass+icol];
                    A[n*irow+icol] -= factor * A[n*iPass+icol];
                }
            }
        }
    }

    free(ac);
    return 1;
}

static void matrix_print(float* A, int m, int n, FILE *fp)
// Matrix print.
{
    // A = input matrix (m x n)
    // m = number of rows in A
    // n = number of columns in A
    int i, j;
    for (i=0;i<m;i++)
    {
        fprintf(fp,"| ");
        for(j=0;j<n;j++)
        {
            fprintf(fp,"%7.3f ", A[n*i+j]);
        }
        fprintf(fp,"|\n");
    }
}


// n states
// m inputs
// r outputs
#define state 4
#define input 4
#define output 4
#define mnoise 0.1f
#define par 0.001f
#define snoise 0.1f

void kalman_update(float KalmanPar[4][4], float x1,float y1, float dx1, float dy1, float xc1,float yc1, float dxc1, float dyc1, float &xp, float &yp, float &xmovp,float &ymovp)
{
	// A is an 4 by 4 matrix
    // H is an 4 by 4 matrix
    // Sz is an 4 by 4 matrix
    // Sw is an 4 by 4 matrix
    // xhat is an 4 by 1 vector
    // P is an 4 by 4 matrix
    // Z is an 4 by 1 vector

    // Constants.
	static float A[state][state] = {{1.0f, 0.0f, 1.0f,0.0f},{0.0f, 1.0f,0.0f,1.0f},{0.0f,0.0f,1.0f,0.0f},{0.0f,0.0f,0.0f,1.0f}};
	static float H[output][state] = {{1.0f, 0.0f,0.0f,0.0f}, {0.0f, 1.0f,0.0f,0.0f},{0.0f, 0.0f,1.0f,0.0f},{0.0f, 0.0f,0.0f,1.0f}};  //H
	static float R[output][output] = {{mnoise, 0.0f,0.0f,0.0f}, {0.0f, mnoise,0.0f,0.0f},{0.0f,0.0f,mnoise,0.0f},{0.0f,0.0f,0.0f,mnoise}};
		//{{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise},{mnoise, mnoise,mnoise,mnoise,mnoise,mnoise,mnoise,mnoise}};
    static float Q[state][state] = {{snoise, 0.0f,0.0f,0.0f}, {0.0f, snoise,0.0f,0.0f},{0.0f,0.0f,snoise,0.0f},{0.0f,0.0f,0.0f,snoise}};
		//{{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise},{snoise, snoise,snoise,snoise,snoise,snoise,snoise,snoise}};
	
    // Persistant states.
    static float xhat[state][1] ={{0.0f}, {0.0f},{0.0f},{0.0f}};  
	static float P[state][state] = {{par, par, par, par},{par, par, par, par},{par, par, par, par},{par, par, par, par}};

    // Inputs.
    float Z[input][1]={{0.0f},{0.0f},{0.0f},{0.0f}};  //observation           

    // Predict State
    float Exhat[state][1];          // This is the vector A*xhat   8x8 * 8x1 = 8x1
	float EP[state][state];             // This is the matrix AxP   8x8 * 8x8 = 8x8

	//Measurement Update state
    //K

	float EPR[output][output];           // This is the matrix HP*HT 8x8 * 8x8 = 8x8
	float HPHTR[output][output];         // This is the matrix HPHT+R 8x8 + 8x8 = 8x8
	float EPRInv[output][output];      // This is the matrix inv(HPHTR) 8x8
	float K[state][output];              // This is the Kalman gain. PHT * HPHTRInv 8x8 * 8x8 = 8x8

	//Xhat
	float ZExhat[output][1];                 // This is the vector Z-h*xhat 8x1- 8x8*8x1 = 8x1
	float KZExhat[state][1];					// This is the vector K*Zxhat  8x8*8x1 =8x1

	//p
	float KEP[state][state];            // This is the matrix KH*P 8x8 * 8x8 = 8x8

    for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
		P[i][j]=KalmanPar[i][j];
		}
	}



	
	// Fill in inputs.
    Z[0][0] = x1;
	Z[1][0] = y1;
    Z[2][0] = dx1;
	Z[3][0] = dy1;

	xhat[0][0] = xc1;
	xhat[1][0] = yc1;
    xhat[2][0] = dxc1;
	xhat[3][0] = dyc1;


	
	
	FILE *fp;
	fp=fopen("c:\\tmp.txt","w");

    

//#if 0
    fprintf(fp,"Z =\n");
    matrix_print((float*) Z, input, 1,fp);
    fprintf(fp,"A =\n");
    matrix_print((float*) A, state, state,fp);
    fprintf(fp,"Current State =\n");
    matrix_print((float*) xhat, state, 1,fp);
//#endif

    // Update the state estimate by extrapolating estimate with gyroscope input.
    // xhat_est = A * xhat
    matrix_multiply((float*) A, (float*) xhat, state, state, 1, (float*) Exhat);

//#if 0
    fprintf(fp,"State Estimate =\n");
    matrix_print((float*) Exhat, state, 1,fp);
//#endif

// Compute the new covariance of the estimation error.
    // EP = P + Q
    matrix_addition((float*) P, (float*) Q, state, state, (float*) EP);


 // Compute the covariance of the innovation.
    // s = H * P * H' + R  //R
    matrix_addition((float*) EP, (float*) R, output, output, (float*) EPR);
	
//#if 0
    fprintf(fp,"Covariance of Innovation =\n");
    matrix_print((float*) HPHTR, output, output,fp);
//#endif

	// Compute the kalman gain matrix.
    // K = P * H' * inv(s)
    matrix_inversion((float*) EPR, output, (float*) EPRInv);
    matrix_multiply((float*) EP, (float*) EPRInv, state, state, output, (float*) K);

//#if 0
	fprintf(fp,"Inverse Covariance of Innovation =\n");
    matrix_print((float*) EPRInv, output, output,fp);
    fprintf(fp,"Kalman Gain K=\n");
    matrix_print((float*) K, output, output,fp);
//#endif
	// Compute the innovation.
    // Inn = Z - H * xhat;
    matrix_subtraction((float*) Z, (float*) Exhat, output, 1, (float*) ZExhat);

//#if 0
    fprintf(fp,"Innovation =\n");
    matrix_print((float*) ZExhat, output, 1,fp);
//#endif

   
	  // Update the state estimate.
    // xhat = xhat_est + K * Inn;
    matrix_multiply((float*) K, (float*) ZExhat, state, output, 1, (float*) KZExhat);
    matrix_addition((float*) Exhat, (float*) KZExhat, state, 1, (float*) xhat);

//#if 0
   fprintf(fp,"Xhat =\n");
   matrix_print((float*) xhat, output, 1,fp);
//#endif
    // Compute the new covariance of the estimation error.
    // P = P - K * H * P 
    matrix_multiply((float*) K, (float*) EP, state, state, state, (float*) KEP);
    matrix_subtraction((float*) EP, (float*) KEP, state, state, (float*) P);

//#if 0
    fprintf(fp,"covariance of the estimation error =\n");
    matrix_print((float*) P, output, output,fp);
//#endif
    
    // Return the estimate.
    xp=xhat[0][0];
	yp=xhat[1][0];
	xmovp=xhat[2][0];
	ymovp=xhat[3][0];
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
		KalmanPar[i][j]=P[i][j];
		}
	}


	fclose(fp);
}
