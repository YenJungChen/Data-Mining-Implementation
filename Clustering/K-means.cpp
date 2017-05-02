#include <iostream>
#include <deque>
#include <math.h>
#include <time.h>
#include <string.h>
#include <fstream>
//define the longest distance between center and data point
#define MAX 9999
#define SIZE 514
#define ATTR 8
//the amount of clusters
#define k 8
using namespace std;

class DATA{
public:
    double data[8];
    int index;
    double distance;
}D[SIZE];
double w[8]={0.0084468, 0.72183, 0.0752224, 0.0623064, 0.00354434, 0.0161686, 0.0395071, 0.0729745};

void readin(){
    ifstream fin;
    char output[100];
    int a=0, b=0;
    char *pch;
    fin.open("training.txt");
    if(fin.is_open()){
        while(!fin.eof()){
            fin.getline(output,sizeof(output),'\n');
            pch=strtok (output,",");
            b=0;
            while(pch!=NULL){
                if(b!=ATTR)
                    D[a].data[b]=w[b]*atof(pch);
                //printf("%f\n",D[a].data[b++]);
                pch=strtok (NULL, ",");
                D[a].index=a;
                b++;
            }
            a++;
        }
    }
    fin.close();
}

//use the average X and Y of the cluster to find a closest new center
int NewCenter(double Avg[ATTR], deque <DATA> Cluster) {
    double diff=9999;
    int index=-1;
    for (int i=0; i<Cluster.size(); i++) {
        //find the closest real point with the computational center point
        double current_diff=0;
        for(int j=0; j<ATTR; j++)
            current_diff+=pow(Avg[j]-Cluster[i].data[j], 2);
        current_diff=sqrt(current_diff);
        if(current_diff<diff) {
            diff=current_diff;
            index=i;
        }
    }
    return index;
}

//compute the average X and Y in the cluster
DATA Mean(deque <DATA> Cluster) {
    double SUM[ATTR];
    for(int i=0; i<ATTR; i++)
        SUM[i]=0;
    for (int i=0; i<Cluster.size(); i++) {
        for(int j=0; j<ATTR; j++)
            SUM[j]+=Cluster[i].data[j];
    }
    double AVG[8];
    for(int i=0; i<8; i++){
        AVG[i]=SUM[i]/SIZE;
    }
    int INDEX=NewCenter(AVG, Cluster);
    return Cluster[INDEX];
}

//print the result of the cluster
void PrintCluster(deque <DATA> Cluster) {
    cout<<"index: ";
    for(int i=0; i<Cluster.size(); i++) {
        cout<<Cluster[i].index<<", ";
    }
    cout<<endl;
}

//print the center of the cluster
void PrintCenter(DATA Center) {
    cout<<"center: "<<Center.index<<endl<<endl;
}

void SumDist(deque <DATA> d) {
    int sum=0;
    for(int i=0; i<d.size(); i++)
        sum+=d[i].distance;
    cout<<"sum: "<<sum<<endl;
    cout<<endl;
}

int main(void) {
    readin();
    srand((unsigned)time(NULL));
    DATA temp_cent[k];						//random centers
    for (int i=0; i<k; i++){ 				//the first time decide the centers randomly
        temp_cent[i] = D[rand()%SIZE];         //randomly choose the centers
        for (int j=0; j<i; j++){
            if (temp_cent[i].index==temp_cent[j].index) {
                i--;
                break;
            }
        }
        
    }

    bool stop = 0;
    while (!stop) {
        deque <DATA> cluster[k];			//store the divided data points in clusters
        for (int i=0; i<SIZE; i++) {		//divide data points into clusters
            double last_dist = MAX;			//the distance between data points and each centers
            int cent = -1;					//store which center is the nearest
            
            //find the nearest center of each data points
            for (int j=0; j<k; j++) {
                int curr_dist=0;
                for(int a=0; a<ATTR; a++)
                    curr_dist+=pow(temp_cent[j].data[a]-D[i].data[a], 2);
                curr_dist=sqrt(curr_dist);
                if (curr_dist<last_dist) {
                    last_dist=curr_dist;
                    cent=j;
                }
            }
            D[i].distance = last_dist;
            //add data points into the nearest cluster
            cluster[cent].push_back(D[i]);
 
        }
        
        //count how many centers are unchange to decide whether already find out the real center
        int num_unchange = 0;
        for (int b=0; b<k; b++) {			//find the central point of each cluster
            if (temp_cent[b].index != Mean(cluster[b]).index)
                temp_cent[b] = Mean(cluster[b]);
            else
                num_unchange ++;
        }
        if (num_unchange == k) {
            stop = 1;
            for(int i=0; i<k; i++) {
                cout<<"Cluster "<<i+1<<endl;
                PrintCluster(cluster[i]);
                PrintCenter(temp_cent[i]);
                SumDist(cluster[i]);
            }
        }
    }
}
