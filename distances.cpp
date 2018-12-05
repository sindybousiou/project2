#include "distances.h"
#include <cmath>
#include <iostream>

using namespace std;

extern vector <xclass> x;
extern bool metric;
extern bucketClass bucketsLSH;

double initialRange(vector <int> clusters)
{
	double min;  //find initial range for range search
	for(int i=0;i<clusters.size();i++) //find min distance between clusters
	{
		int axrhsto;
		double thismin=minDistance(clusters,clusters[i],&axrhsto); //min distance of this cluster with rest of clusters
		if(i==0||thismin<min)
			min=thismin;
	}
	return min/2;  //initial range is min/2
}

double minDistance(vector <int> clusters,int j,int *index) //return min distance of a point from a vector of points
{
	double min;                                            //return the distance and the index of the point with the min distance
	for(int i=0;i<clusters.size();i++)
	{
		double distance;
		if(metric==EUC)
			distance=euclDis(j,clusters[i]);
		else
			distance=cosDis(j,clusters[i]);
		if(i==0||distance<min)  //if we are on the first loop, or if we have a lesser value
		{                       //change return values
			min=distance;
			*index=i;
		}
	}
	return min;
}

bool rangeSearch(double range, int flag, vector <vector <int> > *idsInCluster,vector <int> clusters)
{
	bool result=0;
	for(int i=0;i<clusters.size();i++)
	{
		for(int j=0;j<x[clusters[i]].get_bucket().size();j++)
		{
			for(int m=0;m<bucketsLSH[j][x[clusters[i]].get_bucket()[j]].size();m++)
			{
				double distance;
				if(metric==EUC)
					distance=euclDis(clusters[i],bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]);
				else
					distance=cosDis(clusters[i],bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]);
				if(distance <= range)
				{
					result=1;
					if(x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_flag()==flag)
					{
						if(metric==EUC)
						{
							if(euclDis(clusters[i],x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_id())>euclDis(x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_cluster_id(),x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_id()))
							{
								x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_flag(flag);
								x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_cluster_id(i);
								idsInCluster->at(j).push_back(bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]);
							}
						}
						else
						{
							if(cosDis(clusters[i],x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_id())>cosDis(x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_cluster_id(),x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_id()))
							{
								x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_flag(flag);
								x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_cluster_id(i);
								idsInCluster->at(j).push_back(bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]);
							}
						}
					}
					else if(x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].get_flag()==0)
					{
						x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_flag(flag);
						x[bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]].set_cluster_id(i);
						idsInCluster->at(j).push_back(bucketsLSH[j][x[clusters[i]].get_bucket()[j]][m]);
					}
				}
			}
		}
	}
	return 0;
}

double euclDis(int ix, int icluster)  //find euclidean distance for two points, with their ids
{
	double distance=0.0;
	for(int i=0;i<x[ix].get_values().size();i++)
		distance+= pow(x[ix].get_values()[i]-x[icluster].get_values()[i],2);
	return sqrt(distance);
}

double cosDis(int ix, int icluster) //find cosine distance
{
	double distance=0.0;
	double paronomastis1=0.0,paronomastis2=0.0;
	for(int i=0;i<x[ix].get_values().size();i++)
	{
		distance+= x[ix].get_values()[i]*x[icluster].get_values()[i];
		paronomastis1+= pow(x[ix].get_values()[i],2);
		paronomastis2+= pow(x[icluster].get_values()[i],2);
	}
	paronomastis1=sqrt(paronomastis1)*sqrt(paronomastis2);
	return 1-distance/paronomastis1;
}