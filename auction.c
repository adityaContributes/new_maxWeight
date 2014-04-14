#include <stdio.h>
#include <string.h>
#include "maxWeight.h"
void init()
{
	int i,j;
	for (i = 0; i < numPort; i++)
	{
		price[i] = 0;
		for (j = 0; j < numPort; j++)
		{
			toDequeue[i][j] = 0;
			bid[i][j] = 0;
		}
		I[i] = 1;	
		
	}
}

void wMinusPrice()
{
	int i,j;
	for (i =0; i < numPort; i++)
	{
		if ( 1 == I[i] )
		{
			for (j =0; j < numPort; j++)
			{
				wMP[i][j] = w[i][j] - price[j];	
				//printf("wMP[%d][%d] = %d \t",i,j,wMP[i][j]);	
			}
		}
		//printf("\n");
	}
}

void findJi()
{
	int i,j;
	int max;
	int maxJ = 0;
	for (i = 0; i < numPort; i++)
	{
		max = -99999;
		if (1 == I[i])
		{
			for (j = 0; j < numPort; j++)
			{
				if (max < wMP[i][j])
				{
					maxJ = j;
					max = wMP[i][j];
				}
			}
			Ji[i] = maxJ;
			//printf("Ji[%d] = %d\n",i,Ji[i]);
		}
		//printf("Ji[%d] = %d\n",i,Ji[i]);
	}
}

void findUi()
{
	int i,j;
	int max;
	for (i = 0; i < numPort; i++)
	{
		max = -99999;
		if ( 1 == I[i])
		{
			for (j = 0; j < numPort; j++)
			{
				if ( (wMP[i][j] > max) && (j != Ji[i]) )
				{
					Ui[i] = wMP[i][j];	
					max = wMP[i][j];
				}
			}
			//printf("Ui[%d] = %d \n",i,Ui[i]);
		}
	}
}
void findBid()
{
	int i,j;
	for ( i = 0; i < numPort; i++)
	{
		if ( 1 == I[i])
		{
			for (j = 0; j < numPort; j++)
			{
				if ( j == Ji[i] )
				{
					bid[i][j] = w[i][j] - Ui[i] + 1;
					P[j][i] = 1;
					//printf("bid[%d][%d] = %d \n", i,j,bid[i][j]);
				}
				else
				{
					bid[i][j] = 0;
					P[j][i] = 0;
				}
			}
		}
	}
	
}
void findPrice()
{
	int i,j;
	int maxBid;
	int assignPrice;
	for (j = 0; j < numPort; j++)
	{
		assignPrice = 0;
		maxBid = -99999;
		for (i = 0; i < numPort; i++)
		{
			if ( (1 == P[j][i]) && (maxBid < bid[i][j]) )
			{
				maxBid = bid[i][j];
				assignPrice = 1;	
			}
		}
		if (1 == assignPrice)
		{
			price[j] = maxBid;
		}
		//printf("Price[%d] = %d\n", j, price[j]);
	}
}
void step4()
{
	int j,i;
	int maxBidder = -1;
	int maxBid = -1;
	for (j = 0; j < numPort; j++)
	{
		//find first bidder
		for (i = 0; i < numPort; i++)
		{
			if (1 == P[j][i])
			{
				maxBidder = i;
				maxBid = bid[i][Ji[i]];
				break;
			}
		}
		if (-1 == maxBidder )
			continue;
		//find Max Bidder
		for ( i = 0; i < numPort; i++)
        	{   
               		if ( (1 == I[i]) && (maxBid < bid[i][Ji[i]] ) ) 
                	{   
                       		maxBidder = i;
                        	maxBid = bid[i][Ji[i]];
                	}   
        	}   
        	//printf("maxBidder:%d maxBid:%d Ji[maxBidder]:%d\n",maxBidder,maxBid,Ji[maxBidder]);
	
		//Remove max bidder from I and Add that pair to toDequeue
	        I[maxBidder] = 0;
       		toDequeue[maxBidder][Ji[maxBidder]] = 1;

        	for (i = 0; i < numPort; i++)
        	{
               	        if ( (i != maxBidder) && (1 == toDequeue[i][Ji[maxBidder]]) )
                        {
                                toDequeue[i][Ji[maxBidder]] = 0;
                                I[i] = 1;
                        }
        	}

	
	}
}

int sComplete()
{
	int i,j;
	int c = 0;
	for (i = 0; i < numPort; i ++)
	{
		for ( j = 0; j < numPort; j++)
		{
			if (1 == toDequeue[i][j] )
				c++;
		}
	}
	return(c);
}
void printIS()
{
	int i,j;
	for ( i = 0; i < numPort; i++)
	{
		for (j = 0; j < numPort; j++)
		{
			fprintf(log_fp,"toDequeue[%d][%d] = %d\t",i,j,toDequeue[i][j]);
		}
		fprintf(log_fp,"\n");
	}
}
void maxWeight()
{
	int numFilled = 0;
	init();
	int i =0;
	while ( numFilled != numPort)
	{
		wMinusPrice();	
		findJi();
		findUi();
		findBid();
		findPrice();
		step4();
		numFilled = sComplete();
		i++;
	}
	printIS();
}	
