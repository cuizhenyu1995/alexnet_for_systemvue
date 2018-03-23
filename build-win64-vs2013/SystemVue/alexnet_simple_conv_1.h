#pragma once
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

namespace SystemVueModelBuilder
{
	class alexnet_simple_conv_1 : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(alexnet_simple_conv_1);
		alexnet_simple_conv_1();
		virtual ~alexnet_simple_conv_1();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();
		virtual int getres(double f[], double **mm, double **res, int mph, int mpl, int R);
		virtual double max(double a, double b);
		virtual void mych(int tmp);

		/// define parameters
		int LENGTH_OF_SIDE;
		char* INPUT_FILE;

		/// define ports
<<<<<<< HEAD
		IntCircularBuffer IN_N;
		IntMatrixCircularBuffer IN_D;

		IntCircularBuffer OUT_N;
		DoubleMatrixCircularBuffer OUT_D;
=======

		IntMatrixCircularBuffer IN_R;
		IntMatrixCircularBuffer IN_G;
		IntMatrixCircularBuffer IN_B;

		DoubleMatrixCircularBuffer OUT_1;
		DoubleMatrixCircularBuffer OUT_2;
		DoubleMatrixCircularBuffer OUT_3;
>>>>>>> d296eb9cd2398f0c4f0035ea10bb4ca09fb8da3c


	protected:
		
		DoubleMatrix in_tmp;
		DoubleMatrix out;

	};
}