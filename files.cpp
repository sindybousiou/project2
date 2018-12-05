#include "files.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

extern vector <xclass> x;
extern int k;

void readInFile(string inFile)
{
	ifstream in(inFile);
	if(in.fail())
	{
		puts("Unable to open input file. Exiting...");
		exit(-1);
	}
	string line;
	while(getline(in,line))
	{
		vector <double> v;
		string value;
		stringstream ss(line);
		int id;
		bool i=1;
		while(getline(ss,value,',')) //change to while(getline(ss,value,'\t')) or while(getline(ss,value,','))
			if(i)
			{
				id=stoi(value);
				i=0;
			}
			else
				v.push_back(stod(value));
		xclass temp(id,v);
		x.push_back(temp);
	}
	in.close();
}

void readConfFile(string confFile,int *l,int *numOfHF)
{
	ifstream conf(confFile);
	if(conf.fail())
	{
		puts("Unable to open configuration file. Exiting...");
		exit(-1);
	}
	k=-1; //initialize so to know we are given a valid number by file
	string line;
	while(getline(conf,line))
	{
		string value;
		stringstream ss(line);
		while(getline(ss,value,' '))
		{
			if(value.compare("number_of_clusters:")==0)
			{
				getline(ss,value,' ');
				k= stoi(value);
			}
			else if(value.compare("number_of_hash_functions:")==0)
			{
				getline(ss,value,' ');
				*numOfHF= stoi(value);
			}
			else if(value.compare("number_of_hash_tables:")==0)
			{
				getline(ss,value,' ');
				*l= stoi(value);
			}
		}
	}
	if(k<=0)
	{
		puts("Missing or invalid number of clusters. Exiting...");
		exit(-1);
	}
	conf.close();
}


void writeOutFile(string outString,string outFile)
{
	ofstream out (outFile);
	out << outString;
	out.close();
}