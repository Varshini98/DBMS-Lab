#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sw=0;
int min=2;
int max=5;
int k=3;

struct product{
int id;
char product_name[20];
char product_type[15];
char owner_name[20];
int yof;
int price;
};

struct node
{
struct product *a;
int leaf;
int n;
struct node **c;
};



void insert(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof, int price);
void split(struct node **t,int i);
void nonfull(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof, int price);
struct product* search(struct node *t,int id);
void del(struct node **t,int id);
struct product getmax(struct node *t);
struct product getmin(struct node *t);
void merge(struct node **t,int i);
void transfer(struct node **t,int x,int y);
int checksuc(struct node *t);
int checkpre(struct node *t);
void delete_check_root(struct node **t,int id);
void traverse(struct node *t);

void insert(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof, int price)
{
  int i;
  struct node *temp,*s;
  if(*t==NULL)
   {
     temp=(struct node *)malloc(sizeof(struct node));
     temp->n=1;
     temp->leaf=1;
     temp->a=(struct product *)malloc(max*sizeof(struct product));
     temp->a[0].id=id;
     temp->a[0].yof=yof;
     temp->a[0].price=price;
     strcpy(temp->a[0].product_name,product_name);
     strcpy(temp->a[0].product_type,product_type);
strcpy(temp->a[0].owner_name,owner_name);
     temp->c=(struct node **)malloc((max+1)*sizeof(struct node *));
     for(i=0;i<max+1;i++)
      temp->c[i]=NULL;
       *t=temp;
    }
  else
    {
      if((*t)->n==max)
       {
         s=(struct node *)malloc(sizeof(struct node));
         s->a=(struct product *)malloc(max*sizeof(struct product));
         s->c=(struct node **)malloc((max+1)*sizeof(struct node *));
         for(i=0;i<max+1;i++)
           s->c[i]=NULL;

         s->c[0]=*t;
          *t=s;

         s->leaf=0;
         s->n=0;                                             
         split(&s,0);
         nonfull(&s,id,product_name,product_type,owner_name,yof,price);
       }
     else
        nonfull(t,id,product_name,product_type,owner_name,yof,price);
    }
}




void split(struct node **t,int i)
{
  struct node *z,*y;int total,j;
  z=(struct node *)malloc(sizeof(struct node));                     
  z->a=(struct product *)malloc(max*sizeof(struct product));
  z->c=(struct node **)malloc((max+1)*sizeof(struct node *));
  for(j=0;j<max;j++)
      z->c[j]=NULL;

  y=(*t)->c[i];
  z->leaf=y->leaf;                                                                                             
                                                                
  for(j=0;j<min;j++)
     z->a[j]=y->a[j+k];

  if(y->leaf==0)
    {
      for(j=0;j<min+1;j++)
        z->c[j]=y->c[j+k];
    }
  y->n=min;
  z->n=min;

  total=(*t)->n;

  for(j=total+1;j>i+1;j--)
    (*t)->c[j]=(*t)->c[j-1];

   (*t)->c[i+1]=z;

  for(j=total;j>i;j--)
    (*t)->a[j]=(*t)->a[j-1];

   (*t)->a[i]=y->a[min];        
 
  (*t)->n=(*t)->n+1;  

}













void nonfull(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof, int price)
{
  int i=(*t)->n-1;

  if((*t)->leaf==1)
    {
      while(i>=0&&id<(*t)->a[i].id)                //1a
       {
         (*t)->a[i+1]=(*t)->a[i];
         i--;
       }
     
       (*t)->a[i+1].id=id;
     (*t)->a[i+1].yof=yof;
     (*t)->a[i+1].price=price;
     strcpy((*t)->a[i+1].product_name,product_name);
     strcpy((*t)->a[i+1].product_type,product_type);
	strcpy((*t)->a[i+1].owner_name,owner_name);
     (*t)->n=(*t)->n+1; 
    }
  else
    {

      while(i>=0&&id<(*t)->a[i].id)
         i--;
       i=i+1;
     
     if((*t)->c[i]->n==max)
       {
        split(t,i);
        if(id>(*t)->a[i].id)
           i++;
       }
     nonfull(&((*t)->c[i]),id,product_name,product_type,owner_name,yof,price);
    }
}




struct product* search(struct node *t,int id)
{
 int i;
 if(t==NULL)
   return NULL;
  for(i=0;i<t->n;i++)
   {
     if(t->a[i].id==id)
      {
         sw=1;
       return &(t->a[i]);
       }
     if(id<t->a[i].id)
       break;
   }
  search(t->c[i],id);
}



void traverse(struct node *t)
{
int i;
 if(t==NULL)
  {
   printf("tree is empty\n");
    return;
  }
  for(i=0;i<t->n;i++)
   {
     if(t->leaf!=1)
       traverse(t->c[i]);
       printf("%d %s %s %s %d %d\n",t->a[i].id,t->a[i].product_name,t->a[i].product_type,t->a[i].owner_name,t->a[i].yof,t->a[i].price);
   }
  if(t->leaf!=1)
    traverse(t->c[i]);
}










void delete_check_root(struct node **t,int id)                   
{
  struct product num;
  if(*t==NULL)
   {
     printf("tree is empty\n");      
      return;
   }
  if((*t)->n==1)                           
   {
   
     if((*t)->a[0].id==id)     
      {
        if((*t)->leaf==1)
         {
           *t=NULL;return;
         }
        else if(checkpre((*t)->c[0])==1)
          {
           num=getmax((*t)->c[0]);
           (*t)->a[0]=num;
            del(&((*t)->c[0]),num.id);
          }
        else if(checksuc((*t)->c[1])==1)
          {
           num=getmin((*t)->c[1]);
            (*t)->a[0]=num;
             del(&((*t)->c[1]),num.id);
          }
        else
          {
             merge(t,0);
             *t=(*t)->c[0];
             del(t,id);
          }
      }
 
   
     else if(id<(*t)->a[0].id)            
           {
             if((*t)->c[0]->n>min)
                del(&((*t)->c[0]),id);
            
            else if((*t)->c[1]->n>min)
               {
                transfer(t,0,1);
                del(&((*t)->c[0]),id);
               }
            else
               {
                  merge(t,0);
                  *t=(*t)->c[0];
                  del(t,id);
               }
           }

      else                                 
           {
             if((*t)->c[1]->n>min)
                del(&((*t)->c[1]),id);
            
            else if((*t)->c[0]->n>min)
               {
                transfer(t,1,0);
                del(&((*t)->c[1]),id);
               }
            else
               {
                  merge(t,0);
                  *t=(*t)->c[0];
                  del(t,id);
               }
           }
       }   
   else                            
    {
     del(t,id);
    }
}










void del(struct node **t,int id)
{
 int i=0,j,flag;
struct product num;

   if(*t==NULL)
    {
    printf("doesn't exist\n");
     return;
    }

   while(i<(*t)->n&&id>(*t)->a[i].id)         
      i++;
 
   if(i<(*t)->n&&id==(*t)->a[i].id)               
      {
       if((*t)->leaf==1)                           
        {
         for(j=i;j<(*t)->n;j++)
           (*t)->a[j]=(*t)->a[j+1];
         (*t)->n--;
        }
       else
        {
          if(checkpre((*t)->c[i])==1)
           {
            num=getmax((*t)->c[i]);
            (*t)->a[i]=num;
            del(&((*t)->c[i]),num.id);
           }
         else if(checksuc((*t)->c[i+1])==1)
           {
            num=getmin((*t)->c[i+1]);
            (*t)->a[i]=num;
            del(&((*t)->c[i+1]),num.id);
           }
         else
           {
             merge(t,i);
            del(&((*t)->c[i]),id);
           }
          }
     }

  else if((*t)->leaf==1)
    printf("not found\n");

  else                                          
   {
     if((*t)->c[i]->n>min)
      {
       del(&((*t)->c[i]),id);
      }
    
     else
      {
        flag=0;
         for(j=0;j<=(*t)->n;j++)
          {
           if((*t)->c[j]->n>min)
            {
             flag=1;break;
            }      
          }

          if(flag==1)
             transfer(t,i,j);
          else
           {
             if(i==(*t)->n)
                  i--;
             merge(t,i);
           }
        del(&(*t)->c[i],id);
      }
  }  
} 






void merge(struct node **t,int index)           
{                                                                                  
 int j;
                                                                      
                                                                   
   (*t)->c[index]->a[(*t)->c[index]->n]=(*t)->a[index];             
                                                                      
   (*t)->c[index]->n++;
   (*t)->c[index]->c[(*t)->c[index]->n]=(*t)->c[index+1]->c[0];  

   for(j=0;j<(*t)->c[index+1]->n;j++)
    {
      (*t)->c[index]->a[(*t)->c[index]->n]=(*t)->c[index+1]->a[j];   
      (*t)->c[index]->n++;
      (*t)->c[index]->c[(*t)->c[index]->n]=(*t)->c[index+1]->c[j+1];  
    }
 
   for(j=index;j<(*t)->n-1;j++)
    {
     (*t)->a[j]=(*t)->a[j+1];               
     (*t)->c[j+1]=(*t)->c[j+2];              
    }
   // (*t)->c[j+1]=(*t)->c[j+2];
     (*t)->n--;
  
}








void transfer(struct node **t,int x,int sib)                         
{
 int i;
  if(x<sib)       
   {
     (*t)->c[sib-1]->a[(*t)->c[sib-1]->n]=(*t)->a[sib-1];        
                                                                   
                                                                        
      
     (*t)->c[sib-1]->n++;
     if((*t)->c[sib]->leaf==0)
        (*t)->c[sib-1]->c[(*t)->c[sib-1]->n]=(*t)->c[sib]->c[0];      

     (*t)->a[sib-1]=(*t)->c[sib]->a[0];                                  
   
    for(i=0;i<(*t)->c[sib]->n-1;i++)
     {
       (*t)->c[sib]->a[i]=(*t)->c[sib]->a[i+1];                        
     }

  if((*t)->c[sib]->leaf==0)                                                 
    {
      for(i=0;i<(*t)->c[sib]->n;i++)
        (*t)->c[sib]->c[i]=(*t)->c[sib]->c[i+1];   
    }

    (*t)->c[sib]->n--;
     sib--;
     if(x!=sib)
       transfer(t,x,sib);
   }
  else if(x>sib)                                                        

                                                                                          
    {
     for(i=(*t)->c[sib+1]->n;i>0;i--)                                           
        (*t)->c[sib+1]->a[i]=(*t)->c[sib+1]->a[i-1];       
     (*t)->c[sib+1]->a[0]=(*t)->a[sib];                   
     (*t)->c[sib+1]->n++;

    if((*t)->c[sib]->leaf==0)
     {    
        for(i=(*t)->c[sib+1]->n;i>0;i--)                                       
           (*t)->c[sib+1]->c[i+1]=(*t)->c[sib+1]->c[i];                        
        (*t)->c[sib+1]->c[0]=(*t)->c[sib]->c[(*t)->c[sib]->n];            
     }

     (*t)->a[sib]=(*t)->c[sib]->a[(*t)->c[sib]->n-1];                  
                                                                                         
     (*t)->c[sib]->n--;
      sib++;
     if(x!=sib)
        transfer(t,x,sib);
   }
}


struct product getmax(struct node *t)
 {
  int length;
  while(t->leaf!=1)
   {
      length=t->n;
      t=t->c[length];
   }
 length=t->n;
  return t->a[length-1];
 }

struct product getmin(struct node *t)
 {
  while(t->leaf!=1)
   {
      t=t->c[0];
   }
  return t->a[0];
 }





int checkpre(struct node *t)
 {
  if(t->n>min)
    return 1;
  else if(t->leaf==1)
    return 0;
  else
  return checkpre(t->c[t->n]);
 }




int checksuc(struct node *t)
 {
   if(t->n>min)
    return 1;
   else if(t->leaf==1)
   return 0;
 else
   return checksuc(t->c[0]);
 }







int main()
{
 int i,ch,yof,id,price;
char product_name[15];
char product_type[10],owner_name[20];
struct product *temp;
  struct node *root=NULL;

 

while(1)
   {
    printf("Enter the Choice: \n 1:Insert \n 2:Search \n 3:Delete = ");
    scanf("%d",&ch);
    if(ch==-1)
      break;
    switch(ch)
     {
      case 0:
          traverse(root);
          printf("\n");
          break;
         
      case 1:
            //insert
            printf(" Enter: \nId,\nProduct_name,\nProduct_type,\nOwner_name,\nYear_Of_Purchase,\nProduct_price :");
            scanf("%d",&id);
            scanf("%s",product_name);          
                    
            scanf("%s",product_type);
		scanf("%s",owner_name);
            scanf("%d",&yof);
            scanf("%d",&price);
              sw=0;
            search(root,id);
            if(sw==1)
              printf("id already exists\n");
             else
            insert(&root,id,product_name,product_type,owner_name,yof,price);
             traverse(root);             break;
           
      case 2:
            //search
           printf("Enter element to search:\n");
           scanf("%d",&id);
             temp=search(root,id);
           if(temp==NULL)
            printf("no such record\n");
           else
            {
             printf("id          :%d\n",temp->id);
             printf("product_name:%s\n",temp->product_name);
             
             printf("product_type:%s\n",temp->product_type);
		printf("owner_name:%s\n",temp->owner_name);
             printf("yof	:%d\n",temp->yof);
             printf("price      :%d\n",temp->price);
            }
            traverse(root);
          break;
     case 3:
          //delete
          printf("Enter element to delete:\n");
          scanf("%d",&id);
          sw=0;
            search(root,id);
            if(sw==0)
              {printf("id doesn't exists\n");}
           else {
          delete_check_root(&root,id);}
          traverse(root);          break;
    case 4:
          //modify
        //   scanf("%d",&id);
        //   scanf("%d",&yof);
         //  modify(&root,id,yof);
          traverse(root);        break;
      default:
          printf("invalid choice\n");
      }
   }
 
return 0;
}



//void modify(struct node **t,int id,int nol);


/*void modify(struct node **t,int id,int nol)
{
  int i;
 if(*t==NULL)
   {
     printf("no such record\n");
     return;
   }
  for(i=0;i<(*t)->n;i++)
   {
     if((*t)->a[i].id==id)
       {
        (*t)->a[i].nol=nol;
        printf("updated\n");
         return;
       }
     if(id<((*t)->a[i]).id)
       break;
   }
  modify(&((*t)->c[i]),id,nol);
}*/
