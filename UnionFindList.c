/* ========================================================================= *
 * Union find list implementation:
 * ========================================================================= */
#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

struct union_find_t{
	int next;
	int head;
	int value; 
	int size;
	int totalSize;
};

UnionFind* ufCreate(size_t n_items)
{
	if (n_items < 0)
		return NULL;//erreur fatale !!!
	UnionFind *newUF = malloc(n_items * sizeof(UnionFind));
	for (int i = 0; i < n_items; ++i)
	{
		newUF[i].value = i;
		newUF[i].next = -1;
		newUF[i].head = i;
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
		if (item1 >= union_find[0].totalSize || item2 >= union_find[0].totalSize)
		{
			printf("error : choose element between 0 and %d\n", union_find[0].totalSize-1);
			return UF_ERROR;
		}
	}
	int head1 = union_find[item1].head;
	int head2 = union_find[item2].head;

	if(head1 == head2)
		return UF_SAME; 
	if(union_find[head1].size >= union_find[head2].size)
	{
		int i = head2;
		while(union_find[i].next > -1)
		{
			union_find[i].head = union_find[item1].head;
			i = union_find[i].next;
			
		}
		union_find[i].head = head1;
		union_find[i].next = union_find[head1].next;
		union_find[head1].next = head2;
		union_find[head1].size += union_find[head2].size;
		return UF_MERGED;
	}
	else
	{
		int i = head1;
		while(union_find[i].next > -1)
		{
			union_find[i].head = union_find[item2].head;
			i = union_find[i].next;
			
		}
		union_find[i].head = head2;
		union_find[i].next = union_find[head2].next;
		union_find[head2].next = head1;
		union_find[head2].size += union_find[head1].size;
		return UF_MERGED;
	}
	return UF_ERROR;
}

size_t ufFind(const UnionFind* union_find, size_t item)
{
	return (size_t) (union_find[item].head);
}

size_t ufComponentsCount(const UnionFind* union_find)
{
	int componentsCount = 0;
	for (int i = 0; i < union_find[i].totalSize; ++i)
	{
		if (union_find[i].head == i)
			componentsCount++;
	}
	return componentsCount;
}
