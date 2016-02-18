// mallocTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	int *Index = (int*)malloc(2*2*2*sizeof(int));
	int *index1 = (int*)malloc(1*2*2*sizeof(int));
	int *index2 = (int*)malloc(1*1*2*sizeof(int));
	int *index3 = (int*)malloc(1*1*2*sizeof(int));

	int tempPos(0);
	int c(0);
	for (int i = 0;i<1;i++)
	{
		for (int j = 0;j<2;j++)
		{
			tempPos = i*2*2 + j*2;
			index1[tempPos] = c;
			index1[tempPos+1] = c+1;
			c+=2;
		}
	}
	c= 100;
	for (int i =0;i<1;i++)
	{
		for (int j = 0;j<1;j++)
		{
			tempPos = i*2*2 + j*2;
			index2[tempPos] = c;
			index2[tempPos+1] = c+1;
			c+=2;
		}
	}


	c= 200;
	for (int i =0;i<1;i++)
	{
		for (int j = 0;j<1;j++)
		{
			tempPos = i*2*2 + j*2;
			index3[tempPos] = c;
			index3[tempPos+1] = c+1;
			c+=2;
		}
	}

	memcpy(Index,index1,4*sizeof(int));
	memcpy(Index+4,index2,2*sizeof(int));
	memcpy(Index+6,index3,2*sizeof(int));

	for (int i = 0;i<2;i++)
	{
		for (int j = 0; j < 2;j++)
		{
			tempPos = i*2*2 + j*2;
			cout<<Index[tempPos]<<endl;
			cout<<Index[tempPos+1]<<endl;
		}
	}

	system("Pause");
	return 0;
}

