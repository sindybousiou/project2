#include "functions.h"
#include "classes.h"
#include <cstring>
#include "initialization.h"
#include "assignment.h"
#include "update.h"
#include "distances.h"

extern bool metric;
extern vector <xclass> x;
extern int fileSize;
string outString="";

bool read_args(string *inFile,string *confFile, string *outFile, int argc, char ** argv)
{
	bool flagI=1,flagC=1,flagO=1;
	bool complete=0;
	for(int i=0; i<argc; i++)
	{
		if(strcmp(argv[i],"-i") == 0)
		{
			i++;
			*inFile = argv[i];
			flagI=0;
		}
		else if(strcmp(argv[i],"-c")  == 0)
		{
			i++;
			*confFile = argv[i];
			flagC=0;
		}
		else if(strcmp(argv[i],"-o")  == 0)
		{
			i++;
			*outFile = argv[i];
			flagO=0;
		}
		else if(strcmp(argv[i],"-d")  == 0)
		{
			i++;
			if(strcmp(argv[i],"euclidean")==0)
				metric=EUC;
			else if(strcmp(argv[i],"cosine")==0)
				metric=COS;
			else
			{
				puts("Invalid input for metric. Try again.");
				exit(-1);
			}
		}
		else if(strcmp(argv[i],"-complete")  == 0)
			complete=1;
		
	}
	if(flagI || flagC || flagO)  //if input, query and output paths are NOT given
	{
		puts("Invalid arguments from command line. Try again.");
		exit(-1);
	}
	return complete;
}

string combinations(bool complete)
{
	string answer;
	int init,assign,update;
	while(1)
	{
		puts("Do you want program to run for all combinations, specific combination, or exit? (all/specific/exit)");
		cin >> answer;
		if(answer.compare("specific")==0)
		{
			puts("Give initialization algorithm (1 for simplest, 2 for k-means++)");
			cin >> answer;
			init=stoi(answer);
			puts("Give assignment algorithm (1 for lloyd, 2 for lsh, 3 for hypercube)");
			cin >> answer;
			assign=stoi(answer);
			puts("Give update algorithm (1 for k-means, 2 for pam)");
			cin >> answer;
			update=stoi(answer);
			clustering(init,assign,update,complete);
		}
		else if(answer.compare("all")==0)
		{
			clustering(1,1,1,complete);
			clustering(2,1,1,complete);
			clustering(1,2,1,complete);
			clustering(2,2,1,complete);
			//clustering(1,3,1,complete);
			//clustering(2,3,1,complete);
			clustering(1,1,2,complete);
			clustering(2,1,2,complete);
			clustering(1,2,2,complete);
			clustering(2,2,2,complete);
			//clustering(1,3,2,complete);
			//clustering(2,3,2,complete);
			
		}
		else
			break;
	}
	return outString;
}

void clustering(int init,int assign,int update,bool complete)
{
	stringForOutFile("AlgorithmI"+to_string(init)+"A"+to_string(assign)+"U"+to_string(update)+"\nMetric: ");
	if(metric==EUC)
		stringForOutFile("Euclidean\n");
	else
		stringForOutFile("Cosine\n");
	timespec start,end;
	clock_gettime(CLOCK_MONOTONIC,&start);
	vector <int> clusters;
	if(init==1)
		clusters=initRandom();
	else if(init==2)
		clusters=initKmeans();
	else
	{
		puts("Invalid input for initialization algorithm. Going with simplest random selection.");
		clusters=initRandom();
	}
	bool result=1;
	vector <vector <int> > idsInCluster;
	int loop=0;
	while(result&&(loop<40))
	{
		idsInCluster.clear();
		if(assign==1)
			idsInCluster=lloyd(clusters);
		else if(assign==2)
			idsInCluster=lsh(clusters);
		else if(assign==3)
			idsInCluster=cube(clusters);
		else
		{
			puts("Invalid input for assignment algorithm. Going with lloyd's");
			idsInCluster=lloyd(clusters);
		}
		for(int i=0;i<idsInCluster.size();i++)
			cout << idsInCluster[i].size() << " ";
		cout << endl;
		if(update==1)
		{
			result=kmeans(&clusters,idsInCluster);
			loop++;  //reach 40 loops and stop
		}
		else if(update==2)
		{
			result=pam(&clusters,idsInCluster);
			loop += 10;  //reach 4 loops and stop
		}
		else
		{
			puts("Invalid input for update algorithm. Going with kmeans.");
			update=1;
			result=kmeans(&clusters,idsInCluster);
		}
	}
	clock_gettime(CLOCK_MONOTONIC,&end);
	double time=end.tv_sec-start.tv_sec;
	time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
	for(int i=0;i<clusters.size();i++)
	{
		stringForOutFile("\nCluster-"+to_string(i)+" {size: "+to_string(idsInCluster[i].size())+" centroid: ");
		if(x[clusters[i]].get_id()==-1)
			for(int j=0;j<x[clusters[i]].get_values().size();j++)
				if(j==x[clusters[i]].get_values().size()-1)
		stringForOutFile(to_string(x[clusters[i]].get_values()[j])+"}\n");
				else
					stringForOutFile(to_string(x[clusters[i]].get_values()[j])+", ");
		else
			stringForOutFile("<"+to_string(x[clusters[i]].get_id())+">\n");
		if(complete)
		{
			stringForOutFile("{");
			for(int j=0;j<idsInCluster[i].size();j++)
				stringForOutFile(" item_id"+to_string(idsInCluster[i][j]));
			stringForOutFile("\n");
		}
	}
	stringForOutFile("clustering_time: "+to_string(time));
	silhouette(clusters,idsInCluster);
	clearx();
}

void clearx()
{
	for(int i=0;i<fileSize;i++)
	{
		x[i].set_cluster_id(-1);
		x[i].set_flag(0);
	}
	x.erase(x.begin()+fileSize,x.begin()+x.size());
}

void silhouette(vector <int> clusters,vector <vector <int> > idsInCluster)
{
	stringForOutFile("\nSilhouette: [");
	vector <double> ret;
	for(int i=0;i<clusters.size();i++)
	{
		double a=0.0,b=0.0;
		for(int j=0;j<idsInCluster[i].size();j++)
		{
			if(metric==EUC)
				a+= euclDis(idsInCluster[i][j],clusters[i]);
			else
				a+=cosDis(idsInCluster[i][j],clusters[i]);
			vector <int> remainingClusters;
			for(int m=0;m<clusters.size();m++)
				if(!(clusters[m]==clusters[i]))
					remainingClusters.push_back(clusters[m]);
			int axrhsto;
			b+=minDistance(remainingClusters,idsInCluster[i][j],&axrhsto);
		}
		a/=idsInCluster[i].size();
		b/=idsInCluster[i].size();
		if(a>b)
			ret.push_back(a/b-1);
		else if(a<b)
			ret.push_back(1-a/b);
		else
			ret.push_back(0.0);
	}
	double value=0.0;
	for(int i=0;i<ret.size();i++)
	{
		stringForOutFile(to_string(ret[i])+", ");
		value+=ret[i];
	}
	stringForOutFile(to_string(value/ret.size())+"]\n\n");
}

void stringForOutFile(string s)
{
	outString+=s;
	cout << s;  //mporei na vgei
}