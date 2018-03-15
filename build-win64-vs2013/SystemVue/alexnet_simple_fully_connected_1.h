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
#define DATA_SIDE_LENGTH 114

namespace SystemVueModelBuilder
{
	class alexnet_simple_fully_connected_1 : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(alexnet_simple_fully_connected_1);
		alexnet_simple_fully_connected_1();
		virtual ~alexnet_simple_fully_connected_1();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();

		virtual int getres(double f[], double **mm, double *res, int mph, int mpl);

		/// define parameters
		char* INPUT_FILE;

		/// define ports

		DoubleMatrixCircularBuffer IN_1;
		DoubleMatrixCircularBuffer IN_2;
		DoubleMatrixCircularBuffer IN_3;

		DoubleMatrixCircularBuffer OUT_1;


	protected:

		DoubleMatrix in_tmp;
		DoubleMatrix out;

	};
}