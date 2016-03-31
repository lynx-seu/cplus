#ifndef LOGIC_H_
#define LOGIC_H_


#include <set>
#include <vector>
#include <memory>

namespace hxl {

	class Dlx
	{
	public:
		Dlx(int *matrix, int row, int col);
		~Dlx();

		void Dance();
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}


#endif
