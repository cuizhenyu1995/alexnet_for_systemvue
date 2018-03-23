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
	class alexnet_simple_conv_2 : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(alexnet_simple_conv_2);
		alexnet_simple_conv_2();
		virtual ~alexnet_simple_conv_2();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();
		virtual int getres2(double f[], double **mm, double **res, int mph, int mpl, int R);
		virtual double max(double a, double b);
		virtual void mych(int tmp);

		/// define parameters
		int LENGTH_OF_SIDE;
		char* INPUT_FILE;

		/// define ports
		IntCircularBuffer IN_N;
		DoubleMatrixCircularBuffer IN_D;

		IntCircularBuffer OUT_N;
		DoubleMatrixCircularBuffer OUT_D;


	protected:

		DoubleMatrix in_tmp;
		DoubleMatrix out;

	};
}