#include <iostream>
#include <deque>
#include <math.h>
#include <time.h>

//the amount of clusters
#define k 6
//the amount of candidates
#define m 2
using namespace std;

//define data structure
struct Data {
    int x;
    int y;
    int index;
    double distance;
};

//use the average X and Y of the cluster to find a closest new center
int NewCenter(int AvgX, int AvgY, deque <Data> Cluster) {
    int size = (int)Cluster.size();
    int diff = 9999, index = -1;
    for (int i=0; i<size; i++) {
        //find the closest real point with the computational center point
        int current_diff = (int)sqrt(pow((AvgX - Cluster[i].x), 2) + pow((AvgY - Cluster[i].y), 2));
        if(current_diff < diff) {
            diff = current_diff;
            index = i;
        }
    }
    return index;
}

//compute the average X and Y in the cluster
Data Mean(deque <Data> Cluster) {
    int SumX = 0, SumY = 0;
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
}

//print the center of the cluster
void PrintCenter(Data Center) {
    cout<<"center: "<<Center.index;
    cout<<", X: "<<Center.x;
    cout<<", Y: "<<Center.y<<endl;
    
}

//set the sort function
bool SortCriterion(const Data& d1, const Data& d2) {
    return d1.distance < d2.distance || (d1.distance == d2.distance && d1.distance < d2.distance);
}

//display the sum of the distance between each data point and the center
void SumDist(deque <Data> d) {
    int sum = 0;
    for(int i=0; i<d.size(); i++)
        sum += d[i].distance;
    cout<<"sum: "<<sum<<endl;
    cout<<endl;
}

int main(void) {
    int n = 20;							//number of data
    //n data points (pre-set)
    Data data[20] = {{1,3,0}, {3,5,1}, {2,4,2}, {16,20,3}, {14,18,4}, {22,1,5},
        {20,3,6}, {19,2,7}, {18,17,8}, {7,5,9}, {24,6,10}, {8,8,11}, {20,18,12},
        {5,3,13}, {4,4,14}, {15,15,15}, {18,5,16}, {6,8,17}, {13,23,18}, {23,1,19}};
    Data temp_cent[k];					//temporary centers
    for (int a=0; a<k; a++) 			//the first time decide the centers randomly
        temp_cent[a] = data[rand()%n];
    srand((unsigned)time(NULL));        //random time seed
    bool stop = 0;                      //check whether achieve optimal
    
    while (!stop) {
        deque <Data> cluster[k];		//store the divided data points in clusters
        for (int i=0; i<n; i++) {		//divide data points into clusters
             deque <Data> Candidate;    //store the better candidate of possible centers
            
            for (int j=0; j<k; j++) {   //find the "CLOSER" center of each data points
                //distance between data points and temporary center
                double curr_dist = sqrt(pow((data[i].x-temp_cent[j].x),2)+pow((data[i].y-temp_cent[j].y),2));
                temp_cent[j].distance = curr_dist;
                int num = temp_cent[j].index;
                temp_cent[j].index = j;
                Candidate.push_back(temp_cent[j]);  //store the distance and centers for later to sort
                temp_cent[j].index = num;
            }
            sort(Candidate.begin(), Candidate.end(), SortCriterion);    //sort centers based on the distance
            if(Candidate[0].distance == 0) {        //if the point equal to the center
                data[i].distance = 0;               //then put the point into its cluster
                cluster[Candidate[0].index].push_back(data[i]);
            }
            else {
                int INDEX = rand()%m;                           //use GRASP method to random choose one better center
                data[i].distance = Candidate[INDEX].distance;   //add data points into the "CLOSER" cluster
                INDEX = Candidate[INDEX].index;
                cluster[INDEX].push_back(data[i]);
            }
        }
        
        //count how many centers are unchange to decide whether already find out the real center
        int num_unchange = 0;
        for (int b=0; b<k; b++) {       //find the central point of each cluster
            if (temp_cent[b].index != Mean(cluster[b]).index)
                temp_cent[b] = Mean(cluster[b]);
            else
                num_unchange ++;
        }
        if (num_unchange == k) {        //print out the result of clusters
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
