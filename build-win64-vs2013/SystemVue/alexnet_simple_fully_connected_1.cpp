#pragma once
#include "alexnet_simple_fully_connected_1.h"
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

#define MODEL_WEIGHT 3
#define MODEL_GROUP 2
#define MODEL_SIZE 114*114
#define FILE_LENGTH_MAX 255
#define DATA_SIDE_LENGTH 114
#define DATA_WEIGHT 3

char Pre[] = "weights";
char Biase[] = "biases";
char Last[] = ".txt";
double ***Model;
double ***fc1_data;
double **res;


namespace SystemVueModelBuilder {

#ifndef SV_CODE_MRA_RUN
	DEFINE_MODEL_INTERFACE(alexnet_simple_fully_connected_1)
	{
		SET_MODEL_NAME("alexnet_simple_fully_connected_1");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("alexnet_simple_fully_connected");
		ADD_MODEL_HEADER_FILE("alexnet_simple_fully_connected_1.h");
		model.SetModelCodeGenName("alexnet_simple_fully_connected_1");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(INPUT_FILE);

		// Add input/output ports
		DFPort IN_RED = ADD_MODEL_INPUT(IN_1);
		DFPort IN_GREEN = ADD_MODEL_INPUT(IN_2);
		DFPort IN_BLUE = ADD_MODEL_INPUT(IN_3);

		DFPort OUT1 = ADD_MODEL_OUTPUT(OUT_1);



		return true;
	}
#endif

	alexnet_simple_fully_connected_1::alexnet_simple_fully_connected_1()
	{
	}

	alexnet_simple_fully_connected_1::~alexnet_simple_fully_connected_1()
	{
	}

	bool alexnet_simple_fully_connected_1::Setup()
	{
		IN_1.SetRate(1);
		IN_1.SetRate(1);
		IN_1.SetRate(1);

		OUT_1.SetRate(1);

		return true;
	};

	bool alexnet_simple_fully_connected_1::Initialize()
	{
		return true;
	}
	bool alexnet_simple_fully_connected_1::Run()
	{
		
		std::stringstream sut;
		sut << "fully connected start!" << endl;
		POST_INFO(sut.str().c_str());

		//为model获取空间
		int fail = 0;
		Model = new double **[MODEL_WEIGHT + 1];
		if (Model == NULL) fail = 1;
		for (int i = 0; i< MODEL_WEIGHT + 1; i++)
		{
			Model[i] = new double *[MODEL_GROUP + 1];
			if (Model[i] == NULL) fail = 1;
			for (int j = 0; j<MODEL_GROUP + 1; j++)
			{
				Model[i][j] = new double[MODEL_SIZE + 1];
				if (Model[i] == NULL) fail = 1;
			}
		}
		if (fail)
		{
			std::stringstream st;
			st << "Failed when getting space for model!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when getting space for model!" << endl;
			POST_INFO(st.str().c_str());
		}

		//打开model文件
		char s[FILE_LENGTH_MAX] = "";
		strcat(s, INPUT_FILE);
		strcat(s, Pre);
		strcat(s, Last);
		FILE *mf = fopen(s, "r");
		if (mf == NULL)
		{
			std::stringstream st;
			st << "Failed when opening model file!" << endl;
			POST_INFO(st.str().c_str());
			return true;
		}
		else
		{
			std::stringstream st;
			st << "Success when opening model file!" << endl;
			POST_INFO(st.str().c_str());
		}
		
		//读取model信息-------------------------------------------------------------------------
		for (int i = 0; i < MODEL_WEIGHT; i++)
			for (int j = 0; j < MODEL_SIZE; j++)
				for (int k = 0; k < MODEL_GROUP; k++)
					fscanf(mf, "%lf", &Model[i][k][j]);

		std::stringstream sst;
		sst << "Success read model!" << endl;
		POST_INFO(sst.str().c_str());
		
		//为res申请空间
		fail = 0;
		res = new double *[MODEL_WEIGHT + 1];
		if (res == NULL) fail = 1;
		for (int i = 0; i < MODEL_WEIGHT + 1; i++)
		{
			res[i] = new double[MODEL_GROUP + 1];
			if (res[i] == NULL) fail = 1;
		}
		if (fail)
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
		
		for (int i = 0; i < MODEL_WEIGHT + 1; i++)
			for (int j = 0; j < MODEL_GROUP + 1; j++)
				res[i][j] = 0;
		
		//为读取上一层分量申请空间
		fail = 0;
		fc1_data = new double **[DATA_WEIGHT + 1];
		if (fc1_data == NULL) fail = 1;
		for (int i = 0; i<DATA_WEIGHT + 1; i++)
		{
			fc1_data[i] = new double *[DATA_SIDE_LENGTH + 1];
			if (fc1_data[i] == NULL) fail = 1;
			for (int j = 0; j < DATA_SIDE_LENGTH + 1; j++)
			{
				fc1_data[i][j] = new double[DATA_SIDE_LENGTH + 1];
				if (fc1_data[i][j] == NULL) fail = 1;
			}
		}
		if (fail)
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
		
		//读取上一层分量
		for (int i = 0; i < DATA_SIDE_LENGTH; i++)
		{
			for (int j = 0; j < DATA_SIDE_LENGTH; j++)
			{
				fc1_data[0][i][j] = (double)(IN_1[0](i, j));
				fc1_data[1][i][j] = (double)(IN_2[0](i, j));
				fc1_data[2][i][j] = (double)(IN_3[0](i, j));
			}
		}

		//全连接操作-----------------------------------------------------------------------
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			for (int j = 0; j < MODEL_WEIGHT; j++)
			{
				if (getres(Model[j][i], fc1_data[j], &res[j][i], DATA_SIDE_LENGTH, DATA_SIDE_LENGTH) == 0)
				{
					std::stringstream st;
					st << "Failed when calculatting in fully connected layer!" << endl;
					POST_INFO(st.str().c_str());
					return true;
				}
			}

		}
		std::stringstream slst;
		slst << "Success when calculatting in fully connected layer!" << endl;
		POST_INFO(slst.str().c_str());

		//打开偏置文件
		char bis[FILE_LENGTH_MAX] = "";
		strcat(bis, INPUT_FILE);
		strcat(bis, Biase);
		strcat(bis, Last);
		FILE *bf=fopen(bis,"r");
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
		
		//偏置操作&放入matrix
		out.Resize(1, MODEL_GROUP);
		out.Zero();
		for (int i = 0; i < MODEL_GROUP; i++)
		{
			double ttt=0;
			fscanf(bf,"%lf", &ttt);
			for (int j = 0; j < MODEL_WEIGHT; j++)
				ttt += res[j][i];
			out(0, i) = ttt;
			
		}

		//放入输出缓存
		OUT_1[0] = out;

		//释放空间
		for (int i = 0; i<MODEL_WEIGHT + 1; i++)
		{
			for (int j = 0; j<MODEL_GROUP + 1; j++)
				delete[] Model[i][j];
			delete[] Model[i];
		}
		delete[] Model;//new和delete是成对出现的，这里也要多重循环释放掉空间
		Model = NULL;
		
		for (int i = 0; i<DATA_WEIGHT + 1; i++)
		{
			for (int j = 0; j<DATA_SIDE_LENGTH + 1; j++)
				delete[] fc1_data[i][j];
			delete[] fc1_data[i];
		}
		delete[] fc1_data;//new和delete是成对出现的，这里也要多重循环释放掉空间
		fc1_data = NULL;
		
		for (int i = 0; i < DATA_WEIGHT + 1; i++)
		{
			delete[] res[i];
		}
		delete[] res;
		res = NULL;

		std::stringstream stag;
		stag << "fully connected finished!" << endl;
		POST_INFO(stag.str().c_str());
		return true;
	}

	bool alexnet_simple_fully_connected_1::Finalize()
	{
		
		return true;
	}

	int alexnet_simple_fully_connected_1::getres(double f[], double **mm, double *res, int mph, int mpl)
	{
		if (f == NULL || mm == NULL || res == NULL) return 0;
		int tp = 0;
		for (int i = 0; i<mph; i++)
		{
			for (int j = 0; j<mpl; j++)
			{
				*res += f[tp++] * mm[i][j];
			}
		}
		return 1;
	}
}