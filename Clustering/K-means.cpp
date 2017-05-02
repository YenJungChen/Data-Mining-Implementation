#include <iostream>
#include <deque>
#include <math.h>
#include <time.h>
//define the longest distance between center and data point
#define MAX 9999

//the amount of clusters
#define k 4
using namespace std;

//define data structure
struct Data {
    int x;
    int y;
    int index;
    int distance;
};

//use the average X and Y of the cluster to find a closest new center
int NewCenter(int AvgX, int AvgY, deque <Data> Cluster) {
    int size = (int)Cluster.size();
    int diff = 9999;
    int index = -1;
    for (int i=0; i<size; i++) {
        //find the closest real point with the computational center point
        int current_diff = (int)sqrt((AvgX - Cluster[i].x)*(AvgX - Cluster[i].x)+(AvgY - Cluster[i].y)*(AvgY - Cluster[i].y));
        if(current_diff < diff) {
            diff = current_diff;
            index = i;
        }
    }
    return index;
}

//compute the average X and Y in the cluster
Data Mean(deque <Data> Cluster) {
    int SumX = 0;
    int SumY = 0;
    int Size = (int)Cluster.size();
    for (int i=0; i<Size; i++) {
        SumX += Cluster[i].x;
        SumY += Cluster[i].y;
    }
    int avgX = SumX/Size;
    int avgY = SumY/Size;
    int index = NewCenter(avgX, avgY, Cluster);
    return Cluster[index];
}

//print the result of the cluster
void PrintCluster(deque <Data> Cluster) {
    int size = (int)Cluster.size();
    for(int i=0; i<size; i++) {
        cout<<"index: "<<Cluster[i].index;
        cout<<", X: "<<Cluster[i].x;
        cout<<", Y: "<<Cluster[i].y<<endl;
    }
    cout<<endl;
}

//print the center of the cluster
void PrintCenter(Data Center) {
    cout<<"center: "<<Center.index;
    cout<<", X: "<<Center.x;
    cout<<", Y: "<<Center.y<<endl;
    cout<<endl;
}

void SumDist(deque <Data> d) {
    int sum = 0;
    for(int i=0; i<d.size(); i++)
        sum += d[i].distance;
    cout<<"sum: "<<sum<<endl;
    cout<<endl;
}

int main(void) {
    srand((unsigned)time(NULL));
    int n = 20;								//number of data
    
    //n data points (pre-set)
    Data data[20] = {{1,3,0}, {3,5,1}, {2,4,2}, {16,20,3},
        {14,18,4}, {22,1,5}, {20,3,6}, {19,2,7},
        {18,17,8}, {7,5,9}, {24,6,10}, {8,8,11},
        {20,18,12}, {5,3,13}, {4,4,14}, {15,15,15},
        {18,5,16}, {6,8,17}, {13,23,18}, {23,1,19}};
    
    Data temp_cent[k];						//random centers
    for (int a=0; a<k; a++) 				//the first time decide the centers randomly
        temp_cent[a] = data[rand()%n];		//randomly choose the centers
    bool stop = 0;
    while (!stop) {
        deque <Data> cluster[k];			//store the divided data points in clusters
        for (int i=0; i<n; i++) {			//divide data points into clusters
            int last_dist = MAX;			//the distance between data points and each centers
            int cent = -1;					//store which center is the nearest
            
            //find the nearest center of each data points
            for (int j=0; j<k; j++) {
                int curr_dist = (int)sqrt((data[i].x-temp_cent[j].x)*(data[i].x-temp_cent[j].x)+(data[i].y-temp_cent[j].y)*(data[i].y-temp_cent[j].y));
                if (curr_dist < last_dist) {
                    last_dist = curr_dist;
                    cent = j;
                }
            }
            data[i].distance = last_dist;
            //add data points into the nearest cluster
            cluster[cent].push_back(data[i]);
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
