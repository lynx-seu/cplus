#ifndef LOGIC_H_
#define LOGIC_H_

#include "Loki/Pimpl.h"

namespace hxl
{
	class Dlx
	{
	public:
		Dlx(char **matrix, int row, int col);

		void Dance();
	private:
		Loki::PimpleOf<Dlx>::Type dlx_;
	};
};


#endif