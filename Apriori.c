#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct node
{
	int value;
	struct node * ptr;
};

void genRules(struct node * freqItems[], int size);
void gen_rules(struct node * freqItems[], int size);
void gen_rules_new(struct node * freqItems[],int frequent_itemset_array_original[] , int frequent_itemset_array_new[] , int size_original , int size_new ,int frequent_itemset_support,int confidence);

struct node * get_first_frequent_itemset(struct node * itemsets[],int min_support);
int get_transactions(struct node *transactions[]);
struct node *scan_transaction(int count,struct node *transactions[]);
struct node * get_list();
struct node *create_node();
struct node *join_operation();
struct node * get_frequent_itemsets(struct node *itemsets[],int count,struct node *transactions[],int current,int min_support,struct node *frequent_itemsets[]);
struct node  *create_node1(int val);
struct node *scan_transaction1(int count,struct node *transactions[]);
int get_transaction2(struct node *transactions[]);
struct node *create_list(int value,struct node * root);


const int MAX=100;
int no_items;
char mapping_array[100][100];


int get_data_from_file();
int map_input( char oneword[] );
int cal_item_number( char oneword[] );
int main()
{
	int i,j,flag=0;
	int items_count[MAX];
	struct node *temp_ptr;
	int min_support,transaction_count;
	struct node *itemsets[MAX];		/*  To store data after join operation */
	struct node *frequent_itemsets[MAX];	/* To store frequent itemset */
	struct node *transactions[MAX];
	struct node * temp,*temp1,*temp2;
	
	
	
	for(i=0;i<MAX;i++)
	  items_count[i]=0;
	  
	temp_ptr=(struct node *)malloc(sizeof(struct node));
	
	printf("************************This is program for Apriori algorithm***************************************");
	
//	transaction_count=get_transactions(transactions);	  /* will take input from file  of transactions */

	no_items=get_data_from_file();
	
	printf(" no of items are : \n",no_items);
	
//	for(i=0;i<no_items;i++)
//	 printf(" %s ",mapping_array[i]);
	 
	transaction_count=get_transaction2(transactions);

	printf("\n\n Enter minimum support(%) : ");
	scanf("%d",&min_support);
	
//	printf("\n\n Enter number(count) of items  : ");
//	scanf("%d",&no_items);
	
	
	
	min_support=min_support*transaction_count/100;
	
	itemsets[0]=scan_transaction1(transaction_count,transactions);		/* initial parsing of data items */
	
	frequent_itemsets[0]=get_first_frequent_itemset(itemsets,min_support);
	
	for(i=1;i<MAX;i++)
	{
		itemsets[i]=join_operation(i,frequent_itemsets);     /* To get in item set and support count */

		
		if((temp_ptr=get_frequent_itemsets(itemsets,transaction_count,transactions,i,min_support,frequent_itemsets))==NULL)  /* check apriori property and minimum supports count in function temp_ptr=get_frequent_itemsets()*/
	    {	  
			printf("\n\n Algorithm is finished.............");
			frequent_itemsets[i]=temp_ptr;
			break;
		}
		
		frequent_itemsets[i]=temp_ptr;
	
	}

	for(j=0;j<i;j++)
	{
		printf("\nMin_Sup ITEMSETS");
		temp=frequent_itemsets[j];
		for(;temp!=NULL;temp=temp->ptr)
		   { 
			   if(temp->value==-1)
			    {
			        printf("\n");
			        flag=0;
			    } 
	            else
				{
				if(flag==0)	
				{
					
					printf("\n%d	 ",temp->value);
					flag=1;
				}
				else
				printf(" %s ",mapping_array[temp->value-1]);
	            	
	            }  
			
			
		   }			
	}
//	printf("calling gen rules\n");
	gen_rules(frequent_itemsets, i-1);
//	genRules(frequent_itemsets, i-1);
	
	getch();
	return 1;	
}

int cal_item_number( char oneword[])
{
	int i;
	for(i=0;i<no_items;i++)
	{
		if(strcmp(mapping_array[i],oneword)==0)
		return i+1;
	}

}

int get_data_from_file()
{
	FILE *fp;
	int count=0,size;
	fp=fopen("input.txt","r");

	char oneword[100];
    char c;
    
    do {
      	c = fscanf(fp,"%s",oneword); /* got one word from the file */
      	if(c!=EOF)
      	{
      	//	printf("%s\n",oneword);       /* display it on the monitor  */
      		size=map_input(oneword);
      	}
     	
     	if(c=='\n')
     	{
     		count++;
     	}
   } while (c != EOF);  
	
	
	fclose(fp);
//	printf(" \n size is : %d",size);
	
return size;	
	
}
int map_input( char oneword[] )
{
	static int size;
	int i;
	for(i=0;i<size;i++)
	{
		if(strcmp(mapping_array[i],oneword)==0)
		break;
	}	
	if(i==size)
	{
		strcpy(mapping_array[i],oneword);
		size++;
	}
	
}


void gen_rules(struct node * freqItems[], int size)
{
	int frequent_itemset_array[size];
	int frequent_itemset_support;
	
	
	
	 struct node *curr, *subset, *temp;
    int i, j , k, l, p , singleItem, item[100], singleSupport, subsSupport,confidence;
    
    printf("\n Enter value for confidence(percentage) : ");
    scanf("%d",&confidence);
    
    for(temp = freqItems[size];temp!=NULL;temp=temp->ptr)
    {
    	
    	curr = temp;
    	frequent_itemset_support=curr->value;
    
		for(i=0,curr=curr->ptr;i<=size;i++,curr=curr->ptr)
		{
			frequent_itemset_array[i]=curr->value;
		}
	 
		printf(" \n RULES FOR :");
		for(p=0;p<=size;p++)
		printf(" %s ",mapping_array[frequent_itemset_array[p]-1]);
	   	gen_rules_new(freqItems,frequent_itemset_array,frequent_itemset_array,size,size,frequent_itemset_support,confidence);	
	    temp=curr;
	    printf("\n");
    	getch();
    }
    
	
}

void gen_rules_new(struct node * freqItems[],int frequent_itemset_array_original[] , int frequent_itemset_array_new[] , int size_original , int size_new ,int frequent_itemset_support,int confidence)
{
	
	struct node *curr, *subset, *temp;
    int i, j , k, l, p , singleItem, item[100], singleSupport, subsSupport,current_confidence, x , m;
	
 	for(j=0; j<=size_new; j++)
 	{
 		int subs[size_new-1];
                
        for(k=0,p=0;p<=size_new;p++)
		{
			if(abs(j-p)==0)
			{
				 singleItem = frequent_itemset_array_new[p];
				 continue;
			}
				
			subs[k++]=frequent_itemset_array_new[p];
		}
		
		subset = freqItems[size_new-1];
        int currSupport = 0;
        while( subset!= NULL )
		{
        	
             subsSupport = subset->value;
             subset = subset->ptr;
            // printf("\n subsSupport = %d",subsSupport);
             temp = subset;
            // printf("\n temp_value = %d",temp->value);
             k=0;
             while(temp->value != -1)
			 {
                if(temp->value != subs[k++])
					 break;  
				temp=temp->ptr;  
             }
             if(temp->value == -1){
                  break;
             } 
					 
			for(;subset->value!=-1;subset=subset->ptr);
				subset=subset->ptr;
        }
       
       	current_confidence=frequent_itemset_support*100/subsSupport;
       	if(current_confidence>=confidence)
       	  {
			 printf("\n rule : \n");
       		 for(p=0;p<size_new;p++)
       		 {
       		 //	printf(" %d ",subs[p]);
       		 	printf(" %s ",mapping_array[subs[p]-1]);
       		 	
       		 }
       		 
		    	printf(" :-> ");
			
			    for(p=0;p<=size_original;p++)
				{
				
					for(m=0;m<size_new;m++)
					{
						if(frequent_itemset_array_original[p]==subs[m])
						{
							break;
						}
					}
				
					if(m==size_new)
					{
					//	printf(" %d ",frequent_itemset_array_original[p]);
						printf(" %s ",mapping_array[frequent_itemset_array_original[p]-1]);
						
					}
				}
       		 

               printf(" confidence = %d ",current_confidence);  		 
       		 if(size_new>1)
       		 {
       		 	gen_rules_new(freqItems,frequent_itemset_array_original , subs , size_original ,  size_new-1 , frequent_itemset_support, confidence);
       		 }
       		
       		
       		
       	  } 
	            
 	
 	}		
	
}
//gen rules



struct node * get_first_frequent_itemset(struct node * itemsets[],int min_support)
{
	int i,j,k,l=0,m,p;
	
	struct node * temp,*temp1,*list=NULL,*list1=NULL,*temp2,*temp3,*temp4,*temp5,*temp6;
	
	for(temp=itemsets[0];temp!=NULL;temp=temp->ptr)
	{
		if(temp->value>=2)
		{
			if(list==NULL)
			{
				list=temp;
				
				temp1=temp;
			
			}
			else
			{
				temp1->ptr=temp;
				temp1=temp;
				
			}
			temp1=temp1->ptr;
			temp1=temp1->ptr;
		}
		temp=temp->ptr;
		temp=temp->ptr;
	}
	temp1->ptr=NULL;
	return list;
}

struct node * get_frequent_itemsets(struct node *itemsets[],int count,struct node *transactions[],int current,int min_support,struct node *frequent_itemsets[])
{
	int i,j,k,l=0,m,p;
	int a[MAX],b[MAX];
	struct node * temp,*temp1,*list=NULL,*list1=NULL,*temp2,*temp3,*temp4,*temp5,*temp6,*temp11;
	
	//printf("\n inside get frequent itemset ");
	for(temp=itemsets[current];temp!=NULL;temp=temp->ptr)
	{
	
		for(i=0;i<count;i++)
		{
			temp1=temp;
			temp2=transactions[i];
			for(temp1=temp->ptr;temp1->value!=-1;temp1=temp1->ptr)
			{
				for(temp2=transactions[i];temp2!=NULL;temp2=temp2->ptr)
				{
					if(temp2->value==temp1->value)
					{
						break;
					}
				}
				if(temp2==NULL)
				{
					break;
				}
				
				
			}
			if(temp1->value==-1)
			{
				temp->value++;
			}
			//printf("\n %d copmeleted %d",i,temp->value);
		}
		
		for(;temp->value!=-1;temp=temp->ptr);
			
	}
//	printf("\n after first for loop \n");
/*	for(temp11=itemsets[current];temp11!=NULL;temp11=temp11->ptr)
	 printf(" %d ",temp11->value);*/
	 
	for(temp4=itemsets[current];temp4!=NULL;temp4=temp4->ptr)
	{
		if(list==NULL)
		{
				if(temp4->value>=min_support)
				{
					list=temp4;
					temp5=temp4;
					for(;temp4->value!=-1;temp4=temp4->ptr);
					temp5=temp4;
							      
				}
				else
				{
					temp5=NULL;
					for(;temp4->value!=-1;temp4=temp4->ptr);
				}
		}
		else
		{
			if(temp4->value>=min_support)
			{
				//temp5->ptr=temp4;
				temp5->ptr=temp4;
				
				for(;temp4->value!=-1;temp4=temp4->ptr);
					  
				temp5=temp4;
				
			}
			else
			{
			//	temp5->ptr=NULL;
			for(;temp4->value!=-1;temp4=temp4->ptr);
			}
			
		}
	
	//	for(;temp4->value!=-1;temp4=temp4->ptr);
		
	}
	
	 if(list!=NULL)
	  	temp5->ptr=NULL;
/*	  printf(" \n Before current>2 step \n");
	  for(temp11=list;temp11!=NULL;temp11=temp11->ptr)
	      printf(" %d ",temp11->value);	*/
	  	
	if(current>=2){
	
	for(temp=list;temp!=NULL;temp=temp->ptr)
	{
		for(temp1=temp->ptr,i=0;temp1->value!=-1;temp1=temp1->ptr,i++)
		{
		//	printf("\n inside for loop \n");
			a[i]=temp1->value;
		//	printf (" arr %d ",a[i]);
		}
	/*	printf(" \n");
		for(j=0;j<i;j++)
		printf(" a[%d]=%d ",j,a[j]);*/
		for(j=0;j<i;j++)
		{
			for(k=0,p=0;p<i;p++)
			{
				if(abs(j-p)==0)
				continue;
				b[k++]=a[p];
			}
			/*	printf(" \n");
				for(p=0;p<i-1;p++)
					printf(" b[%d]=%d ",p,b[p]);*/
			temp2=frequent_itemsets[current-1];
			for(;temp2!=NULL;temp2=temp2->ptr)
			{
				for(k=0,temp2=temp2->ptr;temp2->value!=-1;temp2=temp2->ptr,k++)	
				{
				//	printf("\nb[k]= %d t2=%d  ",b[k],temp2->value);
					if(b[k]!=temp2->value)
					break;
					
				}
				if(temp2->value==-1)
				{
					break;
				}
				for(;temp2->value!=-1;temp2=temp2->ptr);
			}
			if(temp2==NULL)
			 {
			 	break;
			 }
		}
		
		if(j==i)
		{
			if(list1==NULL)
			{
				list1=temp;
				temp4=temp;
				for(;temp->value!=-1;temp=temp->ptr);
				temp4=temp;
				
			}
			else
			{
				temp4->ptr=temp;
				for(;temp->value!=-1;temp=temp->ptr);
				temp4=temp;
				
			}
			
			
			
		}
	//	for(;temp->value!=-1;temp=temp->ptr);
	}
	if(list1!=NULL)
	{
		temp4->ptr=NULL;
	}

	
	return list1;
    }
   /* printf("\n before returning \n");
	for(temp11=list;temp11!=NULL;temp11=temp11->ptr)
	 printf(" %d ",temp11->value);*/
    return list;
}



 struct node  *create_node(){
  
     struct node *element;
     element= (struct node *)malloc(sizeof(struct node));
     printf(" \n Enter the item : ");
     scanf("%d",&element->value);
  //   items_count[element->value]++;
     element->ptr=NULL;
   
     return element;
  }  
  
struct node * get_list(){
    
    struct node *head;
    struct node  *list;
    char ch;
    int flag=1;
    
    list=create_node();
     
    head=list;
      
    while(flag)
    {
     printf("\n DO you want to enter more item:");
     ch=getch();
    
     if(ch!='y')
     {
         break;
     }    
     list->ptr=create_node();
     list=list->ptr;
    }  
    
   
    return head;    
        
}  
int get_transactions(struct node *transactions[])
{
	int count,i;
	
	printf("\n\n Enter the number of transactions : ");
	scanf("%d",&count);
	
	for(i=0;i<count;i++)
	{
	  printf("\n      DATA  FOR TRANSACTION %d : ",i+1);
	  transactions[i]=get_list(); /* to get a single transactions */
    }
    return count;
}

struct node *create_list(int value,struct node * root)
{
	struct node * temp;
	int flag=0;
	if(root==NULL)
	{
		root=create_node1(value);
		temp=root;
		
	}
	else
	{
		temp=root;
	}
	for(;temp->ptr!=NULL;temp=temp->ptr)
	{
		if(temp->value==value)
		{
			return root;
		}
		
	}
	if(temp->value==value)
		return root;
	else
	temp->ptr=create_node1(value);
	
	return root;
}

int get_transaction2(struct node *transactions[])
{
	FILE *fp;
	char d;
	int n = 0;
	int prev,dp,item_no;
	char input[100];
	int count=0,i=0,j=0;
	struct node * root=NULL;
	
	fp = fopen("input.txt","r");
	
	if(fp == NULL) 
	  {
	   perror("Error in opening file");
	   return(-1);
	  }
	else{
	//	printf("\n File opened successfully ");
	}
	
	 do
	   {
//	   	 printf("\n insideloop ");
	      d= fgetc(fp);
//	      printf("\n d= %c ",d);
	      
	      if( feof(fp) )
	      {
	      	if(prev!=10)
	      	{
	      		input[i++]='\0';
	      		item_no=cal_item_number(input);
		    	root=create_list(item_no,root);
		    	transactions[j++]=root;
	  		//	printf(" %s ",input);
	      		count++;
	  	
	      	}
		    break ;
		  }
//		  printf("\n after first if ");
		  
		  if(d=='\n')
		   	{
		   		if(prev==10)
				   {
 		     			break;
		     		}
		     	else
				 {
		     		input[i++]='\0';
		    		i=0;
		    		item_no=cal_item_number(input);
		    		root=create_list(item_no,root);
		    		transactions[j++]=root;
		    		root=NULL;
		    	//	printf(" %s ",input);
		     	 }
		     	 
		      	
		     	 count++;
		   	}
//		   	printf("\n after second if ");
		    if(d==' ')
		    {
				input[i++]='\0';
		    	i=0;
		    	item_no=cal_item_number(input);
//		    	printf("\n item_no : %d input %s ",item_no,input);
		    	root=create_list(item_no,root);
		    //	printf(" %s ",input);
		    }
		    else
		    {
		    	if(d!='\n')
				{
		    		  input[i++]=d;
		    	}
		  
		    }
//		    printf("\n after third if ");
		      prev=d;
		      
	  }while(1);
		
		   fclose(fp);
//		  	printf(" \n count = %d \n",count);
			
			//getch();
			
			
	return count;
	
}


struct node *join_operation(int current,struct node *frequent_itemsets[])
{
	int i,j,k,l=0,m,p,x;
	struct node * temp,*temp1,*list=NULL,*temp2,*temp3,*temp4,*temp5,*temp6,*temp11;
	//printf("\n inside join ");
	
	for(temp=frequent_itemsets[current-1];temp!=NULL;temp=temp->ptr)
	{
		if(temp->value==-1)
	   {
	   		l++;
	   } 
	}
	//	printf("\n  join  for loop 1 %d ",l);
	for(i=0,temp=frequent_itemsets[current-1];i<l;i++)
	{
	//	printf("\n  join  for loop 2_1");
			for(temp1=temp,m=0;m<i;temp1=temp1->ptr)
			{
				if(temp1->value==-1)
	  		    {
	  		 		m++;
	 		    } 
			 	
			}
		//	printf("\n  join  for loop 2_2 %d ",m);
			for(temp2=temp1;temp2->value!=-1;temp2=temp2->ptr);
			/*if(temp2!=temp1)
			 printf(" \n not equal t2= %d  t1= %d  ",temp2->value,temp1->value );
			printf("\n  join  for loop 2_3");*/
			for(temp2=temp2->ptr;temp2!=NULL;temp2=temp2->ptr)
			{	
				temp3=temp1;
			
				for(k=0,temp2=temp2->ptr,temp3=temp3->ptr;k<current-1;k++,temp2=temp2->ptr,temp3=temp3->ptr)
				{
					//	printf("\n t2= %d t3=%d ",temp2->value,temp3->value);
					if(temp2->value!=temp3->value)
					 	break;
					
				}
			//	printf("\n  k= %d ",k);
				if(k==current-1)
				{
					//	printf("\n inside if 1");
					if(list==NULL)
				 	{
				 	//	printf("\n inside if 2");
				 		temp4=temp1;
		 				temp5=create_node1(0);
		 				list=temp5;
		 				for(temp4=temp1->ptr,p=0;p<current;p++,temp4=temp4->ptr)
		 				{
		 					temp5->ptr=create_node1(temp4->value);
		 					temp5=temp5->ptr;
		 				}
		 				temp5->ptr=create_node1(temp2->value);
		 				temp5=temp5->ptr;
		 				temp5->ptr=create_node1(-1);
		 				
		 				temp5=temp5->ptr;
		 			/*	printf("\n list \n");
		 				for(x=0,temp11=list;x<4;x++,temp11=temp11->ptr)
		 				printf("  %d ",temp11->value);*/
		 		
		 			}
		 			else
		 			{
		 			//	printf("\n inside if else 2");
		 				temp5->ptr=create_node1(0);
		 			//	printf("\n inside if else 2 2");
		 				temp5=temp5->ptr;
		 				
		 				for(temp4=temp1->ptr,p=0;p<current;p++,temp4=temp4->ptr)
		 				{
		 					temp5->ptr=create_node1(temp4->value);
		 				//	printf("\n inside if else for loop ");
		 					temp5=temp5->ptr;
		 				}
		 				//	printf("\n inside if else 2 after for ");
		 				temp5->ptr=create_node1(temp2->value);
		 				temp5=temp5->ptr;
		 				temp5->ptr=create_node1(-1);
		 				temp5=temp5->ptr;
		 				//temp5=temp5->ptr;
		 				
		 				
		 		/*		for(temp11=list;temp11!=temp5;temp11=temp11->ptr)
		 				printf("  %d ",temp11->value);
		 				printf("  %d ",temp11->value);*/
		 			}
					//printf("\n outside ifelse ");
					temp2=temp2->ptr;
				}
				else
				{
					for(;temp2->value!=-1;temp2=temp2->ptr);
				}
				
				
			}
		
		
	}
	if(list!=NULL)
	   temp5->ptr=NULL;
		
	return list;
}

struct node  *create_node1(int val){
  
     struct node *element;
     element= (struct node *)malloc(sizeof(struct node));
     
     element->value=val;
    
     element->ptr=NULL;
   
     return element;
  } 
  
 struct node *scan_transaction1(int count,struct node *transactions[])
 {
 	
 	int i,j;
	struct node * temp,*temp1,*list=NULL,*temp2,*temp3,*temp4,*temp5;
 	
 	for(i=0;i<no_items;i++)
 	{
 		
 		if(list==NULL)
		 	{
		 		temp2=create_node1(0);
		 		temp2->ptr=create_node1(i+1);
		 		temp2->ptr->ptr=create_node1(-1);
		 		list=temp2;
		 		temp3=temp2->ptr->ptr;
		 		
		 	}
		 	else
		 	{
		 		temp2=create_node1(0);
		 		temp2->ptr=create_node1(i+1);
		 		temp2->ptr->ptr=create_node1(-1);	
		 		temp3->ptr=temp2;
		 		temp3=temp2->ptr->ptr;
		 	}
 		for(j=0;j<count;j++)
 		{
 			for(temp1=transactions[j];temp1!=NULL;temp1=temp1->ptr)
 			 {
 			 	if(temp1->value==(i+1))
 			 	  temp2->value++;
 			 	
 			 }
 				
 		}
 	   	
 	}
 	return list;
 }  
struct node *scan_transaction(int count,struct node *transactions[])
{
	int i;
	struct node * temp,*temp1,*list=NULL,*temp2,*temp3,*temp4,*temp5;
	//temp=(struct node *)malloc(sizeof(struct node));
	
	for(i=0;i<count;i++)
	{
		for(temp1=transactions[i];temp1!=NULL;temp1=temp1->ptr)
		 {
		 	if(list==NULL)
		 	{
		 		temp2=create_node1(1);
		 		temp2->ptr=create_node1(temp->value);
		 		temp2->ptr->ptr=create_node1(-1);
		 		list=temp2;
		 		temp3=temp2->ptr->ptr;
		 		
		 	}
		 	else{
		 		for(temp4=list;temp4!=NULL;temp4=temp4->ptr)
		 		{
		 			temp5=temp4;
		 			temp4=temp4->ptr;
		 			if(temp->value==temp4->value)
		 			 {
		 			 	temp5->value++;
		 			 	break;
		 			 }
		 			temp4=temp4->ptr; 
		 			
		 		}
		 		if(temp4==NULL)
		 		{
		 			temp2=create_node1(1);
		 			temp2->ptr=create_node1(temp->value);
		 			temp2->ptr->ptr=create_node1(-1);
		 			temp3->ptr=temp2;
		 			temp3=temp2->ptr->ptr;
		 			
		 		}
		 		
		 		
		 	}
		 	
		 	
		 }
		
	}
	
	
	return list;
}
