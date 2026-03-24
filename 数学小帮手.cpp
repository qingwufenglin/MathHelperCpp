#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<windows.h>
void LoginMenu();//部分函数声明 
void Login();
void Menu();
void Simple();
int PrKey();
typedef struct _message//链表建立部分 
{
	char name[20];
	char grade[20];
	int score;
	struct _message* next;
}Node;
Node* head = NULL;
Node* p = NULL;
Node* ReadFile()
{
	Node* p1, * p2;
	Node* head = NULL;
	FILE* fp;
	p1 = p2 = (Node*)malloc(sizeof(Node));
	fp = fopen("student.txt", "r");
	if (fp == NULL)
		fp = fopen("student.txt", "w");
	while (fscanf(fp, "%s\t%s\t%d\n", p1->name, p1->grade, &p1->score) != EOF)
	{
		if (head == NULL)
			head = p1;
		else
			p2->next = p1;
		p2 = p1;
		p1 = (Node*)malloc(sizeof(Node));
	}
	p2->next = NULL;
	free(p1);
	fclose(fp);
	return head;
}
void SaveFile(char name[], char grade[])
{
	FILE* fp;
	int score = 0;
	fp = fopen("student.txt", "a");
	fprintf(fp, "%s\t%s\t%d\n", name, grade, score);
	fclose(fp);
}
int SearchFile(char name[])
{
	Node* q;
	for (q = head; q != NULL; q = q->next)
	{
		if (strcmp(name, q->name) == 0)
		{
			p = q;
			return 0;
		}
	}
	return 1;
}
void WriteFile()
{
	FILE* fp;
	Node* q;
	fp = fopen("student.txt", "w");
	for (q = head; q != NULL; q = q->next)
	{
		fprintf(fp, "%s\t%s\t%d\n", q->name,q->grade,q->score);
	}
	fclose(fp);
}
void print()
{
	int i; int key;
	Node* pai;
	for (pai = head, i = 1; pai != NULL; pai = pai->next, i++)
	{
		printf("| %d\t%s\t   %6s\t %3d       |\n",i, pai->name, pai->grade, pai->score);
		if (i % 5 == 0)
		{
			printf("|------------------------------------------|\n");
			printf("*------<0> 返回主页------<1> 继续查看------*\n");
			printf("请输入(0~1):");
			while ((key = PrKey()) == 2)
			{
				printf("输入错误!请重新输入:");
			}
			if (key==0)
			{
				system("cls");
				Menu();
			}
			else if(key==1)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|-----------------{排行榜}-----------------|\n");
				printf("|-------姓名--------年级--------对题数-----|\n");
			}
		}
	}
	printf("|------------------------------------------|\n");
	printf("*-------------按任意键返回主页-------------*\n");
	system("pause");
	Menu();
}
void Sort()
{
	Node* q, * m;
	int t;
	char tname[20];
	char tgrade[20];
	for(m=head;m!=NULL;m=m->next)
		for (q = head; q->next != NULL; q = q->next)
		{
			if (q->score < q->next->score)
			{
				t = q->score;
				q->score = q->next->score;
				q->next->score = t;
				strcpy(tname, q->name);
				strcpy(q->name, q->next->name);
				strcpy(q->next->name, tname);
				strcpy(tgrade, q->grade);
				strcpy(q->grade, q->next->grade);
				strcpy(q->next->grade, tgrade);
			}
		}
}
void   add(double x1,double y1,double x2,double y2,double *x,double *y)//复数计算函数 
{
	*x = x1 + x2;
	*y = y1 + y2;
}
void   mul(double x1,double y1,double x2,double y2,double *x,double *y)
{
	*x = x1*x2 - y1*y2;
	*y = x1*y2+ y1*x2;
}
void   sub(double x1,double y1,double x2,double y2,double *x,double *y)
{
	*x = x1 - x2;
	*y = y1 - y2;
}
void   divi(double x1,double y1,double x2,double y2,double *x,double *y)
{
	*x = (x1*x2 + y1* y2) / (x2 * x2+ y2* y2);
	*y = (y1* x2 -x1 * y2) / (x2 * x2+ y2* y2);
}
int MenuKey()//主要功能实现部分 
{
	char key[50];
	scanf("%s", key);
	if (strcmp(key, "1") == 0)
		return 1;
	else if (strcmp(key, "2") == 0)
		return 2;
	else if (strcmp(key, "3") == 0)
		return 3;
	else if (strcmp(key, "0") == 0)
		return 0;
	else
		return 4;
}
int PrKey()
{
	char key[50];
	scanf("%s", key);
	if (strcmp(key, "1") == 0)
		return 1;
	else if (strcmp(key, "0") == 0)
		return 0;
	else
		return 2;
}
void Practice(int truth) 
{
	char name[20];
	strcpy(name,p->name);
	Sort();
	SearchFile(name);
	int k;
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|------------------------------------------|\n");
	printf("|----------本次练习的正确率为%3d%%----------|\n", truth * 10);
	printf("|----------------请再接再厉----------------|\n");
	printf("|------------------------------------------|\n");
	printf("|--------------<0> 返回主页----------------|\n");
	printf("|--------------<1> 继续练习----------------|\n");
	printf("|------------------------------------------|\n");
	printf("*------------------------------------------*\n");
	printf("请输入(0~1):");
	while ((k = PrKey()) == 2)
	{
		system("cls");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|------------------------------------------|\n");
		printf("|----------本次练习的正确率为%3d%%----------|\n", truth * 10);
		printf("|----------------请再接再厉----------------|\n");
		printf("|------------------------------------------|\n");
		printf("|--------------<0> 返回主页----------------|\n");
		printf("|--------------<1> 继续练习----------------|\n");
		printf("|------------------------------------------|\n");
		printf("*------------------------------------------*\n");
		printf("输入错误!请重新输入:");
	}
	if (k == 0)
	{
		system("cls");
		Menu();
	}
	else if (k == 1)
	{
		system("cls");
		Simple();
	}
}
void Simple()
{
	int a, b, c, answer, t, i, truth;
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|------------欢迎来到口算题训练------------|\n");
	printf("|------------%6s的%-6s同学------------|\n", p->grade,p->name);
	printf("|------------------------------------------|\n");
	printf("|---------您目前已累计做对%3d道题----------|\n",p->score);
	printf("|------------------------------------------|\n");
	printf("|-------------按任意键开始训练-------------|\n");
	printf("|------------------------------------------|\n");
	printf("*------------------------------------------*\n");
	system("pause");
	if (strcmp(p->grade, "一年级") == 0)
	{	
		char ch[] = "+-";
		int key;
		truth = 0;
		for(i=1;i<=10;i++)
		{
			system("cls");
			srand((unsigned)time(NULL));
			a = rand() % 10;
			b = rand() % 10;
			c = rand() % 2;
			if (ch[c] == '+')
				answer = a + b;
			else if (ch[c] == '-')
			{
				if (a < b)
				{t=a;a=b;b=t;}
				answer = a - b;
			}
			printf("*--------------【数学小帮手】--------------*\n");
			printf("*------------------第%2d题------------------*\n",i);
			printf("\t\t %d %c %d = ", a, ch[c], b);
			scanf("%d", &key);
			if (key == answer)
			{
				p->score++;
				truth++;
			}
			else
			{
				printf("\t\t  答案错误!\n\t\t正确答案为:%d\n", answer);
				system("pause");
			}
		}
		WriteFile();
		Practice(truth);
	}
	else if (strcmp(p->grade, "二年级") == 0||strcmp(p->grade,"三年级")==0)
		{
			char ch[] = "+-*/";
			int key;
			double t;
			int f;
			truth = 0;
			for (i = 1; i <= 10; i++)
			{
				system("cls");
				srand((unsigned)time(NULL));
				a = rand() % 90+10;
				b = rand() % 90+10;
				c = rand() % 4;
				if (ch[c] == '+')
					answer = a + b;
				else if (ch[c] == '-')
					answer = a - b;
				else if (ch[c] == '*')
					answer = a * b;
				else if (ch[c] == '/')
				{
					a = rand() % 90+10;
					b = rand() % 20+10;
					if(a<b)
					{f = a; a = b; b = f;}
					t = (double)a / b;
					if (fabs(t - (int)t) < 1e-8)
						answer = a / b;
					else
						while (1)
						{
							a--;
							t = (double)a / b;
							if (fabs(t - (int)t) < 1e-8)
							{
								answer = a / b;
								break;
							}
						}
				}
				printf("*--------------【数学小帮手】--------------*\n");
				printf("*------------------第%2d题------------------*\n", i);
				printf("\t\t %d %c %d = ", a, ch[c], b);
				scanf("%d", &key);
				if (key == answer)
				{
					p->score++;
					truth++;
				}
				else
				{
					printf("\t\t  答案错误!\n\t\t正确答案为:%d\n", answer);
					system("pause");
				}
			}
			WriteFile();
			Practice(truth);
		}
	else
	{
		char ch[] = "+-*/";
		double  x, y, an, key,t;
		int x1, y1;
		int f, k;
		int m=0;
		truth = 0;
		for (i = 1; i <= 10; i++)
		{
			if (strcmp(p->grade, "四年级") == 0 || strcmp(p->grade, "五年级") == 0)
				m = 0;
			else if (strcmp(p->grade, "六年级") == 0)
			{
				srand((unsigned)time(NULL));
				m = rand() % 3;
			}
			else if(strcmp(p->grade,"七年级")==0)
			{
				srand((unsigned)time(NULL));
				m = rand() % 5;
			}
			if (m == 0||m==4)
			{
				system("cls");
				srand((unsigned)time(NULL));
				c = rand() % 4;
				x = 10 + 1.0 * (rand() % RAND_MAX) / RAND_MAX * 90;
				y = 10 + 1.0 * (rand() % RAND_MAX) / RAND_MAX * 90;
				x1 = x * 10;
				x = (double)x1 / 10;
				y1 = y * 10;
				y = (double)y1 / 10;
				if (ch[c] == '+')
					an = x + y;
				else if (ch[c] == '-')
					an = x - y;
				else if (ch[c] == '*')
					an = x * y;
				else if (ch[c] == '/')
				{
					if (x < y)
					{
						t = x; x = y; y = t;
					}
					an = x / y;
				}
				f = an * 1000;
				an = (double)f / 10;
				f = (int)(an + 0.5);
				an = (double)f / 100;
				printf("*--------------【数学小帮手】--------------*\n");
				printf("*------------------第%2d题------------------*\n", i);
				printf("\t       %.1lf %c %.1lf = ", x, ch[c], y);
				scanf("%lf", &key);
				if (fabs(key - an - 0.01) < 1e-8|| fabs(key - an + 0.01) < 1e-8|| fabs(key - an) < 1e-8)
				{
					p->score++;
					truth++;
				}
				else
				{
					printf("\t\t  答案错误!\n\t\t正确答案为:%.2lf\n", an);
					system("pause");
				}
			}
			else if (m == 1)
			{
				system("cls");
				srand((unsigned)time(NULL));
				a = rand() % 9 + 1;
				b = rand() % 5;
				answer = pow(a, b);
				printf("*--------------【数学小帮手】--------------*\n");
				printf("*------------------第%2d题------------------*\n", i);
				printf("\t\t%d ^ %d = ", a, b);
				scanf("%d", &k);
				if (k == answer)
				{
					p->score++;
					truth++;
				}
				else
				{
					printf("\t\t  答案错误!\n\t\t正确答案为:%d\n", answer);
					system("pause");
				}
			}
			else if (m == 2)
			{
				system("cls");
				srand((unsigned)time(NULL));
				a = rand() % 1000;
				b = rand() % 500;
				if (a < b)
				{
					t = a; a = b; b = t;
				}
				answer = a % b;
				printf("*--------------【数学小帮手】--------------*\n");
				printf("*------------------第%2d题------------------*\n", i);
				printf("\t\t%d %% %d = ", a, b);
				scanf("%d", &k);
				if (k == answer)
				{
					p->score++;
					truth++;
				}
				else
				{
					printf("\t\t  答案错误!\n\t\t正确答案为:%d\n", answer);
					system("pause");
				}
			}
			else if(m==3)
			{
				system("cls");
				char ch[] = "+-*/";
				double k1=8, k2=0;
				int n;
				double a1, b1;
				srand((unsigned)time(NULL));
				double a = rand() % 10;
				double b = rand() % 10;
				double c = rand() % 10;
				double d = rand() % 10;
				n = rand() % 4;
				if (ch[n] == '+')
					add(a,b,c,d,&a1,&b1);
				if (ch[n] == '-')
					sub(a,b,c,d, &a1, &b1);
				if (ch[n] == '*')
					mul(a,b,c,d, &a1, &b1);
				if (ch[n] == '/')
					divi(a,b,c,d, &a1, &b1);
				t = a1* 100;
				a1= (double)t / 10;
				t = (int)(a1 + 0.5);
				a1 = (double)t / 10;
				t = b1 * 100;
				b1 = (double)t / 10;
				t = (int)(b1 + 0.5);
				b1= (double)t / 10;
				printf("*--------------【数学小帮手】--------------*\n");
				printf("*------------------第%2d题------------------*\n", i);
				printf("         %.1lf+%.1lfi %c %.1lf+%.1lfi = ", a, b, ch[n],c, d);
				scanf("%lf%lf%*c", &k1,&k2);
				if ((fabs(a1-k1)<1e-8||fabs(a1-k1-0.1)<1e-8||fabs(a1-k1+0.1)<1e-8)&&(fabs(k2-b1)<1e-8||fabs(k2-b1-0.1)<1e-8||fabs(k2-b1+0.1)<1e-8))
				{
					p->score++;
					truth++;
				}
				else
				{
					if(b1>0)
						printf("\t\t  答案错误!\n\t  正确答案为:%.1lf+%.1lfi\n", a1,b1);
					else if(fabs(b1)<1e-8)
						printf("\t\t  答案错误!\n\t  正确答案为:%.1lf\n", a1);
					else
						printf("\t\t  答案错误!\n\t  正确答案为:%.1lf%.1lfi\n", a1, b1);
					system("pause");
				}
			}
		}
		WriteFile();
		Practice(truth);
	}
}
void Application()
{
	system("cls");
	int key;
	int a, b, c;
	const double PI = 3.14;
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|------------欢迎来到应用题训练------------|\n");
	printf("|------------%6s的%-6s同学------------|\n", p->grade, p->name);
	printf("|------------------------------------------|\n");
	printf("|------------请选择要训练的题型------------|\n");
	printf("|------<0> 正方形----------<1> 长方形------|\n");
	printf("|------<2> 三角形----------<3> 圆形--------|\n");
	printf("|------------------------------------------|\n");
	printf("*------------------------------------------*\n");
	printf("请输入(0~3):");
	while ((key = MenuKey()) == 4)
	{
		system("cls");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|------------欢迎来到应用题训练------------|\n");
		printf("|------------%6s的%-6s同学------------|\n", p->grade, p->name);
		printf("|------------------------------------------|\n");
		printf("|------------请选择要训练的题型------------|\n");
		printf("|------<0> 正方形----------<1> 长方形------|\n");
		printf("|------<2> 三角形----------<3> 圆形--------|\n");
		printf("|------------------------------------------|\n");
		printf("*------------------------------------------*\n");
		printf("输入错误!请重新输入:");
	}
	if (key == 0||key == 1)
	{
		system("cls");
		int l, m, l1, m1, l2, m2,t;
		srand((unsigned)time(NULL));
		a = rand() % 24+1;
		b = rand() % 19+1;
		l1 = 4 * a;
		m1= a * a;
		l2 = 2 * a + 2 * b;
		m2 = a * b;
		if (key == 0)
		{
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|----请计算边长为%2d的正方形的周长和面积----|\n", a);
			system("pause");
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|----请计算边长为%2d的正方形的周长和面积----|\n", a);
			printf("|请输入该正方形的周长:");
			scanf("%d", &l);
			printf("|请输入该正方形的面积:");
			scanf("%d", &m);
		}
		else
		{
			if(a<b)
			{t = a; a = b; b = t;}
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|--请计算长为%2d宽为%2d的长方形的周长和面积--|\n", a,b);
			system("pause");
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|--请计算长为%2d宽为%2d的长方形的周长和面积--|\n", a,b);
			printf("|请输入该长方形的周长:");
			scanf("%d", &l);
			printf("|请输入该长方形的面积:");
			scanf("%d", &m);
		}
		system("pasue");
		if (key == 0)
		{
			if (l == l1 && m == m1)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------恭喜你!答对了!--------------|\n");
				printf("|----------------请再接再厉----------------|\n");
				printf("|------------------------------------------|\n");
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("请输入(0~1)");
				while ((key = PrKey()) == 2)
				{
					system("cls");
					printf("*--------------【数学小帮手】--------------*\n");
					printf("|------------------------------------------|\n");
					printf("|--------------恭喜你!答对了!--------------|\n");
					printf("|----------------请再接再厉----------------|\n");
					printf("|------------------------------------------|\n");
					printf("|--------------<0> 返回主页----------------|\n");
					printf("|--------------<1> 继续练习----------------|\n");
					printf("|------------------------------------------|\n");
					printf("*------------------------------------------*\n");
					printf("输入错误!请重新输入:");
				}
				if (key == 0)
				{
					system("cls");
					Menu();
				}
				else
				{
					system("cls");
					Application();
				}
			}
			else
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------很遗憾!答错了!--------------|\n");
				printf("|------------------答案:-------------------|\n");
				printf("|------------周长:%2d----面积:%3d-----------|\n", l1, m1);
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("请输入(0~1):");
				while ((key = PrKey()) == 2)
				{
					system("cls");
					printf("*--------------【数学小帮手】--------------*\n");
					printf("|------------------------------------------|\n");
					printf("|--------------很遗憾!答错了!--------------|\n");
					printf("|------------------答案:-------------------|\n");
					printf("|------------周长:%2d----面积:%3d-----------|\n", l, m);
					printf("|--------------<0> 返回主页----------------|\n");
					printf("|--------------<1> 继续练习----------------|\n");
					printf("|------------------------------------------|\n");
					printf("*------------------------------------------*\n");
					printf("输入错误!请重新输入:");
				}
				if (key == 0)
				{
					system("cls");
					Menu();
				}
				else
				{
					system("cls");
					Application();
				}
			}
		}
		else
		{
			if (l == l2 && m == m2)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------恭喜你!答对了!--------------|\n");
				printf("|----------------请再接再厉----------------|\n");
				printf("|------------------------------------------|\n");
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("请输入(0~1)");
				while ((key = PrKey()) == 2)
				{
					system("cls");
					printf("*--------------【数学小帮手】--------------*\n");
					printf("|------------------------------------------|\n");
					printf("|--------------恭喜你!答对了!--------------|\n");
					printf("|----------------请再接再厉----------------|\n");
					printf("|------------------------------------------|\n");
					printf("|--------------<0> 返回主页----------------|\n");
					printf("|--------------<1> 继续练习----------------|\n");
					printf("|------------------------------------------|\n");
					printf("*------------------------------------------*\n");
					printf("输入错误!请重新输入:");
				}
				if (key == 0)
				{
					system("cls");
					Menu();
				}
				else
				{
					system("cls");
					Application();
				}
			}
			else
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------很遗憾!答错了!--------------|\n");
				printf("|------------------答案:-------------------|\n");
				printf("|------------周长:%2d----面积:%3d-----------|\n", l2, m2);
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("请输入(0~1):");
				while ((key = PrKey()) == 2)
				{
					system("cls");
					printf("*--------------【数学小帮手】--------------*\n");
					printf("|------------------------------------------|\n");
					printf("|--------------很遗憾!答错了!--------------|\n");
					printf("|------------------答案:-------------------|\n");
					printf("|------------周长:%2d----面积:%3d-----------|\n", l2, m2);
					printf("|--------------<0> 返回主页----------------|\n");
					printf("|--------------<1> 继续练习----------------|\n");
					printf("|------------------------------------------|\n");
					printf("*------------------------------------------*\n");
					printf("输入错误!请重新输入:");
				}
				if (key == 0)
				{
					system("cls");
					Menu();
				}
				else
				{
					system("cls");
					Application();
				}
			}
		}
	}
	else if (key == 2)
	{
		system("cls");
		double  m, p;
		double m1;
		int l, l1,t;
		do
		{
			srand((unsigned)time(NULL));
			a = rand() % 9+1;
			b = rand() % 9+1;
			c = rand() % 9+1;
		} while (a + b <= c || a + c <= b || b + c <= a);
		p = 1.0*(a + b + c) / 2;
		m1 = sqrt(p * (p - a) * (p - b) * (p - c));
		t = m1 * 1000;
		m1 = (double)t / 10;
		t = (int)(m1 + 0.5);
		m1 = (double)t / 100;
		l1 = a + b + c;
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|-请计算三边长为 %d %d %d的三角形的周长和面积-|\n", a,b,c);
		printf("|----------(计算结果保留两位小数)----------|\n");
		system("pause");
		system("cls");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|-请计算三边长为 %d %d %d的三角形的周长和面积-|\n", a, b, c);
		printf("|----------(计算结果保留两位小数)----------|\n");
		printf("|请输入该三角形的周长:");
		scanf("%d", &l);
		printf("|请输入该三角形的面积:");
		scanf("%lf", &m);
		system("pause");
		if (l == l1 && fabs(m - m1) < 1e-8)
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|--------------恭喜你!答对了!--------------|\n");
			printf("|----------------请再接再厉----------------|\n");
			printf("|------------------------------------------|\n");
			printf("|--------------<0> 返回主页----------------|\n");
			printf("|--------------<1> 继续练习----------------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			printf("请输入(0~1)");
			while ((key = PrKey()) == 2)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------恭喜你!答对了!--------------|\n");
				printf("|----------------请再接再厉----------------|\n");
				printf("|------------------------------------------|\n");
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("输入错误!请重新输入:");
			}
			if (key == 0)
			{
				system("cls");
				Menu();
			}
			else
			{
				system("cls");
				Application();
			}
		}
		else
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|--------------很遗憾!答错了!--------------|\n");
			printf("|------------------答案:-------------------|\n");
			printf("|-----------周长:%2d----面积:%.2lf-----------|\n", l1, m1);
			printf("|--------------<0> 返回主页----------------|\n");
			printf("|--------------<1> 继续练习----------------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			printf("请输入(0~1):");
			while ((key = PrKey()) == 2)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------很遗憾!答错了!--------------|\n");
				printf("|------------------答案:-------------------|\n");
				printf("|-----------周长:%2d----面积:%.2lf-----------|\n", l1, m1);
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("输入错误!请重新输入:");
			}
			if (key == 0)
			{
				system("cls");
				Menu();
			}
			else
			{
				system("cls");
				Application();
			}
		}
	}
	else if (key == 3)
	{
		system("cls");
		srand((unsigned)time(NULL));
		a = rand() % 14 + 1;
		double l, l1, m1, m;
		int t;
		l = 2 * PI * a;
		m = PI * a * a;
		t = l * 1000;
		l = (double)t / 10;
		t = (int)(l + 0.5);
		l = (double)t / 100;
		t = m * 1000;
		m = (double)t / 10;
		t = (int)(m + 0.5);
		m = (double)t / 100;
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|-----请计算半径为%2d的圆形的周长和面积-----|\n", a);
		printf("|-----(PI取3.14，计算结果保留两位小数)-----|\n");
		system("pause");
		system("cls");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|-----请计算半径为%2d的圆形的周长和面积-----|\n", a);
		printf("|-----(PI取3.14，计算结果保留两位小数)-----|\n");
		printf("|请输入该圆形的周长:");
		scanf("%lf", &l1);
		printf("|请输入该圆形的面积:");
		scanf("%lf", &m1);
		system("pause");
		if (fabs(l-l1)<1e-8 && fabs(m - m1) < 1e-8)
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|--------------恭喜你!答对了!--------------|\n");
			printf("|----------------请再接再厉----------------|\n");
			printf("|------------------------------------------|\n");
			printf("|--------------<0> 返回主页----------------|\n");
			printf("|--------------<1> 继续练习----------------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			printf("请输入(0~1)");
			while ((key = PrKey()) == 2)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------恭喜你!答对了!--------------|\n");
				printf("|----------------请再接再厉----------------|\n");
				printf("|------------------------------------------|\n");
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("输入错误!请重新输入:");
			}
			if (key == 0)
			{
				system("cls");
				Menu();
			}
			else
			{
				system("cls");
				Application();
			}
		}
		else
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|--------------很遗憾!答错了!--------------|\n");
			printf("|------------------答案:-------------------|\n");
			printf("|--------周长:%5.2lf----面积:%6.2lf---------|\n", l, m);
			printf("|--------------<0> 返回主页----------------|\n");
			printf("|--------------<1> 继续练习----------------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			printf("请输入(0~1):");
			while ((key = PrKey()) == 2)
			{
				system("cls");
				printf("*--------------【数学小帮手】--------------*\n");
				printf("|------------------------------------------|\n");
				printf("|--------------很遗憾!答错了!--------------|\n");
				printf("|------------------答案:-------------------|\n");
				printf("|--------周长:%5.2lf----面积:%6.2lf---------|\n", l, m);
				printf("|--------------<0> 返回主页----------------|\n");
				printf("|--------------<1> 继续练习----------------|\n");
				printf("|------------------------------------------|\n");
				printf("*------------------------------------------*\n");
				printf("输入错误!请重新输入:");
			}
			if (key == 0)
			{
				system("cls");
				Menu();
			}
			else
			{
				system("cls");
				Application();
			}
		}
	}
}
int main()//主函数及界面设计 
{
	//system("color 03");
	head = ReadFile();
	LoginMenu();
	Login();
	Menu();
	return 0;
}
void LoginMenu()
{
	time_t tt_;
	char output[20];
	struct tm* tm_;
	time(&tt_);
	tm_ = localtime(&tt_);
	strftime(output, 9, "%H:%M:%S", tm_);
	printf("*--------------【数学小帮手】--------------*\n");\
	printf("|------------------------------------------|\n");
	printf("|------------当前时间--%s------------|\n", output);
	printf("|------------------------------------------|\n");
	printf("|------------------------------------------|\n");
	printf("|------------欢迎来到数学小帮手------------|\n");
	printf("|----------请按任意键进入登陆界面----------|\n");
	printf("|------------------------------------------|\n");
	printf("*------------------------------------------*\n");
	system("pause");
}
void Login()
{
	char name[20], grade[20];
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|--------请根据提示输入您的个人信息--------|\n");
	printf("*------------------------------------------*\n");
	system("pause");
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|--------请根据提示输入您的个人信息--------|\n");
	printf("*------------------------------------------*\n");
	printf("请输入您的姓名:");
	scanf("%s", name);
	if (SearchFile(name))
	{
		printf("请输入您的年级:");
		scanf("%s", grade);
		SaveFile(name, grade);
		head = ReadFile();
		SearchFile(name);
	}
	Sort();
	SearchFile(name);
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|--------------欢--迎--回--来--------------|\n");
	printf("|----------------%-6s同学----------------|\n",name);
	printf("*------------------------------------------*\n");
	system("pause");
}
void Menu()
{
	int key;
	time_t tt_;
	char output[20];
	struct tm* tm_;
	time(&tt_);
	tm_ = localtime(&tt_);
	strftime(output, 9, "%H:%M:%S", tm_);
	system("cls");
	printf("*--------------【数学小帮手】--------------*\n");
	printf("|------------当前时间--%s------------|\n", output);
	printf("|------------------------------------------|\n");
	printf("|-------------<0>-退出系统-----------------|\n");
	printf("|-------------<1>-口算题训练---------------|\n");
	printf("|-------------<2>-应用题训练---------------|\n");
	printf("|-------------<3>-排行榜-------------------|\n");
	printf("|------------------------------------------|\n");
	printf("*------------------------------------------*\n");
	printf("请输入(0~3):");
	while ((key = MenuKey()) == 4)
	{
		system("cls");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|------------当前时间--%s------------|\n", output);
		printf("|------------------------------------------|\n");
		printf("|-------------<0>-退出系统-----------------|\n");
		printf("|-------------<1>-口算题训练---------------|\n");
		printf("|-------------<2>-应用题训练---------------|\n");
		printf("|-------------<3>-排行榜-------------------|\n");
		printf("|------------------------------------------|\n");
		printf("*------------------------------------------*\n");
		printf("输入错误!请重新输入:");
	}
	switch (key)
	{
	case 0:
		exit(0);
		break;
	case 1:
		if(strcmp(p->grade,"一年级")==0|| strcmp(p->grade, "二年级") == 0|| strcmp(p->grade, "三年级") == 0|| strcmp(p->grade, "四年级") == 0|| strcmp(p->grade, "五年级") == 0|| strcmp(p->grade, "六年级") == 0|| strcmp(p->grade, "七年级") == 0)
		Simple();
		else
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|-------本栏目仅对一~七年级同学开放!-------|\n");
			printf("|----------------%-6s同学----------------|\n",p->name);
			printf("|----------您的年级信息为:%-6s-----------|\n",p->grade);
			printf("|------------------------------------------|\n");
			printf("|------如果您的信息有误请联系管理人员------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			system("pause");
			Menu();
		}
		break;
	case 2:
		if (strcmp(p->grade, "六年级") == 0)
			Application();
		else
		{
			system("cls");
			printf("*--------------【数学小帮手】--------------*\n");
			printf("|------------------------------------------|\n");
			printf("|---------本栏目仅对六年级同学开放!--------|\n");
			printf("|----------------%-6s同学----------------|\n", p->name);
			printf("|----------您的年级信息为:%-6s-----------|\n", p->grade);
			printf("|------------------------------------------|\n");
			printf("|------如果您的信息有误请联系管理人员------|\n");
			printf("|------------------------------------------|\n");
			printf("*------------------------------------------*\n");
			system("pause");
			Menu();
		}
		break;
	case 3:
		system("cls");
		//system("color 70");
		printf("*--------------【数学小帮手】--------------*\n");
		printf("|-----------------{排行榜}-----------------|\n");
		printf("|-------姓名--------年级--------对题数-----|\n");
		print();
		system("pause");
		break;
	}
}
