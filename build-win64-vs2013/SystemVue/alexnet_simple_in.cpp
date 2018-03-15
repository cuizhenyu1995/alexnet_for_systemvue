#include "alexnet_simple_in.h"
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
#define DATA_SIDE_LENGTH 227

using namespace std;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef struct tagBITMAPINFOHEADER{
	DWORD biSize;
	long   biWidth;
	long   biHeight;
	WORD   biPlanes;
	WORD   biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

BITMAPINFOHEADER bih;
BYTE *Buffer = NULL;
long LineByteWidth;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_MRA_IN
	DEFINE_MODEL_INTERFACE(alexnet_simple_in)
	{
		SET_MODEL_NAME("alexnet_simple_in");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("alexnet_simple_in");
		ADD_MODEL_HEADER_FILE("alexnet_simple_in.h");
		model.SetModelCodeGenName("alexnet_simple_in");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;

		param = ADD_MODEL_PARAM(INPUT_FILE);
		param.SetParamAsFile();

		// Add input/output ports
		DFPort OUT_BLUE = ADD_MODEL_OUTPUT(OUT_B);
		OUT_BLUE.SetDescription("Output B");
		DFPort OUT_GREEN = ADD_MODEL_OUTPUT(OUT_G);
		OUT_GREEN.SetDescription("Output G");
		DFPort OUT_RED = ADD_MODEL_OUTPUT(OUT_R);
		OUT_RED.SetDescription("Output R");

		return true;
	}
#endif

	alexnet_simple_in::alexnet_simple_in()
	{
	}

	alexnet_simple_in::~alexnet_simple_in()
	{
	}

	bool alexnet_simple_in::Setup()
	{
		//setRate��������Ե���ÿһ���˿ڣ�����˿ں�����˿ڶ�Ҫ�������ã������õĻ�Ĭ��Ϊ1
		//�˿ڿ�����Ϊ��һ��Buffer, ���ε�����˿ڻ´�����ε�����˿ڵ����ݣ��ÿ���Ϊֵ��������˵�����
		OUT_R.SetRate(1);
		OUT_G.SetRate(1);
		OUT_B.SetRate(1);

		return true;
	};

	bool alexnet_simple_in::Initialize()
	{
		return true;
	}

	//Run�������ռ�Ŀ�ľ��ǲ������ݣ�����ֵ����Ӧ�Ķ˿�
	bool alexnet_simple_in::Run()
	{
		std::stringstream sut;
		sut << "read data start!" << endl;
		POST_INFO(sut.str().c_str());

		//����ӿڳ�ʼ��
		OUT_r.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_r.Zero();
		OUT_g.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_g.Zero();
		OUT_b.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_b.Zero();
		
		//��ȡbmp�ļ�
		if (ReadBmp(INPUT_FILE) == 0)
		{
			std::stringstream sut;
			sut << "Failed when reading data!" << endl;
			POST_INFO(sut.str().c_str());
		    return true;
		}

		//��ȡbmp����&����matrix----------------------------------------------------------------
		BYTE r, g, b;
		for (int x = 0; x<DATA_SIDE_LENGTH; x++)
		{
			for (int y = 0; y<DATA_SIDE_LENGTH; y++)
			{
				GetDIBColor( y, x, &r, &g, &b);
				OUT_r(x, y) = (int)r;
				OUT_g(x, y) = (int)g;
				OUT_b(x, y) = (int)b;
			}
		}
		
		//���������
		OUT_R[0] = OUT_r;
		OUT_G[0] = OUT_g;
		OUT_B[0] = OUT_b;

		//�ͷſռ�
		free(Buffer);

		std::stringstream sult;
		sult << "read data start!" << endl;
		POST_INFO(sult.str().c_str());
		return true;
	}


	bool alexnet_simple_in::Finalize() {

		return true;
	}
	//��ȡbmp�ļ�
	int alexnet_simple_in::ReadBmp(const char* szFileName){
		FILE *file;
		WORD bfh[7];
		long dpixeladd;
		if (NULL == (file = fopen(szFileName, "rb")))
		{
			return 0;
		}
		
		fread(&bfh, sizeof(WORD), 7, file);
		if (bfh[0] != (WORD)(((WORD)'B') | ('M' << 8)))
		{
			fclose(file);
			return 0;
		}
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);
		if (bih.biBitCount < 24)
		{
			fclose(file);
			return 0;
		}
		dpixeladd = bih.biBitCount / 8;
		LineByteWidth = bih.biWidth * (dpixeladd);
		if ((LineByteWidth % 4) != 0)
			LineByteWidth += 4 - (LineByteWidth % 4);
		if ((Buffer = (BYTE*)malloc(sizeof(BYTE)* LineByteWidth * bih.biHeight)) != NULL)
		{
			fread(Buffer, LineByteWidth * bih.biHeight, 1, file);
			fclose(file);
			return 1;
		}
		fclose(file);
		return 0;
	}

	//��ȡbmp����
	int alexnet_simple_in::GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b){
		int dpixeladd;
		BYTE *ptr;
		if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)
		{
			return 0;
		}
		dpixeladd = bih.biBitCount / 8;
		ptr = Buffer + X * dpixeladd + (bih.biHeight - 1 - Y) * LineByteWidth;
		*b = *ptr;
		*g = *(ptr + 1);
		*r = *(ptr + 2);
		return 1;
	}
}