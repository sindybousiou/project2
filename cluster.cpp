#include <iostream>
#include "functions.h"
#include "files.h"
#include "classes.h"
#include "hash.h"

using namespace std;

//extern variables needed in a lot of functions-files
int k,fileSize;
vector <xclass> x;
bool metric;
random_device rd;
mt19937 generator(rd());
bucketClass bucketsLSH;

int main(int argc, char ** argv)
{
	string inFile,confFile,outFile;
	
	metric=EUC;   //default
	bool complete=read_args(&inFile,&confFile,&outFile,argc,argv);  //read arguments from command line
	
	readInFile(inFile);  //read input file and create x
	fileSize = x.size(); //we keep the original size of the file, because we are gonna add elements later
	int l=5,numOfHF=4;   //default, k initialize so to know we are given a valid number by conf file
	readConfFile(confFile,&l,&numOfHF);   //read configuration file
	
	lsh_hash(l,numOfHF);  //create hash functions and hash all items, divide them in buckets
	bucketClass bucketsCube=cube_hash(l,numOfHF);
	
	string outString=combinations(complete); //run algorithms and return result in string so to write in file
	
	writeOutFile(outString,outFile);  //write results in file

	return 0;
}