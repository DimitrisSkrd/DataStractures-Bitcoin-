typedef struct TransacElement *TransacElementPtr; //the structs used , more information in README
typedef struct WalletElement *WalletElementPtr;
typedef struct CoinElement *CoinElementPtr;
typedef struct CoinListHeader *CoinListHeaderPtr;
typedef struct TreeElement *TreeElementPtr;
typedef struct Leaf *LeafPtr;
typedef struct UsToTran *UsToTranPtr;
typedef struct Bucket *BucketPtr;


struct SRHashEl
{
    BucketPtr firstel;
};

struct WalHashEl{
	WalletElementPtr firstel;
};

struct CoinHashEl{
	TreeElementPtr firstel;
};

struct TranHashEl{
	TransacElementPtr firstel;
};

struct TransacElement{
     TransacElementPtr nextel;
     int day;
     int month;
     int year;
     int hour;
     int minute;
     int value;
     char sender[50];
     char receiver[50];
     char transid[15];
};


struct WalletElement{
	WalletElementPtr nextel;
	CoinListHeaderPtr coinlist;
	char walletid[50];
	int balance;
};


struct CoinElement{
        CoinElementPtr nextel;
        CoinElementPtr prevel;
        LeafPtr lfpos;
};


struct CoinListHeader{
        CoinElementPtr firstel;
        CoinElementPtr lastel;
};


struct TreeElement{
	LeafPtr fleaf;
    TreeElementPtr nextel;
    int counter;
	char *walletid;
	char *coinid;
};

struct Leaf{
    LeafPtr lleaf;
	LeafPtr rleaf;
    TreeElementPtr root;
    TransacElementPtr usedfr;
    int value;
};

struct UsToTran
{
    UsToTranPtr nextel;
    TransacElementPtr trans;
};

struct BucketEl
{
    UsToTranPtr firstel;
    char walletid[50];
};


struct Bucket{
    BucketPtr nextel;
    struct BucketEl *elements;
};



int hashf(char* symb,int max)//hash function for all the hash tables
{
    int sum=0,ln,mod,i;
    int vl=0;
    ln=strlen(symb);
    for(i=0;i<ln;i++)
        sum=sum+symb[i];
    mod=sum%max;
    return mod;
}

WalletElementPtr addwalletel(char* symb,struct WalHashEl * har,int max)//adds an element in wallet ht
{
    int pos,cont=1;
    pos = hashf(symb,max); //finds the position to place the element
    WalletElementPtr AddWallet,FindPos;
    AddWallet=malloc(sizeof(struct WalletElement));
    if (har[pos].firstel==NULL) //places the element at an empty position
    {
        har[pos].firstel=AddWallet; //needed initialaizations
        strcpy(AddWallet->walletid,symb);
        AddWallet->balance = 0;
        AddWallet->nextel = NULL;
        CoinListHeaderPtr CoinHead ;
        CoinHead=malloc(sizeof(struct CoinListHeader));
        CoinHead->firstel=NULL;
        CoinHead->lastel=NULL;
        AddWallet->coinlist=CoinHead;
        CoinHead=NULL;
    }
    else // if position not empty place the element at the end of the list
    {
        FindPos=har[pos].firstel;
        do
        {
            if (strcmp(FindPos->walletid,symb)==0) {
                free(AddWallet);
			    AddWallet=NULL;
                break;
            }
            else
            {
                if (FindPos->nextel!=NULL) {
                    FindPos=FindPos->nextel;
                }
                else
                {
                    cont=0;
                    FindPos->nextel=AddWallet;
                    strcpy(AddWallet->walletid,symb);
                    AddWallet->balance = 0;
                    AddWallet->nextel = NULL;
                    CoinListHeaderPtr CoinHead ;
                    CoinHead=malloc(sizeof(struct CoinListHeader));
                    CoinHead->firstel=NULL;
                    CoinHead->lastel=NULL;
                    AddWallet->coinlist=CoinHead;
                    CoinHead=NULL;
                }
            }
            
        } while (cont==1);
    }
    return AddWallet; //returns a pointer that points at the newly added element
}


TreeElementPtr addtreeel(char* symb,struct CoinHashEl * har,WalletElementPtr Owner,int value,int max)//same as before adds a tree element at tree element at its own hash table
{
    int pos,cont=1;
    pos = hashf(symb,max);
    TreeElementPtr AddTree,FindPos;
    AddTree=malloc(sizeof(struct TreeElement));
    AddTree->walletid=(char*)malloc(sizeof(char));
    AddTree->coinid=(char*)malloc(sizeof(char));
    if (har[pos].firstel==NULL)
    {
        har[pos].firstel=AddTree;
        strcpy(AddTree->walletid,Owner->walletid);
        strcpy(AddTree->coinid,symb);
        AddTree->counter=0;
        Owner->balance = Owner->balance + value;
        AddTree->nextel = NULL;
        LeafPtr Fileaf ;
        Fileaf=malloc(sizeof(struct Leaf));//also adds the first leaf of the tree
        Fileaf->rleaf=NULL;
        Fileaf->lleaf=NULL;
        Fileaf->usedfr=NULL;
        Fileaf->value=value;
        Fileaf->root=AddTree;
        AddTree->fleaf=Fileaf;
        Fileaf=NULL;
    }
    else
    {
        FindPos=har[pos].firstel;
        do
        {
            if (strcmp(FindPos->coinid,symb)==0) {
                free(AddTree);
			    AddTree=NULL;
                break;
            }
            else
            {
                if (FindPos->nextel!=NULL) {
                    FindPos=FindPos->nextel;
                }
                else
                {
                    cont=0;
                    FindPos->nextel=AddTree;
                    strcpy(AddTree->walletid,Owner->walletid);
                    strcpy(AddTree->coinid,symb);
                    AddTree->counter=0;
                    Owner->balance = Owner->balance + value;
                    AddTree->nextel = NULL;
                    LeafPtr Fileaf ;
                    Fileaf=malloc(sizeof(struct Leaf));
                    Fileaf->rleaf=NULL;
                    Fileaf->lleaf=NULL;
                    Fileaf->usedfr=NULL;
                    Fileaf->value=value;
                    AddTree->fleaf=Fileaf;
                    Fileaf->root=AddTree;
                    Fileaf=NULL;
                }
            }
            
        } while (cont==1);
    }
    return AddTree;
}

TransacElementPtr addtran(char *trid,char *sendid,char *recid,struct TranHashEl  * har,int max,int value,int day,int month,int year,int hour,int minute)//same for the transactions
{
    int pos,cont=1;
    pos = hashf(trid,max);
    TransacElementPtr AddTrans,FindPos;
    AddTrans=malloc(sizeof(struct TransacElement));
    if (har[pos].firstel==NULL)
    {
        har[pos].firstel=AddTrans;
        strcpy(AddTrans->transid,trid);
        strcpy(AddTrans->sender,sendid);
        strcpy(AddTrans->receiver,recid);
        AddTrans->value=value;
        AddTrans->day=day;
        AddTrans->month=month;
        AddTrans->year=year;
        AddTrans->hour=hour;
        AddTrans->minute=minute;
        AddTrans->nextel = NULL;
    }
    else
    {
        FindPos=har[pos].firstel;
        do
        {
            if (strcmp(FindPos->transid,trid)==0) {
                free(AddTrans);
			    AddTrans=NULL;
                break;
            }
            else
            {
                if (FindPos->nextel!=NULL) {
                    FindPos=FindPos->nextel;
                }
                else
                {
                    cont=0;
                    FindPos->nextel=AddTrans;
                    strcpy(AddTrans->transid,trid);
                    strcpy(AddTrans->sender,sendid);
                    strcpy(AddTrans->receiver,recid);
                    AddTrans->value=value;
                    AddTrans->day=day;
                    AddTrans->month=month;
                    AddTrans->year=year;
                    AddTrans->hour=hour;
                    AddTrans->minute=minute;
                    AddTrans->nextel = NULL;
                }
            }
            
        } while (cont==1);
    }
    return AddTrans;
}

void addtrnlist(int max,struct SRHashEl * har,char *symb,int bytes,TransacElementPtr ConTr)//insert ellements at the lists that "start" from the buckets
{
    int pos,i,cont=1,stop=0;
    pos = hashf(symb,max);
    UsToTranPtr AddEl;
    BucketPtr FnBck;
    AddEl=malloc(sizeof(struct UsToTran));
    if (har[pos].firstel==NULL) //if no bucket then add the first bucket as well as the first list element
    {
        BucketPtr myb ;
        myb=malloc(sizeof(struct Bucket));
        myb->elements=malloc(bytes * sizeof(struct BucketEl));
        for(i=0;i<bytes;i++)//initialisation of arrays ellements
	    {
            myb->elements[i].firstel=NULL;
	    }
        myb->elements[0].firstel=AddEl;
        strcpy(myb->elements[0].walletid,symb);
        myb->nextel=NULL;
        har[pos].firstel=myb;
        myb=NULL;
        AddEl->nextel=NULL;
        AddEl->trans=ConTr;//the list element points at the transaction that occured from
    }
    else
    {
        FnBck=har[pos].firstel;
        while(FnBck->nextel != NULL ){ // if there is a bucket checks the bucket(s) 
            for( i = 0; i < bytes; i++)
            {
                if (FnBck->elements[i].firstel!=NULL) { //to find if the user is allready in one bucket
                    if (strcmp(FnBck->elements[i].walletid,symb)==0) { // if there is, add the element at the end of the list
                        cont=0;
                        UsToTranPtr FnPl;
                        FnPl=FnBck->elements[i].firstel;
                        while(stop==0){
                            if (FnPl->nextel==NULL) 
                            {
                                stop=1;
                                FnPl->nextel=AddEl;
                                AddEl->nextel=NULL;
                                AddEl->trans=ConTr;
                            }
                            else
                            {
                                FnPl=FnPl->nextel;
                            }
                        }
                        break;
                    }
                    
                }
            }
            if (cont==0) {
                break;
            }
            else
            {
                FnBck=FnBck->nextel;
            }           
        }
        if (cont==1) { //same checks for the last bucket
            for( i = 0; i < bytes; i++)
            {
                if (FnBck->elements[i].firstel!=NULL) {
                    if (strcmp(FnBck->elements[i].walletid,symb)==0) {
                        cont=0;
                        UsToTranPtr FnPl;
                        FnPl=FnBck->elements[i].firstel;
                        while(stop==0){
                            if (FnPl->nextel==NULL) 
                            {
                                stop=1;
                                FnPl->nextel=AddEl;
                                AddEl->nextel=NULL;
                                AddEl->trans=ConTr;
                            }
                            else
                            {
                                FnPl=FnPl->nextel;
                            }
                        }              
                        break;
                    }
                }
                if ((cont==1)&&(FnBck->elements[i].firstel==NULL)) {
                    FnBck->elements[i].firstel=AddEl;
                    strcpy(FnBck->elements[i].walletid,symb);
                    AddEl->nextel=NULL;
                    AddEl->trans=ConTr;
                    break;
                }     
                else if ((cont==1)&&(i==bytes-1)) {
                    BucketPtr myb ;
                    myb=malloc(sizeof(struct Bucket));
                    myb->elements=malloc(bytes * sizeof(struct BucketEl));
                    for(i=0;i<bytes;i++)//initialisation of arrays ellements
	                {
                        myb->elements[i].firstel=NULL;
	                }
                    myb->elements[0].firstel=AddEl;
                    strcpy(myb->elements[0].walletid,symb);
                    myb->nextel=NULL;
                    FnBck->nextel=myb;
                    myb=NULL;
                    AddEl->nextel=NULL;
                    AddEl->trans=ConTr;
                }
            }
        }
        
        if (FnBck!=NULL ) {
            FnBck=NULL;
        }

    }
}

void addcoin(WalletElementPtr WalPtr,LeafPtr LePtr)//adds a coin at the coin's list of a user. Can add one when reading the bitcoin balance file,or after a transaction 
{
    CoinElementPtr AddCoin;
    AddCoin=malloc(sizeof(struct CoinElement));
    AddCoin->nextel=NULL;
    AddCoin->prevel=NULL;
    AddCoin->lfpos=NULL;
    if (WalPtr->coinlist->firstel==NULL) {
        WalPtr->coinlist->firstel=AddCoin;
        WalPtr->coinlist->lastel=AddCoin;
        AddCoin->lfpos=LePtr;      //the list element points at a leaf, either at the first leaf of a tree or at the leaf that occured from the same transaction
    }                              //after a transaction it can only point at a left leaf as is a coin added to the receiver of the transaction
    else
    {
        WalPtr->coinlist->lastel->nextel=AddCoin;
        AddCoin->prevel=WalPtr->coinlist->lastel;
        WalPtr->coinlist->lastel=AddCoin;
        AddCoin->lfpos=LePtr;
    }
}

void remcoin(WalletElementPtr WalPtr,CoinElementPtr RmCoin) //removes a coin from a user's list when it has a value of 0 for that user
{
    if (WalPtr->coinlist->firstel==RmCoin) {
        if (WalPtr->coinlist->lastel==RmCoin) {
            WalPtr->coinlist->firstel=NULL;
            WalPtr->coinlist->lastel=NULL;
            RmCoin->lfpos=NULL;
            free(RmCoin);
            RmCoin=NULL;
        }
        else
        {
            RmCoin->nextel->prevel=NULL;
            WalPtr->coinlist->firstel=RmCoin->nextel;
            RmCoin->lfpos=NULL;
            RmCoin->nextel=NULL;
            free(RmCoin);
            RmCoin=NULL;
        }
    }
    else
    {
        if (WalPtr->coinlist->lastel==RmCoin) {
            WalPtr->coinlist->lastel=RmCoin->prevel;
            RmCoin->prevel->nextel=NULL;
            RmCoin->prevel=NULL;
            RmCoin->lfpos=NULL;
            free(RmCoin);
            RmCoin=NULL;
        }
        else
        {
            RmCoin->prevel->nextel=RmCoin->nextel;
            RmCoin->nextel->prevel=RmCoin->prevel;
            RmCoin->nextel=NULL;
            RmCoin->prevel=NULL;
            RmCoin->lfpos=NULL;
            free(RmCoin);
            RmCoin=NULL;
        }
    }
}

void maketrans(TransacElementPtr TrPtr,WalletElementPtr SendId,WalletElementPtr RecId,int trval)//adds leaves at the tree after a transaction
{
    int cont=1,value,res;
    CoinElementPtr FnCoin;
    FnCoin=SendId->coinlist->firstel;
    while(cont==1){
        value=FnCoin->lfpos->value;
        res=value-trval;
        LeafPtr AddRLeaf;
        AddRLeaf=malloc(sizeof(struct Leaf));//rigth leaves for the senders
        FnCoin->lfpos->rleaf=AddRLeaf;
        AddRLeaf->lleaf=NULL;
        AddRLeaf->rleaf=NULL;
        AddRLeaf->root=FnCoin->lfpos->root;
        AddRLeaf->usedfr=TrPtr;
        AddRLeaf->root->counter=AddRLeaf->root->counter+1;
        LeafPtr AddLLeaf;
        AddLLeaf=malloc(sizeof(struct Leaf));//left ones for the receivers
        FnCoin->lfpos->lleaf=AddLLeaf;
        AddLLeaf->lleaf=NULL;
        AddLLeaf->rleaf=NULL;
        AddLLeaf->root=FnCoin->lfpos->root;
        AddLLeaf->usedfr=TrPtr;
        addcoin(RecId,AddLLeaf);//update of the receivers coin list , inserting the new one
        if (res>=0)//checks if a transaction is terminated with the use of this coin (if remaining transaction value ==0)
        {
            FnCoin->lfpos->rleaf->value=value-trval;//if it is we don't need to use another coin
            FnCoin->lfpos->lleaf->value=trval;
            FnCoin->lfpos=FnCoin->lfpos->rleaf;
            trval=0;
            cont=0;
            if (res==0) {//if the remaining transaction value was equal with the coin value we remove the coin from the sender
                CoinElementPtr RmCoin;
                RmCoin=FnCoin;
                remcoin(SendId,FnCoin);
            }
        }
        else //if a transactionis not terminated
        {
            FnCoin->lfpos->rleaf->value=0;
            FnCoin->lfpos->lleaf->value=value;
            FnCoin->lfpos=FnCoin->lfpos->rleaf;
            trval=trval-value;
            CoinElementPtr RmCoin;
            RmCoin=FnCoin;
            FnCoin=FnCoin->nextel;//we use the following coin
            remcoin(SendId,RmCoin);//and we remove the coin from the sender
        }
    }
}

void printwallet(int max,struct WalHashEl * har)//prints the ellements' variables values
{
	int i,cont;
	WalletElementPtr PrPn;
	for(i=0;i<max;i++)
	{
		cont=1;
		if(har[i].firstel!=NULL)
                {
                        PrPn=har[i].firstel;
                        do
                        {
				            printf(" ID: %s",PrPn->walletid);
                            printf("  balance: %d \n",PrPn->balance);
                        	if(PrPn->nextel!=NULL)
                        		PrPn=PrPn->nextel;
                             	else
                                	cont=0;
                        }while(cont==1);
		}
	}
}

WalletElementPtr searchwal(char* addrs,struct WalHashEl *  har,int max)//search for an address in the hash table
{
		int pos,ret,cont=1;
		ret=0;
		WalletElementPtr ScPn;
		pos=hashf(addrs,max);
		if(har[pos].firstel==NULL)
			ret=0;
		else
		{
			ScPn=har[pos].firstel;
			do
			{
				if(strcmp(ScPn->walletid,addrs)==0)
				{
					ret=1;
					break;
				}
				else
				{
					if(ScPn->nextel!=NULL)
						ScPn=ScPn->nextel;
					else
						cont=0;
				}
			}while(cont==1);
		}
		if (ret==0)
			ScPn=NULL;
		return ScPn;
}

TreeElementPtr searchtree(char* addrs,struct CoinHashEl *  har,int max)//search for an address in the hash table
{
		int pos,ret,cont=1;
		ret=0;
		TreeElementPtr ScPn;
		pos=hashf(addrs,max);
		if(har[pos].firstel==NULL)
			ret=0;
		else
		{
			ScPn=har[pos].firstel;
			do
			{
				if(strcmp(ScPn->coinid,addrs)==0)
				{
					ret=1;
					break;
				}
				else
				{
					if(ScPn->nextel!=NULL)
						ScPn=ScPn->nextel;
					else
						cont=0;
				}
			}while(cont==1);
		}
		if (ret==0){
			ScPn=NULL;
        }
		return ScPn;
}

int findunspent(TreeElementPtr ScTree)//finds the unspend value of a coin
{
    LeafPtr FnPtr;
    FnPtr=ScTree->fleaf;
    while(FnPtr->rleaf!=NULL)
    {
        FnPtr=FnPtr->rleaf;
    }
    return FnPtr->value;
}

void printasht(char *wid,struct SRHashEl *  har,int max,int d1,int mo1,int y1,int h1,int mi1,int d2,int mo2,int y2,int h2,int mi2,int bytes)
{
    int cont,stop,nba,i,val=0;
    int pos=hashf(wid,max);
    if(har[pos].firstel==NULL)
    {
        printf("can't find user \n");
    }
    else
    {
        nba=1;
        BucketPtr FnBck;
        FnBck=har[pos].firstel;
        while(nba==1 ){
            for( i = 0; i < bytes; i++)
            {
                if (FnBck->elements[i].firstel!=NULL) {
                    if (strcmp(FnBck->elements[i].walletid,wid)==0) {//finds a user at the senders or the receivers hash table
                        cont=0;
                        UsToTranPtr FnPl;
                        FnPl=FnBck->elements[i].firstel;
                        stop=0;
                        while(stop==0){
                            if (FnPl->nextel==NULL) //the following ifs check the date&time of the transactions of the user's list
                            {
                                int chtd=0;
			                    if (FnPl->trans->year>y1) {
				                    chtd=1;
			                    }
			                    else if (FnPl->trans->year==y1) {
				                    if (FnPl->trans->month>mo1) {
					                    chtd=1;
				                    }
				                    else if (FnPl->trans->month==mo1) {
					                    if (FnPl->trans->day>d1) {
						                    chtd=1;
					                    }
					                    else if (FnPl->trans->day==d1) {
						                    if (FnPl->trans->hour>h1) {
							                    chtd=1;
						                    }
						                    else if (FnPl->trans->hour==h1) {
							                    if (FnPl->trans->minute>=mi1) {
								                    chtd=1;
							                    }
				                    		}
				                    	}
				                    }
                                }

                                int chlo=0;
			                    if (FnPl->trans->year<y2) {
				                    chlo=1;
			                    }
			                    else if (FnPl->trans->year==y2) {
				                    if (FnPl->trans->month<mo2) {
					                    chlo=1;
				                    }
				                    else if (FnPl->trans->month==mo2) {
					                    if (FnPl->trans->day<d2) {
						                    chlo=1;
					                    }
					                    else if (FnPl->trans->day==d2) {
						                    if (FnPl->trans->hour<h2) {
							                    chlo=1;
						                    }
						                    else if (FnPl->trans->hour==h2) {
							                    if (FnPl->trans->minute<=mi2) {
								                    chlo=1;
							                    }
				                    		}
				                    	}
				                    }
                                }

                                if ((chlo==1)&&(chtd==1)) {//and if the date&time of one transaction of the list is between the date&time we asked it prints the values
                                    val= val + FnPl->trans->value;//and increases the sum of the values
                                    printf("%s", FnPl->trans->transid);
                                    printf(" %s", FnPl->trans->sender);
                                    printf(" %s", FnPl->trans->receiver);
                                    printf(" %d",FnPl->trans->value);
			                        printf(" %d",FnPl->trans->day);
		                        	printf("-%d",FnPl->trans->month);
			                        printf("-%d",FnPl->trans->year);
			                        printf(" %d",FnPl->trans->hour);
			                        printf(":%d \n",FnPl->trans->minute);
                                }

                                stop=1;
                                
                            }
                            else
                            {
                                int chtd=0;
			                    if (FnPl->trans->year>y1) {
				                    chtd=1;
			                    }
			                    else if (FnPl->trans->year==y1) {
				                    if (FnPl->trans->month>mo1) {
					                    chtd=1;
				                    }
				                    else if (FnPl->trans->month==mo1) {
					                    if (FnPl->trans->day>d1) {
						                    chtd=1;
					                    }
					                    else if (FnPl->trans->day==d1) {
						                    if (FnPl->trans->hour>h1) {
							                    chtd=1;
						                    }
						                    else if (FnPl->trans->hour==h1) {
							                    if (FnPl->trans->minute>=mi1) {
								                    chtd=1;
							                    }
				                    		}
				                    	}
				                    }
                                }

                                int chlo=0;
			                    if (FnPl->trans->year<y2) {
				                    chlo=1;
			                    }
			                    else if (FnPl->trans->year==y2) {
				                    if (FnPl->trans->month<mo2) {
					                    chlo=1;
				                    }
				                    else if (FnPl->trans->month==mo2) {
					                    if (FnPl->trans->day<d2) {
						                    chlo=1;
					                    }
					                    else if (FnPl->trans->day==d2) {
						                    if (FnPl->trans->hour<h2) {
							                    chlo=1;
						                    }
						                    else if (FnPl->trans->hour==h2) {
							                    if (FnPl->trans->minute<=mi2) {
								                    chlo=1;
							                    }
				                    		}
				                    	}
				                    }
                                }

                                if ((chlo==1)&&(chtd==1)) {
                                    val= val + FnPl->trans->value;
                                    printf("%s", FnPl->trans->transid);
                                    printf(" %s", FnPl->trans->sender);
                                    printf(" %s", FnPl->trans->receiver);
                                    printf(" %d",FnPl->trans->value);
			                        printf(" %d",FnPl->trans->day);
		                        	printf("-%d",FnPl->trans->month);
			                        printf("-%d",FnPl->trans->year);
			                        printf(" %d",FnPl->trans->hour);
			                        printf(":%d \n",FnPl->trans->minute);
                                }
                                

                                FnPl=FnPl->nextel;
                            }
                        }
                        break;
                    }
                    
                }
            }
            if (cont==0) {
                printf("total amount %d \n",val);
                break;
            }
            else
            {
                if (FnBck->nextel!=NULL) {
                    FnBck=FnBck->nextel;
                }
                else
                {
                    nba=0;
                }        
            }           
        }

    }
}

void preprint(LeafPtr PrPtr)//prints the values of a tree using the pre order recursive algorithm
{
        if (PrPtr->lleaf!=NULL)
        {                     //note that only prints from the left leaves elements, if it also was printing from the right leaves we would have the same prints twice
            printf("%s", PrPtr->lleaf->usedfr->transid);
            printf(" %s", PrPtr->lleaf->usedfr->sender);
            printf(" %s", PrPtr->lleaf->usedfr->receiver);
            printf(" %d",PrPtr->lleaf->usedfr->value);
			printf(" %d",PrPtr->lleaf->usedfr->day);
		    printf("-%d",PrPtr->lleaf->usedfr->month);
			printf("-%d",PrPtr->lleaf->usedfr->year);
			printf(" %d",PrPtr->lleaf->usedfr->hour);
			printf(":%d \n",PrPtr->lleaf->usedfr->minute);   
            preprint(PrPtr->lleaf);
        }

        if (PrPtr->rleaf!=NULL) {
            preprint(PrPtr->rleaf);
        }
}

void freeleafs(LeafPtr RmLf)//frees a tree's elements using the post order recursive algorithm
{
    if (RmLf->lleaf!=NULL) 
    {
        freeleafs(RmLf->lleaf);
    }
    if (RmLf->rleaf!=NULL) 
    {
        freeleafs(RmLf->rleaf);
    }
    free(RmLf);
}

void freetree(TreeElementPtr RmPtr)//frees the tree header elements
{
    int cont=1,count;
    TreeElementPtr FnPtr;
    FnPtr=RmPtr;
    while(cont==1)//basically frees the elements using a lifo at the list that starts form a hash table element
    {
        FnPtr=RmPtr;
        count=0;
        
        if (FnPtr->nextel!=NULL) 
        {
            while(FnPtr->nextel->nextel!=NULL)
            {
                FnPtr=FnPtr->nextel;
                count++;
            }
            if (FnPtr->nextel->fleaf!=NULL) //also calls the function to free the leaves of each tree
            {
                freeleafs(FnPtr->nextel->fleaf);
            }
            free(FnPtr->nextel);
            FnPtr->nextel=NULL;
        }
        if (count==0) {
            cont=0;
        }
    }
    if (RmPtr->fleaf!=NULL) 
    {
        freeleafs(RmPtr->fleaf);
    }
    free(RmPtr);
}

void freewallist(CoinListHeaderPtr HdPtr)//frees the elements of a user's coin list
{
    while(HdPtr->firstel!=HdPtr->lastel)
    {
        HdPtr->lastel=HdPtr->lastel->prevel;
        free(HdPtr->lastel->nextel);
    }
    free(HdPtr->lastel);
    free(HdPtr);//and the header of the same list
}

void freewallet(WalletElementPtr RmPtr)//same as the tree header elements, now with wallet elements
{
    int cont=1,count;
    WalletElementPtr FnPtr;
    FnPtr=RmPtr;
    while(cont==1)
    {
        FnPtr=RmPtr;
        count=0;
        
        if (FnPtr->nextel!=NULL) 
        {
            while(FnPtr->nextel->nextel!=NULL)
            {
                FnPtr=FnPtr->nextel;
                count++;
            }
            if (FnPtr->nextel->coinlist!=NULL)//also calls the function to free the list of coins
            {
                freewallist(FnPtr->nextel->coinlist);
            }
            free(FnPtr->nextel);
            FnPtr->nextel=NULL;
        }
        if (count==0) {
            cont=0;
        }
    }
    if (RmPtr->coinlist!=NULL) 
    {
        freewallist(RmPtr->coinlist);
    }
    free(RmPtr);
}

void freetrans(TransacElementPtr RmPtr)//same as before lifo for the transactions 
{
    int cont=1,count;
    TransacElementPtr FnPtr;
    FnPtr=RmPtr;
    while(cont==1)
    {
        FnPtr=RmPtr;
        count=0;
        
        if (FnPtr->nextel!=NULL) 
        {
            while(FnPtr->nextel->nextel!=NULL)
            {
                FnPtr=FnPtr->nextel;
                count++;
            }
            free(FnPtr->nextel);
            FnPtr->nextel=NULL;
        }
        if (count==0) {
            cont=0;
        }
    }
    free(RmPtr);
}

void freesrlist(UsToTranPtr RmPtr)//same lifo for the list of senders' or receivens' transactions(the list asked for the exercise)
{
    int cont=1,count;
    UsToTranPtr FnPtr;
    FnPtr=RmPtr;
    while(cont==1)
    {
        FnPtr=RmPtr;
        count=0;
        
        if (FnPtr->nextel!=NULL) 
        {
            while(FnPtr->nextel->nextel!=NULL)
            {
                FnPtr=FnPtr->nextel;
                count++;
            }
            free(FnPtr->nextel);
            FnPtr->nextel=NULL;
        }
        if (count==0) {
            cont=0;
        }
    }
    free(RmPtr);
}

void freesr(BucketPtr RmPtr,int bytes)//same lifo for the buckets
{
    int cont=1,count,i;
    BucketPtr FnPtr;
    FnPtr=RmPtr;
    while(cont==1)
    {
        FnPtr=RmPtr;
        count=0;
        
        if (FnPtr->nextel!=NULL) 
        {
            while(FnPtr->nextel->nextel!=NULL)
            {
                FnPtr=FnPtr->nextel;
                count++;
            }
            for( i = 0 ; i < bytes ; i++)
            {
                if (FnPtr->nextel->elements[i].firstel!=NULL) 
                {
                    freesrlist(FnPtr->nextel->elements[i].firstel);//also calls the function to free the list of senders' or receivens' transactions
                }
                
            }
            
            free(FnPtr->nextel->elements);//also frees the bucket arrays
            free(FnPtr->nextel);
            FnPtr->nextel=NULL;
        }
        if (count==0) {
            cont=0;
        }
    }
    
    for( i = 0 ; i < bytes ; i++)
    {
        if (RmPtr->elements[i].firstel!=NULL) 
        {
            freesrlist(RmPtr->elements[i].firstel);
        }
                
    }
    free(RmPtr->elements);
    free(RmPtr);
}