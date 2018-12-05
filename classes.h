#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <vector>
#include <string>
#include <iostream>

#define EUC 0
#define COS 1

using namespace std;

class xclass
{
	int id;
	vector <double> values;
	int cluster_id;   //position in vector of clusters (0 if cluster 0, 1 if cluster 1), not position in vector x
	int flag;         //if clustered and in which loop, needed for lsh and hypercube
	vector <int> bucket;  //in which buckets it belongs in each hashtable
	
	public:
		xclass(int i, vector <double> v) { flag=0; cluster_id=-1; id=i; values=v;} //constructor with defaults
		//setters-getters
		int get_id() { return id; }
		void set_id(int s) { id=s; }
		vector <double> get_values() { return values; }
		void set_values(vector <double> v) { values=v; }
		int get_cluster_id() { return cluster_id; }
		void set_cluster_id(int cid) { cluster_id=cid; }
		int get_flag() { return flag; }
		void set_flag(int f) { flag=f; }
		vector <int> get_bucket() { return bucket; }
		void set_bucket(vector <int> b) { bucket=b; }
		
		
		void print(){ //for debug, it can be deleted
			cout << "id: " << get_id() << ", values: ";
			for(int i=0;i<get_values().size();i++)
				cout << get_values()[i] << " ";
			cout << "cluster_id: "<<get_cluster_id() << " flag: " << get_flag() <<endl;
		}
};

typedef vector <vector <vector <int> > > bucketClass;

class hashFunction
{
	double t;
	vector <double> v;
	
	public:
		double get_t() { return t; }
		void set_t(double i) { t=i; }
		vector <double> get_v() { return v; }
		void set_v(vector <double> v1){ v=v1; }
};

class GFunction
{
	vector <class hashFunction> h;
	vector <double> r;
	
	public:
		vector <class hashFunction > get_h() { return h; }
		void set_h(vector <class hashFunction> h1) { h=h1; }
		vector <double> get_r() { return r; }
		void set_r(vector <double> v) { r=v; }
};


#endif