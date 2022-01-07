#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include "bitcoin.h"


int main(int argc, char **argv)
{


	char *filenametr=NULL;
    char *filename=NULL;
    char *bitcoinvalue=NULL;
    char *sendhash=NULL;
    char *rechash=NULL;
    char *bucksize=NULL;
    int option_index = 0;
    while (( option_index = getopt(argc, argv, "a:t:v:s:r:b:")) != -1){ //use of getopt in order to take the needed arguments
        switch (option_index) {
            case 'a':
                filename = optarg;
                break;
            case 't':
                filenametr = optarg;
                break;
            case 'v':
                bitcoinvalue = optarg;
                break;
            case 's':
                sendhash = optarg;
                break;
            case 'r':
                rechash = optarg;
                break;        
            case 'b':
                bucksize = optarg;
                break;    
            default:

            return 1;
        } //end block for switch
   }  //end block for while
   	int sh,rh,bytes,value;
   	sh=atoi(sendhash);
   	rh=atoi(rechash);
   	bytes=atoi(bucksize);
   	value=atoi(bitcoinvalue);
	bytes=bytes-sizeof(BucketPtr);//calculation of bucketsize
    bytes=bytes/sizeof(struct BucketEl);


	WalletElementPtr AddWallet,ScWalleta,ScWalletb;//needed structs and vvariables for the program
	TreeElementPtr AddTree,ScTree;
	TransacElementPtr AddTrn;
	int max=50;
	int i,j,nocoin,endbal,k;
	const char s[2] = " ";
	char walid[50],sendid[50],recid[50],coinid[20],trans[15],trn[15];
	char day[3],month[3],year[5],hour[3],minute[3];
	int datr,motr,yetr,hotr,mitr,trval;
	int maxd,maxmo,maxy,maxh,maxmi,maxid=0,posmax;
	int day1,day2,month1,month2,year1,year2,hour1,hour2,minute1,minute2;
	int csaob;
	int unspent;

	struct WalHashEl whar[max];
	for(i=0;i<max;i++)//initialisation of arrays ellements
	{
		whar[i].firstel=NULL;
	}
	
	struct CoinHashEl thar[max];
	for(i=0;i<max;i++)//initialisation of arrays ellements
	{
		thar[i].firstel=NULL;
	}

	struct TranHashEl trnhar[max];
	for(i=0;i<max;i++)//initialisation of arrays ellements
	{
		trnhar[i].firstel=NULL;
	}

	struct SRHashEl sendhar[sh];
	for(i=0;i<sh;i++)//initialisation of arrays ellements
	{
		sendhar[i].firstel=NULL;
	}

	struct SRHashEl rechar[rh];
	for(i=0;i<rh;i++)//initialisation of arrays ellements
	{
		rechar[i].firstel=NULL;
	}


   FILE *file = fopen ( filename, "r" );//atempt to open the file that holds the bit coin balance
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {

				char *token;
				nocoin=0;
   
   				/* get the first token */
   				token = strtok(line, s);//use of strtok to "brake" a line at words (using the ' ' char)
				strcpy(walid,token);
				j=0;
				while(walid[j]!='\0') //the following loop is used in many ocasions in the programm
    			{						//it basically checks if a string has the '\n' char in it and replaces it with the '/0'
        			if(walid[j]=='\n')   //at this point of the programm it is used for getting a walletid , that comes with no coins
        			{
            			walid[j]=' ';
						nocoin=1;
        			}
        			j++;
    			}
				if (nocoin==1) {
					walid[j-1]='\0';
				}
				AddWallet = addwalletel(walid,whar,max); 
   				/* walk through other tokens */
				if (AddWallet!=NULL) 
				{
				
					while( token != NULL ) 
					{

						endbal=0;
    
      					token = strtok(NULL, s);
						if (token != NULL) //same loop as before, when a user has coins,the last one in the line, has the '/n' in it, so we get rid of it
						{
							strcpy(coinid,token);
							j=0;
							while(coinid[j]!='\0')
    						{
      							if(coinid[j]=='\n')
      							{
          						coinid[j]=' ';
											endbal=1;
        						}
       		 					j++;
    						}
								if (endbal==1) {
									coinid[j-1]='\0';
								}
								//addtreeel
								AddTree = addtreeel(coinid,thar,AddWallet,value,max);
								//addcoinel
								if (AddTree != NULL) {
									addcoin(AddWallet,AddTree->fleaf);
								}
								else
								{
									printf("coin allready in the system \n");
									return 1;	
								}						
						}
   					}
				}
      }
      fclose ( file );
   }
   else//error if something wrong occures when oppening the file
   {
      perror ( filename ); /* why didn't the file open? */
   }


   FILE *filetr = fopen ( filenametr, "r" );//oppening the second file with the transacions
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, filetr ) != NULL ) /* read a line */
      {
		  			csaob=1;
		  			k=0;
					char *token;
					token = strtok(line, s);
					

					while( token != NULL ) 
					{
  

						if (k==0) {   //here the programm gets the needed information for the transactions, the first string is the transaction id
							strcpy(trans,token);

						}
						
						k++;
      					token = strtok(NULL, s);

						if (k==1) {           //the second is the sender's id
							strcpy(trn,trans);
							strcpy(sendid,token);
						}
						if (k==2) {           //the third id the receiver's one
							strcpy(recid,token);
						}
						if (k==3) {
							trval = atoi(token);
						}
						if (k==4) {          // then we take the chars from each string tha reprecent the day,month an year of the transaction and thay are cnverted to ints
							strncpy ( day, token, 2 );
							day[3] = '\0';
							strncpy ( month, token+3, 2 );
							month[3] = '\0';
							strncpy ( year, token+6, 4 );
							year[5] = '\0';
							datr = atoi(day);
							motr = atoi(month);
							yetr = atoi(year);
						}
						if (k==5) {          //same with the hour an the minutes
							strncpy ( hour, token, 2 );
							hour[3] = '\0';
							strncpy ( minute, token+3, 2 );
							minute[3] = '\0';
							hotr = atoi(hour);
							mitr = atoi(minute);
						}
						
   					}
					ScWalleta=searchwal(recid,whar,max);
					if (ScWalleta != NULL) { //if the receiver's id!=Null
						ScWalletb=searchwal(sendid,whar,max);
						if (ScWalletb != NULL) { //also the sender's
							if (ScWalletb->balance >= trval ) { //if the sender's balance> the transaction value
								if (strcmp(ScWalletb->walletid,ScWalleta->walletid)!=0) { //and if the sender and receiver are not the same user
									AddTrn=addtran(trn,sendid,recid,trnhar,max,trval,datr,motr,yetr,hotr,mitr); //the transaction is executed
									ScWalletb->balance=ScWalletb->balance-trval; //update of users balance
									ScWalleta->balance=ScWalleta->balance+trval;
									addtrnlist(sh,sendhar,ScWalletb->walletid,bytes,AddTrn); //update of hash tables' lists
									addtrnlist(rh,rechar,ScWalleta->walletid,bytes,AddTrn);
									maketrans(AddTrn,ScWalletb,ScWalleta,trval);
									maxd=datr; //update of maximum day,month,year,hour and minute (as the transactions are given in chronoligical order the latest is allways the last in the file
									maxmo=motr;
									maxy=yetr;
									maxh=hotr;
									maxmi=mitr;
									posmax=atoi(trn);
									if (posmax>maxid) { //update of the greatest transaction id (all the maximums are used in requestTransactio)
										maxid=posmax;
									}
									csaob=0;
									printf("transaction executed \n");
								}
							}
						}
					}
					if (csaob==1)
					{
						printf("transaction not executed \n");
					}
					
      }
      fclose ( filetr );
   }
   else
   {
      perror ( filenametr ); /* why didn't the file open? */
   }

	
	
	
	char *token;
	int cont=1;
	char csh[150];


	do {   //instructions from user are given here
		
		fgets(csh, sizeof(csh), stdin);
		token = strtok(csh, s);//strtok 


		j=0;
		nocoin=0;
		while(csh[j]!='\0')//and this loop in the same use as before
    			{
        			if(csh[j]=='\n')
        			{
            			csh[j]=' ';
						nocoin=1;
        			}
        			j++;
    			}
				if (nocoin==1) {
					csh[j-1]='\0';
				}
		
		
		if (strcmp(token,"requestTransaction")==0) 
		{
			maxid=maxid+1;
			csaob=1;
			k=0;
			while( token != NULL ) {
      			token = strtok(NULL, s);
				k++;
				if (token!=NULL) {
					if (k==1) {
							strcpy(sendid,token); //same as before, except from the transaction id
						}
						if (k==2) {
							strcpy(recid,token);
						}
						if (k==3) {
							trval = atoi(token);
						}
						if (k==4) {
							strncpy ( day, token, 2 );
							day[3] = '\0';
							strncpy ( month, token+3, 2 );
							month[3] = '\0';
							strncpy ( year, token+6, 4 );
							year[5] = '\0';
							datr = atoi(day);
							motr = atoi(month);
							yetr = atoi(year);
						}
						if (k==5) {
							strncpy ( hour, token, 2 );
							hour[3] = '\0';
							strncpy ( minute, token+3, 2 );
							minute[3] = '\0';
							hotr = atoi(hour);
							mitr = atoi(minute);
						}
				}
   			}
			if (k==4) { //if date and time values not given, the transaction takes the date and time of the execution
				time_t now;
				time(&now);	

				// localtime converts a time_t value to calendar time and returns a pointer
				// to a tm structure with its members filled with the corresponding values
				struct tm *local = localtime(&now);

				hotr = local->tm_hour;			// get hours since midnight	(0-23)
				mitr = local->tm_min;		// get minutes passed after the hour (0-59)

				datr = local->tm_mday;			// get day of month (1 to 31)
				motr = local->tm_mon + 1;		// get month of year (0 to 11)
				yetr = local->tm_year + 1900;	// get year since 1900
			}
			
			snprintf(trn, 10, "%d", maxid);//transaction takes its id
			int chtd=0;
			if (yetr>maxy) { // if date and time are greater than the transactions from the second file
				chtd=1;
			}
			else if (yetr==maxy) {
				if (motr>maxmo) {
					chtd=1;
				}
				else if (motr==maxmo) {
					if (datr>maxd) {
						chtd=1;
					}
					else if (datr==maxd) {
						if (hotr>maxh) {
							chtd=1;
						}
						else if (hotr==maxh) {
							if (mitr>=maxmi) {
								chtd=1;
							}
						}
					}
				}
			}

			if (chtd==1) 
			{
				ScWalleta=searchwal(recid,whar,max);
					if (ScWalleta != NULL) {
						ScWalletb=searchwal(sendid,whar,max);
						if (ScWalletb != NULL) {
							if (ScWalletb->balance >= trval ) {
								if (strcmp(ScWalletb->walletid,ScWalleta->walletid)!=0) {	//and all the other criteria as before 
									AddTrn=addtran(trn,sendid,recid,trnhar,max,trval,datr,motr,yetr,hotr,mitr); //the transaction is made
									ScWalletb->balance=ScWalletb->balance-trval;
									ScWalleta->balance=ScWalleta->balance+trval;
									addtrnlist(sh,sendhar,ScWalletb->walletid,bytes,AddTrn);
									addtrnlist(rh,rechar,ScWalleta->walletid,bytes,AddTrn);
									maketrans(AddTrn,ScWalletb,ScWalleta,trval);
									maxd=datr;
									maxmo=motr;
									maxy=yetr;
									maxh=hotr;
									maxmi=mitr;
									csaob=0;
									printf("transaction executed \n");
								}
							}
						}
					}
			}
			if (csaob==1)
			{
				printf("transaction not executed \n");
			}


        }
		else if (strcmp(csh,"requestTransactions")==0) //same commands as before,except now the transactions are read from a file and they have allready date and time
		{
			
			int mtr=0;
			char filenament[50];
			while( token != NULL ) {
    
      			token = strtok(NULL, s);
				if (token!=NULL) {


					j=0;
					nocoin=0;
					while(token[j]!='\0')
    				{
        				if(token[j]=='\n')
        				{
            				token[j]=' ';
							nocoin=1;
        				}
        				j++;
    				}
					if (nocoin==1) {
						token[j-1]='\0';
					}
					strcpy(filenament,token);
				}
   			}
   			FILE *filent = fopen ( filenament, "r" );
   			if ( filent != NULL )
   			{
     		char line [ 128 ]; /* or other suitable maximum line size */
      		while ( fgets ( line, sizeof line, filent ) != NULL ) /* read a line */
      		{
				  	csaob=1;
		  			k=0;
					char *token;
					token = strtok(line, s);
					
					strcpy(sendid,token);
					while( token != NULL ) 
					{
						
						k++;
      					token = strtok(NULL, s);

						if (k==1) {
							strcpy(recid,token);
						}
						if (k==2) {
							trval = atoi(token);
						}
						if (k==3) {
							strncpy ( day, token, 2 );
							day[3] = '\0';
							strncpy ( month, token+3, 2 );
							month[3] = '\0';
							strncpy ( year, token+6, 4 );
							year[5] = '\0';
							datr = atoi(day);
							motr = atoi(month);
							yetr = atoi(year);
						}
						if (k==4) {
							const char cht = ';';// importand difference: all the request must have the ';' char at their end
							char *ret;
   							ret = strchr(token, cht);
							if (ret!=NULL) {
								mtr=1;
							}
							strncpy ( hour, token, 2 );
							hour[3] = '\0';
							strncpy ( minute, token+3, 2 );
							minute[3] = '\0';
							hotr = atoi(hour);
							mitr = atoi(minute);
						}
   					}
					if (mtr==1) {
						ScWalleta=searchwal(recid,whar,max);
						if (ScWalleta != NULL) {
							ScWalletb=searchwal(sendid,whar,max);
							if (ScWalletb != NULL) {
								if (ScWalletb->balance >= trval ) {
									if (strcmp(ScWalletb->walletid,ScWalleta->walletid)!=0) {	
										maxid=maxid+1;
										snprintf(trn, 10, "%d", maxid);
										AddTrn=addtran(trn,sendid,recid,trnhar,max,trval,datr,motr,yetr,hotr,mitr);
										ScWalletb->balance=ScWalletb->balance-trval;
										ScWalleta->balance=ScWalleta->balance+trval;
										addtrnlist(sh,sendhar,ScWalletb->walletid,bytes,AddTrn);
										addtrnlist(rh,rechar,ScWalleta->walletid,bytes,AddTrn);
										maketrans(AddTrn,ScWalletb,ScWalleta,trval);
										maxd=datr;
										maxmo=motr;
										maxy=yetr;
										maxh=hotr;
										maxmi=mitr;
										csaob=0;
										printf("transaction executed \n");
										posmax=atoi(trn);
										if (posmax>maxid) {
											maxid=posmax;
										}
									}
								}
							}
						} 
					}
					if (csaob==1) 
					{
						printf("transaction not executed \n");
					}
					
      			}
     		fclose ( filent );
  		 	}
   			else
   			{
      			perror ( filenament ); /* why didn't the file open? */
	  		}

        }
		else if (strcmp(csh,"findEarnings")==0) 
		{
			k=0;
			while( token != NULL ) {
    
     			token = strtok(NULL, s);
				k++;
				
				if (token!=NULL) { //similar commands in order to take the times and dates that we'll check in between for transactions
						if (k==1) 
						{
							j=0;
							nocoin=0;
							while(token[j]!='\0')
    						{
        						if(token[j]=='\n')
        						{
            						token[j]=' ';
									nocoin=1;
        						}
        						j++;
    						}
							if (nocoin==1) {
								token[j-1]='\0';
							}
							strcpy(walid,token);
						}
						if (k==2) {
							const char ch = '-';
							char *ret;
   							ret = strchr(token, ch);
							if (ret==NULL) {
								csaob=1;
								strncpy ( hour, token, 2 );
								hour[3] = '\0';
								strncpy ( minute, token+3, 2 );
								minute[3] = '\0';
								hour1 = atoi(hour);
								minute1 = atoi(minute);
							}
							else
							{
								csaob=2;
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day1 = atoi(day);
								month1 = atoi(month);
								year1 = atoi(year);
							}
								
						}
						if (k==3) {
							if (csaob==1) {
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day1= atoi(day);
								month1 = atoi(month);
								year1 = atoi(year);
							}
							else if (csaob==2)
							{
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day2 = atoi(day);
								month2 = atoi(month);
								year2 = atoi(year);
							}
						}
						if (k==4) {
								strncpy ( hour, token, 2 );
								hour[3] = '\0';
								strncpy ( minute, token+3, 2 );
								minute[3] = '\0';
								hour2 = atoi(hour);
								minute2 = atoi(minute);
						}
						if (k==5) {
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day2 = atoi(day);
								month2 = atoi(month);
								year2 = atoi(year);
						}
				}
				
   			}

			if (k==4) { //if no times given we give the minimum and possible so that all transactions are in between
				hour1=0;
				minute1=0;
				hour2=23;
				minute2=59;
			}
			if (k==2) { //same if neither time nor date given
				hour1=0;
				minute1=0;
				hour2=23;
				minute2=59;
				day1=0;
				month1=0;
				year1 =0;
				day2=31;
				month2=12;
				year2=9999;
			}
			 

			printasht(walid,rechar,rh,day1,month1,year1,hour1,minute1,day2,month2,year2,hour2,minute2,bytes);

        }
		else if (strcmp(csh,"findPayments")==0) //same for the payments
		{

			k=0;
			while( token != NULL ) {
    
     			token = strtok(NULL, s);
				k++;
				
				if (token!=NULL) {
						if (k==1) 
						{
							j=0;
							nocoin=0;
							while(token[j]!='\0')
    						{
        						if(token[j]=='\n')
        						{
            						token[j]=' ';
									nocoin=1;
        						}
        						j++;
    						}
							if (nocoin==1) {
								token[j-1]='\0';
							}
							strcpy(walid,token);
						}
						if (k==2) {
							const char ch = '-';
							char *ret;
   							ret = strchr(token, ch);
							if (ret==NULL) {
								csaob=1;
								strncpy ( hour, token, 2 );
								hour[3] = '\0';
								strncpy ( minute, token+3, 2 );
								minute[3] = '\0';
								hour1 = atoi(hour);
								minute1 = atoi(minute);
							}
							else
							{
								csaob=2;
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day1 = atoi(day);
								month1 = atoi(month);
								year1 = atoi(year);
							}
								
						}
						if (k==3) {
							if (csaob==1) {
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day1= atoi(day);
								month1 = atoi(month);
								year1 = atoi(year);
							}
							else if (csaob==2)
							{
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day2 = atoi(day);
								month2 = atoi(month);
								year2 = atoi(year);
							}
						}
						if (k==4) {
								strncpy ( hour, token, 2 );
								hour[3] = '\0';
								strncpy ( minute, token+3, 2 );
								minute[3] = '\0';
								hour2 = atoi(hour);
								minute2 = atoi(minute);
						}
						if (k==5) {
								strncpy ( day, token, 2 );
								day[3] = '\0';
								strncpy ( month, token+3, 2 );
								month[3] = '\0';
								strncpy ( year, token+6, 4 );
								year[5] = '\0';
								day2 = atoi(day);
								month2 = atoi(month);
								year2 = atoi(year);
						}
				}
				
   			}

			if (k==4) {
				hour1=0;
				minute1=0;
				hour2=23;
				minute2=59;
			}
			if (k==2) {
				hour1=0;
				minute1=0;
				hour2=23;
				minute2=59;
				day1=0;
				month1=0;
				year1 =0;
				day2=31;
				month2=12;
				year2=9999;
			}
			

			printasht(walid,sendhar,sh,day1,month1,year1,hour1,minute1,day2,month2,year2,hour2,minute2,bytes);

        }
		else if (strcmp(csh,"walletStatus")==0) //similar approach with the strtok&loop to take wallet id
		{
			while( token != NULL ) {
     	 		token = strtok(NULL, s);
				if (token!=NULL) {


					j=0;
					nocoin=0;
					while(token[j]!='\0')
    				{
        				if(token[j]=='\n')
        				{
            				token[j]=' ';
							nocoin=1;
        				}
        				j++;
    				}
					if (nocoin==1) {
						token[j-1]='\0';
					}
					ScWalleta=searchwal(token,whar,max);
					if (ScWalleta!=NULL) {  //if the user exists then print the balance
						printf( "%d\n", ScWalleta->balance );
					}
				}
				
   			}
			
        }
		else if (strcmp(csh,"bitCoinStatus")==0) // same with a bitcoin
		{
			while( token != NULL ) {
    
      			token = strtok(NULL, s);
				if (token!=NULL) {


					j=0;
					nocoin=0;
					while(token[j]!='\0')
    				{
        				if(token[j]=='\n')
        				{
            				token[j]=' ';
							nocoin=1;
        				}
        				j++;
    				}
					if (nocoin==1) {
						token[j-1]='\0';
					}
					ScTree=searchtree(token,thar,max);
					if (ScTree!=NULL) {
						unspent=findunspent(ScTree);
						printf( "%s %d %d\n",token,ScTree->counter,unspent);
					}
				}

   			}
        }
		else if (strcmp(csh,"traceCoin")==0) //and here as well
		{

			while( token != NULL ) {
    
      			token = strtok(NULL, s);
				if (token!=NULL) {


					j=0;
					nocoin=0;
					while(token[j]!='\0')
    				{
        				if(token[j]=='\n')
        				{
            				token[j]=' ';
							nocoin=1;
        				}
        				j++;
    				}
					if (nocoin==1) {
						token[j-1]='\0';
					}
					ScTree=searchtree(token,thar,max);
					if (ScTree->fleaf!=NULL) {
						preprint(ScTree->fleaf);
					}
				}

   			}
        }
		else if (strcmp(csh,"exit")==0) //if the instuction is exit the loop ends
		{
			cont = 0;
        }
		else  
		{
			printf( "wrong command\n");
        }
		
	}while (cont==1);

	for(i = 0; i < max; i++) //calling functions for freeing the memory at the hash tables
	{
		if (thar[i].firstel!=NULL) 
		{
			freetree(thar[i].firstel);
		}
		
	}
	
	for(i = 0; i < max; i++)
	{
		if (whar[i].firstel!=NULL) 
		{
			freewallet(whar[i].firstel);
		}
		
	}

	
	for(i=0;i<max;i++)
	{
		if (trnhar[i].firstel!=NULL) 
		{
			freetrans(trnhar[i].firstel);
		}
	}
	

	for(i=0;i<sh;i++)
	{
		if (sendhar[i].firstel!=NULL) 
		{
			freesr(sendhar[i].firstel,bytes);
		}
	}

	for(i=0;i<rh;i++)
	{
		if (rechar[i].firstel!=NULL) 
		{
			freesr(rechar[i].firstel,bytes);
		}
	}

	return 0;
}
