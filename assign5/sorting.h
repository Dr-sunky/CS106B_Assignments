#pragma once
#include "vector.h"
#include "listnode.h"

void runSort(ListNode*& front);
void quickSort(ListNode*& front);
void quickSortRec(ListNode* front, ListNode* end, ListNode*& highStart);

void sort(ListNode* front, ListNode*& left, ListNode*& right, ListNode*& base, ListNode*& leftTail, ListNode*& baseTail);
void subSort(ListNode* front, ListNode* end);

/*
 * utility function
 */
void printList(ListNode* front);
void deallocateList(ListNode* front);
ListNode* createList(Vector<int> values);
int LengthOfLinkedList(ListNode* front);
bool areEquivalent(ListNode* front, Vector<int> v);

void partition(ListNode*& list, ListNode*& less, ListNode*& privot, ListNode*& more);
void concatenate(ListNode*& list, ListNode*& less, ListNode*& privot, ListNode*& more);
ListNode* getTail (ListNode* list);
