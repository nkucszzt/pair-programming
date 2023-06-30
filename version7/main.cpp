#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include "permutation.h"
#include "Sudoku.h"
#include "SudokuSolver.h"
#include<vector>
#include<random>
#include <fstream>
#include <functional>
#include <cstdio> 
using namespace std;

const int SEED_MAX = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1;//由于固定一位，所以为8!
const int ID = 3;
const int SIZE = 9;

void handleCreate(string);
void handleSolve(string);
void paramError();
void fileNotFound();
void creategame(string,int,string c);
void readSudoku(const std::string& filename, std::vector<std::vector<int>>& grid,int randi);
void randomizeGrid(std::vector<std::vector<int>>& grid, int seed, int numZeros);
void printSudoku(const std::vector<std::vector<int>>& grid);
bool isValid(const std::vector<std::vector<int>>& grid, int row, int col, int num);
bool generateSudoku(std::vector<std::vector<int>>& grid, int numHoles);

int main(int argc, char* argv[])
{
	
	//handleCreate(string("1000"));
	//判断参数是否合法
	if (string(argv[1]) == "-help")
	{
		cout << "请参考指南";
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
		else if (string(argv[3]) == "-u" && temp == "-n")
		{
			remove("sudoku.txt");
			remove("game.txt");
			//creategame(string(argv[2]), 0, "5~10");
			for (int i = 0; i < 20; i++)
				handleCreate(string("1"));
			std::vector<std::vector<int>> grid(SIZE, std::vector<int>(SIZE, 0));
			srand(int(time(NULL)));
			stringstream ss_num(argv[2]);
			int a;
			ss_num >> a;
			for (int i = 0; i < a; i++)
			{
				int randi = (rand()%20+20)%20;
				readSudoku("sudoku.txt", grid, randi);
				generateSudoku(grid, randi);
				printSudoku(grid);
				cout << endl;
			}
		}
	}
	else
	{
		if (string(argv[1]) == "-c")
		{
			remove("sudoku.txt");
			stringstream ss_num(argv[2]);
			int a;
			ss_num >> a;

			for (int i = 0; i < a; i++)
				handleCreate(string("1"));
		}
		else if (string(argv[1]) == "-s")
		{
			handleSolve(string(argv[2]));
		}
		else if (string(argv[1]) == "-n")
		{
			creategame(string(argv[2]), 0, "0");
		}
		else
		{
			paramError();
		}
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
			printf("请输入1-1000000的数字！\n");
		}
		//TODO:将种子更新变为定向，避免两次随机结果一样
		//srand(int(time(NULL)));
		int seed = rand() % SEED_MAX + 1;
		while (true)
		{
			Sudoku sudoku(seed, ID);
			int seeds[6] = { 0 };//控制一个基本数独的变换
			bool flag = false;//是否达到数量
			for (seeds[0] = 0; seeds[0] < 2; ++seeds[0])//变换23行
			{
				for (seeds[1] = 0; seeds[1] < 6; ++seeds[1])//变换456行
				{
					for (seeds[2] = 0; seeds[2] < 6; ++seeds[2])//变换789行
					{
						for (seeds[3] = 0; seeds[3] < 2; ++seeds[3])//变换23列
						{
							for (seeds[4] = 0; seeds[4] < 6; ++seeds[4])//变换456列
							{
								for (seeds[5] = 0; seeds[5] < 6; ++seeds[5])//变换789列
								{
									Sudoku n_sudoku(sudoku.changeState(seeds));
									if (!(--num))//生成指定数量
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
	else//输入不是数字
		paramError();
	fclose(f);
	return;
}

void creategame(string b,int hard,string c)
{
	remove("sudoku.txt");
	remove("game.txt");
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
		cout << "难度在1-3之间";
		return;
	}
	if (c != "0")
	{
		int A, B;
		std::istringstream iss(c);
		std::string token;

		std::getline(iss, token, '~');  // 使用'~'作为分隔符，提取A
		std::istringstream(token) >> A;

		std::getline(iss, token);  // 提取B
		std::istringstream(token) >> B;
		down = A;
		up = B;
	}
		stringstream ss_num(b);
		int bb;
		ss_num >> bb;
		srand((unsigned)time(NULL));
		int a = 20;
			for (int i = 0; i < a; i++)
				handleCreate(string("1"));
		std::vector<std::vector<int>> grid(9, std::vector<int>(9, 0));
		std::string filename = "sudoku.txt";  // 文件名
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
	if (filename == "sudoku.txt") //因为输出要用这个文件
	{
		printf("请更换文件名！\n");
		return;
	}
	FILE *fp;
	
	if (fp = fopen(filename.data(), "r"))
	{
		int prob[81];
		FILE* fo;
		fo = fopen("sudoku.txt", "w");
		while (true)
		{
			
			for (int i = 0; i < 81; ++i) {
				char symbol;
				if (fscanf(fp, " %c", &symbol) == 1) {
					if (symbol == '$') {
						prob[i] = 0;
					}
					else {
						prob[i] = symbol - '0'; // 将字符转换为整数
					}
				}
				
			}
			
			SudokuSolver solver(prob);
			if (solver.solve())
			{
				//solver.print();
				solver.toFile(fo);
			}
			else
				printf("该数独无法求解！\n");
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
	//处理输入不合法
	printf("输入不合法，请使用 \"suodoku.exe -c 数字\" 或 \"sudoku.exe -s 文件名\" 格式输入。\n");
	exit(0);
}



void fileNotFound()
{
	printf("请检查-s参数文件是否存在。\n");
}

void readSudoku(const std::string& filename, std::vector<std::vector<int>>& grid,int randi) {
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		int row = 0;
		for (int i = 0; i < randi*10; ++i) {
			std::getline(file, line); // 读取并丢弃一行数据
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
	ofstream file("game.txt",ios::app);
	if (file.is_open()) {
		for (const auto& row : grid) {
			for (int num : row) {
				if (num == 0) {
					file <<'$'<<" ";
				}
				else {
					file << num << " ";
				}
			}
			file << "\n";
		}
		file << "\n";

		file.close();
		std::cout << "数独已成功写入文件: " << "game.txt" << std::endl;
	}
	else {
		std::cout << "无法打开文件: " << "game.txt" << std::endl;
	}
}


// 检查在指定位置 (row, col) 放置数字 num 是否合法
bool isValid(const std::vector<std::vector<int>>& grid, int row, int col, int num) {
	// 检查行是否合法
	for (int i = 0; i < SIZE; i++) {
		if (grid[row][i] == num) {
			return false;
		}
	}

	// 检查列是否合法
	for (int i = 0; i < SIZE; i++) {
		if (grid[i][col] == num) {
			return false;
		}
	}

	// 检查3x3小方格是否合法
	int startRow = row - row % 3;
	int startCol = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (grid[startRow + i][startCol + j] == num) {
				return false;
			}
		}
	}

	return true;
}

bool generateSudoku(std::vector<std::vector<int>>& grid, int numHoles) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 9);

	std::vector<int> indices;
	for (int i = 0; i < SIZE * SIZE; i++) {
		indices.push_back(i);
	}

	std::shuffle(indices.begin(), indices.end(), gen);

	for (int count = 0; count < numHoles; count++) {
		int index = indices[count];
		int row = index / SIZE;
		int col = index % SIZE;

		if (grid[row][col] == 0) {
			continue;
		}

		int num = grid[row][col];
		grid[row][col] = 0;

		// 检查挖空后的数独是否有唯一解
		std::vector<std::vector<int>> tempGrid = grid;
		int solutions = 0;

		std::function<void()> solveSudoku = [&]() {
			for (int r = 0; r < SIZE; r++) {
				for (int c = 0; c < SIZE; c++) {
					if (tempGrid[r][c] == 0) {
						for (int num = 1; num <= SIZE; num++) {
							if (isValid(tempGrid, r, c, num)) {
								tempGrid[r][c] = num;
								solveSudoku();
								tempGrid[r][c] = 0;
							}
						}
						return;
					}
				}
			}
			solutions++;
		};

		solveSudoku();

		if (solutions != 1) {
			grid[row][col] = num;
			count--;
		}
	}

	return true;
}