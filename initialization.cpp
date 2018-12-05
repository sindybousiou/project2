#include "initialization.h"
#include <cmath>
#include "distances.h"

extern int k;
extern vector <xclass> x;
extern mt19937 generator;   //check f generator works OK

vector <int> initRandom()
{
	vector <int> clusters;
	uniform_int_distribution <int> distribution(0,x.size()-1);
	for(int i=0;i<k;i++)
	{
		while(1)  //to check if already exists before pushing
		{
			int j,index=distribution(generator);  //generate and check
			for(j=0;j<clusters.size();j++)
				if(index==clusters[j])
					break;
			if(j==clusters.size())   //not found, since reached clusters.size()
			{                        //so will break and not continue while(1) loop to select another number
				clusters.push_back(index);
				break;
			}
		}
	}
	return clusters;
}

vector <int> initKmeans()
{
	vector <int> clusters;
	uniform_int_distribution <int> distribution(0,x.size());  //take first randomly
	int index=distribution(generator);
	clusters.push_back(index);
	
	for(int i=1;i<k;i++)  //go on for the second and rest centroids
	{
		vector <double> p;
		double max;
		for(int j=0;j<x.size();j++)  //find min distance from existing clusters
		{
			int index;
			double min=minDistance(clusters,j,&index);
			if(!(min==0))
				p.push_back(pow(min,2));
			if(j==0||pow(min,2)>max)  //keep max for normalization
				max=pow(min,2);
		}
		for(int j=0;j<p.size();j++)
		{
			p[j]=p[j]/max;  //normalization
			if(j>0)
				p[j]=p[j-1]+p[j];
		}
		uniform_real_distribution <double> distribution2(0.0,p[p.size()-1]); //random from 0 to max of p, which is in the last place
		double random=distribution2(generator);
		for(int j=0;j<p.size();j++)            //optimization with binary search???
			if(p[j]>random)
			{
				clusters.push_back(j);
				break;
			}
	}
	return clusters;
}