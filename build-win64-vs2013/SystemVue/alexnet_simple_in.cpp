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
<<<<<<< HEAD
#include <malloc.h>
#define DATA_SIDE_LENGTH 227
#define FILE_LENGTH_MAX 255
#define CONST_BIASE_R 124
#define CONST_BIASE_G 117
#define CONST_BIASE_B 104

#define WIDTHBYTES(bits) (((bits)+31)/32*4)//用于使图像宽度所占字节数为4byte的倍数
#define MYDRAW_HEIGHT 227  //目标图像高度
#define MYDRAW_WIDTH 227  //目标图像宽度
#define DATA_WEIGHT 3
=======
#define DATA_SIDE_LENGTH 227
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c

using namespace std;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
<<<<<<< HEAD
typedef long                LONG;

typedef struct tagBITMAPFILEHEADER {
	DWORD  bfSize;          //文件大小
	WORD   bfReserved1;     //保留字，不考虑
	WORD   bfReserved2;     //保留字，同上
	DWORD  bfOffBits;       //实际位图数据的偏移字节数，即前三个部分长度之和
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	//public:
	DWORD   biSize;             //指定此结构体的长度，为40
	LONG    biWidth;            //位图宽
	LONG    biHeight;           //位图高
	WORD    biPlanes;           //平面数，为1
	WORD    biBitCount;         //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
	DWORD   biCompression;      //压缩方式，可以是0，1，2，其中0表示不压缩
	DWORD   biSizeImage;        //实际位图数据占用的字节数
	LONG    biXPelsPerMeter;    //X方向分辨率
	LONG    biYPelsPerMeter;    //Y方向分辨率
	DWORD   biClrUsed;          //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
	DWORD   biClrImportant;     //重要颜色数，如果为0，则表示所有颜色都是重要的
=======
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
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c
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
<<<<<<< HEAD
		DFPort OUT_DATA = ADD_MODEL_OUTPUT(OUT_D);
		OUT_DATA.SetDescription("Output DATA");
		DFPort OUT_NUM = ADD_MODEL_OUTPUT(OUT_N);
		OUT_NUM.SetDescription("Output NUMBER");
=======
		DFPort OUT_BLUE = ADD_MODEL_OUTPUT(OUT_B);
		OUT_BLUE.SetDescription("Output B");
		DFPort OUT_GREEN = ADD_MODEL_OUTPUT(OUT_G);
		OUT_GREEN.SetDescription("Output G");
		DFPort OUT_RED = ADD_MODEL_OUTPUT(OUT_R);
		OUT_RED.SetDescription("Output R");
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c

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
		//setRate的设置针对的是每一个端口，输入端口和输出端口都要进行设置，不设置的话默认为1
		//端口可以认为是一个Buffer, 下游的输入端口会拷贝上游的输出端口的内容，该拷贝为值拷贝或者说是深拷贝
<<<<<<< HEAD
		OUT_N.SetRate(1);
		OUT_D.SetRate(3);
=======
		OUT_R.SetRate(1);
		OUT_G.SetRate(1);
		OUT_B.SetRate(1);
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c

		return true;
	};

	bool alexnet_simple_in::Initialize()
	{
		return true;
	}

	//Run函数的终极目的就是产生数据，并赋值到相应的端口
	bool alexnet_simple_in::Run()
	{
		std::stringstream sut;
		sut << "read data start!" << endl;
		POST_INFO(sut.str().c_str());

		//输出接口初始化
		OUT_r.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_r.Zero();
		OUT_g.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_g.Zero();
		OUT_b.Resize(DATA_SIDE_LENGTH, DATA_SIDE_LENGTH);
		OUT_b.Zero();
		
		//读取bmp文件
<<<<<<< HEAD
		//if (ReadBmp(INPUT_FILE) == 0)
		//{
		//	std::stringstream sut;
		//	sut << "Failed when reading data!" << endl;
		//	POST_INFO(sut.str().c_str());
		//    return true;
		//}

		//if (bih.biWidth != DATA_SIDE_LENGTH || bih.biHeight != DATA_SIDE_LENGTH)
		{
			BITMAPFILEHEADER bitHead, writebitHead;
			BITMAPINFOHEADER bitInfoHead, writebitInfoHead;
			FILE* pfile;//输入文件
			FILE* wfile;//输出文件
			
			//char strFile[FILE_LENGTH_MAX] = INPUT_FILE;//打开图像路径，BMP图像必须为24位真彩色格式
			char strFile[FILE_LENGTH_MAX];
			int leng = strlen(INPUT_FILE);
			for (int i = 0; i < leng; i++) strFile[i] = INPUT_FILE[i];
			strFile[leng] = '\0';
			char strFilesave[FILE_LENGTH_MAX];//处理后图像存储路径
			strcpy(strFilesave, strFile);
			strFilesave[leng - 4] = '_';
			strFilesave[leng - 3] = '.';
			strFilesave[leng - 2] = 'b';
			strFilesave[leng - 1] = 'm';
			strFilesave[leng - 0] = 'p';
			strFilesave[leng + 1] = '\0';



			pfile = fopen(strFile, "rb");
			//fopen_s(&pfile, strFile, "rb");//文件打开图像
			wfile = fopen(strFilesave, "wb");
			//fopen_s(&wfile, strFilesave, "wb");//打开文件为存储修改后图像做准备
			//读取位图文件头信息.

			//std::stringstream sut;
			//sut << "JINAAAAAAAAAAAAAAAAA!" << endl;
			//POST_INFO(sut.str().c_str());

			WORD fileType;
			fread(&fileType, 1, sizeof(WORD), pfile);
			fwrite(&fileType, 1, sizeof(WORD), wfile);
			//    if (fileType != 0x4d42)
			//    {
			//        printf("file is not .bmp file!");
			//        return 0;
			//    }
			//std::stringstream sgt;
			//sgt << "JINBBBBBBBBBBBBBBBBB!" << endl;
			//POST_INFO(sgt.str().c_str());
			//读取位图文件头信息
			fread(&bitHead, 1, sizeof(tagBITMAPFILEHEADER), pfile);
			writebitHead = bitHead;//由于截取图像头和源文件头相似，所以先将源文件头数据赋予截取文件头
			//读取位图信息头信息
			fread(&bitInfoHead, 1, sizeof(BITMAPINFOHEADER), pfile);
			writebitInfoHead = bitInfoHead;//同位图文件头相似

			writebitInfoHead.biHeight = MYDRAW_HEIGHT;//为截取文件重写位图高度
			writebitInfoHead.biWidth = MYDRAW_WIDTH;//为截取文件重写位图宽度
			int mywritewidth = WIDTHBYTES(writebitInfoHead.biWidth*writebitInfoHead.biBitCount);//BMP图像实际位图数据区的宽度为4byte的倍数，在此计算实际数据区宽度
			writebitInfoHead.biSizeImage = mywritewidth*writebitInfoHead.biHeight;//计算位图实际数据区大小

			writebitHead.bfSize = 54 + writebitInfoHead.biSizeImage;//位图文件头大小为位图数据区大小加上54byte
			fwrite(&writebitHead, 1, sizeof(tagBITMAPFILEHEADER), wfile);//写回位图文件头信息到输出文件
			fwrite(&writebitInfoHead, 1, sizeof(BITMAPINFOHEADER), wfile);//写回位图信息头信息到输出文件

			int width = bitInfoHead.biWidth;
			int height = bitInfoHead.biHeight;
			//分配内存空间把源图存入内存
			int l_width = WIDTHBYTES(width*bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为4byte的倍数
			int write_width = WIDTHBYTES(writebitInfoHead.biWidth*writebitInfoHead.biBitCount);//计算写位图的实际宽度并确保它为4byte的倍数

			BYTE    *pColorData = (BYTE *)malloc(height*l_width);//开辟内存空间存储图像数据
			memset(pColorData, 0, height*l_width);

			BYTE    *pColorDataMid = (BYTE *)malloc(mywritewidth*MYDRAW_HEIGHT);//开辟内存空间存储图像处理之后数据
			memset(pColorDataMid, 0, mywritewidth*MYDRAW_HEIGHT);

			long nData = height*l_width;
			long write_nData = mywritewidth*MYDRAW_HEIGHT;//截取的位图数据区长度定义

			//把位图数据信息读到数组里
			fread(pColorData, 1, nData, pfile);//图像处理可通过操作这部分数据加以实现


			//双线性插值

			for (int hnum = 0; hnum < MYDRAW_HEIGHT; hnum++)
				for (int wnum = 0; wnum < MYDRAW_WIDTH; wnum++)
				{
					double d_original_img_hnum = hnum*height / (double)MYDRAW_HEIGHT;
					double d_original_img_wnum = wnum*width / (double)MYDRAW_WIDTH;
					int i_original_img_hnum = d_original_img_hnum;
					int i_original_img_wnum = d_original_img_wnum;
					double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;//在原图像中与a点的水平距离
					double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;//在原图像中与a点的垂直距离

					int original_point_a = i_original_img_hnum*l_width + i_original_img_wnum * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点A
					int original_point_b = i_original_img_hnum*l_width + (i_original_img_wnum + 1) * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点B
					int original_point_c = (i_original_img_hnum + 1)*l_width + i_original_img_wnum * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点C
					int original_point_d = (i_original_img_hnum + 1)*l_width + (i_original_img_wnum + 1) * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点D
					if (i_original_img_hnum + 1 == MYDRAW_HEIGHT - 1)
					{
						original_point_c = original_point_a;
						original_point_d = original_point_b;
					}
					if (i_original_img_wnum + 1 == MYDRAW_WIDTH - 1)
					{
						original_point_b = original_point_a;
						original_point_d = original_point_c;
					}

					int pixel_point = hnum*write_width + wnum * 3;//映射尺度变换图像数组位置偏移量
					pColorDataMid[pixel_point] =
						pColorData[original_point_a] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
						pColorData[original_point_b] * distance_to_a_x*(1 - distance_to_a_y) +
						pColorData[original_point_c] * distance_to_a_y*(1 - distance_to_a_x) +
						pColorData[original_point_c] * distance_to_a_y*distance_to_a_x;
					pColorDataMid[pixel_point + 1] =
						pColorData[original_point_a + 1] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
						pColorData[original_point_b + 1] * distance_to_a_x*(1 - distance_to_a_y) +
						pColorData[original_point_c + 1] * distance_to_a_y*(1 - distance_to_a_x) +
						pColorData[original_point_c + 1] * distance_to_a_y*distance_to_a_x;
					pColorDataMid[pixel_point + 2] =
						pColorData[original_point_a + 2] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
						pColorData[original_point_b + 2] * distance_to_a_x*(1 - distance_to_a_y) +
						pColorData[original_point_c + 2] * distance_to_a_y*(1 - distance_to_a_x) +
						pColorData[original_point_c + 2] * distance_to_a_y*distance_to_a_x;

				}
			//双线性插值

			fwrite(pColorDataMid, 1, write_nData, wfile);   //将处理完图像数据区写回文件
			fclose(pfile);
			fclose(wfile);

			strcpy(INPUT_FILE, strFilesave);
			std::stringstream siut;
			siut << INPUT_FILE << endl;
			POST_INFO(siut.str().c_str());
			fclose(pfile);
			fclose(wfile);
		}

=======
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c
		if (ReadBmp(INPUT_FILE) == 0)
		{
			std::stringstream sut;
			sut << "Failed when reading data!" << endl;
			POST_INFO(sut.str().c_str());
<<<<<<< HEAD
			return true;
		}
		else
		{
			std::stringstream sut;
			sut << "success when reading data!" << endl;
			POST_INFO(sut.str().c_str());
		}
		//std::stringstream sutk;
		//sutk << "w=" << bih.biWidth<< " h="<<bih.biHeight << endl;
		//POST_INFO(sutk.str().c_str());
=======
		    return true;
		}

>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c
		//读取bmp数据&存入matrix----------------------------------------------------------------
		BYTE r, g, b;
		for (int x = 0; x<DATA_SIDE_LENGTH; x++)
		{
			for (int y = 0; y<DATA_SIDE_LENGTH; y++)
			{
				GetDIBColor( y, x, &r, &g, &b);
<<<<<<< HEAD
				OUT_r(x, y) = (int)r-CONST_BIASE_R;
				OUT_g(x, y) = (int)g-CONST_BIASE_G;
				OUT_b(x, y) = (int)b-CONST_BIASE_B;
			}
		}
		//for (int i = 0; i < 20; i++)
		//{
		//	std::stringstream rrr;
		//	rrr << OUT_b(0, i) - 104 << " " << OUT_g(0, i) - 117 << " " << OUT_r(0, i) - 124 << "  no." << i << endl;
		//	POST_INFO(rrr.str().c_str());
		//}
		//向后发送数据
		OUT_N[0] = DATA_WEIGHT;
		OUT_D[0] = OUT_b;
		OUT_D[1] = OUT_g;
		OUT_D[2] = OUT_r;

		//释放空间
		free(Buffer);
		
		std::stringstream sult;
		sult << "read data fffff!" << endl;
=======
				OUT_r(x, y) = (int)r;
				OUT_g(x, y) = (int)g;
				OUT_b(x, y) = (int)b;
			}
		}
		
		//向后发送数据
		OUT_R[0] = OUT_r;
		OUT_G[0] = OUT_g;
		OUT_B[0] = OUT_b;

		//释放空间
		free(Buffer);

		std::stringstream sult;
		sult << "read data start!" << endl;
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c
		POST_INFO(sult.str().c_str());
		return true;
	}


	bool alexnet_simple_in::Finalize() {

		return true;
	}
	//读取bmp文件
	int alexnet_simple_in::ReadBmp(const char* szFileName){
		FILE *file;
		WORD bfh[7];
		long dpixeladd;
		if (NULL == (file = fopen(szFileName, "rb")))
		{
<<<<<<< HEAD
			fclose(file);
=======
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c
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

	//读取bmp数据
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