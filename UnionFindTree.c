/* ========================================================================= *
 * Union find tree implementation:
 * ========================================================================= */
#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

// value 

struct union_find_t{
	int parent;
	int value; 
	int size;
	int totalSize;
};

UnionFind* ufCreate(size_t n_items)
{
	if (n_items < 0)
		return NULL;//fatal error !!!
	UnionFind *newUF = malloc(n_items * sizeof(UnionFind));
	for (int i = 0; i < n_items; ++i)
	{
		newUF[i].value = i;
		newUF[i].parent = i;
		newUF[i].size = 1;
		newUF[i].totalSize = n_items;
	}
	return newUF;
}

void ufFree(UnionFind* union_find)
{
	free(union_find);
	return;
}

ufStatus ufUnion(UnionFind* union_find, size_t item1, size_t item2)
{
	if(union_find){
		if (item1 >= union_find[0].totalSize || item2 >= union_find[0].totalSize || item1 < 0 || item2 <0)
		{
			printf("error wrong element number\n");
			return UF_ERROR;
		}
	}
	//	check if same
	int i = item1, j = item2;
	while((union_find[i].parent != i) || (union_find[j].parent != j))
	{
		i = union_find[i].parent;
		j = union_find[j].parent;
	}
	if (i == j)
	{
		return UF_SAME;
	}

	int root1 = i;
	int root2 = j;

	if(union_find[root1].size >= union_find[root2].size)
	{
		union_find[root2].parent = root1;
		union_find[root1].size += union_find[root2].size;
		return UF_MERGED;
	}
	else
	{
		union_find[root1].parent = root2;
		union_find[root2].size += union_find[root1].size;
		return UF_MERGED;
	}
	return UF_ERROR;
}

size_t ufFind(const UnionFind* union_find, size_t item)
{
	int i = item;
	while(union_find[i].parent != i)
	{
		i = union_find[i].parent;
	}
	size_t root = i;
	return root;
}

size_t ufComponentsCount(const UnionFind* union_find)
{
	int componentsCount = 0;
	for (int i = 0; i < union_find[i].totalSize; ++i)
	{
		if (union_find[i].parent == i)
			componentsCount++;
	}
	return componentsCount;
}
