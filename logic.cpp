#include "logic.h"
#include <set>
#include <vector>
#include <functional>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include <limits.h>

namespace hxl {

	class Dlx::Impl
	{
		friend class Dlx;

		struct data_object //A module in the sparse matrix data structure.
		{
			data_object* L;                //Link to next object left.
			data_object* R;                //         "          right.
			data_object* U;                //         "          up.
			data_object* D;                //         "          down.
			data_object* C;                //Link to column header.
			int x;                         //In a column header: number of ones 
												//in the column. Otherwise: row index.
			void cover()                   //Covers a column.
			{
				data_object* i=D;
				data_object* j;
				R->L=L; L->R=R;
				while (i!=this)
				{
					j=i->R;
					while (j!=i)
					{
						j->D->U=j->U; j->U->D=j->D;
						j->C->x--;
						j=j->R;
					}
					i=i->D;
				}
			}
			void uncover()                 //Uncovers a column.
			{
				data_object* i=U;
				data_object* j;
				while (i!=this)
				{
					j=i->L;
					while (j!=i)
					{
						j->C->x++;
						j->D->U=j; j->U->D=j;
						j=j->L;
					}
					i=i->U;
				}
				R->L=this; L->R=this;
			}
		};

		Impl(int *matrix, int rows, int cols);
		~Impl();
		void Search(int k);
		static data_object* DLX_Knuth_S_heuristic(data_object* root);

	private:
		data_object *root_;			//root
	};

	Dlx::Impl::Impl(int *matrix, int rows, int cols)
		: root_ (new data_object)
	{
		data_object *P = root_, *Q;
		//array of pointers to column headers
		data_object** walkers=new data_object*[cols];

		for (int i=0; i<cols; i++)
		{
			(P->R=new data_object)->L = P;
			walkers[i] = Q = P = P->R;
			P->x = 0;
			for (int j=0; j<rows; j++)
				if (matrix[i+cols*j])
				{
					(Q->D=new data_object)->U = Q;
					Q = Q->D;
					Q->C = P; P->x++;
					Q->x = j;
				}
			Q->D = P; P->U = Q;
		}
		P->R = root_; root_->L = P;
		//eliminate empty columns
		P=root_;
		for (int i=0; i<cols; i++)
		{
			P=P->R;
			if (!P->x)
			{
				P->L->R=P->R;
				P->R->L=P->L;
			}
		}
		//now construct the L/R links for the data objects.
		P=new data_object;
		for (int i=0; i<rows; i++)
		{
			Q=P;
			for (int j=0; j<cols; j++)
				if (matrix[j+cols*i]) //a one
				{
					//in _this_ row...
					walkers[j]=walkers[j]->D;
					//create L/R links
					(Q->R=walkers[j])->L=Q;
					//advance pointer
					Q=Q->R;
				}
				if (Q==P) continue;
				Q->R=P->R;       //link it to the first one in this row.
				P->R->L=Q;       //link the first one to the last one.
		}
		delete P;                //P is no longer needed
		delete walkers;          //walkers are no longer needed

	}

	Dlx::Impl::~Impl()
	{

	}

	void Dlx::Impl::Search(int k)
	{

	}

	Dlx::Impl::data_object* Dlx::Impl::DLX_Knuth_S_heuristic(Dlx::Impl::data_object* root)
	{
		data_object* P=root->R;
		data_object* res;
		int best=INT_MAX/2;
		while (P!=root)
		{
			if (P->x<best)
			{
				best=P->x;
				res=P;
			}
			P=P->R;
		}
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	Dlx::Dlx(int *matrix, int rows, int cols)
		: pImpl(new Impl(matrix, rows, cols))
	{

	}

	Dlx::~Dlx()
	{

	}

	void Dlx::Dance()
	{
		pImpl->Search(0);
	}

} // end namespace hxl

