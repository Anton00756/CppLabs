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
            AVL_Node* P, *T;
            
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
            AVL_Node* tmp = dynamic_cast<AVL_Node*>(obj->root), *tmp_2 = tmp, *tmp_3 = nullptr;
            TKey deleted_key = key;
            AVL_Node *S = tmp, *P = tmp, *T = tmp;
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
                        /*T = dynamic_cast<AVL_Node*>(stack.top());
                        if (T->right == P)
                            T->right = obj->balance(P);
                        else
                            T->left = obj->balance(P);
                        break;*/

                        T = dynamic_cast<AVL_Node*>(stack.top());
                        if (T->right == P)
                            T->right = obj->balance(P);
                        else
                            T->left = obj->balance(P);
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

    AVL_Node* balance(AVL_Node* S)
    {
        AVL_Node* R = nullptr, *P = nullptr;
        int a;

        if (S->level == -2)
            a = -1;
        else if (S->level == 2)
            a = 1;
        else
            return S;
        
        R = dynamic_cast<AVL_Node*>(this->get(a, S));
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
            RB_Node* Q = dynamic_cast<RB_Node*>(stack.top()), *P = nullptr, *G = nullptr, *U = nullptr;
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
            else if(P->col == B)
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
            RB_Node* tmp = dynamic_cast<RB_Node*>(obj->root), *tmp_2 = tmp;
            
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

            int a = obj->comp->compare(key, stack.top()->key);
            if (a == 0)
                a = 1;

            balance(stack, del_clr, a);
            return 0;
        }

        int balance(stack<typename BST<TKey, TValue>::Node*>& stack, COLOR del_clr, int A)
        {
            if (del_clr == R)
                return 0;

            RB_Node* P = dynamic_cast<RB_Node*>(stack.top()), *Q = nullptr, *E = nullptr, *D = nullptr, *C = nullptr, *G = nullptr;
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

#endif