#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//element of priority queue
struct node
{
  FILE* fp;
  struct node* next;
  struct rec* data;
};

//node data strcture
struct rec
{
    char name[30];
    char time[6];

};
//struct node* initialize_list(struct node *p);
//void initialize_list(struct node **p);
void addfirstentry(struct node **p);
void addentry(struct node **p);
int comparetime(char t1[],char t2[]);
void viewlist(struct node *p);
void delete_entry(struct node **p);
void toptasks(struct node *p,int n);
void viewtask(struct node *p);
void tasksbytime(struct node*p);
void display_structure(struct rec *r, int i);





void display_structure(struct rec *r, int i)
{
  //displays structure elements
  printf("%d. %s\t  %s\n",i,r->name,r->time);

}

void toptasks(struct node *p,int n)   //prints top n tasks from file
{
  int i=1;
  printf("\n\n    TASK NAME\tDEADLINE\n\n");

  while(p!=NULL&&i<=n)
  {
    display_structure(p->data,i);
    i++;
    p=p->next;
  }
}
void tasksbytime(struct node *p)  //displays tasks to be completed before entered time
{
  printf("\nEnter time to see which tasks are to be done by that time:");
  char t[6];   //entered time
  int i=1;
  scanf("%s",t);
  int flag=0;
  printf("\n\n    TASK NAME\tDEADLINE\n\n");

  while(p!=NULL && comparetime(p->data->time,t))
  {
    flag=1;
      display_structure(p->data,i);
      i++;
      p=p->next;
  }
  if(flag==0)
  {
    printf("\n\nNo tasks to be done before given time.");
  }
}
void viewtask(struct node *p)   //view task by name
{
  int flag=0;
  printf("\nEnter the task you want to view: ");
  char t[20];
  scanf("%s", t);
  char *task=t;
  strcat(task, ".txt");   //task file name
  printf("\n\n    TASK NAME\tDEADLINE\n\n");

  while(p!=NULL)
  {
    if(strcmp(p->data->name, task )==0)
    {
      display_structure(p->data,1);
      flag=1;
      break;
    }
    else
    {
      p=p->next;
    }

  }
  if(flag==0)
  {
    printf("\nEntered task not found.");
  }

}

void viewlist(struct node *q)  //prints all tasks
{

  printf("\n\n    TASK NAME\tDEADLINE\n\n");
  int i=1;
  while(q!=NULL)
  {
    display_structure(q->data,i);
    i++;

    q=q->next;

  }
}
int comparetime(char t1[],char t2[])   //returns 1 if t1 is before t2 time
{
    int tvalue1=0;
    int tvalue2=0;

    char hours1[3];
    hours1[0]=t1[0];
    hours1[1]=t1[1];
    hours1[2]='\0';
    char minutes1[3];
    minutes1[0]=t1[3];
    minutes1[1]=t1[4];
    minutes1[2]='\0';

    tvalue1= atoi(hours1)*60+atoi(minutes1);  //calculates total value in minutes (from 00:00 being 0, 1:00 being 60 and so on)

    char hours2[3];
    hours2[0]=t2[0];
    hours2[1]=t2[1];
    hours2[2]='\0';
    char minutes2[3];
    minutes2[0]=t2[3];
    minutes2[1]=t2[4];
    minutes2[2]='\0';

    tvalue2= atoi(hours2)*60+atoi(minutes2);  //calculates total value in minutes (from 00:00 being 0, 1:00 being 60 and so on)

    if(tvalue1<tvalue2)
        return 1;
    else
        return 0;
}


void addentry(struct node **p)
{
    if(*p==NULL)
    {
      //adding first entry
      struct node *record;
      struct rec *newrec= (struct rec*)malloc(sizeof(struct rec));
      record=(struct node*)malloc(sizeof(struct node));
      record->next=NULL;

      printf("\nPlease enter the details:\nEntry name: ");
      scanf("%s",newrec->name);
      printf("\nDeadline time[hh:mm]: ");
      scanf("%s",newrec->time);



      record->data=newrec;
      strcat(record->data->name,".txt");

      record->fp=fopen(record->data->name,"a+");

      //ADDING TO MOTHER file
      FILE *f= fopen("mother.txt","a");
      char filename[20]="";
      strcat(filename,(record->data->name));
      strcat(filename,"\n");
      fputs(filename, f);
      fclose(f);


      FILE *file= fopen(record->data->name,"wb+");
      fwrite(newrec,sizeof(struct rec), 1,file);
      rewind(file);
      struct rec getrec; //to get structure from file and print

      fread(&getrec,sizeof(struct rec), 1,file);

      printf("\nRecord name: %s\tRecorded time: %s",getrec.name, getrec.time);

      if(record->fp==NULL)
      {
        printf("\nSYSTEM ERROR.");
        printf("\nPRESS ANY KEY TO EXIT");
        return ;
      }
      else{
         printf("\nRECORD ADDED SUCCESSFULLY\n\n");
      }

      *p=record;  //head of the queue is created
      fclose(file);
      fclose(record->fp);
    }
    else
    {
        struct node *record;
        struct rec *newrec= (struct rec*)malloc(sizeof(struct rec));
        record=(struct node*)malloc(sizeof(struct node));

        record->next=NULL;
        record->fp=NULL;
        record->data=NULL;

        printf("\nPlease enter the details:\nEntry name: ");
        scanf("%s",newrec->name);

        strcat(newrec->name,".txt");

        //ADDING TO MOTHER file
        FILE *f= fopen("mother.txt", "a");
        char filename[20]="";
        strcat(filename, newrec->name);
        strcat(filename, "\n");
        fputs(filename, f);
        fclose(f);

        printf("\nDeadline time[hh:mm]: ");
        scanf("%s",newrec->time);

        record->data=newrec;

        record->fp=fopen(record->data->name,"w+");
        printf("\nFile created");
        FILE *file= fopen(record->data->name,"wb+");
        fwrite(newrec,sizeof(struct rec), 1,file);
        rewind(file);
        fclose(file);


    struct node *q;
    q=*p;
    struct node *prev;
    prev=NULL;
    while(q!=NULL && comparetime(q->data->time,newrec->time))   //priority wise insertion (based on time)
    {

      prev=q;
      q=q->next;
    }
    if(prev==NULL)
    {
      record->next=q;
      *p=record;
    }
  else
  {
    record->next=prev->next;
    prev->next=record;
  }
    if(record->fp==NULL)
    {
    printf("\nSYSTEM ERROR.");
    printf("\nPRESS ANY KEY TO EXIT");
    return ;
    }
    else
    {
        printf("\nRECORD ADDED SUCCESSFULLY\n\n");
    }
    fclose(record->fp);
  }
}


void delete_entry(struct node **p)  //deletes based on task name
{
  char del[20];
  struct node *q,*prev;
  int flag=0;
  if(*p==NULL)
  {
      printf("\nNo files exist.");

      return;
  }

  printf("\nPlease enter the name of the entry to be deleted: ");
  scanf("%s",del);
  char *delname=del;
  strcat(delname, ".txt");
  prev=NULL;
  q=*p;


  while((q!=NULL)&&(strcmp(q->data->name,delname)!=0))
  {
    prev=q;
    q=q->next;
  }

  if(strcmp(q->data->name,delname)==0)
  {
    flag=1;
    if(remove(q->data->name)==0)
    {
            char curname[20];
            FILE *f=fopen("mother.txt", "r");
            rewind(f);
            FILE *f2=fopen("temp.txt","a");
           while((!feof(f))&&fscanf(f,"%s",curname))
            {
              char *c= curname;
              if(strcmp(c,delname)!=0)
              {
                if(!feof(f))
                fprintf(f2,"%s\n",c);
              }
              else{
                fprintf(f2,"");
              }

            }
            fclose(f);
            remove("mother.txt");
            fclose(f2);
            rename("temp.txt","mother.txt");
            if(prev!=NULL)
            {
              prev->next=q->next; //deleting from our list
            }
            else
            {
              *p=q->next;
            }
            free(q);
            printf("\nDELETED SUCCESFULLY.");
            return;
    }
    else{
        printf("\nDeletion unsuccessful");
        return;
    }

  }
  else
  {
       printf("\nFile not found.\n");

  }
  if(flag==0)
  {
     printf("\nFile not found.\n");
  }


}

int main()
{
  struct node *p=NULL;
  //p=initialize_list(&p);
  FILE *fp= fopen("mother.txt","r");   //stores all names of files (tasks)

  if(fp==NULL)
  {
    printf("\n\nFirst run, mother file not present.\n");
   //First entry into the file and *p is NULL
  }
  else
  {   rewind(fp);
      //Inserting first file name into linked list from mother file
      struct node *record=(struct node*)malloc(sizeof(struct node));
      struct rec newrec;

      //gets file NAME from mother.txt
      char curname[20];
      fscanf(fp,"%s",curname);
      char *c= curname;

      FILE *file= fopen(c,"r");
      //printf("\ncurname= %s ", c);

       if(!file)
       {
          printf("\nError\n");
          exit(0);
        }

        fread(&newrec,sizeof(struct rec), 1,file); //stores all data from curname file in newrec structure.
        //printf("\ngot record= %s ", c);
        record->fp= file;
        record->data=&newrec;
        record->next=NULL;
        //printf("\nrecord   %s\n", record->data->time);
        p=record; //first insertion
        fclose(file);

        struct node *q;
        struct node *prev;

        while(fscanf(fp,"%s",curname)&&!feof(fp))
        {

            struct node *r=(struct node*)malloc(sizeof(struct node));
            struct rec newr;

           //gets file NAME from mother.txt
            char *c= curname;
            //printf("\n inside while?");
            FILE *file= fopen(c, "r");
            fread(&newr,sizeof(struct rec), 1,file); //stores all data from curname file in newrec structure.
            r->fp= file;
            r->data=&newr;
            r->next=NULL;
            //printf("\ndata %s %s *p = %s %s", r->data->name, r->data->time, (p)->data->name, (p)->data->time);
            //p->next=r;
            q=p;
            prev=NULL;

            fclose(file);

            while(q!=NULL && comparetime(q->data->time,r->data->time))
            {
                //  printf("\ncheck= %d",comparetime(q->data->time,r->data->time));
                  prev=q;
                  q=q->next;
                  //viewlist(p);
            }
            if(prev==NULL)
            {
              r->next=p; //insert before First
              p=r;
            }
           else
            {
              r->next=q;
              prev->next=r;
            }
            //viewlist(p);
          strcpy(curname,"");

        }//end of while

        fclose(fp);
      }//end of else

    int ch=0;
    int n;
    while(1)
    {
                printf("\n-------------------------------------------------\n");
                printf("\n\n\t\tMAIN MENU:");
                printf("\n\n\t1. ADD TASK");
                printf("\n\t2. VIEW ALL TASKS");
                printf("\n\t3. VIEW TOP TASK");
                printf("\n\t4. DELETE TASK BY NAME");
                printf("\n\t5. VIEW TASK BY NAME");
                printf("\n\t6. SEE TOP n TASKS");
                printf("\n\t7. TASKS TO BE DONE BEFORE ENTERED TIME");
                printf("\n\t8. EXIT");
                printf("\n\n-------------------------------------------------\n\n");

                printf("\n\n\tENTER YOUR CHOICE:");

                scanf("%d",&ch);
                switch(ch)

                    {

                            case 1:
                            addentry(&p);
                            break;

                            case 2:
                             viewlist(p);
                            break;

                           case 3:
                           printf("\n\nTOP TASK:");
                            toptasks(p, 1);
                            break;

                            case 4:
                            delete_entry(&p);
                            break;

                            case 5:
                            viewtask(p);
                            break;

                            case 6:

                            printf("\nEnter the number of tasks you want to see:");
                            scanf("%d",&n);
                            printf("\n\nThe top %d tasks are: \n", n);
                            toptasks(p,n);
                            break;

                            case 7:
                            tasksbytime(p);
                            break;

                            case 8:
                            printf("\n\n\tTHANK YOU\n");
                            exit(0);


                            default:

                            printf("\nWRONG ENTRY");

                            printf("\nPRESS ANY KEY TO TRY AGAIN");

                            // getch();

                            break;

                    }

                //system("cls");

        }
        exit(0);
        return 0;

}
