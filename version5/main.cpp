#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sstream>
#include "permutation.h"
#include "Sudoku.h"
#include "SudokuSolver.h"
#include<vector>
#include<random>
#include <fstream>
#include <sstream>
using namespace std;

const int SEED_MAX = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1;//���ڹ̶�һλ������Ϊ8!
const int ID = 3;

void handleCreate(string);
void handleSolve(string);
void paramError();
void fileNotFound();
void creategame(string,int,string c);
void readSudoku(const std::string& filename, std::vector<std::vector<int>>& grid,int randi);
void randomizeGrid(std::vector<std::vector<int>>& grid, int seed, int numZeros);
void printSudoku(const std::vector<std::vector<int>>& grid);

int main(int argc, char* argv[])
{
	
	//handleCreate(string("1000"));
	//�жϲ����Ƿ�Ϸ�
	if (string(argv[1]) == "-help")
	{
		cout << "��ο�ָ��";
		return 0;
	}
	string temp = argv[1];
	if (argv[3])
	{
		if (string(argv[3]) == "-m" && temp == "-n")
		{
			int hard;
			stringstream ss_num(argv[4]);
			ss_num >> hard;
			creategame(string(argv[2]), hard,"0");
			return 0;
		}
		else if (string(argv[3]) == "-r"&&temp=="-n")
		{
			creategame(string(argv[2]), 0, string(argv[4]));
		}
	}
	else if (argc != 3)
	{
		printf("�������Ϸ���\n");
	}
	if (string(argv[1]) == "-c")
	{
		stringstream ss_num(argv[2]);
		int a;
		ss_num >> a;
		
		for(int i=0;i<a;i++)
		handleCreate(string("1"));
	}
	else if (string(argv[1]) == "-s")
	{
		handleSolve(string(argv[2]));
	}
	else if (string(argv[1]) == "-n")
	{
		creategame(string(argv[2]),0,"0");
	}
	
	else
	{
		paramError();
	}
}

void handleCreate(string amount)
{
	int num;
	stringstream ss_num(amount);
	FILE* f = fopen("sudoku.txt", "a");
	if (ss_num >> num)
	{
		if (!(1 <= num&&num <= 100000))
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

void creategame(string b,int hard,string c)
{
	int down, up;
	if (hard == 0)
	{
		down = 5;
		up = 45;
	}
	else if (hard == 1)
	{
		down = 5;
		up = 15;
	}
	else if (hard == 2)
	{
		down = 15;
		up = 30;
	}
	else if (hard == 3)
	{
		down = 30;
		up = 45;
	}
	else
	{
		cout << "�Ѷ���1-3֮��";
		return;
	}
	if (c != "0")
	{
		int A, B;
		std::istringstream iss(c);
		std::string token;

		std::getline(iss, token, '~');  // ʹ��'~'��Ϊ�ָ�������ȡA
		std::istringstream(token) >> A;

		std::getline(iss, token);  // ��ȡB
		std::istringstream(token) >> B;
		down = A;
		up = B;
	}
		stringstream ss_num(b);
		int bb;
		ss_num >> bb;
		srand((unsigned)time(NULL));
		string a = "20";
		handleCreate(a);
		std::vector<std::vector<int>> grid(9, std::vector<int>(9, 0));
		std::string filename = "sudoku.txt";  // �ļ���
		for (int i = 0; i < bb; i++)
		{
			int randi = (rand() % 20 + 20) % 20;
			readSudoku(filename, grid, randi);
			int seed = rand(), num = down + std::rand() % (up - down + 1);
			randomizeGrid(grid, seed, num);
			printSudoku(grid);
			cout << endl;
		}
		return;
	
}
void handleSolve(string filename)
{
	if (filename == "sudoku.txt") //��Ϊ���Ҫ������ļ�
	{
		printf("������ļ�����\n");
		return;
	}
	FILE *fp, *fo;
	int prob[81];
	if (fp = fopen(filename.data(), "r"))
	{
		fo = fopen("sudoku.txt", "w");
		while (true)
		{
			int tmp;
			for (int i = 0; i < 81; ++i)
				tmp = fscanf(fp, "%d", &prob[i]);
			SudokuSolver solver(prob);
			if (solver.solve())
			{
				//solver.print();
				solver.toFile(fo);
			}
			else
				printf("�������޷���⣡\n");
			if (feof(fp))
				break;
			else
			{
				//printf("\n");
				solver.appendLine(fo);
			}
		}
	}
	else
	{
		fileNotFound();
	}
	return;
}

void paramError()
{
	//�������벻�Ϸ�
	printf("���벻�Ϸ�����ʹ�� \"suodoku.exe -c ����\" �� \"sudoku.exe -s �ļ���\" ��ʽ���롣\n");
	exit(0);
}

void typeErrorNum()
{
	printf("����-c�����Ƿ�Ϊ���֡�\n");
}

void fileNotFound()
{
	printf("����-s�����ļ��Ƿ���ڡ�\n");
}

void readSudoku(const std::string& filename, std::vector<std::vector<int>>& grid,int randi) {
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		int row = 0;
		for (int i = 0; i < randi*10; ++i) {
			std::getline(file, line); // ��ȡ������һ������
		}
		while (std::getline(file, line) && row < 9) {
			std::istringstream iss(line);
			int col = 0;
			std::string token;
			while (iss >> token && col < 9) {
				if (token == " ") {
					grid[row][col] = 0;
				}
				else {
					grid[row][col] = std::stoi(token);
				}
				col++;
			}
			row++;
		}
		file.close();
	}
}
void randomizeGrid(std::vector<std::vector<int>>& grid, int seed, int numZeros) {
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> rowDist(0, 8);
	std::uniform_int_distribution<> colDist(0, 8);

	int count = 0;
	while (count < numZeros) {
		int row = rowDist(gen);
		int col = colDist(gen);

		if (grid[row][col] != 0) {
			grid[row][col] = 0;
			count++;
		}
	}
}

void printSudoku(const std::vector<std::vector<int>>& grid) {
	for (const auto& row : grid) {
		for (int num : row) {
			if(num==0)
				std::cout << '$' << " ";
			else
			std::cout << num << " ";
		}
		std::cout << std::endl;
	}
}