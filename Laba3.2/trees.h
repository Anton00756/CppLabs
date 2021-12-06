#ifndef TREES
#define TREES
#include "parts.h"

template <class TKey, class TValue>
class AVL : public BST<TKey, TValue>
{
private:
    class AVL_Node : public  BST<TKey, TValue>::Node
    {
    public:
        void set(typename BST<TKey, TValue>::Node* nd) override
        {
            AVL_Node* tmp = dynamic_cast<AVL_Node*>(nd);
            this->key = tmp->key;
            this->value = tmp->value;
            this->level = tmp->level;
        }

        int level = 0;
    };

public:
    AVL(const Comparer<TKey>* cmp) : BST<TKey, TValue>(cmp, new Inserting(this), new Removing(this), new Finding(this)) {}
    AVL(const AVL<TKey, TValue>& tree) : BST<TKey, TValue>(tree)
    {
        copyConstructor();
        this->copy(tree.root, this->root);
    }
    ~AVL() {}

private:
    void copyConstructor() override
    {
        this->inserter_cl = new Inserting(this);
        this->remover_cl = new Removing(this);
        this->finder_cl = new Finding(this);
    }

    class Inserting : public BST<TKey, TValue>::Inserting
    {
    public:
        Inserting(AVL<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key, const TValue& value) override
        {
            return _insert(key, value);
        }

    protected:
        AVL<TKey, TValue>* obj;

        int _insert(const TKey& key, const TValue& value) override
        {
            stack<typename BST<TKey, TValue>::Node*> stack;
            AVL_Node* P, * T;

            if (obj->root == nullptr)
            {
                obj->root = new AVL_Node;
                obj->root->value = value;
                obj->root->key = key;
                return 0;
            }

            int dir = obj->down(stack, key);
            P = dynamic_cast<AVL_Node*>(stack.top());
            if (dir == 1)
            {
                P->right = new AVL_Node;
                P->right->value = value;
                P->right->key = key;
            }
            else if (dir == -1)
            {
                P->left = new AVL_Node;
                P->left->value = value;
                P->left->key = key;
            }
            else
                P->value = value;

            do
            {
                if (stack.empty())
                    break;
                P = dynamic_cast<AVL_Node*>(stack.top());
                stack.pop();
                if (obj->comp->compare(key, P->key) == -1)
                    P->level -= 1;
                else if (obj->comp->compare(key, P->key) == 1)
                    P->level += 1;
            } while (abs(P->level) == 1);

            if (stack.empty())
                T = P;
            else
                T = dynamic_cast<AVL_Node*>(stack.top());
            if (P == dynamic_cast<AVL_Node*>(T->right))
                T->right = obj->balance(P);
            else if (P == dynamic_cast<AVL_Node*>(T->left))
                T->left = obj->balance(P);
            else
                obj->root = obj->balance(P);
            return 0;
        }
    };

    class Removing : public BST<TKey, TValue>::Removing
    {
    public:
        Removing(AVL<TKey, TValue>* obj) : BST<TKey, TValue>::Removing(obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key) override
        {
            return _remove(key);
        }

    protected:
        AVL<TKey, TValue>* obj;

        int _remove(const TKey& key) override
        {
            AVL_Node* tmp = dynamic_cast<AVL_Node*>(obj->root), * tmp_2 = tmp, * tmp_3 = nullptr;
            TKey deleted_key = key;
            AVL_Node* S = tmp, * P = tmp, * T = tmp;
            stack<typename BST<TKey, TValue>::Node*> stack;

            int cur_bf = 0;

            if ((tmp == nullptr) || obj->down(stack, key))
                throw typename BST<TKey, TValue>::Remove_Err();

            tmp = dynamic_cast<AVL_Node*>(stack.top());
            stack.pop();
            if (!stack.empty())
                tmp_2 = dynamic_cast<AVL_Node*>(stack.top());
            else
                tmp_2 = tmp;
            stack.push(tmp);

            this->deleting(stack, tmp, tmp_2);
            deleted_key = stack.top()->key;
            delete stack.top();
            stack.pop();

            do
            {
                if (stack.empty())
                    break;
                P = dynamic_cast<AVL_Node*>(stack.top());
                stack.pop();
                cur_bf = P->level;

                if (obj->comp->compare(deleted_key, P->key) == 0)
                    P->level -= 1;
                else if (obj->comp->compare(key, P->key) == -1)
                    P->level += 1;
                else if (obj->comp->compare(key, P->key) == 1)
                    P->level -= 1;

                if (abs(P->level) == 2)
                {
                    if (!stack.empty())
                    {
                        bool break_flag = false;
                        T = dynamic_cast<AVL_Node*>(stack.top());
                        if (T->right == P)
                            T->right = obj->balance(P, &break_flag);
                        else
                            T->left = obj->balance(P, &break_flag);
                        if (break_flag)
                            break;
                    }
                    else
                        obj->root = obj->balance(P);
                }
            } while (abs(cur_bf) == 1);
            return 0;
        }
    };

    class Finding : public BST<TKey, TValue>::Finding
    {
    public:
        Finding(AVL<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        TValue& operator()(const TKey& key) override
        {
            return _find(key);
        }

    protected:
        AVL<TKey, TValue>* obj;

        TValue& _find(const TKey& key) override
        {
            AVL_Node* tmp = dynamic_cast<AVL_Node*>(obj->root);
            while (true)
            {
                if (tmp == nullptr)
                    throw typename BST<TKey, TValue>::Find_Err();
                switch (obj->comp->compare(key, tmp->key))
                {
                case 1:
                    tmp = dynamic_cast<AVL_Node*>(tmp->right);
                    break;
                case -1:
                    tmp = dynamic_cast<AVL_Node*>(tmp->left);
                    break;
                default:
                    return tmp->value;
                    break;
                }
            }
        }
    };

    AVL_Node* balance(AVL_Node* S, bool* breaker = nullptr)
    {
        AVL_Node* R = nullptr, * P = nullptr;
        int a;

        if (S->level == -2)
            a = -1;
        else if (S->level == 2)
            a = 1;
        else
            return S;

        R = dynamic_cast<AVL_Node*>(this->get(a, S));
        if ((breaker != nullptr) && (R->level == 0))
            *breaker = true;
        if (R->level != -a)
        {
            P = R;
            this->get(a, S) = this->get(-a, R);
            this->get(-a, R) = S;
            if (R->level == 0)
            {
                S->level = a;
                R->level = -a;
            }
            else
            {
                S->level = 0;
                R->level = 0;
            }
        }
        else if (R->level == -a)
        {
            P = dynamic_cast<AVL_Node*>(this->get(-a, R));
            this->get(-a, R) = this->get(a, P);
            this->get(a, P) = R;
            this->get(a, S) = this->get(-a, P);
            this->get(-a, P) = S;
            if (P->level == a)
            {
                S->level = -a;
                R->level = 0;
            }
            else if (!P->level)
                S->level = R->level = 0;
            else if (P->level == -a)
            {
                S->level = 0;
                R->level = a;
            }
        }
        return P;
    }
};

template<class TKey, class TValue>
class RB : public BST<TKey, TValue>
{
private:
    enum COLOR { R, B };

    class RB_Node : public BST<TKey, TValue>::Node
    {
    public:
        void set(typename BST<TKey, TValue>::Node* nd) override
        {
            const RB_Node* tmp = dynamic_cast<const RB_Node*>(nd);
            this->key = tmp->key;
            this->value = tmp->value;
            this->col = tmp->col;
        }

        COLOR col = R;
    };

public:
    RB(const Comparer<TKey>* cmp) : BST<TKey, TValue>(cmp, new Inserting(this), new Removing(this), new Finding(this)) {}
    RB(const RB<TKey, TValue>& tree) : BST<TKey, TValue>(tree)
    {
        copyConstructor();
        this->copy(tree.root, this->root);
    }
    ~RB() {}

private:
    void copyConstructor() override
    {
        this->inserter_cl = new Inserting(this);
        this->remover_cl = new Removing(this);
        this->finder_cl = new Finding(this);
    }

    class Inserting : public BST<TKey, TValue>::Inserting
    {
    public:
        Inserting(RB<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key, const TValue& value) override
        {
            return _insert(key, value);
        }

    protected:
        RB<TKey, TValue>* obj;

        int _insert(const TKey& key, const TValue& value) override
        {
            if (obj->root == nullptr)
            {
                obj->root = new RB_Node;
                obj->root->key = key;
                obj->root->value = value;
                dynamic_cast<RB_Node*>(obj->root)->col = B;
                return 0;
            }

            RB_Node* P = dynamic_cast<RB_Node*>(obj->root);
            stack<typename BST<TKey, TValue>::Node*> stack;

            int dir = obj->down(stack, key);
            P = dynamic_cast<RB_Node*>(stack.top());
            if (dir == 1)
            {
                P->right = new RB_Node;
                P->right->value = value;
                P->right->key = key;
                stack.push(P->right);
            }
            else if (dir == -1)
            {
                P->left = new RB_Node;
                P->left->value = value;
                P->left->key = key;
                stack.push(P->left);
            }
            else
                P->value = value;

            balance(stack);
            return 0;
        }

        int balance(stack<typename BST<TKey, TValue>::Node*>& stack)
        {
            RB_Node* Q = dynamic_cast<RB_Node*>(stack.top()), * P = nullptr, * G = nullptr, * U = nullptr;
            int a_1, a_2;
            stack.pop();
            while (!stack.empty())
            {
                P = G = U = nullptr;
                if (!stack.empty())
                {
                    P = dynamic_cast<RB_Node*>(stack.top());
                    stack.pop();
                }

                if (!stack.empty())
                {
                    G = dynamic_cast<RB_Node*>(stack.top());
                    stack.pop();
                }

                if (P == nullptr)
                {
                    Q->col = B;
                    return 0;
                }

                if (P->col == B)
                    return 0;

                a_1 = (P->right == Q) ? 1 : -1;
                a_2 = (G->right == P) ? 1 : -1;

                U = dynamic_cast<RB_Node*>(obj->get(-a_2, G));
                if ((U == nullptr) || (U->col == B))
                {
                    if (a_2 != a_1)
                    {
                        obj->get(a_1, P) = obj->get(-a_1, Q);
                        obj->get(-a_1, Q) = P;
                        P = Q;
                        Q = dynamic_cast<RB_Node*>(obj->get(-a_1, P));
                    }
                    obj->get(a_2, G) = obj->get(-a_2, P);
                    obj->get(-a_2, P) = G;
                    G->col = R;
                    P->col = B;
                    break;
                }
                else
                {
                    P->col = U->col = B;
                    G->col = R;
                    Q = P = G;
                }
            }

            if (!stack.empty())
            {
                U = dynamic_cast<RB_Node*>(stack.top());
                if (U->right == G)
                    U->right = P;
                else
                    U->left = P;
            }
            else if (P->col == B)
                obj->root = P;
            else
                P->col = B;
            return 0;
        }
    };

    class Removing : public BST<TKey, TValue>::Removing
    {
    public:
        Removing(RB<TKey, TValue>* obj) : BST<TKey, TValue>::Removing(obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key) override
        {
            return _remove(key);
        }

    protected:
        RB<TKey, TValue>* obj;

        int _remove(const TKey& key) override
        {
            stack<typename BST<TKey, TValue>::Node*> stack;
            RB_Node* tmp = dynamic_cast<RB_Node*>(obj->root), * tmp_2 = tmp;

            if ((tmp == nullptr) || obj->down(stack, key))
                throw typename BST<TKey, TValue>::Remove_Err();

            tmp = dynamic_cast<RB_Node*>(stack.top());
            stack.pop();
            if (!stack.empty())
                tmp_2 = dynamic_cast<RB_Node*>(stack.top());
            else
                tmp_2 = tmp;
            stack.push(tmp);

            this->deleting(stack, tmp, tmp_2);
            COLOR del_clr = dynamic_cast<RB_Node*>(stack.top())->col;
            delete stack.top();
            stack.pop();

            if (!stack.empty())
            {
                int a = obj->comp->compare(key, stack.top()->key);
                if (a == 0)
                    a = 1;

                balance(stack, del_clr, a);
            }
            return 0;
        }

        int balance(stack<typename BST<TKey, TValue>::Node*>& stack, COLOR del_clr, int A)
        {
            if (del_clr == R)
                return 0;

            RB_Node* P = dynamic_cast<RB_Node*>(stack.top()), * Q = nullptr, * E = nullptr, * D = nullptr, * C = nullptr, * G = nullptr;
            int a;

            if ((P->right != nullptr) && (P->left != nullptr))
            {
                Q = dynamic_cast<RB_Node*>(obj->get(A, P));
                Q->col = B;
            }
            else
            {
                while (!stack.empty())
                {
                    P = dynamic_cast<RB_Node*>(stack.top());
                    stack.pop();
                    a = ((Q == P->right) ? 1 : -1);

                    E = dynamic_cast<RB_Node*>(obj->get(-a, P));
                    C = dynamic_cast<RB_Node*>(obj->get(a, E));
                    D = dynamic_cast<RB_Node*>(obj->get(-a, E));

                    if (E->col == R)
                    {
                        obj->rot(E, P, a);
                        P->col = R;
                        E->col = B;
                        dynamic_cast<RB_Node*>(obj->get(-a, P))->col = R;
                        P->col = B;
                        Q = E;
                        break;
                    }
                    if (E->col == B)
                    {
                        if (((obj->get(a, E) == nullptr) || (dynamic_cast<RB_Node*>(obj->get(a, E))->col == B)) && ((obj->get(-a, E) == nullptr) || (dynamic_cast<RB_Node*>(obj->get(-a, E))->col == B)))
                        {
                            E->col = R;
                            Q = P;
                            if (P->col == R)
                            {
                                P->col = B;
                                break;
                            }
                        }

                        if (((obj->get(-a, E) == nullptr) || (dynamic_cast<RB_Node*>(obj->get(-a, E))->col == B)) && ((obj->get(a, E) != nullptr) && (dynamic_cast<RB_Node*>(obj->get(a, E))->col == R)))
                        {
                            obj->rot(C, E, -a);
                            C->col = B;
                            E->col = R;
                            E = C;
                        }

                        if ((obj->get(-a, E) != nullptr) && (dynamic_cast<RB_Node*>(obj->get(-a, E))->col == R))
                        {
                            obj->rot(E, P, a);
                            E->col = P->col;
                            P->col = B;
                            dynamic_cast<RB_Node*>(obj->get(-a, E))->col = B;
                            Q = E;
                            break;
                        }
                    }
                }

                if (!stack.empty())
                {
                    G = dynamic_cast<RB_Node*>(stack.top());
                    if (G->right == P)
                        G->right = Q;
                    else
                        G->left = Q;
                }
                else
                    obj->root = Q;
            }
            return 0;
        }
    };

    class Finding : public BST<TKey, TValue>::Finding
    {
    public:
        Finding(RB<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        TValue& operator()(const TKey& key)
        {
            return _find(key);
        }

    protected:
        RB<TKey, TValue>* obj;

        TValue& _find(const TKey& key) override
        {
            RB_Node* tmp = dynamic_cast<RB_Node*>(obj->root);
            while (true)
            {
                if (tmp == nullptr)
                    throw typename BST<TKey, TValue>::Find_Err();
                switch (obj->comp->compare(key, tmp->key))
                {
                case 1:
                    tmp = dynamic_cast<RB_Node*>(tmp->right);
                    break;
                case -1:
                    tmp = dynamic_cast<RB_Node*>(tmp->left);
                    break;
                default:
                    return tmp->value;
                    break;
                }
            }

        }
    };

    void rot(RB_Node* A, RB_Node* B, int a)
    {
        this->get(-a, B) = this->get(a, A);
        this->get(a, A) = B;
    }
};

template<class TKey, class TValue>
class Splay : public BST<TKey, TValue>
{
public:
    Splay(const Comparer<TKey>* cmp) : BST<TKey, TValue>(cmp, new Inserting(this), new Removing(this), new Finding(this)) {}

private:
    enum ZZ { ZIG = -1, ZAG = 1, NONE = 0 };

    void copyConstructor() override
    {
        this->inserter_cl = new Inserting(this);
        this->remover_cl = new Removing(this);
        this->finder_cl = new Finding(this);
    }

    Splay(const Splay<TKey, TValue>& tree) : BST<TKey, TValue>(tree)
    {
        copyConstructor();
        this->copy(tree.root, this->root);
    }

    class Inserting : public BST<TKey, TValue>::Inserting
    {
    public:
        Inserting(Splay<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key, const TValue& value) override
        {
            return _insert(key, value);
        }

    private:
        Splay<TKey, TValue>* obj;

        int _insert(const TKey& key, const TValue& value) override
        {
            if (obj->root == nullptr)
            {
                obj->root = new typename BST<TKey, TValue>::Node;
                obj->root->key = key;
                obj->root->value = value;
                return 0;
            }

            typename BST<TKey, TValue>::Node* cur_node = obj->root;
            stack<typename BST<TKey, TValue>::Node**> stack;
            stack.push(&obj->root);

            while (true)
            {
                if (obj->comp->compare(key, cur_node->key) == 1)
                {
                    stack.push(&cur_node->right);
                    if (cur_node->right == nullptr)
                    {
                        cur_node->right = new typename BST<TKey, TValue>::Node;
                        cur_node->right->key = key;
                        cur_node->right->value = value;
                        break;
                    }
                    cur_node = cur_node->right;
                }
                else if (obj->comp->compare(key, cur_node->key) == -1)
                {
                    stack.push(&cur_node->left);
                    if (cur_node->left == nullptr)
                    {
                        cur_node->left = new typename BST<TKey, TValue>::Node;
                        cur_node->left->key = key;
                        cur_node->left->value = value;
                        break;
                    }
                    cur_node = cur_node->left;
                }
                else
                    throw typename BST<TKey, TValue>::Insert_Err();
            }
            obj->splay_f(stack);
            return 0;
        }
    };

    class Removing : public BST<TKey, TValue>::Removing
    {
    public:
        Removing(Splay<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        int operator()(const TKey& key) override
        {
            return _remove(key);
        }

    private:
        Splay<TKey, TValue>* obj;

        int _remove(const TKey& key) override
        {
            stack<typename BST<TKey, TValue>::Node**> stack;
            typename BST<TKey, TValue>::Node* tmp = obj->root;
            stack.push(&obj->root);

            while (true)
            {
                if (tmp == nullptr)
                    throw typename BST<TKey, TValue>::Remove_Err();
                if (obj->comp->compare(key, tmp->key) == 1)
                {
                    stack.push(&tmp->right);
                    tmp = tmp->right;
                }
                else if (obj->comp->compare(key, tmp->key) == -1)
                {
                    stack.push(&tmp->left);
                    tmp = tmp->left;
                }
                else
                {
                    obj->splay_f(stack);
                    break;
                }
            }

            tmp = obj->root->right;
            if (tmp != nullptr)
            {
                stack.push(&obj->root->right);
                while (tmp->left != nullptr)
                {
                    stack.push(&tmp->left);
                    tmp = tmp->left;
                }

                obj->splay_f(stack);
                tmp->left = obj->root->left;
                delete obj->root;
                obj->root = tmp;
            }
            else
            {
                tmp = obj->root;
                obj->root = tmp->left;
                delete tmp;
            }
            return 0;
        }
    };

    class Finding : public BST<TKey, TValue>::Finding
    {
    public:
        Finding(Splay<TKey, TValue>* obj)
        {
            this->obj = obj;
        }

        TValue& operator()(const TKey& key) override
        {
            return _find(key);
        }

    private:
        Splay<TKey, TValue>* obj;

        TValue& _find(const TKey& key) override
        {
            stack<typename BST<TKey, TValue>::Node**> stack;
            typename BST<TKey, TValue>::Node* tmp = obj->root;

            stack.push(&obj->root);

            while (true)
            {
                if (tmp == nullptr)
                    throw typename BST<TKey, TValue>::Find_Err();
                if (obj->comp->compare(key, tmp->key) == 1)
                {
                    stack.push(&tmp->right);
                    tmp = tmp->right;
                }
                else if (obj->comp->compare(key, tmp->key) == -1)
                {
                    stack.push(&tmp->left);
                    tmp = tmp->left;
                }
                else
                {
                    obj->splay_f(stack);
                    return tmp->value;
                }
            }
        }
    };

    void splay_f(stack<typename BST<TKey, TValue>::Node**>& stack)
    {
        typename BST<TKey, TValue>::Node* tmp, ** tmp_2, ** tmp_3, * cur_node = *(stack.top());
        stack.pop();
        ZZ a_1, a_2;

        while (stack.size() > 1)
        {
            tmp_3 = stack.top();
            tmp = *tmp_3;
            stack.pop();
            tmp_2 = stack.top();
            stack.pop();
            if ((tmp)->right == cur_node)
                a_1 = ZAG;
            else if ((tmp)->left == cur_node)
                a_1 = ZIG;
            if ((*tmp_2)->right == tmp)
                a_2 = ZAG;
            else if ((*tmp_2)->left == tmp)
                a_2 = ZIG;

            if (a_1 == a_2)
            {
                this->get(a_2, *tmp_2) = this->get(-a_2, tmp);
                this->get(-a_2, tmp) = *tmp_2;
                *tmp_2 = tmp;

                this->get(a_1, tmp) = this->get(-a_1, cur_node);
                this->get(-a_1, cur_node) = tmp;
            }
            else
            {
                this->get(a_1, tmp) = this->get(-a_1, cur_node);
                this->get(-a_1, cur_node) = tmp;
                *tmp_3 = cur_node;

                this->get(a_2, *tmp_2) = this->get(-a_2, cur_node);
                this->get(-a_2, cur_node) = *tmp_2;
            }
            *tmp_2 = cur_node;
        }

        if (!stack.empty())
        {
            tmp_2 = stack.top();
            stack.pop();
            if ((*tmp_2)->right == cur_node)
                a_1 = ZAG;
            else if ((*tmp_2)->left == cur_node)
                a_1 = ZIG;
            this->get(a_1, *tmp_2) = this->get(-a_1, cur_node);
            this->get(-a_1, cur_node) = *tmp_2;
            *tmp_2 = cur_node;
        }
    }
};

template<class TKey, class TValue>
class T_23 : public Ass_Cont<TKey, TValue>
{
private:
    class Node
    {
    public:
        Node() {}

        int find(const TKey& key)
        {
            for (int i = 0; i < size; i++)
                if (key == keys[i])
                    return i;
            return -1;
        }

        bool insert(const Comparer<TKey>* cmp, const TKey& key, const TValue& value)
        {
            int a = cmp->compare(key, keys[0]);
            if (a == 1)
            {
                keys[1] = key;
                values[1] = value;
            }
            else if (a == -1)
            {
                keys[1] = keys[0];
                values[1] = values[0];
                keys[0] = key;
                values[0] = value;
            }
            else
                return true;
            size++;
            return false;
        }

        vector<Node*> childs;
        TKey keys[2];
        TValue values[2];
        int size = 1;
        bool is_leaf = false;
    };

private:
    Node* root;
    const Comparer<TKey>* comp;
    int deepness;

public:
    T_23(const Comparer<TKey>* cmp)
    {
        this->comp = cmp;
        root = nullptr;
    }

    ~T_23()
    {
        if (root != nullptr)
            _clean(root);
    }

    void insert(const TKey& key, const TValue& value) override
    {
        _insert(key, value);
    }

    void remove(const TKey& key) override
    {
        _remove(key);
    }

    TValue& find(const TKey& key) override
    {
        if (root == nullptr)
            throw std::exception("Дерево пустое!");

        Node* cur_node = root;
        while (cur_node->find(key) == -1)
        {
            if (comp->compare(key, cur_node->keys[0]) != 1)
            {
                if (cur_node->childs.size())
                    cur_node = cur_node->childs[0];
                else
                    throw std::exception("Такого ключа не существует!");
            }
            else if (comp->compare(key, cur_node->keys[cur_node->size - 1]) == 1)
            {
                if (cur_node->childs.size() == cur_node->size + 1)
                    cur_node = cur_node->childs[cur_node->size];
                else
                    throw std::exception("Такого ключа не существует!");
            }
            else if (cur_node->childs.size() >= 2)
                cur_node = cur_node->childs[1];
            else
                throw std::exception("Такого ключа не существует!");
        }
        return cur_node->values[cur_node->find(key)];
    }

private:
    void _insert(const TKey& key, const TValue& value)
    {
        if (root == nullptr)
        {
            root = newNode(key, value);
            deepness++;
            return;
        }
        TKey tkey = key;
        TValue tvalue = value;
        Node* tmp = root;
        stack<Node*> stack;
        for (int i = 0; i < deepness - 1; i++)
        {
            if (comp->compare(key, tmp->keys[0]) == -1)
            {
                stack.push(tmp);
                tmp = tmp->childs[0];
            }
            else if (comp->compare(key, tmp->keys[tmp->size - 1]) == 1)
            {
                stack.push(tmp);
                tmp = tmp->childs[tmp->size];
            }
            else if ((tmp->childs.size() >= 2) && !comp->compare(key, tmp->keys[0]) && !comp->compare(key, tmp->keys[tmp->size - 1]))
            {
                stack.push(tmp);
                tmp = tmp->childs[1];
            }
            else
                throw std::exception("Такой элемент уже существует!");
        }

        while (true)
        {
            if (tmp->size == 1)
            {
                if (tmp->insert(comp, tkey, tvalue))
                    throw std::exception("Такой элемент уже существует!");
                return;
            }
            else if (stack.empty())
            {
                split(nullptr, tmp, tkey, tvalue);
                return;
            }
            else
            {
                split(stack.top(), tmp, tkey, tvalue);
                tmp = stack.top();
                stack.pop();
            }
        }
    }

    void _remove(const TKey& key)
    {
        int a_1, a_2;
        TKey tkey = key;
        Node* del_elem = root, * del_node = nullptr;
        stack<Node*> stack;

        while (true)
        {
            stack.push(del_elem);

            if (!(a_1 = comp->compare(key, del_elem->keys[0])) || !(a_2 = comp->compare(key, del_elem->keys[del_elem->size - 1])))
                break;
            else if (a_1 == -1)
            {
                if (!del_elem->childs.size())
                    throw std::exception("Такого элемента не существует!");
                del_elem = del_elem->childs[0];
            }
            else if (comp->compare(key, del_elem->keys[del_elem->size - 1]) == 1)
            {
                if (!del_elem->childs.size())
                    throw std::exception("Такого элемента не существует!");
                del_elem = del_elem->childs[del_elem->size];
            }
            else if (del_elem->childs.size() >= 2)
                del_elem = del_elem->childs[1];
            else
                throw std::exception("Такого элемента не существует!");
        }

        del_node = del_elem;
        if (del_node->childs.size())
            del_node = del_node->childs[del_node->size];

        while (del_node->childs.size())
        {
            stack.push(del_node);
            del_node = del_node->childs[0];
        }

        if (del_elem != del_node)
        {
            tkey = del_elem->keys[del_elem->find(key)];
            del_elem->keys[del_elem->find(key)] = del_node->keys[0];
            del_elem->values[del_elem->find(key)] = del_node->values[0];
            del_node->keys[0] = tkey;
            stack.push(del_node);
        }
        balance(stack, tkey);
    }

    void balance(stack<Node*>& stack, TKey& key)
    {
        Node* Q = stack.top(), * P, * B;
        int index, ind;

        stack.pop();
        index = Q->find(key);

        if (Q->size == 2)
        {
            if (!index)
            {
                Q->keys[0] = Q->keys[1];
                Q->size = 1;
            }
            return;
        }

        if (!stack.size())
        {
            delete Q;
            root = nullptr;
            return;
        }

        while (!stack.empty())
        {
            ind = 5;
            P = stack.top();
            stack.pop();
            for (int i = 0; i <= P->size; i++)
            {
                if (P->childs[i] == Q)
                    index = i;
                if (P->childs[i]->size == 2)
                    ind = i;
            }

            if (abs(index - ind) == 1)
            {
                if (index < ind)
                {
                    B = P->childs[ind];
                    Q->keys[0] = P->keys[ind - 1];
                    Q->values[0] = P->values[ind - 1];
                    P->keys[ind - 1] = B->keys[0];
                    P->values[ind - 1] = B->values[0];
                    B->keys[0] = B->keys[1];
                    B->values[0] = B->values[1];
                    B->size = 1;
                    Q->size = 1;
                    if (B->childs.size())
                    {
                        Q->childs[1] = B->childs[0];
                        B->childs[0] = B->childs[1];
                        B->childs[1] = B->childs[2];
                        B->childs.pop_back();
                    }
                }
                else
                {
                    B = P->childs[ind];
                    Q->keys[0] = P->keys[index - 1];
                    Q->values[0] = P->values[index - 1];
                    P->keys[index - 1] = B->keys[1];
                    P->values[index - 1] = B->values[1];
                    B->size = 1;
                    Q->size = 1;
                    if (B->childs.size())
                    {
                        Q->childs[1] = Q->childs[0];
                        Q->childs[0] = B->childs[2];
                        B->childs.pop_back();
                    }
                }
                return;
            }
            else if (P->size == 2)
            {
                if (index < 2)
                {
                    Q->keys[0] = P->keys[0];
                    Q->values[0] = P->values[0];
                    P->keys[0] = P->keys[1];
                    P->values[0] = P->values[1];
                    P->size = 1;
                    merge(P->childs[0], P->childs[1]);
                    P->childs[1] = P->childs[2];
                    P->childs.pop_back();
                }
                else
                {
                    B = P->childs[1];
                    B->keys[1] = P->keys[1];
                    B->values[1] = P->values[1];
                    delete Q;
                    P->childs.pop_back();
                }
                return;
            }
            else
            {
                Q->keys[0] = P->keys[0];
                Q->values[0] = P->values[0];
                merge(P->childs[0], P->childs[1]);
                Q = P;
            }
        }

        root = Q->childs[0];
        deepness--;
        delete Q;
    }

    void merge(Node* f, Node* s)
    {
        f->keys[1] = s->keys[0];
        f->values[1] = s->values[0];
        f->size = 2;

        if (f->childs.size())
        {
            f->childs[1] = s->childs[0];
            if (s->size == 2)
            {
                s->childs[0] = s->childs[1];
                s->childs[1] = s->childs[2];
                s->childs.pop_back();
            }
            else
            {
                f->childs.push_back(s->childs[1]);
                delete s;
            }
        }
        else
            delete s;
    }

    void split(Node* P, Node* Q, TKey& key, TValue& value)
    {
        Node* tmp = nullptr;
        TKey tkey;
        TValue tvalue;
        int i = 0, a_1 = comp->compare(key, Q->keys[0]), a_2 = comp->compare(key, Q->keys[1]);
        if (!a_1 || !a_2)
            throw std::exception("Такой элемент уже существует!");
        if (a_1 == -1)
        {
            tmp = newNode(Q->keys[1], Q->values[1]);
            tkey = Q->keys[0];
            tvalue = Q->values[0];
            Q->keys[0] = key;
            Q->values[0] = value;
            Q->size = 1;
        }
        else
        {
            Q->size = 1;
            if (a_2 == -1)
            {
                tmp = newNode(Q->keys[1], Q->values[1]);
                tkey = key;
                tvalue = value;
            }
            else
            {
                tmp = newNode(key, value);
                tkey = Q->keys[1];
                tvalue = Q->values[1];
            }
        }
        if (Q->childs.size() != 0)
        {
            tmp->childs.push_back(Q->childs[2]);
            tmp->childs.push_back(Q->childs[3]);
            Q->childs.erase(Q->childs.begin() + 2, Q->childs.end());
        }
        if (P == nullptr)
        {
            P = newNode(tkey, tvalue);
            P->childs.push_back(Q);
            P->childs.push_back(tmp);
            root = P;
            deepness++;
        }
        else
        {
            if (P->childs[0] == Q)
                i = 0;
            else if (P->childs[1] == Q)
                i = 1;
            else
                i = 2;
            P->childs.insert(P->childs.begin() + i + 1, tmp);
        }
        key = tkey;
        value = tvalue;
    }

    Node* newNode(const TKey& key, const TValue& value)
    {
        Node* nd = new Node;
        nd->keys[0] = key;
        nd->values[0] = value;
        return nd;
    }

    void _clean(Node* nd)
    {
        for (int i = 0; i < nd->childs.size(); i++)
            _clean(nd->childs[i]);
        delete nd;
    }
};

#endif