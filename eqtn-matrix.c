#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <tgmath.h>


int checkNvar(size_t n, char const line[n]);
int compareRows(size_t const n, size_t const t, size_t const vn);
int checkLine(size_t n, char const line[n]);
int gaussEliminate(size_t y, size_t x, size_t n);
int build_matrix(size_t n, int i, char line[n]);
int solve(size_t vn, double xv[vn]);
double v[50][50] = {0};// This array contains the matrix A and the vector b

int main(void)
{
    printf("This program solves a system of linear equations\n"
           "The system must be in matrix form, e.g Ax = b,\n"
           "Enter the number of variables to solve\n");
    
    char *line = 0;
    size_t vbuf = 40;

    if (!(line = calloc(vbuf, sizeof(char)))){
     printf("Memory allocation failed\n");
     exit(1);
    }
   
    size_t n = 0;
    n = getline(&line,&vbuf,stdin);   
   
      
    while (checkNvar(n,line)){ // check that the input was valid 
 
        printf("Not a vaild input\n"
               "please enter a postive integer\n");
        memset(line,0,vbuf);
        n = getline(&line,&vbuf,stdin);
        if (n > 1){
          line[n-1] = '\0';
        }
         
   } 
   
   // vn is the number of variables for the system
    long vn = strtol(line,NULL,10);
    memset(line,0,vbuf); 
    double xv[vn]; // this array contains the values of the variables 

    for (int i = 0;i < vn;++i){ 
     printf("Enter the coefficients of equation %d: ", i+1);
     n = getline(&line,&vbuf,stdin);
    
     if (checkLine(n,line)){ // check that the input was valid
        printf("Incorrect input in equation %d\n", i+1);
        memset(line,0,vbuf);       
        --i;
        continue;
     }
    
     build_matrix(n,i,line); //start building the matrix
     memset(line,0,vbuf);
   }
   
   for (int i = 0;i < vn;++i){ 
    printf("Enter the value for the b vector"
           " corresponding to equation %d: ", i+1);
    n = getline(&line,&vbuf,stdin);
    if (n > 1){
      line[n-1] = '\0';
    }
    if (checkNvar(n,line)){ // check that the input was valid
      printf("Incorrect value for equation %d\n", i+1);
      --i;
      memset(line,0,vbuf);
      continue;
    }
    v[i][vn] = strtod(line,NULL);
    memset(line,0,vbuf);
   }
   //free the memory 
   free(line);

   printf("The coefficients of the system and the corresponding b value are:\n"); 
   for (int x = 0;x < vn;++x){
     for (int y = 0;y <= vn;++y){
         printf("%.3g\t", v[x][y]);
     }
     printf("\n");
   }

   for (int i = 0;i < vn-1;++i){
    compareRows(i,i,vn);  
    printf("\n"); 
   
    // printing the matrix after compareRows
     for (int x = 0;x < vn;++x){
      for (int y = 0;y <= vn;++y){
         printf("%.3g\t", v[x][y]);
      }
      printf("\n");
     }

    gaussEliminate(i,i,vn);
    printf("\n");
   
   //printing the matrix after gaussEliminate
     for (int x = 0;x < vn;++x){
      for (int y = 0;y <= vn;++y){
         printf("%.3g\t", v[x][y]);
      }
      printf("\n");
     }
   }
   
   printf("\n");
   printf("The new matrix is:\n");
   for (int x = 0;x < vn;++x){
     for (int y = 0;y <= vn;++y){
      printf("%.3g\t", v[x][y]);
     }
     printf("\n");
   }

   printf("\nSolving the system:\n");
   solve(vn,xv);
   printf("\n"); 
   for (int i = 0;i < vn;++i){
     printf("Variable x%d = %.3g\n", i+1, xv[i]);
   }
     return 0;

}

//build_matrix builds the A matrix
int  build_matrix(size_t n, int i, char line[n])
{

   int x = i;
   int y = 0;
 
   // This tokenizes the ptr string to read the coefficients
   char  *token = strtok(line, " ");
   while (token != NULL){
     v[x][y] = strtod(token,NULL);
     ++y;
       
    token = strtok(NULL, " ");
   }
   
   return 1; 

}

// checkNvar checks that their was a valid input
int checkNvar(size_t n, char const line[n])
{

    if (n == 1)
     return -1;
   
    for (size_t i = 0;i < n-1;++i){
        if (!(isdigit(line[i])) || line[0] == '0'){
         return -1; 
        } 
    } 

    return 0;
}

// checkLine checks that there was a valid input
int checkLine(size_t n, char const line[n])
{
        
    if (n == 1)
     return -1;
   
    for (size_t i = 0;i < n-1;++i){
      if ((isdigit(line[i])) || isspace(line[i]) || ((line[i] == '-')
            && isdigit(line[i+1]))){
        continue; 
      } else 
          return -1; 
    } 

    return 0;
}

// compareRows checks which row has the highest absolute value of a column and switches it with the top row
int compareRows(size_t const n, size_t const t, size_t const vn)
{
    size_t hitemp = 0;

    if (fabs(v[t][n]) > fabs(v[t+1][n])){
      hitemp = t; 
    } else{
       hitemp = t+1;
    }
    
    for (size_t y = t+2;y < vn;++y){
      if (fabs(v[hitemp][n]) > fabs(v[y][n])){
       continue;
      } else{
         hitemp = y;
      }
    }
    
    if (hitemp == t){
      return 1;
    }

    double temp = 0.0;
    
    for (size_t i = 0;i <= vn;++i){
      temp = v[t][i];
      v[t][i] = v[hitemp][i];
      v[hitemp][i] = temp;
    }

    return 0;
}

// gaussEliminate begins to solve the equations by using gauss elimination
int gaussEliminate(size_t y, size_t x, size_t n)
{
    if (!(v[y][x]))
      return -1;

    double factor = 0;
    for (size_t j = y+1;j < n;++j){
      double temp = v[j][x]/v[y][x];
      int sign = 0;
      if ((v[j][x] > 0 && v[y][x] > 0) ||
         (v[j][x] < 0 && v[y][x] <  0)){
          sign = 1;
      } else{
         sign = 0;
      }
      for (size_t i = x;i <= n;++i){
       factor = (v[y][i] * temp);
       if (sign){
         v[j][i] = v[j][i] - factor;
       } else {
          v[j][i] = v[j][i] + factor;
       } 
      } 
    }

    return 0;
}


// solve solves the equations
int solve(size_t vn, double xv[vn])
{

    for (int j = vn-1;j >= 0;--j){
     xv[j] = v[j][vn];
      for (int i = vn-1;i >= 0;--i){
       if (i > j){
        xv[j] = xv[j] - (xv[i] * v[j][i]);
       } else{
          break;
       }
      }
      xv[j] = xv[j] / v[j][j];
    }


    return 0;
}
