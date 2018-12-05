#include "update.h"
#include "distances.h"

extern vector <xclass> x;
extern int fileSize;
extern int metric;

bool kmeans(vector <int> *clusters,vector <vector <int> > idsInCluster)
{
	bool allflag=0;
	for(int i=0;i<clusters->size();i++)
	{
		bool flag=0;
		vector <double> ds;
		for(int j=0;j<x[0].get_values().size();j++)
		{
			double d=0.0;
			for(int m=0;m<idsInCluster[i].size();m++)
				d+=x[idsInCluster[i][m]].get_values()[j];
			d/=idsInCluster[i].size();
			ds.push_back(d);
		}
		for(int j=0;j<x[0].get_values().size();j++)
		{
			if(!(x[clusters->at(i)].get_values()[j]==ds[j]))
			{
				flag=1;
				break;
			}
		}
		if(flag)
		{
			xclass temp(-1,ds);
			x.push_back(temp);
			clusters->at(i)=x.size()-1;  //last item pushed
		}
		allflag=allflag||flag;
	}
	return allflag;
}

bool pam(vector <int> *clusters,vector <vector <int> > idsInCluster)  //optimization like k-medoids
{
	bool flag=0;
	for(int i=0;i<clusters->size();i++)
	{
		double min;
		int mini;
		for(int j=0;j<idsInCluster[i].size();j++)
		{
			double distance=0.0;
			for(int m=0;m<idsInCluster[i].size();m++)
				distance+=euclDis(idsInCluster[i][m],idsInCluster[i][j]);
			if((distance<min)||(j==0))
			{
				min=distance;
				mini=j;
			}
		}
		if(!(mini==clusters->at(i)))
		{
			clusters->at(i)=mini;
			flag=1;
		}
	}
	//return flag;
	return 0;
}	