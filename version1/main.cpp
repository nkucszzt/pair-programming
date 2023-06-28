#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sstream>
#include "permutation.h"
#include "Sudoku.h"
#include<vector>
#include<random>
#include <fstream>
#include <sstream>
using namespace std;

const int SEED_MAX = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1;//���ڹ̶�һλ������Ϊ8!
const int ID = 3;
void handleCreate(string);
void paramError();
int main(int argc, char* argv[])
{
	for (int i = 0; i < 20; i++)
		handleCreate(string("1"));
	return 0;
	
	if (argc != 3)
	{
		printf("�������Ϸ���\n");
		return 0;
	}
	if (string(argv[1]) == "-c")
	{
		stringstream ss_num(argv[2]);
		int a;
		ss_num >> a;

		for (int i = 0; i < a; i++)
			handleCreate(string("1"));
	}
	else
	{
		paramError();
	}
}
void paramError()
{
	//�������벻�Ϸ�
	printf("���벻�Ϸ�����ʹ�� \"suodoku.exe -c ����\" �� \"sudoku.exe -s �ļ���\" ��ʽ���롣\n");
	exit(0);
}
void handleCreate(string amount)
{
	int num;
	stringstream ss_num(amount);
	FILE* f = fopen("sudoku.txt", "a");
	if (ss_num >> num)
	{
		if (!(1 <= num && num <= 100000))
		{
			printf("������1-1000000�����֣�\n");
		}
		//TODO:�����Ӹ��±�Ϊ���򣬱�������������һ��
		//srand(int(time(NULL)));
		int seed = rand() % SEED_MAX + 1;
		while (true)
		{
			Sudoku sudoku(seed, ID);
			int seeds[6] = { 0 };//����һ�����������ı任
			bool flag = false;//�Ƿ�ﵽ����
			for (seeds[0] = 0; seeds[0] < 2; ++seeds[0])//�任23��
			{
				for (seeds[1] = 0; seeds[1] < 6; ++seeds[1])//�任456��
				{
					for (seeds[2] = 0; seeds[2] < 6; ++seeds[2])//�任789��
					{
						for (seeds[3] = 0; seeds[3] < 2; ++seeds[3])//�任23��
						{
							for (seeds[4] = 0; seeds[4] < 6; ++seeds[4])//�任456��
							{
								for (seeds[5] = 0; seeds[5] < 6; ++seeds[5])//�任789��
								{
									Sudoku n_sudoku(sudoku.changeState(seeds));
									if (!(--num))//����ָ������
									{
										n_sudoku.toFile(f);
										flag = true;
										break;
									}
									else
									{
										n_sudoku.toFile(f);
										n_sudoku.appendLine(f);
									}
									if (flag)
										break;
								}
								if (flag)
									break;
							}
							if (flag)
								break;
						}
						if (flag)
							break;
					}
					if (flag)
						break;
				}
				if (flag)
					break;
			}
			if (flag)
				break;
		}
	}
	else//���벻������
		paramError();
	fclose(f);
	return;
}