#include <stdio.h>
#include<string.h>
#include <stdlib.h>

typedef struct AVL
{
    char month[20];
    struct AVL *left;
    struct AVL *right;
    struct AVL *parent;
    int balFactor;
} AVL;

typedef AVL* node;


void Init(node* root);
void InsertNode(node* root,char* month);
void RemoveNode(node* root,char * month);
void Traverse(node root);
void _deleteTree(node root);
void destroyTree(node *Node_ref);

int main()
{
    node root;
    Init(&root);
    while (1)
    {
        int command = 0;
        printf("Enter digits to execute command\n");
        printf("1.Insert all the months of the year\n2.Remove a particular month\n3.Display all the trees \n4.Delete all the months in tree\n5.Exit\n");
        scanf("%d", &command);
        if (command < 1 || command > 5)
        {
            printf("Enter a valid number to proceed.....");
            continue;
        }
        switch (command)
        {
            char month[10];
        case 1:
            InsertNode(&root, "Dec");
            InsertNode(&root, "Jan");
            InsertNode(&root, "Apr");
            InsertNode(&root, "Jul");
            InsertNode(&root, "Aug");
            InsertNode(&root, "Oct");
            InsertNode(&root, "Feb");
            InsertNode(&root, "Nov");
            InsertNode(&root, "Mar");
            InsertNode(&root, "May");
            InsertNode(&root, "Jun");

            printf("All months succesfully inserted");
            break;
        case 2:
            printf("Enter one of the following words to delete it :\n");
            printf("Mar, Feb, Jan, Apr, May, Jun, Jul, Aug, Sept, Oct, Nov, Dec\n");
            printf("Enter Month to delete: ");
            scanf("%s", month);
            RemoveNode(&root, month);
            break;
        case 3:
            Traverse(root);
            break;
        case 4:
            destroyTree(&root);
            break;
        default:
            break;
        }
        if (command == 5)
            break;
    }
    return 0;
}

int max(int a, int b)
{
    if(a>b)
	return a;
    else
	return b;
}
void Init(node *root)
{
    (*root) = NULL;
}

int height(node root)
{
    int left,right;
    if (!root)
        return 0;
    left=height(root->left);
    right=height(root->right);
    if(left>right)
	return left+1;
    else
	return right+1;
}

void updateBalanceFactor(node root)
{
    if (!root)
        return;
    root->balFactor = height(root->left) - height(root->right);
    updateBalanceFactor(root->parent);
}

// int getBalance(node root)
// {
//     if (!root)
//         return 0;
//     else
//         return height(root->left)-height(root->right);
// }

node findFirstUnbalanced(node root)
{
    if (!root)
        return NULL;
    node p = root;

    while (p)
    {
        if (p->balFactor > 1 || p->balFactor < -1)
            break;
        p = p->parent;
    }
    return p;
}

void LLRotation(node *root, node *n)
{

    node A = (*n);

    node B = ((*n)->left);
    node BR = B->right;
    B->right = A;
    A->left = BR;
    B->parent = A->parent;
    if (BR)
        BR->parent = A;
    A->parent = B;
    B->balFactor = 0;
    A->balFactor = 0;
    if ((*root) == A)
    {
        (*root) = B;
        return;
    }
    if (B->parent->left == A)
        B->parent->left = B;
    else
        B->parent->right = B;
    return;
}

void RRRotation(node *root, node *n)
{
    node A = (*n);
    node B = ((*n)->right);
    node BL = B->left;
    B->left = A;
    A->right = BL;
    B->parent = A->parent;
    if (BL)
        BL->parent = A;
    A->parent = B;
    B->balFactor = 0;
    A->balFactor = 0;
    if ((*root) == A)
    {
        (*root) = B;
        return;
    }
    if (B->parent->left == A)
        B->parent->left = B;
    else
        B->parent->right = B;
    return;
}

void LRRotation(node *root, node *n)
{
    RRRotation(root, &(*n)->left);
    LLRotation(root, n);
}

void RLRotation(node *root, node *n)
{
    LLRotation(root, &(*n)->right);
    RRRotation(root, n);
}

void InsertNode(node *root, char *month)
{
    node newnode = (node)malloc(sizeof(AVL));
    if (!newnode)
        return;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;
    newnode->balFactor = 0;
    strcpy(newnode->month, month);
    if (!(*root))
    {
        (*root) = newnode;
        return;
    }
    node p, q;
    q = NULL;
    p = (*root);
    while (p)
    {
        q = p;
        if (strcmp(p->month, month) > 0)
            p = p->left;
        else if (strcmp(p->month, month) < 0)
            p = p->right;
        else
            return;
    }

    newnode->parent = q;
    if (strcmp(q->month, month) > 0)
        q->left = newnode;
    else
        q->right = newnode;

    updateBalanceFactor(q);
    node unbal = NULL;
    unbal = findFirstUnbalanced(q);
    do
    {

        if (!unbal)
            return;
        node parentOfUnbal = unbal->parent;

        if (unbal->balFactor >= 2 && unbal->left->balFactor == 1)
            LLRotation(root, &unbal);
        else if (unbal->balFactor >= 2 && unbal->left->balFactor == -1)
            LRRotation(root, &unbal);
        else if (unbal->balFactor <= -2 && unbal->right->balFactor == 1)
            RLRotation(root, &unbal);
        else if (unbal->balFactor <= -2 && unbal->right->balFactor == -1)
        {
            RRRotation(root, &unbal);
        }

        updateBalanceFactor(parentOfUnbal);
        unbal = findFirstUnbalanced(parentOfUnbal);
    } while (unbal);

    return;
}

void RemoveNode(node *root, char *month)
{
    if (!(*root))
        return;
    node p, q;
    p = (*root), q = NULL;
    while (p)
    {
        if (strcmp(p->month, month) > 0)
            p = p->left;
        else if (strcmp(p->month, month) < 0)
            p = p->right;
        else
            break;
    }
    if (!p)
        return;
    q = p->parent;

    if (!p->left && !p->right)
    {
        if (p == q->left)
            q->left = NULL;
        else
            q->right = NULL;
        free(p);
        updateBalanceFactor(q);
        node unbal = NULL;
        unbal = findFirstUnbalanced(q);
        do
        {

            if (!unbal)
                return;
            node parentOfUnbal = unbal->parent;

            if (unbal->balFactor >= 2 && unbal->left->balFactor == 1)
                LLRotation(root, &unbal);
            else if (unbal->balFactor >= 2 && unbal->left->balFactor == -1)
                LRRotation(root, &unbal);
            else if (unbal->balFactor <= -2 && unbal->right->balFactor == 1)
                RLRotation(root, &unbal);
            else if (unbal->balFactor <= -2 && unbal->right->balFactor == -1)
                RRRotation(root, &unbal);

            updateBalanceFactor(parentOfUnbal);
            unbal = findFirstUnbalanced(parentOfUnbal);
        } while (unbal);
    }
    else if (!p->left || !p->right)
    {
        node temp = p->left ? p->left : p->right;
        if (p == q->left)
            q->left = temp;
        else
            q->right = temp;
        temp->parent = q;
        free(p);

        updateBalanceFactor(q);
        node unbal = NULL;
        unbal = findFirstUnbalanced(q);
        do
        {

            if (!unbal)
                return;
            node parentOfUnbal = unbal->parent;

            if (unbal->balFactor == 2 && unbal->left->balFactor == 1)
                LLRotation(root, &unbal);
            else if (unbal->balFactor == 2 && unbal->left->balFactor == -1)
                LRRotation(root, &unbal);
            else if (unbal->balFactor == -2 && unbal->right->balFactor == 1)
                RLRotation(root, &unbal);
            else if (unbal->balFactor == -2 && unbal->right->balFactor == -1)
            {
                RRRotation(root, &unbal);
            }

            updateBalanceFactor(parentOfUnbal);
            unbal = findFirstUnbalanced(parentOfUnbal);
        } while (unbal);
    }
    else
    {
        node *temp1 = &(p->left);
        while ((*temp1)->right)
            temp1 = &(*temp1)->right;
        strcpy(p->month, (*temp1)->month);
        RemoveNode(temp1, p->month);
    }
}

void Traverse(node root)
{
    char temp[20] = {'\0'};
    if (!root)
        return;
    Traverse(root->left);
    if (root->parent)
        strcpy(temp, root->parent->month);
    printf("Month = %s, Parent = %s,BalanceFactor = %d\n", root->month, temp, root->balFactor);
    Traverse(root->right);
}

void _deleteTree(node root)
{
    if (root == NULL)
        return;
    _deleteTree(root->left);
    _deleteTree(root->right);
    free(root);
}
void destroyTree(node *Node_ref)
{
    _deleteTree(*Node_ref);
    *Node_ref = NULL;
    return;
}
