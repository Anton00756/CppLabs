#ifndef HEAPS
#define HEAPS
#include "comp.h"
#include<vector>

class HeapExc : public std::exception
{
public:
	virtual string text() = 0;

protected:
	string error;
};

class HeapInsertExc : public HeapExc
{
public:
	HeapInsertExc() { this->error = "ќшибка вставки!"; }
	string text() override { return error; }
};

class HeapGetMinExc : public HeapExc
{
public:
	HeapGetMinExc() { this->error = "ћинимум найти невозможно, так как куча пуста!"; }
	string text() override { return error; }
};

class HeapDelMinExc : public HeapExc
{
public:
	HeapDelMinExc() { this->error = "”даление минимума невозможно, так как куча пуста!"; }
	string text() override { return error; }
};

template <class TKey, class TValue>
class Heap
{
public:
	virtual void insert(const TKey& key, const TValue& value) = 0;
	virtual TValue get_min() = 0;
	virtual TValue del_min() = 0;
	virtual Heap* merge(Heap* heap) = 0;
	virtual ~Heap() {}
};

template <class TKey, class TValue>
class BinHeap : public Heap<TKey, TValue>
{
public:
	BinHeap(const Comparer<TKey>* cmp)
	{
		this->comp = cmp;
	}

	~BinHeap()
	{
		for (int i = 0; i < arr.size(); i++)
			delete arr[i];
	}

	void insert(const TKey& key, const TValue& value) override
	{
		arr.push_back(new Node(key, value));
		up(arr.size() - 1);
	}

	TValue get_min() override
	{
		if (arr.empty())
			throw HeapGetMinExc();
		return arr[0]->value;
	}

	TValue del_min() override
	{
		if (arr.empty())
			throw HeapDelMinExc();
		TValue tmp = arr[0]->value;
		swap(0, arr.size() - 1);
		delete arr[arr.size() - 1];
		arr.pop_back();
		return tmp;
	}

	Heap<TKey, TValue>* merge(Heap<TKey, TValue>* heap) override
	{
		BinHeap* tmp = dynamic_cast<BinHeap*>(heap);
		for (int i = 0; i < tmp->arr.size(); i++)
			arr.push_back(tmp->arr[i]);
		heaper();
		heap = tmp;
		return heap;
	}

private:
	struct Node
	{
		Node(TKey k, TValue v) { key = k; value = v; }
		TKey key;
		TValue value;
	};

	vector<Node*> arr;
	const Comparer<TKey>* comp;

	void up(int index)
	{
		while (index && (comp->compare(arr[index]->key, arr[(index - 1) / 2]->key) < 0))
		{
			swap(index, (index - 1) / 2);
			index = (index - 1) / 2;
		}
	}

	void down(int ind)
	{
		int min_ind;
		while (true)
		{
			if (ind * 2 + 1 < arr.size())
				min_ind = ind * 2 + 1;
			else
				break;
			if (ind * 2 + 2 < arr.size())
				if (comp->compare(arr[ind * 2 + 1]->key, arr[ind * 2 + 2]->key) > 0)
					min_ind = ind * 2 + 2;
			swap(ind, min_ind);
			ind = min_ind;
		}
	}

	void heaper()
	{
		for (int i = arr.size() / 2 - 1; i != 0; i--)
			down(i);
	}

	void swap(int ind1, int ind2)
	{
		Node tmp(arr[ind1]->key, arr[ind1]->value);
		arr[ind1]->key = arr[ind2]->key;
		arr[ind1]->value = arr[ind2]->value;
		arr[ind2]->key = tmp.key;
		arr[ind2]->value = tmp.value;
	}
};

template<class TKey, class TValue>
class BinomialHeap : public Heap<TKey, TValue>
{
private:
	struct Node
	{
		Node(const TKey& k, const TValue& v) { key = k; value = v; }
		Node* parent = nullptr, *child = nullptr, *sibling = nullptr;
		TKey key;
		TValue value;
		int degree = 0;
	};

	Node* root = nullptr;
	const Comparer<TKey>* comp;

public:
	BinomialHeap(const Comparer<TKey>* cmp)
	{
		this->comp = cmp;
	}

	~BinomialHeap()
	{
		clean(root);
	}

	Heap<TKey, TValue>* merge(Heap<TKey, TValue>* heap) override
	{
		BinomialHeap* new_heap = dynamic_cast<BinomialHeap*>(heap);
		Node* tmp_1 = root, *tmp_2 = new_heap->root;
		vector<Node*> new_root;

		if ((root == nullptr) || (new_heap->root == nullptr))
		{
			if (root == nullptr)
				root = new_heap->root;
			return this;
		}

		while ((tmp_1 != nullptr) && (tmp_2 != nullptr))
		{
			if (comp->compare(tmp_1->degree, tmp_2->degree) < 0)
			{
				new_root.push_back(tmp_1);
				tmp_1 = tmp_1->sibling;
			}
			else
			{
				new_root.push_back(tmp_2);
				tmp_2 = tmp_2->sibling;
			}
		}

		while (tmp_1 != nullptr)
		{
			new_root.push_back(tmp_1);
			tmp_1 = tmp_1->sibling;
		}

		while (tmp_2 != nullptr)
		{
			new_root.push_back(tmp_2);
			tmp_2 = tmp_2->sibling;
		}

		root = new_root[0];
		for (int i = 0; i < new_root.size(); i++)
		{
			if (i == new_root.size() - 1)
				new_root[i]->sibling = nullptr;
			else
				new_root[i]->sibling = new_root[i + 1];
		}
		
		Node* prev = nullptr, *cur = root, *next = root->sibling;

		while (next != nullptr)
		{
			if (!comp->compare(cur->degree, next->degree) && !((next->sibling != nullptr) && !comp->compare(next->sibling->degree, cur->degree)))
			{
				if (comp->compare(cur->key, next->key) < 0)
				{
					cur->sibling = next->sibling;
					next->sibling = cur->child;
					cur->child = next;
					next->parent = cur;
					cur->degree++;
					next = cur->sibling;
				}
				else
				{
					if (prev != nullptr)
						prev->sibling = next;
					else
						root = next;
					cur->sibling = next->child;
					next->child = cur;
					cur->parent = next;
					next->degree++;
					cur = next;
					next = next->sibling;
				}
			}
			else
			{
				prev = cur;
				cur = next;
				next = next->sibling;
			}
		}
		new_heap->root = nullptr;
		return this;
	}

	void insert(const TKey& key, const TValue& value) override
	{
		if (root == nullptr)
		{
			root = new Node(key, value);
			return;
		}
		BinomialHeap heap(comp);
		heap.root = new Node(key, value);
		merge(&heap);
	}

	TValue del_min() override
	{
		TValue del_val;
		BinomialHeap heap(comp);
		Node* tmp = root, *tmp_2, *tmp_3 = nullptr, *min_node = root, *pre_min = nullptr;
		if (root == nullptr)
			throw HeapDelMinExc();
		while (tmp->sibling != nullptr)
		{
			if (comp->compare(tmp->sibling->key, min_node->key) < 0)
			{
				pre_min = tmp;
				min_node = tmp->sibling;
			}
			tmp = tmp->sibling;
		}

		del_val = min_node->value;
		if (pre_min != nullptr)
			pre_min->sibling = min_node->sibling;
		else
			root = min_node->sibling;
		tmp = min_node->child;
		if (tmp != nullptr)
			while (tmp != nullptr) // разворачиваем список деревьев
			{
				tmp->parent = nullptr;
				tmp_2 = tmp->sibling;
				tmp->sibling = tmp_3;
				tmp_3 = tmp;
				tmp = tmp_2;
			}
		heap.root = tmp_3;
		delete min_node;
		merge(&heap);
		heap.root = nullptr;
		return del_val;
	}

	TValue get_min()override
	{
		if (root == nullptr)
			throw HeapGetMinExc();
		Node* tmp = root, *min_node = root;
		while (tmp != nullptr)
		{
			if (comp->compare(tmp->key, min_node->key) < 0)
				min_node = tmp;
			tmp = tmp->sibling;
		}
		return min_node->value;
	}

private:
	void clean(Node* nd)
	{
		Node* tmp;
		while (nd != nullptr)
		{
			tmp = nd;
			clean(nd->child);
			nd = nd->sibling;
			delete tmp;
		}
	}
};

template<class TKey, class TValue>
class FibonacciHeap : public Heap<TKey, TValue>
{
private:
	struct Node
	{
		Node(const TKey& k, const TValue& v) { key = k; value = v; }
		Node *child = nullptr, *parent = nullptr, *right = this, *left = this;
		TKey key;
		TValue value;
		int degree = 0;
	};

	Node* min = nullptr;
	int size = 0;
	const Comparer<TKey>* comp;

public:
	FibonacciHeap(const Comparer<TKey>* cmp)
	{
		this->comp = cmp;
	}

	~FibonacciHeap()
	{
		clean(min);
	}

	Heap<TKey, TValue>* merge(Heap<TKey, TValue>* heap) override
	{
		FibonacciHeap* new_heap = dynamic_cast<FibonacciHeap*>(heap);
		min = root_merge(min, new_heap->min);
		size += new_heap->size;
		return this;
	}

	void insert(const TKey& key, const TValue& value) override
	{
		Node* new_node = new Node(key, value);
		min = root_merge(min, new_node);
		size++;
	}

	TValue del_min() override
	{
		if (!size)
			throw HeapDelMinExc();
		TValue ret_val = min->value;
		Node *L, *R, *tmp;

		if (size == 1)
		{
			delete min;
			min = nullptr;
			size = 0;
			return ret_val;
		}

		L = min->left;
		R = min->right;
		L->right = R;
		R->left = L;
		if (min == R)
			R = nullptr;

		tmp = min->child;
		while (tmp != nullptr)
		{
			tmp->parent = nullptr;
			tmp = tmp->right;
			if (tmp == min->child)
				break;
		}
		R = root_merge(R, tmp);
		delete min;
		min = R;
		unite();
		size--;
		return ret_val;
	}

	TValue get_min()
	{
		if (!size)
			throw HeapGetMinExc();
		return min->value;
	}

private:
	Node* root_merge(Node* first, Node* second)
	{
		if (second == nullptr)
			return first;
		else if (first == nullptr)
			return second;
		Node *new_min = (comp->compare(first->key, second->key) < 0 ? first : second), *R = first->right, *L = second->left;
		first->right = second;
		second->left = first;
		R->left = L;
		L->right = R;
		return new_min;
	}

	Node* tree_merge(Node* first, Node* second)
	{
		Node *L = second->left, *R = second->right;
		L->right = R;
		R->left = L;

		if (comp->compare(first->key, second->key) < 0)
		{
			second->left = second;
			second->right = second;
			first->child = root_merge(first->child, second);
			first->degree++;
			return first;
		}
		else
		{
			if ((L == R) && (L == first))
			{
				second->left = second;
				second->right = second;
				second->child = root_merge(second->child, first);
				second->degree++;
				return second;
			}
			L = first->left;
			R = first->right;
			R->left = second;
			L->right = second;
			second->right = R;
			second->left = L;
			first->left = first;
			first->right = first;
			second->child = root_merge(second->child, first);
			second->degree++;
			return second;
		}
	}

	void unite()
	{
		Node** A = new Node*[size];
		for (int i = 0; i < size; i++)
			A[i] = nullptr;
		Node* cur = min;
		int tmp;
		while (cur != A[cur->degree])
		{
			if (comp->compare(cur->key, min->key) < 0)
				min = cur;
			if (A[cur->degree] == nullptr)
				A[cur->degree] = cur;
			else
			{
				tmp = cur->degree;
				cur = tree_merge(cur, A[cur->degree]);
				A[tmp] = nullptr;
				continue;
			}
			cur = cur->right;
		}
		delete A;
	}

	void clean(Node* nd)
	{
		Node* tmp;
		while (nd != nullptr)
		{
			tmp = nd;
			clean(nd->child);
			nd = nd->right;
			delete tmp;
		}
	}
};

#endif