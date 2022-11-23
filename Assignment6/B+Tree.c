#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
int sw=0;



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
struct node *next;
};

struct search_res
{
struct node *s;
int index;
};

int min=2;
int max=5;
int k=3;

void insert(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof, int price);
void split(struct node **t,int index);
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

struct search_res* pos_search(struct node *t,int id);
struct product second_min(struct node *t);






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
            printf("Enter: \nId,\nProduct_name,\nProduct_type,\nOwner_name,\nYear_Of_Purchase,\nProduct_price :");
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
          
          traverse(root);        break;
      default:
          printf("invalid choice\n");
      }
   }
 
return 0;
}



void insert(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof,int price)
{
  int i,total;
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
     temp->next=NULL;
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
  
void split(struct node **t,int index)
{
  struct node *z,*y;int total,j;
  z=(struct node *)malloc(sizeof(struct node));
  z->a=(struct product *)malloc(max*sizeof(struct product));
  z->c=(struct node **)malloc((max+1)*sizeof(struct node *));
  for(j=0;j<max;j++)
      z->c[j]=NULL;

  y=(*t)->c[index];
  z->leaf=y->leaf;

 if(y->leaf==1)                                  
  {
   z->next=y->next;
   y->next=z;
   for(j=0;j<min+1;j++)
     z->a[j]=y->a[j+k-1];
  y->n=min;
  z->n=min+1;
  }

 else
  {                                             
   for(j=0;j<min;j++)
     z->a[j]=y->a[j+k];
  
      for(j=0;j<min+1;j++)
        z->c[j]=y->c[j+k];
  y->n=min;
  z->n=min;
   }
  

  total=(*t)->n;

  for(j=total+1;j>index+1;j--)
    (*t)->c[j]=(*t)->c[j-1];

   (*t)->c[index+1]=z;

  for(j=total;j>index;j--)
    (*t)->a[j]=(*t)->a[j-1];

   (*t)->a[index]=y->a[min];        
 
  (*t)->n=(*t)->n+1;  
}

void nonfull(struct node **t,int id,char product_name[],char product_type[],char owner_name[],int yof,int price)
{

  int i=(*t)->n-1;

  if((*t)->leaf==1)
    {
      while(i>=0&&id<(*t)->a[i].id)
       {
         (*t)->a[i+1]=(*t)->a[i];
         i--;
       }
     (*t)->a[i+1].id=id;
     (*t)->a[i+1].yof=yof;
     (*t)->a[i+1].price=price;
     strcpy((*t)->a[i+1].product_name,product_name);
     strcpy((*t)->a[i+1].product_type,product_type);
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

struct search_res* pos_search(struct node *t,int id)
{
int i;
struct search_res *req;

req=(struct search_res*)malloc(sizeof(struct search_res));

 if(t==NULL)
   return NULL;
  for(i=0;i<t->n;i++)
   {
     if(t->a[i].id==id)
       {
         req->s=t;
         req->index=i;
         return req;
       }
     if(id<t->a[i].id)
       break;
    }
  pos_search(t->c[i],id);
}


 
void delete_check_root(struct node **t,int id)
{
  int i;
  struct product num;
   struct search_res *succ;
   struct search_res *pre;
  if(*t==NULL)
   {
     printf("tree is empty\n");
      return;
   }
 else if((*t)->n==1)
   {   
      if((*t)->a[0].id!=id&&(*t)->leaf==1)
        {
          printf("not found\n");
          return;
        }
    
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
            succ=pos_search((*t)->c[1],id);
            pre=pos_search((*t)->c[0],num.id);
            succ->s->a[succ->index]=pre->s->a[pre->index];
            del(&((*t)->c[0]),num.id);
          }
        else if(checksuc((*t)->c[1])==1)
          {
           num=(*t)->a[0];
            (*t)->a[0]=second_min((*t)->c[1]);
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
  struct search_res *succ;
   struct search_res *pre;

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
            succ=pos_search((*t)->c[i+1],id);
            pre=pos_search((*t)->c[i],num.id);
            succ->s->a[succ->index]=pre->s->a[pre->index];
              del(&((*t)->c[i]),num.id);
             }
         
          else if(checksuc((*t)->c[i+1])==1)
             {
              num=(*t)->a[i];
             (*t)->a[i]=second_min((*t)->c[i+1]);
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
   {
    printf("not found\n");
  }

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

 if((*t)->c[index]->leaf==0)                                           
 {
   (*t)->c[index]->a[(*t)->c[index]->n]=(*t)->a[index];
   (*t)->c[index]->n++;
   (*t)->c[index]->c[(*t)->c[index]->n]=(*t)->c[index+1]->c[0];

  for(j=1;j<=(*t)->c[index+1]->n;j++)
    (*t)->c[index]->c[(*t)->c[index]->n+j]=(*t)->c[index+1]->c[j];     

   for(j=0;j<(*t)->c[index+1]->n;j++)
      (*t)->c[index+1]->c[j]=(*t)->c[index+1]->c[j+1];
 }
   for(j=0;j<(*t)->c[index+1]->n;j++)
    {
      (*t)->c[index]->a[(*t)->c[index]->n]=(*t)->c[index+1]->a[j];      
      (*t)->c[index]->n++;
    }
 
   for(j=index;j<(*t)->n-1;j++)
    {
     (*t)->a[j]=(*t)->a[j+1];
     (*t)->c[j+1]=(*t)->c[j+2];                                         
    }
   (*t)->c[j+1]=(*t)->c[j+2];
     (*t)->n--;  
   if((*t)->c[index]->leaf==1&&(*t)->c[index]->next!=NULL)
      (*t)->c[index]->next=(*t)->c[index]->next->next;

   

}

void transfer(struct node **t,int x,int sib)
{
 int i;
  if(x<sib)                                                                  
   {
     (*t)->c[sib-1]->a[(*t)->c[sib-1]->n]=(*t)->a[sib-1];                    
     (*t)->c[sib-1]->n++;                                                           
     if((*t)->c[sib]->leaf==0)                                               
       {
        (*t)->c[sib-1]->c[(*t)->c[sib-1]->n]=(*t)->c[sib]->c[0];
        (*t)->a[sib-1]=(*t)->c[sib]->a[0];

        for(i=0;i<(*t)->c[sib]->n-1;i++)
          (*t)->c[sib]->a[i]=(*t)->c[sib]->a[i+1];
     
          for(i=0;i<(*t)->c[sib]->n;i++)
           (*t)->c[sib]->c[i]=(*t)->c[sib]->c[i+1];   
        }
    else                                                                     
      {
        (*t)->a[sib-1]=(*t)->c[sib]->a[1];
        for(i=0;i<(*t)->c[sib]->n-1;i++)
           (*t)->c[sib]->a[i]=(*t)->c[sib]->a[i+1];
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

     if((*t)->c[sib]->leaf==0)                                          
       {
        (*t)->c[sib+1]->a[0]=(*t)->a[sib];
        (*t)->c[sib+1]->n++;
    
        for(i=(*t)->c[sib+1]->n;i>0;i--)
           (*t)->c[sib+1]->c[i+1]=(*t)->c[sib+1]->c[i];
        (*t)->c[sib+1]->c[0]=(*t)->c[sib]->c[(*t)->c[sib]->n];

         (*t)->a[sib]=(*t)->c[sib]->a[(*t)->c[sib]->n-1];
         (*t)->c[sib]->n--;
       }
    else
      {
        (*t)->a[sib]=(*t)->c[sib]->a[(*t)->c[sib]->n-1];              
         (*t)->c[sib+1]->a[0]=(*t)->a[sib];
         (*t)->c[sib+1]->n++;
         (*t)->c[sib]->n--;
      }
     
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

struct product second_min(struct node *t)
{
if(t->leaf==1)
     return t->a[1];
  else if(t->c[0]->leaf==0)
     return second_min(t->c[0]);
  else if(t->c[0]->n==1)
     return t->a[0];
  else
     return t->c[0]->a[1];
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

void traverse(struct node *t)
{
int i;
  if(t==NULL)
  {
    printf("tree is empty\n");
    return;
  }
 while(t->leaf!=1)
   {
      t=t->c[0];
   }
  while(t!=NULL)
    {
      for(i=0;i<t->n;i++)
       printf("%d %s %s %s %d %d\n",t->a[i].id,t->a[i].product_name,t->a[i].product_type,t->a[i].owner_name,t->a[i].yof,t->a[i].price);
    t=t->next;
    }
}


