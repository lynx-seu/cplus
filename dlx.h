#ifndef LOGIC_H_
#define LOGIC_H_

#include <memory>

namespace hxl {

////////////////////////////////////////////////////////////////////////////////
// class Dlx
// Use to slove exact cover problem
////////////////////////////////////////////////////////////////////////////////
	class Dlx
	{
	public:
		enum { nullrow = -1 };
		typedef void (*FUNC)(int);

		Dlx(int *matrix, int row, int col);
		~Dlx();

		void Dance(FUNC send_row);
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}


#endif
