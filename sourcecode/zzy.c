#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct
{
	int c_custkey;    	   //�˿ͱ��
	char c_mkgsegment[20]; //��Ӧ��ĳ���г�����
}customer;				   //�˿ͽṹ��

typedef struct
{
	int o_orderkey;    	 //������
	int o_custkey;    	 //�˿ͱ��
	char o_orderdate[10];//��������
}orders;				 //����

typedef struct
{
	int l_orderkey;//������
	double l_extendedprice;//����۸�
	char l_shipdate[10];//��������
}lineitem; //��Ʒ��Ϣ

typedef struct
{
	int l_orderkey;//������
	char o_orderdate[10];//��������
	double l_extendedprice;//����۸�
}select_result;


customer * read_customer_txt() //��ȡcustomer��txt����
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(101*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("customer.txt","r");
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);

		i++;
	}
	fclose(fp);
	return a;
}
orders * read_orders_txt()//��ȡorders.txt����
{
	int i =0;
	orders * a=NULL;
	a = (orders * )malloc(4001*sizeof(orders));
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;

		i++;
	}
	fclose(fp);
	return a;
}

lineitem * read_lineitem_txt()//��ȡlineitem.txt����
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{

		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);

		i++;
	}
	fclose(fp);
	return l;
}

select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)//����ѡ��
{
    int a, b, c;  //for cycle
    int sindex=0;  //index of slct_res
    double tmp; //for sort
    select_result * slct_res = NULL;

    slct_res = (select_result *)malloc(1001*sizeof(select_result));

    //create slct_res
    for (a=0; a<100; a++){
        if (strcmp(cus[a].c_mkgsegment, mktsegment)==0){
            for (b=0; b<4000; b++){
                if (cus[a].c_custkey==ord[b].o_custkey && strcmp(ord[b].o_orderdate, order_date)<0){
                    for (c=0; c<1000; c++){
                        if (ord[b].o_orderkey==item[c].l_orderkey && strcmp(item[c].l_shipdate, ship_date)>0){
                            slct_res[sindex].l_extendedprice = item[c].l_extendedprice;
                            slct_res[sindex].l_orderkey = item[c].l_orderkey;
                            strcpy(slct_res[sindex].o_orderdate, ord[b].o_orderdate);
                            sindex++;
                        }
                    }
                }
            }
        }
    }

    //now, sindex is length of slct_res

	//remove repetitive elements
	for (a=0; a<sindex-1; a++){
		for (b=a+1; b<sindex; b++){
			if (slct_res[a].l_orderkey==slct_res[b].l_orderkey && strcmp(slct_res[a].o_orderdate, slct_res[b].o_orderdate)==0){
				slct_res[a].l_extendedprice = slct_res[a].l_extendedprice + slct_res[b].l_extendedprice;
				slct_res[b].l_extendedprice = 0;
			}
		}
	}

    //Bubble_Sort on the basis of l_extendedprice
    for (a=0; a<sindex-1; a++){
        for (b=0; b<sindex-1; b++){
            if (slct_res[b].l_extendedprice < slct_res[b+1].l_extendedprice){
                tmp = slct_res[b+1].l_extendedprice;
                slct_res[b+1].l_extendedprice = slct_res[b].l_extendedprice;
                slct_res[b].l_extendedprice = tmp;
            }
        }
    }

    return slct_res;
}

int change_argv_to_number(char s[])//�������������������ַ���ת��Ϊ��������
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);

		i++;
	}
	return number;
}

int main(int argc,char * argv[])//argc��ʾ�������ݵ��ܸ�����argv[]�ڱ��������������
{
	int i,j;
	int num;

	int limit=3;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	select_result *result=NULL;
	customer * cus = NULL;//ָ��ͻ����ָ��
	orders * ord = NULL;//ָ�򶩵����ָ��
	lineitem * item = NULL;//ָ�� ��Ʒ���ָ��
	cus = read_customer_txt();//��ȡcustomer.txt������ ������ͻ���
	ord = read_orders_txt();//��ȡorders.txt������ �����붩����
	item = read_lineitem_txt();//��ȡlineitem.txt������ �������Ʒ��
	num = change_argv_to_number(argv[4]);//�ܹ�����Ĵ���

	for(i=0;num>0;num--,i=i+4)
	{
		strcpy(mktsegment,argv[5+i]);
		strcpy(ship_date,argv[7+i]);
		strcpy(order_date,argv[6+i]);
		limit = change_argv_to_number(argv[8+i]);
//        printf("%d	mktsegment:%s	order_date:%s	ship_date:%s	limit:%d\n",num,mktsegment,order_date,ship_date,limit);
		result=Select(cus,ord,item,order_date,ship_date,mktsegment);
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(result[j].l_extendedprice==0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",result[j].l_orderkey,result[j].o_orderdate,result[j].l_extendedprice);
		}
		free(result);
		result = NULL;
	}


	return 0;
}
