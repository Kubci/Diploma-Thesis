#pragma once
#include <vector>
#include <unordered_set>
#include <future>

#include "Parameters.h"
#include "PixelDataCarrier.h"

template <typename T>
class SetUF
{
public:

	T									item;
	int									rank = 0;
	int									size = 1;
	SetUF<T>*							parent = this;
	Parameters							params;
	bool								isActive = true;
	bool								isCanonical = false;

	static SetUF<T>* makeSet(T item);
	static SetUF<T>* unionByRank(SetUF<T>* first, SetUF<T>* second);
	static SetUF<T>* unionInOrder(SetUF<T>* first, SetUF<T>* second);
	static SetUF<T>* find(SetUF<T>* item);
	static void		 deleteSet(SetUF<T>* item);
	void			 deleteSet();
	
	SetUF();
	SetUF(T item);
	~SetUF();
private:
	static void deleteRec(SetUF<T>* node);
};

typedef SetUF<PixelDataCarrier> SetUFPix;

template <typename T>
SetUF<T>* SetUF<T>::makeSet(T item)
{
	SetUF<T>* singleton = new SetUF<T>(item);
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
	if (first == second)
	{
		return first;
	}

	second->parent = first;
	first->size += second->size;
	return first;
}

template <typename T>
SetUF<T>* SetUF<T>::find(SetUF<T>* set)
{
	if (set->parent == set)	return set;
	set->parent = find(set->parent);
	return set->parent;
}

template<typename T>
void SetUF<T>::deleteRec(SetUF<T>* set)
{
	for (SetUF<T>* n : set->successors)
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

template<typename T>
inline SetUF<T>::SetUF()
{
}

template <typename T>
SetUF<T>::SetUF(T item) : item(item)
{
}

template <typename T>
SetUF<T>::~SetUF()
{
}




