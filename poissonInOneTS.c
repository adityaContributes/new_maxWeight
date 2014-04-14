#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void main()
{
	double lambda = 1;
	double ts = 1;
	int c = 0;
	int packets;
	int totalPackets=0;
	while (c < 10)
	{
		packets = poissonInOneTS(lambda,&ts);
		printf("c:%d\t packets:%d\n",c,packets);
		totalPackets = totalPackets + packets;
		c++;

	}
	printf("TotalPackets:%d\n",totalPackets);
}

int poissonInOneTS(double lambda, double *timeSlot)
{
	double currTime = 0;
	double nxtTime = 0;
	double totalTime = 0;
	double remainTime = *timeSlot;
	int counter;
	double term1 = 0;
	int r;
	if (*timeSlot == 1)
	{
		counter = 0;
	}
	else
	{
		counter = 1;
	}
	if (*timeSlot <= -1)
	{
		*timeSlot = *timeSlot + 1;
		return(0);
	}
	while (totalTime <= *timeSlot)
	{
		r = rand();
		term1 = (double)(r % 10 ) / 10;
		printf("ln(%g) = %g rand:%d \n",term1, log(term1),r);
		nxtTime = -1 * ( log( term1 ))  / lambda;
		remainTime = remainTime - nxtTime;
		if (remainTime >= 0)
		{
			counter++;
		}
		else
		{
			*timeSlot = (1 + *timeSlot)  - (totalTime + nxtTime);
		}
		totalTime = totalTime + nxtTime;
	}
	return(counter);
}
