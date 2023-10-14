#include "SLT.h"

/**
 * @brief  创建一个新的节点
 * @note   
 * @param  xdata: 节点包含的数据
 * @retval newNode: 创建的新节点
 */
SLTNode *BuyListNode(SLTDataType xdata)
{
    SLTNode *newNode = (SLTNode*)malloc(sizeof(SLTNode));

    if(newNode == NULL)
    {
        return NULL;
    }

    newNode -> data = xdata;
    newNode -> next = NULL;

    return newNode;
}

/**
 * @brief  头插
 * @note   
 * @param  **ppHead: 插入的链表的头节点地址
 * @param  xdata: 插入的数据
 * @retval None
 */
void SLTPushFront(SLTNode **ppHead, SLTDataType xdata)
{
    SLTNode *newNode = BuyListNode(xdata);
    newNode -> next = *ppHead;  //将原头节点连接到新节点的next
    *ppHead = newNode;          //将原头节点替换为新的节点

    SLTNode *num = *ppHead;
    uint8_t cnt = 0;
    while(num -> next != NULL)
    {
        num -> Num = cnt;
        cnt++;
    }
    num -> Num = cnt;
}

/**
 * @brief  尾插
 * @note   
 * @param  **ppHead: 
 * @param  xdata: 
 * @retval None
 */
void SLTPushBack(SLTNode **ppHead, SLTDataType xdata)
{
    SLTNode *newNode = BuyListNode(xdata);

    if(newNode == NULL)
    {
        *ppHead = newNode;
    }
    else
    {
        SLTNode *cur = *ppHead;

        while(cur -> next != NULL)
        {
            cur = cur -> next;
        }
        cur -> next = newNode;
        newNode -> Num = (cur -> Num) + 1;
    }
}

/**
 * @brief  头删
 * @note   
 * @param  **ppHead: 
 * @retval None
 */
void SLTPopFront(SLTNode **ppHead)
{
    if(ppHead == NULL)
    {
        return;
    }

    SLTNode *head = *ppHead;

    *ppHead = (*ppHead) -> next;
    free(head);
    head = NULL;

    SLTNode *num = *ppHead;
    uint8_t cnt = 0;
    while(num -> next != NULL)
    {
        num -> Num = cnt;
        cnt++;
    }
    num -> Num = cnt;
}

/**
 * @brief  尾删
 * @note   
 * @param  **ppHead: 
 * @retval None
 */
void SLTPopBack(SLTNode **ppHead)
{
    if((*ppHead) -> next == NULL)
    {
        free(*ppHead);
        *ppHead = NULL;
    }
    else
    {
        SLTNode *cur = *ppHead;

        while(cur -> next != NULL)
        {
            cur = cur -> next;
        }
        free(cur -> next);
        cur -> next = NULL;
    }
}

/**
 * @brief  打印单链表
 * @note   
 * @param  *pHead: 
 * @retval None
 */
void PrintSLT(SLTNode *pHead)
{
    SLTNode *cur = pHead;

    while(cur != NULL)
    {
        printf("%d->",cur -> data);
        cur = cur -> next;
    }
    printf("NULL");
    printf("\n");
}

/**
 * @brief  按值查找节点
 * @note   
 * @param  *pHead: 
 * @param  xdata: 
 * @retval 
 */
SLTNode *SLTNodeFind(SLTNode *pHead, SLTDataType xdata)
{
    SLTNode *find = pHead;

    while(find)
    {
        if(find -> data == xdata)
        {
            return find;
        }
        find = find -> next;
    }
    return NULL;
}

/**
 * @brief  按编号查找链表
 * @note   
 * @param  *pHead: 
 * @param  num: 
 * @retval 
 */
SLTNode *SLTNodeNumFind(SLTNode *pHead, uint8_t num)
{
    SLTNode *find = pHead;

    while(find)
    {
        if(find -> Num == num)
        {
            return find;
        }
        find = find -> next;
    }
    return NULL;
}

/**
 * @brief  删除pos位置的节点
 * @note   
 * @param  **ppHead: 
 * @param  *pos: 
 * @retval None
 */
void SLTErase(SLTNode **ppHead, SLTNode *pos)
{
    if(pos == *ppHead)
    {
        SLTPopFront(ppHead);
    }
    else
    {
        SLTNode *prev = *ppHead;
        while (prev->next != pos)
        {
            prev = prev -> next;
            //如果prev->next已经为空了，说明链表已经查完了，还没有查到pos
			//证明pos传入有误
			if(prev->next == NULL)return;
        }
        prev -> next = pos -> next;
        free(pos);
    }
}

void SLTInsert(SLTNode** pphead, SLTNode* pos, SLTDataType x)
{
	//头插
	if (pos == *pphead)
	{
		SLTPushFront(pphead, x);
	}
	//非头插
	else
	{
		SLTNode* prev = *pphead;
		while (prev->next != pos)
		{
			prev = prev->next;
            if(prev->next == NULL)return;
		}
		SLTNode* newnode = BuyListNode(x);
		prev->next = newnode;
		newnode->next = pos;
	}
}
//单链表结点后插
void SLTInsertBack(SLTNode** pphead, SLTNode* pos, SLTDataType x)
{
	SLTNode* cur = *pphead;
	while (cur != pos)
	{
		cur = cur->next;
		//防止pos传错了
        if(cur->next == NULL)return;
	}
	SLTNode* newnode = BuyListNode(x);
	newnode->next = pos->next;
	pos->next = newnode;
}

// 单链表结点修改
void SLTModify(SLTNode* phead, SLTNode* pos, SLTDataType x)
{
	SLTNode* cur = phead;
	while (cur != pos)
	{
		cur = cur->next;
	}
	pos->data = x;
}
//销毁链表
void SLTDestory(SLTNode** pphead)
{
	SLTNode* cur = *pphead;
	//比cur->next!=NULL更好一些
	while (cur)
	{
		SLTNode* next = cur->next;
		free(cur);
		cur = next;
	}
	*pphead = NULL;
}
