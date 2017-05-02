#include <iostream>
#include <string>
#include <time.h>
#include <deque>
#include <math.h>
#define n 10
#define p 5
using namespace std;
int feature[] = {1,13,18,6,4};
struct Data {
public:
    int attr[p]={0};
    bool train = 0;
}data[n];
struct TS {
public:
    int score=0;
    int attr=0;
}TrainScore[p], TestScore[p];

//set the sort function
bool SortCriterion(const TS& s1, const TS& s2) {
    return s1.score < s2.score || (s1.score == s2.score && s1.score < s2.score);
};

deque <TS> select(deque<TS>& ts, int Score) {
    for(int e=0; e<p; e++) {
        for(int d=0; d<n; d++) {
            int Accu = 0;
            for(int f=0; f<p; f++) {
                if(f != e) {
                    Accu += pow((feature[f]-data[d].attr[f]),2);
                }
            }
            TrainScore[e].score += sqrt(Accu);
        }
        double rate = (double)TrainScore[e].score/Score*100;
        TrainScore[e].score = rate;
        TrainScore[e].attr = e;
        ts.push_back(TrainScore[e]);
    }
    return ts;
}
void print(deque<TS> ts) {
    for(int i=0; i<p; i++)
        cout<<"NO. "<<ts[i].attr<<" score: "<<ts[i].score<<"%"<<endl;
}

int main() {
    int train_num = 0.7*n;
    int Score = 0;

    int DataAttr[n][p] = {{1,13,15,7,30}, {0,13,45,6,9}, {1,38,5,6,25},
        {0,13,69,6,6}, {1,13,19,5,8}, {0,13,16,8,5}, {0,13,38,7,5},
        {1,13,46,29,6}, {1,13,46,6,7}, {0,13,20,6,7}};
    for(int a=0; a<n; a++)
        memcpy(data[a].attr, DataAttr[a], p*4);
    deque <TS> Training;
    for(int i=0; i<train_num; i++) {
        int PickTrain = random()%n;
        data[PickTrain].train = 1;
    }
    for(int b=0; b<n; b++) {
        int Acc = 0;
        for(int c=0; c<p; c++) {
            Acc += pow((feature[c]-data[b].attr[c]),2);
        }
        Score += sqrt(Acc);
    }
    Training = select(Training,Score);
    sort(Training.begin(), Training.end(), SortCriterion);
    print(Training);
}
