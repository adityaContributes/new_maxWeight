#define LOADSTEP 0.05
#define numPort 3 
#include <stdio.h>
char dbgMsg[1024];
int midPoint;
typedef struct packet
{
	int pkt_num;
	int dest;	//Destination port
	int timeEnter;	//time slot at which it entered the input queue 
	int timeLeft;	//time slot at which it left the queue
}packet;
double load[numPort];//The load on the switch
double p[numPort][numPort]; //probability to the destination of packet
double probSlab[numPort][numPort]; //Used in function to find destination port
int simTime;
void parseConfig();
int type1;
int type2;
int I[numPort];
int price[numPort];
int wMP[numPort][numPort];
int Ji[numPort];
int Ui[numPort];
int P[numPort][numPort];
int bid[numPort][numPort];
int w[numPort][numPort];
int toDequeue[numPort][numPort]; //an element will be 1,if packet from port corresponding to index of this element is to be served
FILE *log_fp,*wt_fp,*wtN_fp;
