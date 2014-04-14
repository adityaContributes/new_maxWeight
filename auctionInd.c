#include <stdio.h>
#include <string.h>
#define numPort 4
//int w[numPort][numPort] = {5,5,5,5,5,5,5,5,5};
//int w[numPort][numPort] = {0,0,0,0,0,0,0,0,0};
int w[numPort][numPort] = {5,9,3,26,8,7,8,2,6,10,12,7,3,10,8,6};
int S[numPort][numPort];
int I[numPort];
int price[numPort];
int wMP[numPort][numPort];
int Ji[numPort];
int Ui[numPort];
int P[numPort][numPort];
int bid[numPort][numPort];
int toDequeue[numPort][numPort];
void init()
{
	int i,j;
	int Iflag = 0;
	for (i = 0; i < numPort; i++)
	{
		//Iflag = 0;
		price[i] = 0;
		for (j = 0; j < numPort; j++)
		{
			S[i][j] = 0;
			bid[i][j] = 0;
			//if (0 != w[i][j])
			//	Iflag = 1;
		}
	//	if (Iflag)
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
				printf("wMP[%d][%d] = %d \t",i,j,wMP[i][j]);	
			}
		}
		printf("\n");
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
		printf("Ji[%d] = %d\n",i,Ji[i]);
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
					printf("bid[%d][%d] = %d \n", i,j,bid[i][j]);
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
        	printf("maxBidder:%d maxBid:%d Ji[maxBidder]:%d\n",maxBidder,maxBid,Ji[maxBidder]);
	
		//Remove max bidder from I and Add that pair to S
	        I[maxBidder] = 0;
       		S[maxBidder][Ji[maxBidder]] = 1;

        	for (i = 0; i < numPort; i++)
        	{
               	        if ( (i != maxBidder) && (1 == S[i][Ji[maxBidder]]) )
                        {
                                S[i][Ji[maxBidder]] = 0;
                                I[i] = 1;
                        }
        	}

	
	}
}
void step44()
{
	int i;
	int maxBidder;
	int maxBid;
	//find first bidder
	i = 0;
	while (i < numPort)
	{
		if ( 1 == I[i])
		{	
			maxBidder = i;
			maxBid = bid[i][Ji[i]];
			break;
		}
		i++;
	}

	//find maxBid amd maxBidder
	for ( i = 0; i < numPort; i++)
	{
		if ( (1 == I[i]) && (maxBid < bid[i][Ji[i]] ) )
		{
			maxBidder = i;
			maxBid = bid[i][Ji[i]];
		}
	}
	printf("maxBidder:%d maxBid:%d Ji[maxBidder]:%d\n",maxBidder,maxBid,Ji[maxBidder]);	

	//Remove max bidder from I and Add that pair to S
	I[maxBidder] = 0;
	S[maxBidder][Ji[maxBidder]] = 1;
	
	
	for (i = 0; i < numPort; i++)
	{
			if ( (i != maxBidder) && (1 == S[i][Ji[maxBidder]]) )
			{
				S[i][Ji[maxBidder]] = 0;
				I[i] = 1;
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
			if (1 == S[i][j] )
				c++;
		}
	}
	return(c);
}
void printIS()
{
	int i,j;
	for (i = 0; i < numPort; i++)
	{
		printf("I[%d] = %d \t",i,I[i]);
	}
	printf("\n");
	for ( i = 0; i < numPort; i++)
	{
		for (j = 0; j < numPort; j++)
		{
			printf("S[%d][%d] = %d\t",i,j,S[i][j]);
		}
		printf("\n");
	}
}
int findExitCond()
{
	int tmp;
	int i,j;
	tmp = numPort;
	for (j = 0; j < numPort; j++)
	{
		i = 0;
		while ( (0 == w[i][j]) && (i < numPort) )
			i++;
		if (i == numPort)
			tmp--;
		
	}
	printf("ExitCond = %d\n",tmp);
	return(tmp);	
}
void main()
{
	int numFilled = 0;
	int exitCond;
	exitCond = findExitCond();
	init();
	int i =0;
	printIS();
	while ( numFilled != numPort)
	{
		wMinusPrice();	
		findJi();
		findUi();
		findBid();
		findPrice();
		step4();
		numFilled = sComplete();
		printf("sComplete = %d\n",numFilled);
		printIS();
		i++;
	}
	printf("sComplete = %d\n",numFilled);
	printIS();
	
}	
