#include <stdio.h>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include "global.h"
#include "MySQL_API.h"

char *g_host = NULL;
int g_port = 0;
char *g_user = NULL;
char *g_passwd = NULL;
char *g_database = NULL;

const int g_kGeneratorType = 18;

float kEps = 1E-12;

int InitAll();
bool IsMoreFinded(const std::pair<int, int> &a, const std::pair<int, int> &b);
bool IsBiggerF1(const std::pair<int, std::pair<float, float> > &a, const std::pair<int, std::pair<float, float> > &b);

typedef std::pair<int, int>  PairInt;
typedef std::pair<int, std::pair<float, float> >  PairPair;

int main(int argc, char *argv[])
{
	InitAll();
	CMySQL_API mysql_api(g_user, g_passwd, g_database);
	if(mysql_api.Init_Database_Con(g_host, g_port) != 0)
		return -1;
	std::map<int, int> generator_type_num;
	std::map<int, std::pair<float, float> > query_pr;
	int submmitid = 1;
	for(unsigned int taskid = 1; taskid != 1001; ++taskid)
	{
		mysql_api.Result_Table_Evaluate(taskid, submmitid, generator_type_num, query_pr);
	}

	char result_fname[] = "../result/result0.7.txt";
	FILE *fresult = fopen(result_fname,"w");
	float tasknum = 1000; // just for caculate ratio conveniently
	std::vector<PairInt > generator_type_num_vec(generator_type_num.begin(), generator_type_num.end());
	std::sort(generator_type_num_vec.begin(), generator_type_num_vec.end(), IsMoreFinded);
	for(std::vector<PairInt >::const_iterator iter = generator_type_num_vec.begin(); \
		iter != generator_type_num_vec.end(); ++iter)
	{
		//printf("Type = %d, Find ratio = %f\n", iter->first, iter->second / tasknum);
		fprintf(fresult, "Type = %d, Find ratio = %f\n", iter->first, iter->second / tasknum);
	}
	printf("**************************************************\n");
	fprintf(fresult, "**************************************************\n");

	std::vector<PairPair > query_pr_vec(query_pr.begin(), query_pr.end());
	std::sort(query_pr_vec.begin(), query_pr_vec.end(),IsBiggerF1);
	double mf1 = 0.0;
	for(std::vector<PairPair >::const_iterator iter = query_pr_vec.begin(); iter != query_pr_vec.end(); ++iter)
	{
		//printf("TaskId = %d, precision = %f, recall = %f\n", iter->first, iter->second.first, iter->second.second);
		float f1 = (2.0 * iter->second.first * iter->second.second) / (iter->second.first + iter->second.second +kEps);
		mf1 += f1;
		fprintf(fresult, "TaskId = %d, precision = %f, recall = %f, F1 = %f\n", iter->first, iter->second.first, iter->second.second, f1);
	}
	mf1 /= (tasknum + kEps);
	fprintf(fresult,"*****************************************************\nMean F1 Score = %f\n",mf1);
	printf("Mean F1 Score = %f\n",mf1);
	fclose(fresult);
	mysql_api.Release_Database_Con();
	return 0;
}

bool IsMoreFinded(const PairInt &a, const PairInt &b)
{
	return a.second > b.second;
}

bool IsBiggerF1(const PairPair &a, const PairPair &b)
{
	float fa = (2.0 * a.second.first * a.second.second) / (a.second.first + a.second.second + kEps);
	float fb = (2.0 * b.second.first * b.second.second) / (b.second.first  + b.second.second + kEps);
	return fa > fb;
}

int InitAll()
{
	g_host = "10.25.0.105";
	g_port = 3306;
	g_user = "root";
	g_passwd = "root";
	g_database = "generatortest";
}
