#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MEMBER_INFO_FILE "data/member_info.dat"
#define MEMBER_ID_BASE 10001



typedef struct
{
	unsigned int uid;
	char name[50];
	char card_num[6];
       	int sex;
	//int day;
	unsigned int phone;
	int yhzt; //会员状态
	time_t time; //占8个字节
	//unsigned int nums;	
}User;

void prf(User* user);
void operate();
void chax(int);
void del();
void cat_dl();
void zhuc();
void admin();
int verify(unsigned int a);
void adduser(User newuser);
static unsigned int get_new_uid(void);

void change1(unsigned int a);



int main()
{
		
	int a;
	while(1)
	{
		system("clear");	
		printf("  欢迎来到小浔体育馆！\n");
		printf("-----------------------\n");
		printf("请在空白处输入以下数字进行选择！\n");
		printf("1.会员登录\n");
		printf("2.注册新用户\n");
		printf("3.注销会员\n");
		
		scanf("%d",&a);
		if(a==2)
		{
			zhuc();
		}	
		else if(a==1)
		{
			cat_dl();
			
		}	
		else if(a==3)
		{
			del();
		}
		else if(a ==0)
		{
			admin();
		}
		else{
		printf("输入有误，请重新输入！");	
		}
	}
	return 0;
}

//注册新用户
void zhuc()
{
	User user;
	printf("请输入您的姓名：");
	scanf("%s",user.name);
	printf("请输入您的性别,1.男,2.女:");
	scanf("%d",&user.sex);
	printf("请输入您的手机号：");
	scanf("%d",&user.phone);
	printf("请输入您的卡号：");
	getchar();
	scanf("%s",user.card_num);
	
	user.yhzt = 1;
	user.time = time(NULL);
	
	user.uid = get_new_uid();
	

	int a=0;
	//int b=0;
	while(a!=1)
	{
	a = verify(user.phone);
	if(a)
	{
		//将用户信息写入文件
		FILE* fp = fopen(MEMBER_INFO_FILE,"ab");
		if(fp == NULL)
		{
			perror("会员注册失败！\n");
		}
		fwrite(&user,sizeof(user),1,fp);
		fclose(fp);
		printf("会员注册成功！\n");
		sleep(3);	
		getchar();
		return;

	}
	else if(a==0)
	{
		printf("您的输入的验证码有误，请重新输入！\n");
		
	}
	
	// 选择重新生成验证码或退出没写
	}
}

//验证码
int verify(unsigned int a)
{
	srand(time(NULL));
	int c = rand()%1000000;
	printf("%d\n",c);
	int b;
	printf("请输入验证码：");
	scanf("%d",&b);
	if(c == b)
	{
		return 1;
	}	
	return 0;
}


//生成新id
static unsigned int get_new_uid(void)
{
        User user;
        unsigned int uid;

        FILE* fp = fopen(MEMBER_INFO_FILE, "rb");

        if(fp == NULL)
        {
                return MEMBER_ID_BASE;
        }

        fseek(fp, -sizeof(user), SEEK_END);

        if(fread(&user, sizeof(user), 1, fp) == 1)
        {
                uid = user.uid + 1;
        }
        else
        {
                uid = MEMBER_ID_BASE;
        }

        fclose(fp);

        return uid;
}

//查询/会员登录
void cat_dl()
{
	User user;
	system("clear");
	printf("请输入手机号：");
	unsigned int a;
	scanf("%u",&a);
	getchar();
	FILE* fp = fopen(MEMBER_INFO_FILE,"rb");
	if(fp == NULL)
	{
		perror("会员登录失败");
		return;
	}
	while(fread(&user,sizeof(user),1,fp)==1)
	{
		if(user.phone == a && user.yhzt == 1)
		{
			printf("\n尊敬的会员，您的以下是您的信息：\n");
			printf("\n");
			printf("会员ID:%u\n",user.uid);
			printf("姓名：%s\n",user.name);
			printf("卡号：%s\n",user.card_num);
			printf("性别：%s\n",user.sex==1 ?"男":"女");
		       	printf("手机号：%u\n",user.phone);

			struct tm* dt;
			dt = localtime(&user.time);
			printf("注册时间:%d年%d月%d日 %02d:%02d\n",dt->tm_year+1900,dt->tm_mon +1,dt->tm_mday,dt->tm_hour,dt->tm_min);
			printf("\n");
			printf("您是否选择入场[Y\\N]：");
			char a;
			scanf("%c",&a);
			if(a=='y' || a=='Y')
			{
				printf("欢迎入场！\n");
				//getchar();
				sleep(3);
				getchar();
				return;
			}	
			else if(a=='n'|| a=='N')
			{
				printf("欢迎下次光临！\n");
				//getchar();
				sleep(3);
				//getchar();
				return;
			}

		}
		else if(user.phone == a && user.yhzt==0)
		{
			printf("您的会员已注销，请重新注册!\n");
			sleep(3);
			//getchar();
			return;
		}
			
	}

	if(feof(fp))
	{
		printf("您没有注册过会员，清先注册\n");
		sleep(3);
	}

}

//删除会员
void del()
{
	system("clear");
	User user;
	unsigned int a;
	printf("请输入您的手机号：");
	scanf("%u",&a);
	getchar();
	if(verify(a))
	{
		FILE* fp = fopen(MEMBER_INFO_FILE,"r+");
		if(fp == NULL)
		{
			perror("会员查询失败");
			return;
		}
		while(fread(&user,sizeof(user),1,fp)==1)
		{
			if(user.phone==a && user.yhzt==1)
			{
				printf("\n尊敬的会员，您的以下是您的信息：\n");
                        printf("\n");
                        printf("会员ID:%u\n",user.uid);
                        printf("姓名：%s\n",user.name);
                        printf("卡号：%s\n",user.card_num);
                        printf("性别：%s\n",user.sex==1 ?"男":"女");
                        printf("手机号：%u\n",user.phone);

                        struct tm* dt;
                        dt = localtime(&user.time);
                        printf("注册时间:%d年%d月%d日 %02d:%02d\n",dt->tm_year+1900,dt->tm_mon +1,dt->tm_mday,dt->tm_hour,dt->tm_min);
                        printf("\n");

			printf("是否确定要注销此会员(Y/N):");
			getchar();
			
			char select;
			scanf("%c",&select);
			if(select == 'Y' || select == 'y')
			{
				user.yhzt = 0;

				fseek(fp, -sizeof(user), SEEK_CUR);

				fwrite(&user, sizeof(user), 1, fp);

				printf("\n会员已注销！\n");
				fclose(fp);
				sleep(3);
				getchar();
				return;
			}

			}
		}
		//fclose(fp);
	}

}

//管理员登录
void admin()
{
	char name[30] = "admin";
	int pass = 123456;
	char name1[30];
	int pass1;
	getchar();
	printf("请输入管理员账号：");
	scanf("%s",name1);
	printf("请输入密码：");
	scanf("%d",&pass1);
	if(!strncmp(name,name1,strlen(name)) && pass1 == pass)
	{
		printf("管理员登录成功！\n");
		sleep(3);
		getchar();
		operate();
		printf("\n退出成功\n");
		return;
		
	}
	printf("账号或密码错误，请重试！\n");
	sleep(3);
	getchar();
}

//管理员操作页面
void operate()
{
	
	//修改，查询，
	while(1){
	system("clear");
	printf("  管理员隐藏界面  \n");
	printf("----------------------\n");
	printf("请在空白处输入以下数字进行操作\n");
	printf("1.查询所以用户信息\n");
	printf("2.查询单个用户信息/修改信息\n");
	printf("3.退出管理员界面\n");
	printf("请输入您要进行的操作：");
	int a;
	scanf("%d",&a);
	if( a==1 || a==2)
	{
		chax(a);
	}
	else if(a==3)
	{
		return;
	}
	}
}

//查询用户
void chax(int a)
{
	//a=1,所以；a=2，个人
	FILE* fp=fopen(MEMBER_INFO_FILE,"rb");
	User user;
	if(a==1)
	{
			
		while(fread(&user,sizeof(user),1,fp)==1)
		{
			printf("\n");
			prf(&user);
			//printf("\n按任意键返回。。\n");
			//system("stty -icanon -echo");
			//getchar();
			//system("stty icanon echo");
			
		}
		printf("\n按任意键返回。。\n");
		getchar();
                system("stty -icanon -echo");
                getchar();
                system("stty icanon echo");

	}
	else if(a==2)
	{
		User user;
		//system("clear");
		printf("请输入手机号：");
		unsigned int a;
		scanf("%u",&a);
		getchar();
		//getchar();
		FILE* fp = fopen(MEMBER_INFO_FILE,"rb");
		if(fp == NULL)
		{
			perror("会员登录失败");
			return;
		}
		while(fread(&user,sizeof(user),1,fp)==1)
		{
			if(user.phone == a)
			{
				printf("用户信息如下：\n");
				prf(&user);
				fclose(fp);
				printf("\n");
				printf("是否对信息进行修改(Y/N):");
				char x;
				scanf("%c",&x);
				if(x=='y' || x=='Y')
				{
					change1(a);
				}
				getchar();
				break;
			}
			
		}
	}
	//fclose(fp);
}

//修改信息
void change1(unsigned int a)
{
	User user1;
	User user;
	printf("\n请重新输入用户信息：\n");
	
	printf("\n请输入您的姓名：");
        scanf("%s",(user1.name));
        printf("请输入您的性别,1.男,2.女:");
        scanf("%d",&(user1.sex));
        printf("请输入您的手机号：");
        scanf("%u",&(user1.phone));
        printf("请输入您的卡号：");
        getchar();
        scanf("%s",(user1.card_num));
	getchar();
        user1.yhzt = 1;
        user1.time = time(NULL);

        //user1.uid = get_new_uid();

	printf("是否确定修改(Y/N):");
	char c;
	scanf("%c",&c);
	if(c=='y' || c=='Y')
	{

		FILE* fp = fopen(MEMBER_INFO_FILE,"r+b");
		if(fp == NULL)
		{
			printf("\n用户信息修改失败，请重试！\n");
			sleep(1);
			return;
		}


		while(fread(&user,sizeof(user),1,fp)==1)
                {
			//printf("1");
			//sleep(1);

			if(user.phone == a)
                        {

				user1.uid = user.uid;
				fseek(fp, -sizeof(user), SEEK_CUR);

				fwrite(&user1, sizeof(user1), 1, fp);
				fclose(fp);
				printf("用户信息修改成功\n");
				sleep(3);

			}
		}
	}



}

//输出用户信息
void prf(User* user)
{
	printf("会员ID:%u\n",user->uid);
                        printf("姓名：%s\n",user->name);
                        printf("卡号：%s\n",user->card_num);
                        printf("性别：%s\n",user->sex==1 ?"男":"女");
                        printf("手机号：%u\n",user->phone);

                        struct tm* dt;
                        dt = localtime(&user->time);
                        printf("注册时间:%d年%d月%d日 %02d:%02d\n",dt->tm_year+1900,dt->tm_mon +1,dt->tm_mday,dt->tm_hour,dt->tm_min);
	printf("\n--------------\n");
}
