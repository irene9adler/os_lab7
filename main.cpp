#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define M 4   //物理页数
#define m 20	//工作集移动率
#define t 0.5
#define N 20//虚拟内存的尺寸

typedef struct page
{
	int num;
	int time;
	int flag;
}Page;
//物理页项

Page mm[M];
int queue1[20],queue2[20],queue3[20],queue4[20];	//记录置换的页
int A[m],B[m];
int K=0,S=0,T=0,U=0;//置换页数组的标识
int pos=0;//记录存在最长时间项
int r;//随机数，0 ≤ r ≤ 1。如果r < t，则为p生成一个新值，否则p = (p + 1) mod N；

void test()//页面访问序列随机发生器
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


//初始化内存页表项及存储内存情况的空间
void INIT(){
	int i;
	for(i=0;i<M;i++){
		mm[i].num =-1;
		mm[i].time =0;
		mm[i].flag =0;
	}
	printf("INIT success\n");
}


//取得内存中存在时间最久的位置
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

//检查最长时间不使用页面
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

//检查某页是否在内存
int Equation(int fold){
	int i;
	for(i=0;i<M;i++){
		if(mm[i].num == fold)
			return i;
	}
	return -1;
}

//检查物理内存是否已满,-1表满，其他不满
int Check(){
	int i;
	for(i=0;i<M;i++){
		if(mm[i].num == -1)
			return i;
	}
	return -1;
}


//先进先出算法
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

//最佳置换算法
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

//最近最久未使用算法
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

//页面缓冲算法
int mfree[M] = {1};//空闲页面链表 1表示空闲 0表示非空闲
int modified[M] = {0};//已修改页面链表 1表示已修改 0表示未修改

void PBA(int fold)
{
    int i;
    int l;
	int a,b,c;
	a=Equation(fold);
	//页已存在
	if(a != -1)
       // {mm[a].flag = 1;}
       {}
	//页不存在
	else{
		b=Check();
		//内存还有空闲
		if(b != -1){
			mm[b].num = fold;
			mm[b].flag = 1;
			mfree[b] = 0;//修改空闲页面链表
		}

		//内存已满，需要置换
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
    printf("页面随机访问序列为:\n");
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
	printf("FIFO的");
	printf("调入队列为：");
	for(i=0;i<K;i++)
		printf("%3d",queue1[i]);
	printf("\n缺页次数为：%6d\n缺页率：%16.6f\n\n",K,(float)K/m);

//LRU
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}
	for(i=0;i<m;i++){
		LRU( B[i] );
	}
	printf("LRU的");
	printf("调入队列为：");
	for(i=0;i<S;i++)
		printf("%3d",queue2[i]);
	printf("\n缺页次数为：%6d\n缺页率：%16.6f\n\n",S,(float)S/m);

//OPT
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}

	for(i=0;i<m;i++){
		OPT( B[i] );
	}
	printf("OPT的");
	printf("调入队列为：");
	for(i=0;i<T;i++)
		printf("%3d",queue3[i]);
	printf("\n缺页次数为：%6d\n缺页率：%16.6f\n\n",T,(float)T/m);

//PBA
	INIT();
	for(i=0;i<m;i++){
	B[i]=A[i];
	}

	for(i=0;i<m;i++){
		PBA( B[i] );
	}
	printf("PBA的");
	printf("调入队列为：");
	for(i=0;i<U;i++)
		printf("%3d",queue4[i]);
	printf("\n缺页次数为：%6d\n缺页率：%16.6f\n\n",U,(float)U/m);


	return 0;

}
