#ifndef LOGIC_H_
#define LOGIC_H_


#include <set>
#include <vector>
#include <memory>

namespace hxl {

	class Dlx
	{
	public:
		Dlx(const char **matrix, int row);
		~Dlx();

		void Dance();
		std::vector<std::set<int> > GetRes() const;
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}


#endif