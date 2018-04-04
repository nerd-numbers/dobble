//
//  main.cpp
//  dobble
//
//  Created by Agapkin Anton on 02.04.2018.
//  Copyright © 2018 Agapkin Anton. All rights reserved.
//

#include <iostream>
using namespace std;

#include <random>

#define k 7
#define v ( k*(k - 1) + 1 ) //as example

unsigned long int m[v];


#define SEVEN_BIT_VECTOR_COUNT 32224114 //C_v^k bit len < 32
unsigned long int sixbitvectors[SEVEN_BIT_VECTOR_COUNT];

random_device rdev;
mt19937 e(rdev());
uniform_int_distribution<int> getRand(0,SEVEN_BIT_VECTOR_COUNT-1);

void createSixbitvectors(){
    int positions[7] = {0,1,2,3,4,5,6};
    unsigned count = 0;
    uint64_t temp;
    
    int currentPosition = 7;
    
    do{
        if(currentPosition == 7){
            temp = 0;
            for (int i = 0; i != 7; i++)
                temp ^= ((uint64_t)1 << positions[i]);
            sixbitvectors[count] = temp;
            count++;
            currentPosition = 6;
        }
        
        positions[currentPosition]++;
        if(positions[currentPosition] == (v-(6-currentPosition))){
            currentPosition--;
        }
        else{
            for (currentPosition = currentPosition+1; currentPosition <= 6; currentPosition++){
                positions[currentPosition] = positions[currentPosition-1]+1;
            }
        }
        
    }while(count != SEVEN_BIT_VECTOR_COUNT);
}

void printm(){
    
    for (int i = 0; i != v; ++i){
        for(int j = 0; j != v; ++j){
            cout << ((m[i] >> j) & 0x1) ;
        }
        cout << endl;
    }
    
}

void chooseNext(int strNum){
    cout << "\t\rstrNum = " << strNum << flush;
    if (strNum == v){
        printm();
        return;
    }
    
    if (strNum >= 25){
        printm();
    }
    
    uint64_t temp;
    int i,j;
    
    for (j = 0; j != SEVEN_BIT_VECTOR_COUNT; ++j){
        
        for (i = 0; i != strNum;i++){
            temp = sixbitvectors[j];
            //temp = (strNum > 8) ? sixbitvectors[getRand(e)] : sixbitvectors[j];
            temp = (temp ^ m[i]);
            temp = __builtin_popcount((unsigned int)temp) + __builtin_popcount((unsigned int)(temp >> 32));
            if(temp != 12) break;
        }
        if (i == strNum){
            m[strNum] = sixbitvectors[j];
            chooseNext(strNum+1);
        }
    }
    
}

int main(int argc, const char * argv[]) {
    
    createSixbitvectors();
    
    m[0] = (1 << 7) - 1;
    m[1] = m[0] << 6;
    
    chooseNext(2);
    
    return 0;
}
