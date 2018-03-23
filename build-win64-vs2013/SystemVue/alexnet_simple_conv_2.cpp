#pragma once
#include "alexnet_simple_conv_2.h"
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
#define DATA_SIDE_LENGTH 114
#define RESULT_SIDE_LENGTH 114
#define POOLING_STEP_LENGTH 1

char pre2[] = "weights";
char biase2[] = "biases";
char num2[NUMBER_LENGTH_MAX];
char last2[] = ".txt";
double ***model2;
double ***res2;
double ***c2_data;


namespace SystemVueModelBuilder {

#ifndef SV_CODE_MRA_RUN
	DEFINE_MODEL_INTERFACE(alexnet_simple_conv_2)
	{
		SET_MODEL_NAME("alexnet_simple_conv_2");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("alexnet_simple_conv");
		ADD_MODEL_HEADER_FILE("alexnet_simple_conv_2.h");
		model.SetModelCodeGenName("alexnet_simple_conv_2");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(INPUT_FILE);
		param = ADD_MODEL_PARAM(LENGTH_OF_SIDE);

		// Add input/output ports

		DFPort IN_DATA = ADD_MODEL_INPUT(IN_D);
		DFPort IN_NUM = ADD_MODEL_INPUT(IN_N);

		DFPort OUT_DATA = ADD_MODEL_OUTPUT(OUT_D);
		DFPort OUT_NUM = ADD_MODEL_OUTPUT(OUT_N);



		return true;
	}
#endif

	alexnet_simple_conv_2::alexnet_simple_conv_2()
	{
	}

	alexnet_simple_conv_2::~alexnet_simple_conv_2()
	{
	}

	bool alexnet_simple_conv_2::Setup()
	{
		IN_N.SetRate(1);
		IN_D.SetRate(3);

		OUT_N.SetRate(1);
		OUT_D.SetRate(3);

		return true;
	};

	bool alexnet_simple_conv_2::Initialize()
	{
		return true;
	}
	bool alexnet_simple_conv_2::Run()
	{
		std::stringstream sta;
		sta << "converting start!" << endl;
		POST_INFO(sta.str().c_str());

		//为model2申请空间
		int fal = 0;
		model2 = new double **[MODEL_WEIGHT + 1];
		if (model2 == NULL) fal = 1;
		for (int i = 0; i< MODEL_WEIGHT + 1; i++)
		{
			model2[i] = new double *[MODEL_GROUP + 1];
			if (model2[i] == NULL) fal = 1;
			for (int j = 0; j<MODEL_GROUP + 1; j++)
			{
				model2[i][j] = new double[LENGTH_OF_SIDE*LENGTH_OF_SIDE + 1];
				if (model2[i][j] == NULL) fal = 1;
			}
		}
		if (fal)
		{
			std::stringstream st;
			st << "Failed when getting space for model2!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for model2!" << endl;
			POST_INFO(st.str().c_str());
		}

		//读取model2----------------------------------------------------------------------------
		for (int i = 0; i < LENGTH_OF_SIDE*LENGTH_OF_SIDE; i++)
		{
			char s[FILE_LENGTH_MAX] = "";
			strcat(s, INPUT_FILE);
			strcat(s, pre2);
			mych(i + 1);
			strcat(s, num2);
			strcat(s, last2);

			//打开model2文件
			FILE *mf = fopen(s, "r");
			if (mf == NULL)
			{
				std::stringstream st;
				st << "Failed when opening model2 flie!" << endl;
				POST_INFO(st.str().c_str());
				return true;
			}
			else
			{
				std::stringstream st;
				st << "Success when opening model2 flie!" << endl;
				POST_INFO(st.str().c_str());
			}
			//读取model2数据
			for (int j = 0; j < MODEL_WEIGHT; j++)
			{
				for (int k = 0; k < MODEL_GROUP; k++)
				{
					fscanf(mf, "%lf", &model2[j][k][i]);
				}
			}
			fclose(mf);
		}

		//为res2申请空间
		fal = 0;
		res2 = new double **[MODEL_GROUP + 1];
		if (res2 == NULL) fal = 1;
		for (int i = 0; i< MODEL_GROUP + 1; i++)
		{
			res2[i] = new double *[DATA_SIDE_LENGTH + 1];
			if (res2[i] == NULL) fal = 1;
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
			{
				res2[i][j] = new double[DATA_SIDE_LENGTH + 1];
				if (res2[i][j] == NULL) fal = 1;
			}
		}
		if (fal)
		{
			std::stringstream st;
			st << "Failed when getting space for res2ult!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for res2ult!" << endl;
			POST_INFO(st.str().c_str());
		}
		for (int i = 0; i < MODEL_GROUP + 1; i++)
			for (int j = 0; j < DATA_SIDE_LENGTH + 1; j++)
				for (int k = 0; k < DATA_SIDE_LENGTH + 1; k++)
					res2[i][j][k] = 0;

		//为data申请空间
		int DATA_WEIGHT = IN_N[0];

		fal = 0;
		c2_data = new double **[DATA_WEIGHT + 1];
		if (c2_data == NULL) fal = 1;
		for (int i = 0; i< DATA_WEIGHT + 1; i++)
		{
			c2_data[i] = new double *[DATA_SIDE_LENGTH + 1];
			if (c2_data[i] == NULL) fal = 1;
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
			{
				c2_data[i][j] = new double[DATA_SIDE_LENGTH + 1];
				if (c2_data[i][j] == NULL) fal = 1;
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
				for (int k = 0; k < DATA_WEIGHT; k++)
					c2_data[k][i][j] = (double)(IN_D[k](i, j));
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
				if (getres2(model2[j][i], c2_data[j], res2[i], DATA_SIDE_LENGTH, DATA_SIDE_LENGTH, LENGTH_OF_SIDE) == 0)
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

		//读取biase2文件
		char s[FILE_LENGTH_MAX] = "";
		strcat(s, INPUT_FILE);
		strcat(s, biase2);
		strcat(s, last2);
		FILE *bf = fopen(s, "r");
		if (bf == NULL)
		{
			std::stringstream st;
			st << "Failed when opening biase2 file!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when opening biase2 file!" << endl;
			POST_INFO(st.str().c_str());
		}

		//biase2操作&ReLU操作---------------------------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			double ttt;
			fscanf(bf, "%lf", &ttt);
			for (int ii = 0; ii < DATA_SIDE_LENGTH; ii++)
			{
				for (int jj = 0; jj < DATA_SIDE_LENGTH; jj++)
				{
					res2[i][ii][jj] = res2[i][ii][jj] + ttt;
					res2[i][ii][jj] = max(res2[i][ii][jj], 0.0);
				}
			}
		}

		//max-pooling操作(padding=same)&输出操作--------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			out.Resize(RESULT_SIDE_LENGTH, RESULT_SIDE_LENGTH);
			out.Zero();
			for (int ii = 0; ii < DATA_SIDE_LENGTH; ii += POOLING_STEP_LENGTH)
			{
				for (int jj = 0; jj < DATA_SIDE_LENGTH; jj += POOLING_STEP_LENGTH)
				{
					double now = res2[i][ii][jj];

					out(ii / POOLING_STEP_LENGTH, jj / POOLING_STEP_LENGTH) = now;
				}
			}

			//输出到缓冲区
			OUT_D[i] = out;

		}
		OUT_N[0] = MODEL_GROUP;

		for (int i = 0; i<MODEL_WEIGHT + 1; i++)
		{
			for (int j = 0; j<MODEL_GROUP + 1; j++)
				delete[] model2[i][j];
			delete[] model2[i];
		}
		delete[] model2;//new和delete是成对出现的，这里也要多重循环释放掉空间
		model2 = NULL;

		for (int i = 0; i<MODEL_GROUP + 1; i++)
		{
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
				delete[] res2[i][j];
			delete[] res2[i];
		}
		delete[] res2;//new和delete是成对出现的，这里也要多重循环释放掉空间
		res2 = NULL;

		for (int i = 0; i<MODEL_WEIGHT + 1; i++)
		{
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
				delete[] c2_data[i][j];
			delete[] c2_data[i];
		}
		delete[] c2_data;//new和delete是成对出现的，这里也要多重循环释放掉空间
		c2_data = NULL;

		std::stringstream stag;
		stag << "converting finished!" << endl;
		POST_INFO(stag.str().c_str());
		fclose(bf);
		return true;
	}

	bool alexnet_simple_conv_2::Finalize()
	{

		return true;
	}

	double alexnet_simple_conv_2::max(double a, double b)
	{
		if (a > b) return a;
		return b;
	}
	void alexnet_simple_conv_2::mych(int tmp)
	{
		memset(num2, '\0', sizeof(num2));
		int tp = 0;
		while (tmp)
		{
			num2[tp++] = '0' + (tmp % 10);
			tmp /= 10;
		}
		for (int i = 0; i<tp / 2; i++)
		{
			swap(num2[i], num2[tp - 1 - i]);
		}
		num2[tp++] = '\0';
	}
	int alexnet_simple_conv_2::getres2(double f[], double **mm, double **res2, int mph, int mpl, int R)
	{
		if (f == NULL || mm == NULL || res2 == NULL) return 0;
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
				res2[i][j] += tmp;
			}
		}
		return 1;
	}

}