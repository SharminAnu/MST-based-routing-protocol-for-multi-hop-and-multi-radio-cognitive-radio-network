#include<bits/stdc++.h>
using namespace std;

int Prims(int source, int destination,int channelID,int isSwithingAllowd);
void generateGraph(int totalNode,int channel);
void simulateMySystem(void);
void clearAll(void);
struct node{
    vector<int>neighbourList;
    vector<int>channelList[105];
    vector<int>distance;
    int hasConnectionWith[105][12];
}Network[105];

struct node2
{
    int from,to,channel,cost;
    node2(int a,int b,int chn,int c)
    {
        from=a;
        to=b;
        channel =chn;
        cost=c;
    }
    bool operator <(const node2 &p)const {return p.cost<cost;};
};

struct road{
    int from,to,channel;
    road(int a,int b,int c)
    {
        from = a;
        to = b;
        channel = c ;
    }
};
vector<road>MST,finalMST;
set<int>channelsOfSource;
int totalNode,totalCost,finalCost;
int main()
{
    // freopen("input.txt","r",stdin);
    srand(time(NULL));
    simulateMySystem();

    return 0;
}

void simulateMySystem(void)
{
    int i,j,switching,withSwithcing,withoutSwitching,n;
    for(int nodeNumber=10;nodeNumber<=10;nodeNumber++)
    {
        for(int channelNumber=2;channelNumber<=10;channelNumber++)
        {
            withSwithcing=0;
            withoutSwitching=0;
            for(int numberOfSimulation =1; numberOfSimulation<=100;numberOfSimulation++)
            {

                clearAll();
                // printf("Simulation %d\n",numberOfSimulation);
                generateGraph(nodeNumber,channelNumber);
                // printf("Graph gnerated\n");
                n = Network[1].neighbourList[0];
                withSwithcing+=Prims(1,nodeNumber,Network[1].channelList[n][0],1);
                finalMST=MST;

                channelsOfSource.clear();
                for(i=0;i<Network[1].neighbourList.size();i++)
                {
                    n = Network[1].neighbourList[i];
                    for(j=0;j<Network[1].channelList[n].size();j++)
                    {
                        channelsOfSource.insert(Network[1].channelList[n][j]);
                    }
                }
                set<int>::iterator itt;
                // printf("channelsOfSource: %d\n",channelsOfSource.size());
                for(itt=channelsOfSource.begin();itt!=channelsOfSource.end();++itt)
                {
                    int isPossible = Prims(1,nodeNumber,*itt,0);
                    if(isPossible==1)
                    {
                        withoutSwitching++;
                        break;
                    }
                }

            }
            //system("color 0A");
            printf("For %d nodes and %d channel: %d %d\n",nodeNumber,channelNumber,withoutSwitching,withSwithcing);
            system("color f0");
            //Cost
        /*   if(totalCost==9999999)
              printf("No connection possible.\n");
           else
           {
               printf("Total Cost: %d\n",totalCost);
               for(i=0;i<finalMST.size();i++)
              {
                  printf("Node %d %d connected with %d\n",MST[i].from,MST[i].to,MST[i].channel);
               }

           } */

        }
    }
}

int hasRoad[105][105];
void generateGraph(int totalNode,int channel)
{
    int n,i,j,k,neighbourNo,c,d,temp,source,destination;
        memset(hasRoad,0,sizeof(hasRoad));
        for(i=1;i<=totalNode;i++)
        {
            if(totalNode>2)
                neighbourNo = 1+rand()%min(3,totalNode-1);
            else
                neighbourNo = 1;
            set<int>neighbourSet;
            for(j=1;j<=totalNode;j++)
            {
                if(hasRoad[i][j]==1)
                {
                    neighbourNo--;
                    neighbourNo = max(0,neighbourNo);

                }
            }
            // printf("Neighbour Set generation started... %d %d : %d\n",totalNode,channel,neighbourNo);
            while(neighbourSet.size()<neighbourNo)
            {
                n= 1+rand()%totalNode;
                if(hasRoad[i][n]==0&&n!=i)
                {
                    neighbourSet.insert(n);
                    hasRoad[i][n]=hasRoad[n][i]=1;
                }

            }
            // printf("Neighbour Set generation end\n");
            set<int>::iterator itt;
            for(itt=neighbourSet.begin();itt!=neighbourSet.end();++itt)
            {

                d=1+rand()%1007;
                Network[i].neighbourList.push_back(*itt);
                Network[*itt].neighbourList.push_back(i);
                Network[i].distance.push_back(d);
                Network[*itt].distance.push_back(d);
                set<int>channelSet;
                int channelNumber = 1+rand()%channel;
                while(channelSet.size()!=channelNumber)
                {
                    channelSet.insert(1+rand()%10);
                }
                set<int>::iterator itt2;
                for(itt2=channelSet.begin();itt2!=channelSet.end();++itt2)
                {
                    Network[i].channelList[*itt].push_back(*itt2);
                    Network[*itt].channelList[i].push_back(*itt2);
                    Network[i].hasConnectionWith[*itt][*itt2]=1;
                    Network[*itt].hasConnectionWith[i][*itt2]=1;
                }
            }
        }
//Generate Graph
  /*    for(i=1;i<=totalNode;i++)
         {
            for(j=1;j<=totalNode;j++)
             {
                printf("Node %d->%d: ",i,j);
                for(int k=0;k<Network[i].channelList[j].size();k++)
                    printf("%d ",Network[i].channelList[j][k]);
                printf("\n");
                system("color f0");
            }
       } */
}

priority_queue<node2>PQ;
int isConnected[105];
int Prims(int source, int destination,int channelID,int isSwithingAllowed)
{
    int i,j,to,cost,isPossible=0,chnl;
    memset(isConnected,0,sizeof(isConnected));
    MST.clear();
    totalCost=0;
    while(!PQ.empty())
    {
        PQ.pop();
    }
    for(i=0;i<Network[source].neighbourList.size();i++)
    {
        to = Network[source].neighbourList[i];
        cost =Network[source].distance[i];
        chnl = Network[source].channelList[to][0];
        if(isSwithingAllowed==1)
        {
           PQ.push(node2(source,to,chnl,cost));
        }
        else if(Network[source].hasConnectionWith[to][channelID]==1)
        {
            PQ.push(node2(source,to,channelID,cost));
        }
    }
    isConnected[source]=1;
    while(!PQ.empty())
    {
        node2 temp = PQ.top();
        PQ.pop();
        if(isConnected[temp.to]==0)
        {

            MST.push_back(road(temp.from,temp.to,temp.channel));
            if(temp.to==destination)
                isPossible =1;
            isConnected[temp.to]=1;
            totalCost+=temp.cost;
            for(i=0;i<Network[temp.to].neighbourList.size();i++)
            {
                to = Network[temp.to].neighbourList[i];
                cost =Network[temp.to].distance[i];
                chnl = Network[temp.to].channelList[to][0];
                if(isSwithingAllowed==1)
                {
                    PQ.push(node2(temp.to,to,chnl,cost));
                }
                else if(Network[temp.to].hasConnectionWith[to][channelID]==1)
                {
                    PQ.push(node2(temp.to,to,channelID,cost));
                }
            }
        }
    }
    if(isPossible==0)
        totalCost = 9999999;
    return isPossible;
}

void clearAll(void)
{
    for(int i=1;i<=101;i++)
    {
        Network[i].neighbourList.clear();
        Network[i].distance.clear();
        memset(Network[i].hasConnectionWith,0,sizeof(Network[i].hasConnectionWith));
        for(int j=0;j<104;j++)
        {
            Network[i].channelList[j].clear();
        }

    }
}
