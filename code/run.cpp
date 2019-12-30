#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cassert>
#include <vector>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define THREADS 40

string getCmdResult(const string &strCmd) {
	char buf[10240] = {0};
	FILE *pf = NULL;
	
	if( (pf = popen(strCmd.c_str(), "r")) == NULL ) return "";
 
	string strResult;
	while(fgets(buf, sizeof buf, pf))
		strResult += buf;
	
	pclose(pf);
 
	unsigned int iSize =  strResult.size();
	if(iSize > 0 && strResult[iSize - 1] == '\n')  // linux
		strResult = strResult.substr(0, iSize - 1);
	for (int i = strResult.size() - 1; i >= 0; --i)
		if (strResult[i] == '\n'){
			strResult = strResult.substr(i + 1, strResult.size());
			break;
		}
 
	return strResult;
}


struct ip{
    int a, b, c, id, aliveCount;
    double avgPing;
    bool operator < (const ip &i) const{
        if (a == i.a){
            if (b == i.b)
                return c < i.c;
            return b < i.b;
        }
        return a < i.a;
    }
};
vector<ip> ips[THREADS], res[THREADS], summary;

char buffer[THREADS][200];
void* get_res(void *data_){
    vector<ip>* data = (vector<ip>*) data_;
    for (int i = 0, lim = data->size(); i < lim; ++i){
        ip tmp = (*data)[i];
        // sprintf(buffer[tmp.id], "fping -g %d.%d.%d.0/24 --count=1 --retry=2 --timeout=1000 --stats --quiet -Z \'../result/%03d_%03d_%03d.txt\'", tmp.a, tmp.b, tmp.c, tmp.a, tmp.b, tmp.c);
        sprintf(buffer[tmp.id], "sudo fping -g %d.%d.%d.0/24 --count=1 --retry=2 --timeout=1000 --stats --quiet", tmp.a, tmp.b, tmp.c);
        // system(buffer[tmp.id]);
        string s = getCmdResult(string(buffer[tmp.id]));
        if (tmp.id == 0)
            printf("Finished %d / %d\n", i+1, lim);
        int aliveCount;
        double avgPing;
        stringstream ss;
        ss << s;
        ss >> aliveCount >> avgPing;
        res[tmp.id].push_back((ip){tmp.a, tmp.b, tmp.c, tmp.id, aliveCount, avgPing});
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv){
    pthread_t threads[THREADS];
    pthread_attr_t attr;
    void* status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    assert(argc == 7);
    int st0(atoi(argv[1])), ed0(atoi(argv[4])); 
    int st1(atoi(argv[2])), ed1(atoi(argv[5])); 
    int st2(atoi(argv[3])), ed2(atoi(argv[6])); 

    int thread_id = 0;
    for (int a = st0; a <= ed0; ++a)
        for (int b = st1; b <= ed1; ++b)
            for (int c = st2; c <= ed2; ++c){
                ips[thread_id].push_back((ip){a, b, c, thread_id, 0, 0.0});
                thread_id = (1 + thread_id) % THREADS;
            }

    int rc;
    for (int i = 0; i < THREADS; ++i)
        rc = pthread_create(&threads[i], NULL, get_res, (void*)&ips[i]);

    pthread_attr_destroy(&attr);
    for (int i = 0; i < THREADS; ++i)
        rc = pthread_join(threads[i], &status);
    
    for (int i = 0; i < THREADS; ++i)
        summary.insert(summary.begin(), res[i].begin(), res[i].end());
    sort(summary.begin(), summary.end());

    sprintf(buffer[0], "../final_result/%03d_%03d_%03d_%03d_%03d_%03d.txt", st0, st1, st2, ed0, ed1, ed2);

    FILE *fout = fopen(buffer[0], "w");
    for (int i = 0, lim = summary.size(); i < lim; ++i)
        // printf("%d %d %d %d %.2f\n", summary[i].a, summary[i].b, summary[i].c, summary[i].aliveCount, summary[i].avgPing);
        fprintf(fout, "%d %.2f\n", summary[i].aliveCount, summary[i].avgPing);
    fclose(fout);

    pthread_exit(NULL);

    return 0;
}