#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <string.h>
#define SIZE 100
#define datanum 150
#define aim 0.000000000000001
using namespace std;
class datapair{
public:
    double pair[4]; //0=number, 1=data1, 2=data2, 3=distance
    double data[2][8];//0=data1, 1=data2
}DP[150];
double w[8];

//input data info. from files
void readin(){
    ifstream fin;
    fin.open("record.txt");
    char output[100];
    int i=0, j=0;
    if(fin.is_open()){
        while(!fin.eof()){
            fin>>output;
            if(output[0]>47&&output[0]<58){
                DP[i].pair[j++]=atof(output);
                //cout<<output<<endl;
            }
            if(j>3){
                j=0;
                i++;
            }
        }
    }
    fin.close();
    double dataset[600][9];
    int a=0, b=0;
    char *pch;
    fin.open("training.txt");
    if(fin.is_open()){
        while(!fin.eof()){
            fin.getline(output,sizeof(output),'\n');
            pch=strtok (output,",");
            b=0;
            while(pch!=NULL){
                dataset[a][b++]=atof(pch);
                //printf("%f\n",dataset[a][b-1]);
                pch=strtok (NULL, ",");
            }
            a++;
        }
    }
    fin.close();
    
    for(int i=0; i<datanum; i++){
        for(int j=0; j<a; j++){
            for(int k=0; k<2; k++){
                if(j+1==DP[i].pair[k+1]){
                    for(int l=0; l<8; l++){
                        DP[i].data[k][l]=dataset[j][l];
                        //cout<<DP[i].data[k][l]<<" ";
                    }
                    //cout<<endl;
                }
            }
        }
    }
}

//normalize weights
void nor(){
    double sum=0;
    for(int i=0; i<8; i++)
        sum+=w[i];
    for(int i=0; i<8; i++)
        w[i]=w[i]/sum;
}

//normalize attributes, divide by the maximum
void A_nor(){
    for(int i=0; i<8; i++){
        double max=0;
        for(int j=0; j<datanum; j++){
            if(DP[j].data[0][i]>max)
                max=DP[j].data[0][i];
            if(DP[j].data[1][i]>max)
                max=DP[j].data[1][i];
        }
        for(int j=0; j<datanum; j++){
            DP[j].data[0][i]=DP[j].data[0][i]/max;
            DP[j].data[1][i]=DP[j].data[1][i]/max;
        }
    }
}

//the distance between two data, euclidean distance, index=the ?th data
double dist(int index){
    double sum=0;
    for(int i=0; i<8; i++)
        sum+=pow(w[i]*(DP[index].data[0][i]-DP[index].data[1][i]),2);
    sum=sqrt(sum);
    return sum;
}

//+/- weight, i=the ?th attribute, j=the ?th data, b=+/-
void tuneW(int i, bool b){
    double sum;
    if(b==1)
        w[i]*=1.05;
    else
        w[i]*=0.95;
    nor();
    for(int j=0; j<datanum; j++)
        sum+=dist(j);
    cout<<"w"<<i<<": "<<w[i]<<"/"<<sum<<endl;
}

//similarity
double diff(int index){
    double dif=0;
    //for(int j=0; j<datanum; j++)
    dif=pow((DP[index].pair[3]-dist(index)),2);
    return dif;
}
//sum of similarity
double diff(){
    double dif=0;
    for(int j=0; j<datanum; j++)
        dif=pow((DP[j].pair[3]-dist(j)),2);
    return dif;
}

//current distance<=aim, return 1
bool curr_dist(){
    bool check=0;
    for(int i=0; i<datanum; i++){
        if(dist(i)<=DP[i].pair[3])
            check=1;
        else{
            check=0;
            break;
        }
    }
    return check;
}

int main(){
    readin();
    for(int i=0; i<8; i++){
        w[i]=0.00001*(rand()%100000+1);
    }
    A_nor();
    nor();
    double dif=diff();
    while(abs(dif)>aim){
        double curr_dif=0;
        for(int i=0; i<8; i++){
            double past_dif=diff();
            //current distance<aim, so + distance
            if(abs(past_dif)>aim){
                //ith weight increase
                tuneW(i,1);
                //calculate distance
                curr_dif=diff();
                //if +weight and diffdrence decreases (difference reduce, close to aim), keep increasing weight
                if((past_dif-curr_dif)>0){
                    //0<w<1, difference keep decreasing
                    while(w[i]<1 && w[i]>0 && past_dif>curr_dif && abs(diff())>aim){
                        past_dif=diff();
                        tuneW(i,1);
                        if(abs(curr_dif-diff())<aim)
                            break;
                        else
                            curr_dif=diff();
                    }
                    if(past_dif<curr_dif){
                        //weight exceeds, cut it down
                        tuneW(i,0);
                        continue;
                    }
                }
                //if +weight and difference increases (more far from aim), minus weight
                else{
                    past_dif=curr_dif;
                    tuneW(i,0);
                    curr_dif=diff();
                    while(w[i]<1 && w[i]>0 && past_dif>curr_dif && abs(diff())>aim){
                        past_dif=diff();
                        tuneW(i,0);
                        if(abs(curr_dif-diff())<aim)
                            break;
                        else
                            curr_dif=diff();
                    }
                    if(past_dif<curr_dif){
                        //weight exceeds, cut it down
                        tuneW(i,1);
                        continue;
                    }
                }
            }
        }
        dif=diff();
    }
    cout<<"diff: "<<diff()<<endl;
    for(int i=0; i<8; i++)
        cout<<"w"<<i<<": "<<w[i]<<endl;
    return 0;
}

