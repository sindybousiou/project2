#include "classes.h"
#define MAXV 20
#define W 2

using namespace std;

void lsh_hash(int,int);
bucketClass cube_hash(int, int);

////LSH/////
vector <GFunction> defineEuclideanFunctions(int,int);
vector <vector <double> > defineCosineFunctions(int,int);
void createHashTables(int,int);
void hashingEUC(int,int, vector <GFunction>);
long double vinh(vector <double>,int,int,vector <GFunction>);
void hashingCOS(int,int,vector <vector <double> >);