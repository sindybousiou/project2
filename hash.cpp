#include "hash.h"
#include <random>

using namespace std;

unsigned int M=pow(2,32)-5;

extern bool metric;
extern vector <xclass> x;
extern mt19937 generator;
extern int fileSize;
extern bucketClass bucketsLSH;

void lsh_hash(int l,int numOfHF)
{
	vector <GFunction> g;
	vector <vector <double> > cosineFunctions;
	createHashTables(l,numOfHF);
	if(metric==EUC)
	{
		g=defineEuclideanFunctions(l,numOfHF);
		hashingEUC(l,numOfHF,g);
	}
	else
	{
		cosineFunctions=defineCosineFunctions(l,numOfHF);
		hashingCOS(l,numOfHF,cosineFunctions);
	}
}


bucketClass cube_hash(int l, int numOfHF)
{
	bucketClass bucketsCube;
	//create the buckets
	return bucketsCube;
}

///////LSH////////////////
vector <GFunction> defineEuclideanFunctions(int l,int numOfHF)
{
	vector <GFunction> g;
	for(int i=0;i<l;i++)
	{
		GFunction gf;
		vector <hashFunction > htemp;
		vector <double> r;
		for(int j=0;j<numOfHF;j++)
		{
			normal_distribution <double> distr2(0,MAXV/3);
			hashFunction hf;
			uniform_real_distribution <double> distr(0,W);
			double t=distr(generator);
			vector <double> temp;
			for(int m=0;m<x[0].get_values().size();m++)
				temp.push_back(distr2(generator));
			hf.set_t(t);
			hf.set_v(temp);
			htemp.push_back(hf);
			r.push_back(distr2(generator));
		}
		gf.set_h(htemp);
		gf.set_r(r);
		g.push_back(gf);
	}
	return g;
}

vector <vector <double> > defineCosineFunctions(int l,int numOfHF)
{
	vector <vector <double> > cosineFunctions;
	for(int i=0;i<l;i++)
	{
		for(int j=0;j<numOfHF;j++)
		{
			normal_distribution <double> distr2(0,MAXV/3);
			vector <double> temp;
			for(int m=0;m<x[0].get_values().size();m++)
				temp.push_back(distr2(generator));
			cosineFunctions.push_back(temp);
		}
	}
	return cosineFunctions;
}

void createHashTables(int l,int numOfHF)
{
	for(int i=0;i<l;i++)
	{
		vector <vector <int> > temp1;
		if(metric==EUC)
			for(int j=0;j<(fileSize/8);j++)   //tablesize is n/8, it can change
			{
				vector <int> temp2;
				temp1.push_back(temp2);
			}
		else
			for(int j=0;j<=pow(2,numOfHF);j++)  //tablesize is 2^k (k for the 1st excercise means number of hash functions)
			{
				vector <int> temp2;
				temp1.push_back(temp2);
			}
		bucketsLSH.push_back(temp1);
	}
}

void hashingEUC(int l,int numOfHF, vector <GFunction> g)
{
	for(int i=0;i<fileSize;i++)
	{
		vector <int> temp;
		for(int j=0;j<l;j++)
		{
			long long int result=0;
			for(int m=0;m<numOfHF;m++)
			{
				long long int res =(vinh(x[i].get_values(),j,m,g)*g[j].get_r()[m]);
				res =res%M;
				if(res>0)                          //explanation in readme
					result += res;               //for overflow
				else
					result+=(-1)*res;
			}
			if(result<0)
				result=(-1)*result;
			result=result%M;
			result = result%(fileSize/8);
			bucketsLSH[j][result].push_back(i);
			temp.push_back(result);
		}
		x[i].set_bucket(temp);
	}
}

void hashingCOS(int l,int numOfHF, vector <vector <double> > cosineFunctions)
{
	for(int i=0;i<fileSize;i++)
	{
		vector <int> temp;
		for(int j=0;j<l;j++)
		{
			int result=0;
			for(int m=0;m<numOfHF;m++)
			{
				int temp=0;
				for(int n=0;n<x[i].get_values().size();n++)
					temp+=x[i].get_values()[n]*cosineFunctions[j*numOfHF+m][n];
				result+= (temp>=0) * (pow(2,numOfHF-m-1));
			}
			bucketsLSH[j][result].push_back(i);
			temp.push_back(result);
		}
		x[i].set_bucket(temp);
	}
}

long double vinh(vector <double> p,int i,int j,vector <GFunction> g)
{
	long double result=0.0;
	for(int m=0;m<x[0].get_values().size();m++)
	{
		int res =p[m]*g[i].get_h()[j].get_v()[m];
		if(res>0)
			result+= res%M;
		else
			result+=(-1)*res*(M-1)%M;
	}
	double res = result+g[i].get_h()[j].get_t();
	res /= W;
	return result;
}