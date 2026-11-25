#include "checks.h"
#include<stdio.h>

coords clickCheck( int x, int y){//Not a comment by gpt('v'): I have assumed the positions of board for this
    coords a={-1,-1};
    if(x>400 && x<892 && y>300 && y<792){
        a.x=(x-413)/4, a.y=(y-313)/4;
        if(((int)(a.y/5)*5==a.y || (int)(a.y/5)*5+1==a.y) && ((int)(a.x/5)*5==a.x || (int)(a.x/5)*5+1==a.x)){
            a.y/=5;
            a.x/=5;
        }
        else{
            a.y=-1, a.x=-1;
        }
    }
    return a;
}

int checkVictory(int arr[24][24],int player){
    for(int i=1;i<23;i++)
        for(int j=1;j<23;j++)
            if((arr[0][i] && arr[0][i]==arr[23][j]) || (arr[i][0] && arr[i][0]==arr[j][23]))   return player;
    
    return 0;
}

int checkIntersect(coords a1, coords a2, coords b1, coords b2){
    float x=(((float)(a1.x)*a2.y - a1.y*a2.x)*(b1.x - b2.x) - (a1.x - a2.x)*(b1.x*b2.y - b1.y*b2.x))/((a1.x - a2.x)*(b1.y - b2.y) - (a1.y - a2.y)*(b1.x - b2.x));
    if(((x<a1.x && x>a2.x)||(x>a1.x && x<a2.x))&&((x<b1.x && x>b2.x)||(x>b1.x && x<b2.x)))  return 1;
    else return 0;
}

int checkConnectivity(coordLL* coordll[24][24], coords a, coords b){
    coords arr[4]={{a.x,b.y},{b.x,a.y}};
    if(a.x-b.x==1 || a.x-b.x==-1){
        arr[2].x=a.x,arr[2].y=(a.y+b.y)/2;
        arr[3].x=b.x,arr[3].y=(a.y+b.y)/2;
    }
    else{
        arr[2].y=a.y,arr[2].x=(a.x+b.x)/2;
        arr[3].y=b.y,arr[3].x=(a.x+b.x)/2;
    }
    coordLL* traverse;
    for(int i=0;i<4;i++){
        traverse=coordll[arr[i].y][arr[i].x];
        while(traverse!=NULL){
            if(checkIntersect(a,b,arr[i],traverse->pos)) return 0;
            traverse=traverse->next;
        }
    }
    return 1;
}