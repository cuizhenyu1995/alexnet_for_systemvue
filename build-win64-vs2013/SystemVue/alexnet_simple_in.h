#pragma once
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <iostream>

typedef unsigned char       BYTE;

namespace SystemVueModelBuilder
{

	class alexnet_simple_in : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(alexnet_simple_in);
		alexnet_simple_in();
		virtual ~alexnet_simple_in();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();

		virtual int ReadBmp(const char* szFileName);
		int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b);


		/// define useless parameters, just for illustrating how to add parameters 
		//字符串类型，只能用char*， 而不能用string类型


		char* INPUT_FILE;


		/// define input and output ports
		IntMatrixCircularBuffer OUT_R;
		IntMatrixCircularBuffer OUT_G;
		IntMatrixCircularBuffer OUT_B;


	protected:
		IntMatrix OUT_r;
		IntMatrix OUT_g;
		IntMatrix OUT_b;
	};
}