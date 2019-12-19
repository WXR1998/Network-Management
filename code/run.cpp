#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cassert>
#include <vector>

#define THREADS 50

using namespace std;

struct ip{
    int a, b, c, id;
};
vector<ip> ips[THREADS];

char buffer[THREADS][200];
void* get_res(void *data_){
    vector<ip>* data = (vector<ip>*) data_;
    for (int i = 0, lim = data->size(); i < lim; ++i){
        ip tmp = (*data)[i];
        sprintf(buffer[tmp.id], "fping -g %d.%d.%d.0/24 --count=1 --retry=2 --timeout=1000 --stats --quiet -Z \'../result/%03d_%03d_%03d.txt\'", tmp.a, tmp.b, tmp.c, tmp.a, tmp.b, tmp.c);
        system(buffer[tmp.id]);
        if (tmp.id == 0)
            printf("Finished %d / %d\n", i+1, lim);
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
                ips[thread_id].push_back((ip){a, b, c, thread_id});
                thread_id = (1 + thread_id) % THREADS;
            }

    int rc;
    for (int i = 0; i < THREADS; ++i)
        rc = pthread_create(&threads[i], NULL, get_res, (void*)&ips[i]);

    pthread_attr_destroy(&attr);
    for (int i = 0; i < THREADS; ++i)
        rc = pthread_join(threads[i], &status);

    pthread_exit(NULL);

    return 0;
}