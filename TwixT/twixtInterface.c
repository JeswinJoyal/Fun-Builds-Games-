#include "twixtInterface.h"

void insertToCoordll(coordLL* coordll[24][24],coords a, coords b){
    coordLL* alist=malloc(sizeof(coordLL));
    alist->pos.x=b.x;
    alist->pos.y=b.y;
    alist->next=NULL;
    coordLL* blist=malloc(sizeof(coordLL));
    blist->pos.x=a.x;
    blist->pos.y=a.y;
    blist->next=NULL;

    coordLL* traverse=coordll[a.y][a.x];
    if(traverse==NULL)  coordll[a.y][a.x]=alist;
    else{
        while(traverse->next!=NULL)traverse=traverse->next;
        traverse->next=alist;
    }

    traverse=coordll[b.y][b.x];
    if(traverse==NULL)  coordll[b.y][b.x]=blist;
    else{
        while(traverse->next!=NULL)traverse=traverse->next;
        traverse->next=blist;
    }
    return;
}

void updateGroup(int arr[24][24], coordLL* coordll[24][24], coords node, int groupId){
    if(arr[node.y][node.x]==groupId)    return;
    arr[node.y][node.x]=groupId;
    coordLL* traverse=coordll[node.y][node.x];
    while(traverse!=NULL){
        updateGroup(arr,coordll,traverse->pos,groupId);
        traverse=traverse->next;
    }
}

void updateLine(SDL_Renderer *renderer, coords ind, int arr[24][24], coordLL* coordll[24][24]){
    int player = arr[ind.y][ind.x];
    int temp[] = {-1,0,1,2,1,0,-1,-2,-1};
    coords a = {ind.x-2, ind.y-1};
    for(int k=0; k<8;a.y+=temp[k], a.x+=temp[k+2], k++){
        if(a.y>=0 && a.y<24 && a.x>=0 && a.x<24 &&((arr[a.y][a.x]>1 && player>1) || (arr[a.y][a.x]<-1 && player<-1)) && checkConnectivity(coordll,ind,a)){
            insertToCoordll(coordll,ind,a);
            updateGroup(arr,coordll,a,arr[ind.y][ind.x]);
        }
    }

    return;
}

int update(SDL_Renderer* renderer, int x, int y, int arr[24][24], coordLL* coordll[24][24], playerData* playerdata){
    coords ind = clickCheck(x,y);
    if(ind.y==-1 ||  arr[ind.y][ind.x]!=0)   return 0;

    if(ind.y>0 && ind.y<23 && ind.x>0 && ind.x<23){
        if(playerdata->player==1)   arr[ind.y][ind.x] = playerdata->p1group++;
        else  arr[ind.y][ind.x] = playerdata->p2group--;
    }
    else if(ind.x==0 || ind.x==23){
        if(playerdata->player==-1) arr[ind.y][ind.x] = playerdata->p2group--;
        else return 0;
    }
    else {
        if(playerdata->player==1)  arr[ind.y][ind.x] = playerdata->p1group++; 
        else return 0;
    }
    updateLine(renderer, ind, arr, coordll);
    playerdata->wonPlayer=checkVictory(arr,playerdata->player);

    return 1;

}

void restartGame(int arr[24][24], coordLL* coordll[24][24], playerData* playerdata){
    for(int i=0;i<24;i++){
        for(int j=0;j<24;j++){
            arr[i][j]=0;
            coordLL *traverse=coordll[i][j],*temp;
            while(traverse!=NULL){
                temp=traverse;
                traverse=traverse->next;
                free(temp);
            }
            coordll[i][j]=NULL;
        }
    }
    arr[0][0]=1,arr[0][23]=1,arr[23][0]=1,arr[23][23]=1;
    playerdata->p1group=2;
    playerdata->p2group=-2;
    playerdata->wonPlayer=0;
    playerdata->player=1;

    return;
}
