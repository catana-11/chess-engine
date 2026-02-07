// bishop moves generator

#include <iostream>
using namespace std;


int main(){

    int arr[50] = {0};
    int j = 0;

    // moves ahead formula: +-8 +- 1(i)
    for(int i=0; i<7;i++){
        arr[j] = 8+1*i;
        j+=1;
    }

    for(int i=0; i<7;i++){
        arr[j] = 8-1*i;
        j+=1;
    }

    for(int i=0; i<7;i++){
        arr[j] = -8+1*i;
        j+=1;
    }

    for(int i=0; i<7;i++){
        arr[j] = -8-1*i;
        j+=1;
    }

    for (int i = 0; i<50; i++){
        cout<<arr[i]<<endl;
    }



    return 0;
}