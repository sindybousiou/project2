#include <string>
#include <random>
#include <vector>

using namespace std;

bool read_args(string *,string *, string *, int, char **);
string combinations(bool);
void clustering(int,int,int,bool);
void clearx();
void silhouette(vector <int>,vector <vector <int> >);
void stringForOutFile(string);