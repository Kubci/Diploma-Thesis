#pragma once
#include <vector>
#include <unordered_set>
#include <future>

// Doubly connected union find set data-structure. 
template <typename T>
class SetUF
{
public:

	T									item;
	int									rank = 0;
	int									size = 1;
	SetUF<T>*							parent = this;
	std::unordered_set<SetUF<T>*>		successors;

	static SetUF<T>* makeSet(T item);
	static SetUF<T>* unionByRank(SetUF<T>* first, SetUF<T>* second);
	static SetUF<T>* unionInOrder(SetUF<T>* first, SetUF<T>* second);
	static SetUF<T>* find(SetUF<T>* item);
	static SetUF<T>* findRec(SetUF<T>* item);
	static SetUF<T>* findWithoutCompression(SetUF<T>* item);
	static void		 deleteSet(SetUF<T>* item);
	void			 deleteSet();
	
	// Destroys all SetUF created. Use only for cleanup. Otherwise programmer 
	// is responsible for deleting object.
	static void destroy();

	~SetUF();
private:
	SetUF(T item);
	static void deleteRec(SetUF<T>* node);
	static std::unordered_set<SetUF<T>*> all_created;
};

template <typename T>
std::unordered_set<SetUF<T>*> SetUF<T>::all_created;

template <typename T>
SetUF<T>* SetUF<T>::makeSet(T item)
{
	SetUF<T>* singleton = new SetUF<T>(item);
	all_created.insert(singleton);
	return singleton;
}

template <typename T>
SetUF<T>* SetUF<T>::unionByRank(SetUF<T>* first, SetUF<T>* second)
{
	SetUF<T>* repre_first = find(first);
	SetUF<T>* repre_second = find(second);

	if (repre_first == repre_second) {
		return repre_first;
	}

	if (repre_first->rank < repre_second->rank) {
		SetUF<T> * tmp = repre_first;
		repre_first = repre_second;
		repre_second = tmp;
	}

	repre_first->successors.insert(repre_second);
	repre_second->parent = repre_first;
	
	if (repre_first->rank == repre_second->rank)
	{
		repre_first->rank += 1;
	}
	repre_first->size += repre_second->size;

	return repre_first;
}

template<typename T>
inline SetUF<T>* SetUF<T>::unionInOrder(SetUF<T>* first, SetUF<T>* second)
{
	SetUF<T>* repre_first = find(first);
	SetUF<T>* repre_second = find(second);

	if (repre_first == repre_second)
	{
		return repre_first;
	}

	repre_first->successors.insert(repre_second);
	repre_second->parent = repre_first;

	repre_first->size += repre_second->size;
	return repre_first;
}

template <typename T>
SetUF<T>* SetUF<T>::find(SetUF<T>* item)
{
	SetUF<T>* current = item;
	std::vector<SetUF<T>*> to_rewire;
	while (current->parent != current)
	{
		to_rewire.push_back(current);
		current->parent->successors.erase(current);
		current = current->parent;
	}

	for (SetUF<T>* s : to_rewire)
	{
		s->parent = current;
		current->successors.insert(s);
	}

	return current;
}

template <typename T>
SetUF<T>* findRec(SetUF<T>* item, std::vector<SetUF<T>*>& successors)
{
	if (item->parent == item) {
		item->successors.insert(successors);
		return item;
	}
	successors.insert(item);
	item->parent->successors.erase(item);
	item->parent = findRec(item->parent, successors);
	return item->parent;
}

template <typename T>
SetUF<T>* SetUF<T>::findRec(SetUF<T>* item)
{
	std::vector<SetUF<T>*> to_rewire;
	SetUF<T>* representant = findRec(item, to_rewire);

	return representant;
}

template <typename T>
SetUF<T>* SetUF<T>::findWithoutCompression(SetUF<T>* item)
{
	SetUF<T>* current = item;
	while (current->parent != current)
	{
		current = current->parent;
	}
	return current;
}

template<typename T>
void SetUF<T>::deleteRec(SetUF<T>* node)
{
	for (SetUF<T>* n : node->successors)
	{
		deleteRec(n);
	}
	all_created.erase(node);
	delete node;
}

template <typename T>
void SetUF<T>::deleteSet(SetUF<T>* item)
{
	SetUF<T>* repre = find(item);
	deleteRec(repre);
}

template <typename T>
void SetUF<T>::deleteSet()
{
	deleteSet(find(this));
}

template <typename T>
void SetUF<T>::destroy()
{
	for (SetUF<T>* s : all_created)
	{
		delete s;
	}
}

template <typename T>
SetUF<T>::SetUF(T item) : item(item)
{
}

template <typename T>
SetUF<T>::~SetUF()
{
}




