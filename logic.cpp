#include "logic.h"
#include <set>
#include <vector>
#include <cassert>
#include <functional>
#include <algorithm>

namespace hxl {

	struct Node;
	typedef std::function<void (Node *)> FUNC; 

	struct Node
	{
		friend inline void for_each_vertical(Node *, const FUNC&);
		friend inline void for_each_horizontal(Node *, const FUNC&);
		typedef Node * NodePtr;

		int r, c;
		NodePtr U, R, D, L;

		Node(int row=0, int col=0, NodePtr u=0, NodePtr r=0, NodePtr d=0, NodePtr l=0)
		: r(row), c(col)
		, U(u), R(r), D(d), L(l)
		{ }

		void Set(NodePtr u, NodePtr r, NodePtr d, NodePtr l)
		{
			U = u; R = r; D = d; L = l;
		}
	};

	inline void for_each_vertical(Node * node, const FUNC& func)
	{
		Node *d = node->D;
		while (d != node)
		{
			func(node);
			d = d->D;
		}
	}

	inline void for_each_horizontal(Node * node, const FUNC& func)
	{
		Node *r = node->R;
		while (r != node)
		{
			func(node);
			r = r->R;
		}
	}

	inline void release_node(Node *node)
	{
		if (node)
		{
			delete node;
			node = 0;
		}
	}

} // end namespace Loki


namespace Loki {
	using namespace hxl;

	template<>
	class ImplOf<Dlx>
	{
	public:
		ImplOf(): pHead_(new Node) {}
		~ImplOf()
		{
			delete pHead_;

			std::for_each(colHeadArr_.begin(), colHeadArr_.end(), [&] (Node *node) {
				for_each_vertical(node, release_node);
				release_node(node);
			});
		}

		void init (char **matrix, int row, int col)
		{
			colHeadArr_.resize(col, 0);
			colCount_.resize(col, 0);

			pHead_->Set(pHead_, pHead_, pHead_, pHead_);
			for (int c=0; c<col; c++)
			{
				Node *pNode = new Node(-1, c);
				pNode->Set(pNode, pHead_, pNode, pHead_->L);
				pNode->R->L = pNode;
				pNode->L->R = pNode;
				colHeadArr_.at(c) = pNode;
			}

			for (int r=0; r<row; r++)
			{
				bool first = true;
				for (int c=0; c<col; c++)
				{
					char ch = matrix[r][c];
					assert(ch == '0' || ch == '1');

					if (ch == '0') continue;
					Node *pColHead = colHeadArr_.at(c);
					Node *pRowHead = 0;
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

					colCount_[col]++;
				}
			}
		}
		void Dance()
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
			FUNC f2 = [&](Node *node) {
				Remove(node->c);
			};
			FUNC f3 = [&](Node *node) {
				Resume(node->c);
			};
			FUNC f1 = [&](Node *node) {
				res.insert(node->r);
				for_each_horizontal(node, f2);
				Dance();
				res.erase(node->r);
				for_each_horizontal(node, f3);
			};
			for_each_vertical(pColHead, f1);
			Resume(col);
		}
	private:
		inline int GetMinCol() const
		{
			int min = 0xFFFFFF;

			for (size_t i=0; i<colCount_.size(); i++)
			{
				int cnt = colCount_.at(i);
				if (min > cnt) min = cnt;

				if (min <= 1) break;
			}
			return min;
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

			FUNC f1 = [&](Node *node)
			{
				for_each_horizontal(node, f2);
			};
			for_each_vertical(pColHead, f1);
		}

		void Resume(int col)
		{
			Node *pColHead = colHeadArr_[col];

			FUNC f2 = [&, col] (Node *node)
			{
				colCount_[col] ++;
				node->D->U = node; node->U->D = node;
			};

			FUNC f1 = [&](Node *node)
			{
				for_each_horizontal(node, f2);
			};
			for_each_vertical(pColHead, f1);


			pColHead->R->L = pColHead; pColHead->L->R = pColHead;
		}

	private:
		Node *pHead_;
		std::vector<Node *> colHeadArr_;
		std::vector<int>    colCount_;
		std::vector<std::set<int> > res_;
	};
} // end namespace Loki

