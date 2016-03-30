#ifndef LOGIC_H_
#define LOGIC_H_

namespace hxl
{
	class Dlx
	{
	public:
		Dlx();

		void Dance();
	private:
		Loki::PimpleOf<Dlx>::Type imp_;
	};
};


#endif