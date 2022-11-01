#include<iostream>
#include<vector>
#include<fstream>
#include<opencv2/core/core.hpp>
using namespace std;
using namespace cv;

//获取文件数据行数
int CountLines(const char* filename)//获取文件的行数
{
	ifstream ReadFile;
	int n = 0;
	string temp;
	ReadFile.open(filename, ios::in);//ios::in 表示以只读的方式读取文件
	while (getline(ReadFile, temp))
	{
		n++;
	}
	return n;
	ReadFile.close();
}

void printMat(Mat m)
{
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			cout << m.at<double>(i, j)<<"  ";
		}
	}
}

int main()
{
	int count;//点的个数
	count = CountLines("./indoor/pts3d.txt");
	cout << "点个数" << count << endl;

	//读取三维点数据
	vector<double> x(count), y(count), z(count);
	ifstream txt3d;
	txt3d.open("./indoor/pts3d.txt",ios::in);
	if (txt3d.fail())
	{
		cout << "文件不存在!" << endl;
		txt3d.close();
	}
	else
	{
		string line;
		int i = 0;
		while (getline(txt3d, line))//逐行从输入读取数据
		{	
			istringstream pointrecord(line);//将一行点数据绑定到刚读入的行
			pointrecord >> x[i] >> y[i] >> z[i];
			i++;
		}
		txt3d.close(); //关闭文件
	}
	
	//读取图片pic_a二维点数据
	vector<double> au(count), av(count);
	ifstream txt2d_a;
	txt2d_a.open("./indoor/pts2d-pic_a.txt", ios::in);
	if (txt2d_a.fail())
	{
		cout << "文件不存在!" << endl;
		txt2d_a.close();
	}
	else
	{
		string line;
		int i = 0;
		while (getline(txt2d_a, line))//逐行从输入读取数据
		{
			istringstream pointrecord(line);//将一行点数据绑定到刚读入的行
			pointrecord >> au[i] >> av[i];
			i++;
		}
		txt2d_a.close(); //关闭文件
	}

	//读取图片pic_b二维点数据
	vector<double> bu(count), bv(count);
	ifstream txt2d_b;
	txt2d_b.open("./indoor/pts2d-pic_b.txt", ios::in);
	if (txt2d_b.fail())
	{
		cout << "文件不存在!" << endl;
		txt2d_b.close();
	}
	else
	{
		string line;
		int i = 0;
		while (getline(txt2d_b, line))//逐行从输入读取数据
		{
			istringstream pointrecord(line);//将一行点数据绑定到刚读入的行
			pointrecord >> bu[i] >> bv[i];
			i++;
		}
		txt2d_a.close(); //关闭文件
	}

	//系数矩阵A
	//方程个数2*点个数，未知数个数为11
	Mat aA = Mat::zeros(2 * count, 11, CV_64F);
	Mat bA = Mat::zeros(2 * count, 11, CV_64F);
	for (int i = 0; i < count; i++)
	{
		aA.at<double>(2 * i, 0) = x[i];
		aA.at<double>(2 * i, 1) = y[i];
		aA.at<double>(2 * i, 2) = z[i];
		aA.at<double>(2 * i, 3) = 1;
		aA.at<double>(2 * i, 4) = 0;
		aA.at<double>(2 * i, 5) = 0;
		aA.at<double>(2 * i, 6) = 0;
		aA.at<double>(2 * i, 7) = 0;
		aA.at<double>(2 * i, 8) = -au[i] * x[i];
		aA.at<double>(2 * i, 9) = -au[i] * y[i];
		aA.at<double>(2 * i, 10) =  -au[i] * z[i];

		aA.at<double>(2 * i + 1, 0) = 0;
		aA.at<double>(2 * i + 1, 1) = 0;
		aA.at<double>(2 * i + 1, 2) = 0;
		aA.at<double>(2 * i + 1, 3) = 0;
		aA.at<double>(2 * i + 1, 4) = x[i];
		aA.at<double>(2 * i + 1, 5) = y[i];
		aA.at<double>(2 * i + 1, 6) = z[i];
		aA.at<double>(2 * i + 1, 7) = 1;
		aA.at<double>(2 * i + 1, 8) = -av[i] * x[i];
		aA.at<double>(2 * i + 1, 9) = -av[i] * y[i];
		aA.at<double>(2 * i + 1, 10) = -av[i] * z[i];
	}
	//printMat(aA);

	for (int i = 0; i < count; i++)
	{
		bA.at<double>(2 * i, 0) = x[i];
		bA.at<double>(2 * i, 1) = y[i];
		bA.at<double>(2 * i, 2) = z[i];
		bA.at<double>(2 * i, 3) = 1;
		bA.at<double>(2 * i, 4) = 0;
		bA.at<double>(2 * i, 5) = 0;
		bA.at<double>(2 * i, 6) = 0;
		bA.at<double>(2 * i, 7) = 0;
		bA.at<double>(2 * i, 8) = -bu[i] * x[i];
		bA.at<double>(2 * i, 9) = -bu[i] * y[i];
		bA.at<double>(2 * i, 10) = -bu[i] * z[i];

		bA.at<double>(2 * i + 1, 0) = 0;
		bA.at<double>(2 * i + 1, 1) = 0;
		bA.at<double>(2 * i + 1, 2) = 0;
		bA.at<double>(2 * i + 1, 3) = 0;
		bA.at<double>(2 * i + 1, 4) = x[i];
		bA.at<double>(2 * i + 1, 5) = y[i];
		bA.at<double>(2 * i + 1, 6) = z[i];
		bA.at<double>(2 * i + 1, 7) = 1;
		bA.at<double>(2 * i + 1, 8) = -bv[i] * x[i];
		bA.at<double>(2 * i + 1, 9) = -bv[i] * y[i];
		bA.at<double>(2 * i + 1, 10) = -bv[i] * z[i];
	}

	//b矩阵
	Mat ab = Mat::zeros(2 * count, 1, CV_64F);
	Mat bb = Mat::zeros(2 * count, 1, CV_64F);
	for (int i = 0; i < count; i++)
	{
		ab.at<double>(2 * i) = au[i];
		ab.at<double>(2 * i + 1) = av[i];
	}
	//printMat(ab);
	for (int i = 0; i < count; i++)
	{
		bb.at<double>(2 * i) = bu[i];
		bb.at<double>(2 * i + 1) = bv[i];
	}

	Mat m1 = Mat::zeros(11, 1, CV_64F);
	Mat m2 = Mat::zeros(11, 1, CV_64F);
	//根据最小二乘法公式计算投影矩阵，得到11维的列向量
	m1 = ((aA.t() * aA).inv()) * (aA.t()) * ab;
	m2 = (bA.t() * bA).inv() * bA.t() * bb;
	printMat(aA.t() * aA);
	cout << (aA.t() * aA).size()<<endl;

	//转换为3×4的矩阵形式
	vector<double> M1;
	vector<double> M2;
	for (int i = 0; i < 11; i++)
	{
		M1.push_back(m1.at<double>(i, 0));
		M2.push_back(m2.at<double>(i, 0));
	}
	//第12个数据值为1
	M1.push_back(1);
	M2.push_back(1);

	//输出投影矩阵
	cout << "图片pic_a的投影矩阵为：" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << M1[i * 4 + j]<<"   ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "图片pic_b的投影矩阵为：" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << M2[i * 4 + j] << "   ";
		}
		cout << endl;
	}

	return 0;
}
