#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define KRED  "\e[31m"
#define KNRM  "\e[0m"
#define KBLU  "\e[34m"
#define KCYN  "\e[36m"
#define KWHT  "\e[37m"
#define HIDE "\e[8m"
#define BOLD "\e[1m"
#define DIM "\e[2m"
typedef struct maze
{
	int block;

	struct maze *up;
	struct maze *down;
	struct maze *left;
	struct maze *right;
	struct maze *head;

}labirinth;
void insert(labirinth **header,int choice);
void construct_lab(labirinth ***main_header, const int rows, const int cols);
void deconstruct_lab(labirinth **header);
void size_lab(FILE *fp, int *rows, int *cols);
void fill_lab(labirinth **header,FILE *fp, const int rows, const int cols);
int path(labirinth *header, labirinth **runner, int check);
void show(labirinth *header);
void delay_sec(double sec);
int main(void)
{
	FILE *fp;
	int i, j, rows=0, cols=0, check=0;
	labirinth *header=NULL, **head=(&header), *tmp_1=NULL, *tmp_2=NULL;

	if (!(fp=fopen("lab_module1.txt","r")))//Calculating the size of the text's labirinth
	{	
		printf(KRED);
		for(i=0;i<8;i++)
			printf(" ");

		printf("\n~Error opening the file lab_module.txt~");
		printf(KNRM);
		printf(KCYN);
		printf("\n\n\nPress ENTER key to Continue...\n");
		printf(HIDE);
		getchar();
		printf(KNRM);
		while(getchar()!='\n');
	}
	else
	{
		size_lab(fp,&rows,&cols);
		fclose(fp);
	}

	construct_lab(&head,rows,cols);

	tmp_1=header->down;

	if (!(fp=fopen("lab_module1.txt","r")))
	{	
		printf(KRED);
		for(i=0;i<8;i++)
			printf(" ");

		printf("\n~Error opening the file lab_module.txt~");
		printf(KNRM);
		printf(KCYN);
		printf("\n\n\nPress ENTER key to Continue...\n");
		printf(HIDE);
		getchar();
		printf(KNRM);
		while(getchar()!='\n');
	}
	else
	{
		fill_lab(&header,fp,rows,cols);
		fclose(fp);
		printf(KCYN);
		printf("\n\nPress ENTER key to Continue...\n");
		printf(HIDE);
		getchar();
		printf(KNRM);
		while(getchar()!='\n');
	}

	tmp_1=header;

	check=path(header,&header,1);

	header=tmp_1;

	deconstruct_lab(&header);

	if(check)
		printf("The exit has been found!!!\n\n");
	else
		printf("The exit hasn't been found!!!\n\n");

	

return 0;

}
void insert(labirinth **header,int choice)
{
	labirinth *previousPtr=NULL, *new=NULL, *currentPtr=NULL;

	new=(labirinth*)malloc(sizeof(labirinth));/*Creating new block*/

	if(new==NULL)
	{
		printf("Not enough space\n");

		return;
	}

	new->block=0;
	new->left=NULL;
	new->right=NULL;
	new->up=NULL;
	new->down=NULL;
	new->head=(*header);

	previousPtr=NULL;
	currentPtr=(*header);

	switch(choice)
	{
		case 1:
		{
			while(currentPtr!=NULL)
			{
				previousPtr=currentPtr;         
				currentPtr=currentPtr->right;  
			}

			if(previousPtr==NULL)
			{ 
				(*header)=new;
				(*header)->head=new;
			}
			else
			{
				previousPtr->right=new;
				new->left=previousPtr;
				new->right=currentPtr;
			}
		}
		break;
		case 2:
		{
			if(currentPtr==NULL)
				exit(1);

			currentPtr->down=new;
			new->up=currentPtr;
			new->head=new;
		}
		break;
		default:
		break;
	}

	return;
}
void construct_lab(labirinth ***main_header, const int rows, const int cols)
{
	int i, j;
	labirinth **header=(*main_header), *head=NULL, *tmp=NULL;

	for(i=0;i<rows;i++)/*Creating the labirinth*/
	{
		if(i!=0)
		{
			insert(header,2);//Creating the down block
			(*header)=(*header)->down;

			for(j=1;j<cols;j++)/*Creating the double linked row*/
				insert(header,1);
		}



		if(i==0)
		{
			for(j=0;j<cols;j++)/*Creating the double linked row*/
				insert(header,1);

			head=(*header);/*Saving the original header of the labirinth*/
		}
	}

	(*header)=head;/*Restoring the original header of the labirinth*/

	tmp=head->down;

	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			if(j!=0)
			{
				head->down=tmp;//Linking the up and down the blocks
				tmp->up=head;
			}


			head->down=tmp;/*Linking the up and down the blocks*/
			tmp->up=head;

			if((tmp->right==NULL)||(head->right==NULL))/*Checking if the blocks are the last ones*/
				break;

			head=head->right;/*Moving one block ahead to the right*/
			tmp=tmp->right;
		}

		head=head->head;/*Making the pointers look at the start of its row*/
		tmp=tmp->head;

		if(tmp->down==NULL)/*Checking if the blocks are the last ones*/
			break;

		head=head->down;/*Moving one block down*/
		tmp=tmp->down;
	}

	return;
}
void size_lab(FILE *fp, int *rows, int *cols)
{
	labirinth **tmp=NULL;
	char *str=NULL, *c=NULL;
	int i;

	(*rows)=(*cols)=0;

	str=(char*)malloc(sizeof(char)*1000);

	if(str==NULL)
	{
		printf("\n%33s\n","No Memory Available");
		exit(1);
	}

	while (!feof(fp))
	{
		(*cols)=0;	

		fscanf(fp,"%[^\n]\n",str);

		//printf("%s\n",str);

		c=strtok(str, " ,	");
		
		(*cols)++;
		(*rows)++;

		while ((c=strtok(NULL," ,	"))!=NULL)
			(*cols)++;
	}

	free(str);

	return;
}
void fill_lab(labirinth **header,FILE *fp, const int rows, const int cols)
{
	int i, j;
	labirinth *tmp_1=NULL, *tmp_2=NULL,**tmp=NULL;
	char *str=NULL, *c=NULL;

	str=(char*)malloc(sizeof(char)*1000);

	if(str==NULL)
	{
		printf("\n%33s\n","No Memory Available");
		exit(1);
	}

	tmp_1=tmp_2=(*header);

	while (!feof(fp))
	{
		fscanf(fp,"%[^\n]\n",str);

		c=strtok(str, " ,	");

		tmp_1->block=atoi(c);

		tmp_1=tmp_1->right;

		while ((c=strtok(NULL," ,	"))!=NULL)
		{
			tmp_1->block=atoi(c);

			tmp_1=tmp_1->right;
		}

		tmp_1=tmp_2->head;

		tmp_1=tmp_1->down;

		tmp_2=tmp_1;
	}

	free(str);

	return;
}
int path(labirinth *header, labirinth **runner, int check)
{
	if(check==1)
	{
		while((*runner)->block != 0)
		{
			(*runner)=(*runner)->down;
		}

		if((*runner)==NULL)
		{
			printf("Can't found entrance!\n");
			return 0;
		}

		(*runner)->block = 2;
		(*runner)=(*runner)->right;
	}

	(*runner)->block = -1;

	system("clear");

	printf("\n\n%31s","~Finding the path~\n");

	show(header);

	delay_sec(0.17);

	(*runner)->block = 2;

	if(((*runner)->right == NULL)||((*runner)->left == NULL)||((*runner)->up == NULL)||((*runner)->down == NULL))
		return 1;

	if ((*runner)->right->block == 0)
	{
		if(path(header,&((*runner)->right),0))
			return 1;
	}
	if ((*runner)->left->block == 0)
	{
		if(path(header,&((*runner)->left),0))
			return 1;
	}
	if ((*runner)->up->block == 0)
	{
		if(path(header,&((*runner)->up),0))
			return 1;
	}
	if ((*runner)->down->block == 0)
	{
		if(path(header,&((*runner)->down),0))
			return 1;
	}

	return 0;

}
void show(labirinth *header)//Printing the blocks
{
	labirinth *tmp_1=NULL, *tmp_2=NULL;

	printf("\n\n");

	tmp_2=header;

	tmp_1=tmp_2;

	do
	{
		do
		{
			printf("%1c",' ');

			if(tmp_1->block==1)
			{
				printf(BOLD);	
				printf(KWHT);

				printf("%d",tmp_1->block);
			}
			else if(tmp_1->block==0)
			{
				printf(HIDE);

				printf("%d",tmp_1->block);
			}
			else if(tmp_1->block==2)
			{
				printf(KBLU);
				printf(DIM);

				printf("%1c",'x');
			}
			else
			{
				printf(KBLU);
				printf(BOLD);

				printf("%1c",'@');

			}

			tmp_1=tmp_1->right;

			printf(KNRM);

		}while(tmp_1 != NULL);

		printf("\n");

		tmp_1=tmp_2->head;
		tmp_1=tmp_1->down;
		tmp_2=tmp_2->down;

	}while(tmp_1 != NULL);

	printf("\n\n");

	return;
}
void deconstruct_lab(labirinth **header)
{
	labirinth *tmp1=NULL, *tmp2=NULL;

	tmp1=tmp2=(*header);

	while(tmp2!=NULL)
	{
		tmp1=tmp2;
		tmp2=tmp2->down;

		while(tmp1!=NULL)
		{
			free(tmp1);
			tmp1=tmp1->right;
		}
	}

	return;
}
void delay_sec(double sec)
{
	double a=time(NULL), b;

	sec*=35;

	for(b=time(NULL);(b-a)<sec;b+=0.000001)
	{
		printf("%c",'\0');
	}

}

