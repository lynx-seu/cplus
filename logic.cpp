#include "logic.h"
#include <set>
#include <vector>
#include <functional>
#include <algorithm>
#include <assert.h>
#include <string.h>

namespace hxl {

	//////////////////////////////////////////////////////////////////////////
	// Dlx Impl
	class Dlx::Impl
	{
	public:
		friend class Dlx;

		struct Node
		{
			Node(int r = -1, int c = -1): r(r), c(c) {}
			void Set(Node *u, Node *r, Node *d, Node *l)
			{ U = u; R = r; D = d; L = l; }

			int r, c;
			struct Node *U, *R, *D, *L;
		};
		typedef std::function<void (Node *)> FUNC;

		Impl(const char **matrix, int row) :pHead_(new Node)
		{
			int col = 0;
			for (int r=0; r<row; r++)
			{
				int c = strlen(matrix[r]);
				if (col < c) col = c;
			}
			colHeadArr_.resize(col, 0);
			colCount_.resize(col, 0);

			pHead_->Set(pHead_, pHead_, pHead_, pHead_);
			for (int c=0; c<col; c++)
			{
				Node *pNode = new Node(-1, c);
				pNode->Set(pNode, pHead_, pNode, pHead_->L);
				pNode->R->L = pNode; pNode->L->R = pNode;
				colHeadArr_.at(c) = pNode;
			}

			for (int r=0; r<row; r++)
			{
				bool first = true;
				Node *pRowHead = 0;
				for (int c=0; c<col; c++)
				{
					char ch = matrix[r][c];
					assert(ch == '0' || ch == '1');

					if (ch == '0') continue;
					Node *pColHead = colHeadArr_.at(c);
					Node *pNode = new Node(r, c);
					if (first)
					{
						first = false;
						pRowHead = pNode;
						pNode->Set(pColHead->U, pNode, pColHead, pNode);
					}
					else
					{
						pNode->Set(pColHead->U, pRowHead, pColHead, pRowHead->L);
						pNode->R->L = pNode; pNode->L->R = pNode;
					}
					pNode->U->D = pNode; pNode->D->U = pNode;

					colCount_[c]++;
				}
			}
		}

		~Impl() 
		{
			ReleaseNode(pHead_);
			std::for_each(colHeadArr_.begin(), colHeadArr_.end(), [&](Node *node) {
				Node *d = node->D;
				while (d != node) 
				{
					Node *tmp = d->D;
					ReleaseNode(d);
					d = tmp;
				}
				ReleaseNode(node);
			});
		}

		void Dance ()
		{
			static std::set<int> res;

			if (pHead_->R == pHead_)
			{
				res_.push_back(res);
				res.clear();
				return;
			}

			int col = GetMinCol();
			Remove(col);

			Node *pColHead = colHeadArr_.at(col);
			Node *d = pColHead->D;
			while (d != pColHead)
			{
				res.insert(d->r);
				Node *dr = d->R;
				while (dr != d) { Remove(dr->c); dr = dr->R; }

				Dance();

				res.erase(d->r);
				dr = d->R;
				while (dr != d) { Resume(dr->c); dr = dr->R; }
				d = d->D;
			}

			Resume(col);
		}

	private:

		static inline void ReleaseNode(Node *node)
		{
			if (node)
			{
				delete node;
				node = 0;
			}
		}

		static inline void ForEach(Node *node, const FUNC& func)
		{
			Node *d = node->D;
			while (d != node)
			{
				Node *dr = d->R;
				while (dr != d)
				{
					func(dr);
					dr = dr->R;
				}

				d = d->D;
			}

		}

		inline int GetMinCol() const
		{
			int min = 0xFFFFFF, col=0;

			Node *r = pHead_->R;
			while (r != pHead_)
			{
				int cnt = colCount_.at(r->c);
				if (min > cnt)
				{
					min = cnt;
					col = r->c;
				}
				if (min <= 1) break;

				r = r->R;
			}

			return col;
		}

		void Remove(int col)
		{
			Node *pColHead = colHeadArr_[col];
			pColHead->R->L = pColHead->L; pColHead->L->R = pColHead->R;

			FUNC f2 = [&, col] (Node *node)
			{
				colCount_[col]--;
				node->D->U = node->U; node->U->D = node->D;
			};
			ForEach(pColHead, f2);
		}

		void Resume(int col)
		{
			Node *pColHead = colHeadArr_[col];

			FUNC f2 = [&, col] (Node *node)
			{
				colCount_[col] ++;
				node->D->U = node; node->U->D = node;
			};
			ForEach(pColHead, f2);

			pColHead->R->L = pColHead; pColHead->L->R = pColHead;
		}

	private:
		Node *pHead_;
		std::vector<Node *> colHeadArr_;
		std::vector<int>    colCount_;
		std::vector<std::set<int> > res_;
	};

	//////////////////////////////////////////////////////////////////////////
	Dlx::Dlx(const char **matrix, int row)
		: pImpl(new Impl(matrix, row))
	{

	}

	Dlx::~Dlx()
	{

	}

	void Dlx::Dance()
	{
		pImpl->Dance();
	}

	std::vector<std::set<int> > Dlx::GetRes() const
	{
		return pImpl->res_;
	}

} // end namespace hxl

