#include <stdlib.h>
#include <stdio.h>

void num2CN(int,char *);

int main()
{
	char *CN=(char *)malloc(sizeof(char)*20);
	num2CN(-25,CN);

	printf("%s\n",CN);

return 0;
}

void num2CN(int number,char *cn)
{	
	int i;
	int a,b;
	char * CN_BUF[11];
	char *ones=(char *)malloc(sizeof(char)*4);
	char *tens=(char *)malloc(sizeof(char)*4);
	char CN[20];

for(i=0;i<11;i++)
	{
		CN_BUF[i]=(char *)malloc(sizeof(char)*4);
		
		if(i== 0)
			strcpy(CN_BUF[0],"��");
		if(i==1)
			strcpy(CN_BUF[1],"һ");
		if(i==2)
			strcpy(CN_BUF[2],"��");
		if(i==3)
			strcpy(CN_BUF[3],"��");
	    if(i==4)
			strcpy(CN_BUF[4],"��");
		if(i==5)
			strcpy(CN_BUF[5],"��");
		if(i==6)
			strcpy(CN_BUF[6],"��");
		if(i==7)
			strcpy(CN_BUF[7],"��");
		if(i==8)
			strcpy(CN_BUF[8],"��");
		if(i==9)
			strcpy(CN_BUF[9],"��");
		if(i==10)
			strcpy(CN_BUF[10],"ʮ");
	}
	
		memset(CN,'\0',20);
	
	if(number>9)
	{
		a=number/10;//ȡʮλ��
		b=number%10;//ȡ��λ��
		
		strcpy(ones,CN_BUF[b]);
		strcpy(tens,CN_BUF[a]);
		strcat(CN,tens);
		strcat(CN,"ʮ");
		strcat(CN,ones);
	}
	else if(number>-1)
	{
		b=number%10;
		strcpy(CN,CN_BUF[b]);
	}
	else if(number>-10)
	{
		number=number*(-1);
		b=number%10;
		strcpy(CN,"��");
		strcat(CN,CN_BUF[b]);
		
	}else
	{
		number=number*(-1);
		a=number/10;
		b=number%10;

		strcpy(ones,CN_BUF[b]);
		strcpy(tens,CN_BUF[a]);
		strcpy(CN,"��");
		strcat(CN,tens);
		strcat(CN,"ʮ");
		strcat(CN,ones);
		strcat(CN,"��");
	}

	strcpy(cn,CN);
}
