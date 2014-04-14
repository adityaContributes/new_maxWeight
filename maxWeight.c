#ifdef DEBUG
#	define dbg(x) printf("%s",x);
#else
#	define dbg(x) {} //NO DEBUG
#endif
#include <stdlib.h>
#include "my402list.h"
#include "maxWeight.h"
#include <stdio.h>
#include <error.h>
#include <string.h>
int main (int argc, char * argv[])
{
	int timeSlot; 
	int packet_c; 
	int finPacket_c; 
	int i,j;
	int dest,pcktArrived;	//destination port of packet arrived
	int pc[numPort][numPort];	//Packet count in particular queues
	int ld;
	double ldM;
	int tmpC;
	parseConfig();
	initProbSlab();
	printProbSlab();
	My402ListElem *f; //Used for dequeue
	packet *srvdPckt; //Used for dequeue
	memset(&toDequeue,0,sizeof(toDequeue));
	midPoint = RAND_MAX / 2;
	My402List Q[numPort * numPort];
	memset(&Q,0,sizeof(Q));
	
	if ( (wt_fp = fopen("wt.dat","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}	
	if ( (wtN_fp = fopen("wtNorm.dat","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}
	if ( (log_fp = fopen("Simulation.log","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}
	for (i = 0; i < numPort; i++)
	{
		for (j = 0; j < numPort; j++)
		{
			fprintf(wt_fp,"W[%d][%d]\t",i,j);
			fprintf(wtN_fp,"W[%d][%d]\t",i,j);
		}
	}
	fprintf(wt_fp,"\n");
	fprintf(wtN_fp,"\n");
	srand (time(NULL));
	
	InitQueues(&Q);		//Initialise input queues
	packet_c = 0; 
	finPacket_c = 0; 
	for (timeSlot = 1; timeSlot < simTime; timeSlot++)
	{
	//Generate packet
	for (i = 0; i < numPort; i++)	//For all input ports
	{
		dest = -1;
		pcktArrived = -1;
		ld = (int)load[i];
		ldM = load[i] - ld;
		tmpC = 0;
		while (tmpC < ld)
		{
			if (1)	//Packet arrived
			{
				packet_c++;
				dest = destPort(i);	//Get the destination port
				packet * pkt = (packet *) malloc(sizeof(packet));
				if (NULL == pkt)
				{
					fprintf(stderr,"Error in malloc\n");
					perror("Malloc::");
					exit(1);
				}
				pkt->timeEnter = timeSlot;
				pkt->timeLeft = -1;
				pkt->dest = dest;
				pkt->pkt_num = packet_c;
				if (!(My402ListAppend(&Q[i * numPort + dest],(void *)pkt)))
				{
					fprintf(stderr,"Error Appending packet%d to Queue%d%d\n",packet_c,i,dest);
					exit(1);
				}
				pc[i][pkt->dest]++;	
				fprintf(log_fp,"TimeSlot:%d\tp%d\ti/p Port:%d\to/p Port:%d\n",timeSlot,packet_c,i,dest);
			}
		tmpC++;
		}
		dest = -1;
		pcktArrived = -1;
		if (1 == (pcktArrived = packetArrives(ldM)) )	//Packet arrived
		{
			packet_c++;
			//printf("Input: %d\n",i);
			dest = destPort(i);	//Get the destination port
			packet * pkt = (packet *) malloc(sizeof(packet));
			pkt->timeEnter = timeSlot;
			pkt->timeLeft = -1;
			pkt->dest = dest;
			pkt->pkt_num = packet_c;
			if (!(My402ListAppend(&Q[i * numPort + dest],(void *)pkt)))
			{
				fprintf(stderr,"Error Appending packet%d to Queue%d%d\n",packet_c,i,dest);
				exit(1);
			}
			pc[i][pkt->dest]++;	
			//printf("Packet Arrived Destined to %d\n",dest);
			fprintf(log_fp,"TimeSlot:%d\tp%d\ti/p Port:%d\to/p Port:%d\n",timeSlot,packet_c,i,dest);
		}
		
	}
	getWeight(&Q);
	printWeights(wt_fp,wtN_fp,timeSlot);
	maxWeight();	//Do the biparted graph matching

	//Service a packet based on maxWeight. Use the toDequeue array.
	//This assumes that toDequeue array will have atleast one queue marked as 1 per input port
	for (i = 0; i < numPort; i++)
	{
		for ( j = 0; j < numPort; j++)
		{
			if ( (w[i][j] > 0) && toDequeue[i][j] ) //i.e. if Q[i][j] is not empty
			{
				f = My402ListFirst(&Q[i * numPort + j]);
				if (f != NULL)
				{
					srvdPckt = (packet *) f->obj;
					My402ListUnlink(&Q[i * numPort + j],f);
					srvdPckt->timeLeft = timeSlot;
					finPacket_c++;
					fprintf(log_fp,"TimeSlot:%d\tp%d\tfrom port:%d\tto port:%d\tserviced\n",timeSlot,srvdPckt->pkt_num,i,j);
					free(srvdPckt);
				}
				else
				{
					fprintf(stderr,"Logical flaw. Even if weight is greater than zero the Q[%d][%d] is empty\n",i,j);
					exit(1);
				}
			}
		}
	}
	getWeight(&Q);
	printWeights(wt_fp,wtN_fp,timeSlot);
	fprintf(log_fp,"TimeSlot:%d ####Ends####\n",timeSlot);
	}
	return(0);
}
