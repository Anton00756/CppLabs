#include "trees.h"

template<typename TKey, typename TValue>
class BST : public Ass_Cont<TKey, TValue>
{
protected:
	struct node
	{
		TKey key;
		TValue value;
		node* left, *right;
	};

	class Inserting;
	class Removing;
	class Finding;

	BST(const Comparer<TKey>* cmp, Inserting* ins, Removing* rem, Finding* find)
	{
		inserter_cl = ins;
		remover_cl = rem;
		finder_cl = find;
		comp = cmp;
		root = nullptr;
	}

	node* root;
	Inserting* inserter_cl;
	Removing* remover_cl;
	Finding* finder_cl;
	const Comparer<TKey>* comp;

public:
	virtual BST& operator=(const BST<TKey, TValue>& tree)
	{
		if (root != nullptr)
			clean(root);
		root = copy(this->root, tree.root);
		return *this;
	}

    BST(const BST<TKey, TValue>& tree)
    {
        comp = tree.comp;
        this->copyConstructor();
        copy(tree.root, this->root);
    }

    BST(const Comparer<TKey>* cmp)
    {
        inserter_cl = new Inserting(this);
        remover_cl = new Removing(this);
        finder_cl = new Finding(this);
        comp = cmp;
        root = nullptr;
    }

    virtual ~BST()
    {
        delete inserter_cl, remover_cl, finder_cl;
        if (root != nullptr)
            clean(root);
    }

    virtual void prefix(void (*func)(TKey&, TValue&, int&))
    {
        _prefix(func, root);
    }

    virtual void infix(void (*func)(TKey&, TValue&, int&))
    {
        _infix(func, root);
    }

    virtual void postfix(void (*func)(TKey&, TValue&, int&))
    {
        _postfix(func, root);
    }

    void insert(const TKey& key, const TValue& value) override
    {
        (*inserter_cl)(key, value);
    }

    TValue remove(const TKey& key) override
    {
        (*remover_cl)(key);
    }

    TValue& find(const TKey& key) override
    {
        (*finder_cl)(key);
    }

protected:
    virtual void copyConstructor()
    {
        inserter_cl = new Inserting(this);
        remover_cl = new Removing(this);
        finder_cl = new Finding(this);
    }

    virtual node* copy(node* in, node* out)
    {
        if (in == nullptr)
            return out;
        out = new node{ in->key, in->value, nullptr, nullptr };
        out->left = copy(in->left, out->left);
        out->right = copy(in->right, out->right);
        return out;
    }

    virtual void _prefix(void (*func)(TKey&, TValue&, int&), node* cur_node, int deep = 0)
    {
        if (cur_node == nullptr)
            return;

        func((cur_node)->key, (cur_node)->value, deep);
        _prefix(func, cur_node->left, deep + 1);
        _prefix(func, cur_node->right, deep + 1);
    }
    
    virtual void _infix(void (*func)(TKey&, TValue&, int&), node* cur_node, int deep = 0)
    {
        if (cur_node == nullptr)
            return;

        _infix(func, cur_node->left, deep + 1);
        func(cur_node->key, cur_node->value, deep);
        _infix(func, cur_node->right, deep + 1);
    }
    
    virtual void _postfix(void (*func)(TKey&, TValue&, int&), node* cur_node, int deep = 0)
    {
        if (cur_node == nullptr)
            return;

        _postfix(func, cur_node->left, deep + 1);
        _postfix(func, cur_node->right, deep + 1);
        func(cur_node->key, cur_node->value, deep);
    }
    
    class Inserting
    {
    public:
        Inserting(BST<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        virtual int operator()(const TKey& key, const TValue& value)
        {
            return _insert(key, value);
        }

    protected:
        BST<TKey, TValue>* obj = nullptr;
        virtual int _insert(const TKey& key, const TValue& value)
        {
            if (obj->root == nullptr)
            {
                obj->root = new node{ key, value, nullptr, nullptr };
                return 0;
            }
            stack<node *> stack;

            int dir = obj->down(stack, key);
            node* tmp = stack.top();
            if (dir == 1)
                tmp->right = new node{ key, value, nullptr, nullptr };
            else if (dir == -1)
                tmp->left = new node{ key, value, nullptr, nullptr };
            else
                tmp->value = value;
        }
    };

    class Removing
    {
    public:
        Removing(BST<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        virtual int operator()(const TKey& key)
        {
            return _remove(key);
        }

    protected:
        BST<TKey, TValue>* obj = nullptr;
        virtual int _remove(const TKey& key)
        {
            node* tmp = obj->root, *tmp_2 = obj->root;
            stack<node *> stack;
            
            if ((tmp == nullptr) || obj->down(stack, key))
                throw typename BST<TKey, TValue>::remove_err();
            tmp = stack.top();
            stack.pop();
            if (!stack.empty())
                tmp_2 = stack.top();
            else
                tmp_2 = tmp;
            stack.push(tmp);

            deleting(stack, tmp, tmp_2);
            delete stack.top();

        }

        void deleting(stack<node*>& stack, node* tmp, node* tmp_2)
        {
            int a = 0;
            node* tmp_3 = nullptr;

            if ((tmp->right == nullptr) && (tmp->left == nullptr))
            {
                if (tmp_2->right == tmp)
                    tmp_2->right = nullptr;
                else if (tmp_2 == tmp)
                    obj->root = nullptr;
                else
                    tmp_2->left = nullptr;
            }
            else if ((tmp->right != nullptr) && (tmp->left != nullptr))
            {
                tmp_2 = tmp;
                tmp_3 = tmp_2;
                tmp = tmp->right;
                stack.push(tmp);
                while (tmp->left != nullptr)
                {
                    tmp_3 = tmp;
                    tmp = tmp->left;
                    stack.push(tmp);
                }
                if (tmp_3->right == tmp)
                    tmp_3->right = tmp->right;
                else
                    tmp_3->left = tmp->right;

                tmp_2->key = tmp->key;
                tmp_2->value = tmp->value;
            }
            else
            {
                if (tmp->right != nullptr)
                    a = 1;
                else
                    a = -1;
                if (tmp == obj->root)
                    obj->root = obj->get(a, tmp);
                else if (tmp_2->right == tmp)
                    tmp_2->right = obj->get(a, tmp);
                else
                    tmp_2->left = obj->get(a, tmp);
            }
        }
    };

    class Finding
    {
    public:
        Finding(BST<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        virtual TValue operator()(const TKey& key)
        {
            return _find(key);
        }

    protected:
        BST<TKey, TValue>* obj = nullptr;
        virtual TValue _find(const TKey& key)
        {
            node* tmp = obj->root;
            while (true)
            {
                if (tmp == nullptr)
                    throw find_error();
                switch (obj->comp->compare(key, tmp->key))
                {
                case 1:
                    tmp = tmp->right;
                    break;
                case -1:
                    tmp = tmp->left;
                    break;
                default:
                    return tmp->value;
                    break;
                }
            }
        }
    };

    int down(stack<node*>& stack, const TKey& key)
    {
        node* tmp = root;
        int dir = 0;
        stack.push(root);
        while (dir = comp->compare(key, tmp->key))
        {
            switch (dir)
            {
            case 1:
                if (tmp->right == nullptr)
                    return 1;
                tmp = tmp->right;
                stack.push(tmp);
                break;
            case -1:
                if (tmp->left == nullptr)
                    return -1;
                tmp = tmp->left;
                stack.push(tmp);
                break;
            default:
                break;
            }
        }
        return 0;
    }

    int clean(node* cur_node)
    {
        if (cur_node->left != nullptr)
            clean(cur_node->left);
        if (cur_node->right != nullptr)
            clean(cur_node->right);
        delete cur_node;

        return 0;
    }
    
    node* get(int a, node* node)
    {
        if (a == 1)
            return node->right;
        else if (a == -1)
            return node->left;
    }

};

int main()
{
    system("chcp 1251>nul");

    cout << "\tЛабораторная работа №3.2\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

    system("pause>nul");
}
