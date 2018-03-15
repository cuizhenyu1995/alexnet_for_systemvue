#pragma once
#include "alexnet_simple_conv_1.h"
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


using namespace std;
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using std::sort;
using std::fabs;

#define FILE_LENGTH_MAX 255
#define NUMBER_LENGTH_MAX 10
#define MODEL_WEIGHT 3
#define MODEL_GROUP 3
#define DATA_SIDE_LENGTH 227
#define CONST_BIASE_R 124
#define CONST_BIASE_G 117
#define CONST_BIASE_B 104
#define RESULT_SIDE_LENGTH 114
#define POOLING_STEP_LENGTH 2

char pre[] = "weights";
char biase[] = "biases";
char num[NUMBER_LENGTH_MAX];
char last[] = ".txt";
double ***model;
double ***res;
double ***c1_data;


namespace SystemVueModelBuilder {

#ifndef SV_CODE_MRA_RUN
	DEFINE_MODEL_INTERFACE(alexnet_simple_conv_1)
	{
		SET_MODEL_NAME("alexnet_simple_conv_1");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("alexnet_simple_conv");
		ADD_MODEL_HEADER_FILE("alexnet_simple_conv_1.h");
		model.SetModelCodeGenName("alexnet_simple_conv_1");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(INPUT_FILE);
		param = ADD_MODEL_PARAM(LENGTH_OF_SIDE);

		// Add input/output ports
		
		DFPort IN_BLUE = ADD_MODEL_INPUT(IN_B);
		DFPort IN_GREEN = ADD_MODEL_INPUT(IN_G);
		DFPort IN_RED = ADD_MODEL_INPUT(IN_R);

		DFPort OUT1 = ADD_MODEL_OUTPUT(OUT_1);
		DFPort OUT2 = ADD_MODEL_OUTPUT(OUT_2);
		DFPort OUT3 = ADD_MODEL_OUTPUT(OUT_3);

		

		return true;
	}
#endif

	alexnet_simple_conv_1::alexnet_simple_conv_1()
	{
	}

	alexnet_simple_conv_1::~alexnet_simple_conv_1()
	{
	}

	bool alexnet_simple_conv_1::Setup()
	{
		IN_R.SetRate(1);
		IN_G.SetRate(1);
		IN_B.SetRate(1);

		OUT_1.SetRate(1);
		OUT_2.SetRate(1);
		OUT_3.SetRate(1);

		return true;
	};

	bool alexnet_simple_conv_1::Initialize()
	{
		return true;
	}
	bool alexnet_simple_conv_1::Run()
	{
		std::stringstream sta;
		sta << "converting start!"<< endl;
		POST_INFO(sta.str().c_str());

		//为model申请空间
		int fal = 0;
		model = new double **[MODEL_WEIGHT+1];
		if (model == NULL) fal = 1;
		for (int i = 0; i< MODEL_WEIGHT + 1; i++)
		{
			model[i] = new double *[MODEL_GROUP + 1];
			if (model[i] == NULL) fal = 1;
			for (int j = 0; j<MODEL_GROUP + 1; j++)
			{
				model[i][j] = new double[LENGTH_OF_SIDE*LENGTH_OF_SIDE + 1];
				if (model[i][j] == NULL) fal = 1;
			}
		}
		if (fal)
		{
			std::stringstream st;
			st << "Failed when getting space for model!"<<endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for model!" << endl;
			POST_INFO(st.str().c_str());
		}

		//读取model----------------------------------------------------------------------------
		for (int i = 0; i < LENGTH_OF_SIDE*LENGTH_OF_SIDE; i++)
		{
			char s[FILE_LENGTH_MAX] = "";
			strcat(s, INPUT_FILE);
			strcat(s, pre);
			mych(i+1);
			strcat(s, num);
			strcat(s, last);
			
			//打开model文件
			FILE *mf=fopen(s, "r");
			if (mf == NULL)
			{
				std::stringstream st;
				st << "Failed when opening model flie!" << endl;
				POST_INFO(st.str().c_str());
				return true;
			}
			else
			{
				std::stringstream st;
				st << "Success when opening model flie!" << endl;
				POST_INFO(st.str().c_str());
			}
			//读取model数据
			for (int j = 0; j < MODEL_WEIGHT; j++)
			{
				for (int k = 0; k < MODEL_GROUP; k++)
				{
					fscanf(mf,"%lf", &model[MODEL_WEIGHT-1-j][k][i]);
				}
			}
		}
		
		//为res申请空间
		fal = 0;
		res = new double **[MODEL_GROUP + 1];
		if (res == NULL) fal = 1;
		for (int i = 0; i< MODEL_GROUP + 1; i++)
		{
			res[i] = new double *[DATA_SIDE_LENGTH + 1];
			if (res[i] == NULL) fal = 1;
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
			{
				res[i][j] = new double[DATA_SIDE_LENGTH + 1];
				if (res[i][j] == NULL) fal = 1;
			}
		}
		if (fal)
		{
			std::stringstream st;
			st << "Failed when getting space for result!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for result!" << endl;
			POST_INFO(st.str().c_str());
		}
		for (int i = 0; i < MODEL_GROUP + 1; i++)
			for (int j = 0; j < DATA_SIDE_LENGTH + 1; j++)
				for (int k = 0; k < DATA_SIDE_LENGTH + 1; k++)
					res[i][j][k] = 0;

		//为data申请空间
		fal = 0;
		c1_data = new double **[MODEL_WEIGHT + 1];
		if (c1_data == NULL) fal = 1;
		for (int i = 0; i< MODEL_WEIGHT + 1; i++)
		{
			c1_data[i] = new double *[DATA_SIDE_LENGTH + 1];
			if (c1_data[i] == NULL) fal = 1;
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
			{
				c1_data[i][j] = new double[DATA_SIDE_LENGTH + 1];
				if (c1_data[i][j] == NULL) fal = 1;
			}
		}
		if (fal)
		{
			std::stringstream st;
			st << "Failed when getting space for data!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for data!" << endl;
			POST_INFO(st.str().c_str());
		}

		//获取数据-----------------------------------------------------------------------------
		for (int i = 0; i < DATA_SIDE_LENGTH; i++)
		{
			for (int j = 0; j < DATA_SIDE_LENGTH; j++)
			{
				c1_data[0][i][j] = (double)(IN_R[0](i, j) - CONST_BIASE_R);
				c1_data[1][i][j] = (double)(IN_G[0](i, j) - CONST_BIASE_G);
				c1_data[2][i][j] = (double)(IN_B[0](i, j) - CONST_BIASE_B);
			}
		}
		std::stringstream st;
		st << "Success getting data!" << endl;
		POST_INFO(st.str().c_str());

		//卷积操作-----------------------------------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)//group
		{
			for (int j = 0; j < MODEL_WEIGHT; j++)//weight
			{
				if (getres(model[j][i], c1_data[j], res[i], DATA_SIDE_LENGTH, DATA_SIDE_LENGTH, LENGTH_OF_SIDE) == 0)
				{
					std::stringstream st;
					st << "Failed when conv!" << endl;
					POST_INFO(st.str().c_str());
					return true;
				}
			}
		}
		std::stringstream stss;
		stss << "Success when conv!" << endl;
		POST_INFO(stss.str().c_str());

		//读取biase文件
		char s[FILE_LENGTH_MAX] = "";
		strcat(s, INPUT_FILE);
		strcat(s, biase);
		strcat(s, last);
		FILE *bf = fopen(s,"r");
		if (bf == NULL)
		{
			std::stringstream st;
			st << "Failed when opening biase file!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when opening biase file!" << endl;
			POST_INFO(st.str().c_str());
		}
		
		//biase操作&ReLU操作---------------------------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			double ttt;
			fscanf(bf,"%lf", &ttt);
			for (int ii = 0; ii < DATA_SIDE_LENGTH; ii++)
			{
				for (int jj = 0; jj < DATA_SIDE_LENGTH; jj++)
				{
					res[i][ii][jj] = res[i][ii][jj] + ttt;
					res[i][ii][jj]=max(res[i][ii][jj],0.0);
				}
			}
		}

		//max-pooling操作(padding=same)&输出操作--------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			out.Resize(RESULT_SIDE_LENGTH, RESULT_SIDE_LENGTH);
			out.Zero();
			for (int ii = 0; ii < DATA_SIDE_LENGTH; ii+=POOLING_STEP_LENGTH)
			{
				for (int jj = 0; jj < DATA_SIDE_LENGTH; jj += POOLING_STEP_LENGTH)
				{
					double now = res[i][ii][jj];
					if (ii + 1 < DATA_SIDE_LENGTH) now = max(now, res[i][ii + 1][jj]);
					else now = max(now, 0.0);
					if (jj + 1 < DATA_SIDE_LENGTH) now = max(now, res[i][ii][jj + 1]);
					else now = max(now, 0.0);
					if (ii + 1 < DATA_SIDE_LENGTH && jj + 1 < DATA_SIDE_LENGTH) now = max(now, res[i][ii + 1][jj + 1]);
					else now = max(now, 0.0);
					
					out(ii / POOLING_STEP_LENGTH, jj / POOLING_STEP_LENGTH) = now;
				}
			}

			//输出到缓冲区
			if (i == 0) OUT_1[0] = out;
			else if (i == 1) OUT_2[0] = out;
			else OUT_3[0] = out;

		}

		for (int i = 0; i<MODEL_WEIGHT + 1; i++)
		{
			for (int j = 0; j<MODEL_GROUP + 1; j++)
				delete[] model[i][j];
			delete[] model[i];
		}
		delete[] model;//new和delete是成对出现的，这里也要多重循环释放掉空间
		model = NULL;

		for (int i = 0; i<MODEL_GROUP + 1; i++)
		{
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
				delete[] res[i][j];
			delete[] res[i];
		}
		delete[] res;//new和delete是成对出现的，这里也要多重循环释放掉空间
		res = NULL;

		for (int i = 0; i<MODEL_WEIGHT + 1; i++)
		{
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
				delete[] c1_data[i][j];
			delete[] c1_data[i];
		}
		delete[] c1_data;//new和delete是成对出现的，这里也要多重循环释放掉空间
		c1_data = NULL;

		std::stringstream stag;
		stag << "converting finished!" << endl;
		POST_INFO(stag.str().c_str());
		return true;
	}

	bool alexnet_simple_conv_1::Finalize()
	{
		
		return true;
	}

	double alexnet_simple_conv_1::max(double a, double b)
	{
		if (a > b) return a;
		return b;
	}
	void alexnet_simple_conv_1::mych(int tmp)
	{
		memset(num, '\0', sizeof(num));
		int tp = 0;
		while (tmp)
		{
			num[tp++] = '0' + (tmp % 10);
			tmp /= 10;
		}
		for (int i = 0; i<tp / 2; i++)
		{
			swap(num[i], num[tp - 1 - i]);
		}
		num[tp++] = '\0';
	}
	int alexnet_simple_conv_1::getres(double f[], double **mm, double **res, int mph, int mpl, int R)
	{
		if (f == NULL || mm == NULL || res == NULL) return 0;
		for (int i = 0; i<mph; i++)
		{
			for (int j = 0; j<mpl; j++)
			{
				int ll = j - R / 2, rr = j + R / 2;
				int nn = i - R / 2, ss = i + R / 2;

				int tp = 0;
				double tmp = 0;
				for (int k = nn; k <= ss; k++)
				{
					for (int l = ll; l <= rr; l++)
					{
						if (k >= 0 && k<mph && l >= 0 && l<mpl)
						{
							tmp += f[tp] * mm[k][l];
						}
						tp++;
					}
				}
				res[i][j] += tmp;
			}
		}
		return 1;
	}

}