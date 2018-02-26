#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define M 4   //����ҳ��
#define m 20	//�������ƶ���
#define t 0.5
#define N 20//�����ڴ�ĳߴ�

typedef struct page
{
	int num;
	int time;
	int flag;
}Page;
//����ҳ��

Page mm[M];
int queue1[20],queue2[20],queue3[20],queue4[20];	//��¼�û���ҳ
int A[m],B[m];
int K=0,S=0,T=0,U=0;//�û�ҳ����ı�ʶ
int pos=0;//��¼�����ʱ����
int r;//�������0 �� r �� 1�����r < t����Ϊp����һ����ֵ������p = (p + 1) mod N��

void test()//ҳ������������������
{
    int p = -1;
    int i;

    for(i = 0;i<m;i++)
    {
       /*r = rand()/(double)(RAND_MAX);
        if (r<t) p = rand()%N;
        else p = (p+1)%N;
        A[i] = p;*/
        A[i] = rand()%N;
    }
}


//��ʼ���ڴ�ҳ����洢�ڴ�����Ŀռ�
void INIT(){
	int i;
	for(i=0;i<M;i++){
		mm[i].num =-1;
		mm[i].time =0;
		mm[i].flag =0;
	}
	printf("INIT success\n");
}


//ȡ���ڴ��д���ʱ����õ�λ��
int GetMax(){
	int max=-1;
	int i;
	for(i=0;i<M;i++){
		if(mm[i].time > max){
			max=mm[i].time ;
			pos=i;
		}
	}
	return pos;
}

//����ʱ�䲻ʹ��ҳ��
int longesttime(int fold)
{
	int i;
	int max=-1;
	for(i=fold;i<m;i++){
		if(mm[0].num!=i){
			mm[0].time++;
		}
		if(mm[1].num!=i){
			mm[1].time++;
		}
		if(mm[2].num!=i){
			mm[2].time++;
		}
		if(mm[3].num!=i){
			mm[3].time++;
		}
	}
	for(i=0;i<M;i++){
		if(mm[i].time>max){
			max=mm[i].time;
			pos=i;
		}
	}
	return pos;
}

//���ĳҳ�Ƿ����ڴ�
int Equation(int fold){
	int i;
	for(i=0;i<M;i++){
		if(mm[i].num == fold)
			return i;
	}
	return -1;
}

//��������ڴ��Ƿ�����,-1��������������
int Check(){
	int i;
	for(i=0;i<M;i++){
		if(mm[i].num == -1)
			return i;
	}
	return -1;
}


//�Ƚ��ȳ��㷨
void FIFO(int fold){
	int i;
	int a,b,c;
	a=Equation(fold);
	if(a != -1){}
	else{
		b=Check();
		if(b != -1){
			mm[b].num = fold;
		}

		else {
			c=GetMax();
			mm[c].num = fold;
			mm[c].time = 0;
		}
		queue1[K++]=fold;
	}
	for(i=0;i<M;i++){
		if(mm[i].num != -1){
			mm[i].time ++;
		}
	}
}

//����û��㷨
void OPT(int fold)
{
	int a,b,c;
	a=Equation(fold);
	if(a == -1){
		b=Check();

		if(b != -1){
			mm[b].num = fold;
		}
		else{
			c=longesttime(fold);
			mm[c].num = fold;
			mm[c].time = 0;
		}
		queue3[T++]=fold;
	}
}

//������δʹ���㷨
void LRU(int fold)
{
	int i;
	int a,b;
	int p;
	a=Equation(fold);
	if(a != -1)
	{
		if(a==3)
			return;
		else
		{
			p=Equation(-1);
			if(p==-1)
			{
				for(;a<3;a++)
					mm[a].num=mm[a+1].num;
				mm[3].num=fold;
			}
			else if(p<=3)
			{
				for(;a<p-1;a++)
					mm[a].num=mm[a+1].num;
				mm[a].num=fold;
			}

		}
	}
	else
	{
		b=Check();
		if(b!=-1)
			mm[b].num=fold;
		else
		{
			for(i=0;i<3;i++)
				mm[i].num=mm[i+1].num;
			mm[3].num=fold;
		}
		queue2[S++]=fold;
	}

}

//ҳ�滺���㷨
int mfree[M] = {1};//����ҳ������ 1��ʾ���� 0��ʾ�ǿ���
int modified[M] = {0};//���޸�ҳ������ 1��ʾ���޸� 0��ʾδ�޸�

void PBA(int fold)
{
    int i;
    int l;
	int a,b,c;
	a=Equation(fold);
	//ҳ�Ѵ���
	if(a != -1)
       // {mm[a].flag = 1;}
       {}
	//ҳ������
	else{
		b=Check();
		//�ڴ滹�п���
		if(b != -1){
			mm[b].num = fold;
			mm[b].flag = 1;
			mfree[b] = 0;//�޸Ŀ���ҳ������
		}

		//�ڴ���������Ҫ�û�
		else {
			c=GetMax();
			mm[c].num = fold;
			mm[c].time = 0;
			for(l=0;l<M;l++)
            {
                if(mm[l].flag == 1) modified[l]=l;
                else mfree[l] = 0;
            }
		}
		queue4[U++]=fold;
	}
	for(i=0;i<M;i++){
		if(mm[i].num != -1){
			mm[i].time ++;
		}
	}
}


int main(void)
{
	int i;
	INIT();
    test();
    printf("ҳ�������������Ϊ:\n");
    for(i=0;i<m;i++){
	printf("%d ",A[i]);
	}
	printf("\n");

//FIFO
	for(i=0;i<m;i++){
	B[i]=A[i];
	}
	for(i=0;i<m;i++){
		FIFO( B[i] );
	}
	printf("FIFO��");
	printf("�������Ϊ��");
	for(i=0;i<K;i++)
		printf("%3d",queue1[i]);
	printf("\nȱҳ����Ϊ��%6d\nȱҳ�ʣ�%16.6f\n\n",K,(float)K/m);

//LRU
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}
	for(i=0;i<m;i++){
		LRU( B[i] );
	}
	printf("LRU��");
	printf("�������Ϊ��");
	for(i=0;i<S;i++)
		printf("%3d",queue2[i]);
	printf("\nȱҳ����Ϊ��%6d\nȱҳ�ʣ�%16.6f\n\n",S,(float)S/m);

//OPT
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}

	for(i=0;i<m;i++){
		OPT( B[i] );
	}
	printf("OPT��");
	printf("�������Ϊ��");
	for(i=0;i<T;i++)
		printf("%3d",queue3[i]);
	printf("\nȱҳ����Ϊ��%6d\nȱҳ�ʣ�%16.6f\n\n",T,(float)T/m);

//PBA
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}

	for(i=0;i<m;i++){
		PBA( B[i] );
	}
	printf("PBA��");
	printf("�������Ϊ��");
	for(i=0;i<U;i++)
		printf("%3d",queue4[i]);
	printf("\nȱҳ����Ϊ��%6d\nȱҳ�ʣ�%16.6f\n\n",U,(float)U/m);


	return 0;

}
