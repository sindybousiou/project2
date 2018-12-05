#include "assignment.h"
#include "distances.h"
#include "hash.h"

extern vector <xclass> x;
extern int fileSize;

vector <vector <int> > lloyd(vector <int> clusters)
{
	vector <vector <int> > idsInCluster;
	for(int i=0;i<clusters.size();i++)  //initialization
	{
		vector <int> temp;
		idsInCluster.push_back(temp);
	}
	for(int i=0;i<fileSize;i++)
	{
		int cluster_id=-1;
		minDistance(clusters,i,&cluster_id);
		x[i].set_cluster_id(cluster_id);
		idsInCluster[cluster_id].push_back(x[i].get_id()-1);  //-1, because ids start from 1, while indexes start from 0
	}
	return idsInCluster;
}

vector <vector <int> > lsh(vector <int> clusters)
{
	vector <vector <int> > idsInCluster;
	for(int i=0;i<clusters.size();i++)  //initialization
	{
		vector <int> temp;
		idsInCluster.push_back(temp);
	}
	bool result=1;
	int flag=1;
	double range=initialRange(clusters);
	while(result)
	{
		result=rangeSearch(range,flag,&idsInCluster,clusters);
		range *= 2;
		flag++;
	}
	for(int i=0;i<fileSize;i++)   //loyd assignment for every point that hasn't been assigned
		if(x[i].get_flag()==0)
		{
			int cluster_id=-1;
			minDistance(clusters,i,&cluster_id);
			x[i].set_cluster_id(cluster_id);
			idsInCluster[cluster_id].push_back(x[i].get_id()-1);  //-1, because ids start from 1, while indexes start from 0
		}
	return idsInCluster;
}

vector <vector <int> > cube(vector <int> clusters)
{
	vector <vector <int> > idsInCluster;

	return idsInCluster;
}	