#ifndef RELATION
#define RELATION
#include "trees.h"
#include <map>
#include <set>

class LenStrComparer : public Comparer<string>
{
public:
    int compare(const string& left, const string& right) const override
    {
        if (left.length() > right.length())
            return 1;
        else if (left.length() < right.length())
            return -1;
        return 0;
    }

    ~LenStrComparer() override {}
};

class LexicStrComparer : public Comparer<string>
{
public:
    int compare(const string& left, const string& right) const override
    {
        if (left > right)
            return 1;
        else if (left < right)
            return -1;
        return 0;
    }

    ~LexicStrComparer() override {}
};

template<class TKey, class TValue>
class Relation
{
public:
    Relation() {}
    ~Relation()
    {
        base.clear();
    }

    enum tree {AVL_Tree, RB_Tree, Splay_Tree, TwoThree_Tree};
    Relation& add_index(const string& index, const tree tree_name, const Comparer<TKey>* comp)
    {
        if (base.count(index))
            throw Relation_Err("Такой идентификатор уже существует!");
        if (!index.length())
            throw Relation_Err("Идентификатор пуст!");
        int index_ids_count = count(index.begin(), index.end(), ',');
        if (index_ids_count)
        {
            int pos = 0, new_pos = 0;
            set <string> input;
            while ((new_pos = index.find(',', pos)) != -1)
            {
                input.insert(index.substr(pos, new_pos-pos));
                pos = new_pos + 1;
            }
            input.insert(index.substr(pos));
            typename map <string, Ass_Cont<TKey, TValue>*> ::iterator it;
            for (it = base.begin(); it != base.end(); it++)
            {
                if (index_ids_count == count(it->first.begin(), it->first.end(), ','))
                {
                    pos = 0;
                    try
                    {
                        if (!input.count(it->first.substr(it->first.rfind(',') + 1)))
                            throw;
                        while ((new_pos = it->first.find(',', pos)) != -1)
                        {
                            if (!input.count(it->first.substr(pos, new_pos - pos)))
                                throw;
                            pos = new_pos + 1;
                        }
                    }
                    catch (...)
                    {
                        continue;
                    }
                    cout << index;
                    throw Relation_Err("Такой идентификатор уже существует!");
                }
            }
        }

        switch (tree_name)
        {
        case AVL_Tree:
            base[index] = new AVL<TKey, TValue>(comp);
            break;
        case RB_Tree:
            base[index] = new RB<TKey, TValue>(comp);
            break;
        case Splay_Tree:
            base[index] = new Splay<TKey, TValue>(comp);
            break;
        default:
            base[index] = new T_23<TKey, TValue>(comp);
            break;
        }

        Ass_Cont<TKey, TValue>* new_tree = base[index];
        for (int i = 0; i < data.size(); i++)
            new_tree->insert(data[i]->key, data[i]->value);
        return *this;
    }

    Relation& add_data(const TKey& key, const TValue& value)
    {
        for (int i = 0; i < data.size(); i++)
            if (data[i]->key == key)
                throw Relation_Err("Данные с таким ключом уже существуют!");
        data.push_back(new Node{key, value});
        typename map <string, Ass_Cont<TKey, TValue>*> ::iterator it;
        for (it = base.begin(); it != base.end(); it++)
            it->second->insert(key, value);
        return *this;
    }

    TValue& find_data(const string& index_key, const TKey& data_key)
    {
        typename map <string, Ass_Cont<TKey, TValue>*> ::iterator it = base.find(index_key);
        if (it == base.end())
            throw Relation_Err("Индекса с таким ключом не существует!");
        return it->second->find(data_key);
    }

    void del_data(const TKey& key)
    {
        if (!data.size())
            throw Relation_Err("В отношении нет никаких данных!");
        int i = 0;
        for (i = 0; i < data.size(); i++)
            if (data[i]->key == key)
                break;
            else if (i + 1 == data.size())
                throw Relation_Err("Данных с таким ключом не существует!");
        data.erase(data.begin() + i);
        typename map <string, Ass_Cont<TKey, TValue>*> ::iterator it;
        for (it = base.begin(); it != base.end(); it++)
            it->second->remove(key);
    }

    void del_index(const string& tree_name)
    {
        if (!base.count(tree_name))
            throw Relation_Err("Индекса с таким ключом не существует!");
        delete base[tree_name];
        base.erase(tree_name);
    }

    class Indexer
    {
    public:
        Indexer() {}
        ~Indexer()
        {
            IDs.clear();
        }

        Indexer& add_id(const string& ID_name)
        {
            for (int i = 0; i < IDs.size(); i++)
                if (IDs[i] == ID_name)
                    return *this;
            IDs.push_back(ID_name);
            return *this;
        }

        string out()
        {
            if (!IDs.size())
                throw Relation_Err("Попытка создания пустого индекса!");
            string result = IDs[0];
            for (int i = 1; i < IDs.size(); i++)
                result += "," + IDs[i];
            return result;
        }

    private:
        vector <string> IDs;
    };

    class Relation_Err : public exception
    {
    public:
        Relation_Err() {}
        Relation_Err(const string& about)
        {
            inf = about;
        }

        const char* what() const noexcept
        {
            return inf.c_str();
        }

    private:
        string inf;
    };

private:
    map <string, Ass_Cont<TKey, TValue>*> base;

    struct Node
    {
        TKey key;
        TValue value;
    };

    vector <Node*> data;
};

#endif