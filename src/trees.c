#include "..//include//trees.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 10E-6

TreeNode *newNode(TeamNode *data) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->score = data->score;
    node->teamName = data->teamName;
    node->left = node->right = NULL;
    return node;
}

TreeNode *insert(TreeNode *node, TeamNode *team) {
    if (node == NULL)
        return newNode(team);
    if (team->score < node->score)
        node->left = insert(node->left, team);
    else if (team->score >= node->score)
        node->right = insert(node->right, team);
    return node;
}

void initTree(TreeNode **root, TeamNode *d) {
    *root = (TreeNode *)malloc(sizeof(TreeNode));
    (*root)->left = (*root)->right = NULL;
    (*root)->score = d->score;
    (*root)->teamName = d->teamName;
}

void buildTree(TreeNode *root, TeamNode *team) {
    while (team != NULL) {
        insert(root, team);
        team = team->nextTeam;
    }
}

void printDescending(TreeNode *root, char *outputFilePath) {

    if (root->right != NULL)
        printDescending(root->right, outputFilePath);

    FILE *outputFile = fopen(outputFilePath, "at");
    fprintf(outputFile, "%-34s-  %.2f\n", root->teamName, root->score);
    fclose(outputFile);

    if (root->left != NULL)
        printDescending(root->left, outputFilePath);
}

void sortNodesByName(TeamNode *team) {
    TeamNode *i = team;
    TeamNode *j = NULL;
    while (i->nextTeam) {
        j = i->nextTeam;
        while (j) {
            if (fabs(i->score - j->score) < EPS)
                if (strcasecmp(i->teamName, j->teamName) < 0) {
                    char *auxCh = i->teamName;
                    i->teamName = j->teamName;
                    j->teamName = auxCh;

                    float auxIn = i->score;
                    i->score = j->score;
                    j->score = auxIn;
                }
                     else if (i->score < j->score) {
                    char *auxCh = i->teamName;
                    i->teamName = j->teamName;
                    j->teamName = auxCh;

                    float auxIn = i->score;
                    i->score = j->score;
                    j->score = auxIn;
                } 

            j = j->nextTeam;
        }
        i = i->nextTeam;
    }
}

void sortNodesByValue(TeamNode *team) {
    TeamNode *i = team;
    TeamNode *j = NULL;
    while (i->nextTeam) {
        j = i->nextTeam;
        while (j) {
            if (fabs((i->score - j->score)) < EPS) // daca sunt egale
                if (strcmp(i->teamName, j->teamName) < 0) {
                    char *auxCh = i->teamName;
                    i->teamName = j->teamName;
                    j->teamName = auxCh;
                }

            j = j->nextTeam;
        }

        i = i->nextTeam;
    }
}

int max(int a, int b) {
    return ((a > b) ? a : b);
}

int nodeHeight(TreeNode *root) {
    if (root == NULL)
        return -1;
    else
        return root->height;
}

TreeNode *LeftRotation(TreeNode *z) {
    TreeNode *y = z->right;
    TreeNode *T2 = y->left;
    y->left = z;
    z->right = T2;
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;

    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}

TreeNode *LRRotation(TreeNode *Z) {
    Z->left = LeftRotation(Z->left);
    return RightRotation(Z);
}

TreeNode *RLRotation(TreeNode *Z) {
    Z->right = RightRotation(Z->right);
    return LeftRotation(Z);
}

TreeNode *RightRotation(TreeNode *z) {
    TreeNode *y = z->left;
    TreeNode *T3 = y->right;
    y->right = z;
    z->left = T3;
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}

TreeNode *insertAvl(TreeNode **node, TreeNode *value) {
    // 1. inserare nod
    if (*node == NULL) {
        *node = value;
        (*node)->height = 0;
        // optional
        (*node)->left = (*node)->right = NULL;
        return (*node);
        // node->left = NULL;
        // node->right = NULL;
    }
    if (value->score == (*node)->score) {
        if (strcasecmp(value->teamName, (*node)->teamName) < 0)
            (*node)->left = insertAvl(&(*node)->left, value);
        else
            (*node)->right = insertAvl(&(*node)->right, value);
    } else if (value->score < (*node)->score)
        (*node)->left = insertAvl(&(*node)->left, value);
    else
        (*node)->right = insertAvl(&(*node)->right, value);
      /*  if (value->score < (*node)->score)
		    (*node)->left = insertAvl(&(*node)->left, value);
	    else if (value->score > (*node)->score)
		    (*node)->right = insertAvl(&(*node)->right, value);
	    else {
            if(strcasecmp(value->teamName, (*node)->teamName))
                (*node)->right = insertAvl(&(*node)->right, value);
            else
                (*node)->left = insertAvl(&(*node)->left, value);
        }        */    
    // 2. updateaza inaltimea nodurilor stramos
    // de jos in sus la iesirea din apelul recurent
    (*node)->height = 1 + max(nodeHeight((*node)->left), nodeHeight((*node)->right));
    // 3. afla factorul de echilibru al nodului stramos
    // testul se face de jos in sus ,
    // pentru toate nodurile intre cel adaugat si radacina
    int balance = (nodeHeight((*node)->left) - nodeHeight((*node)->right));
    // 4. daca nodul nu este echilibrat -¿ echilibreaza
    // LL Case
    // k ¿1 ¡=¿ y este in stanga
    // key ¡ valoarea din nodul stang =¿ x in stanga lui y
     if (balance > 1) {
         if (value->score == (*node)->left->score) {
             if (strcasecmp(value->teamName, (*node)->left->teamName) < 0)
                 return RightRotation((*node));
             else
                 return LRRotation(*node);
         }
         else if (value->score < (*node)->left->score)
             return RightRotation(*node);
         else
             return LRRotation(*node);

     }
     else if (balance < -1) {
         if (value->score == (*node)->right->score) {
             if (strcasecmp(value->teamName, (*node)->right->teamName) < 0)
                 return RLRotation(*node);
             else
                 return LeftRotation(*node);
         }
         else if (value->score > (*node)->right->score)
             return LeftRotation(*node);
         else
             return RLRotation(*node);
     }
     return *node;

   /*  if (balance > 1 && value->score <= (*node)->left->score)
        return RightRotation(*node);
    // RR Case
    // k ¡-1 ¡=¿ y este in dreapta
    // key ¿ valoarea din nodul drept =¿ x in dreapta lui y
    if (balance < -1 && value->score >= (*node)->right->score)
        return LeftRotation((*node));
    // LR Case
    // k ¿1 ¡=¿ y este in stanga
    // key ¿ valoarea din nodul stang =¿ x in dreapta lui y
    if (balance > 1 && value->score >= (*node)->left->score)
        return LRRotation((*node));
    // RL Case
    if (balance < -1 && value->score <= (*node)->right->score)
        return RLRotation((*node));
    return *node;*/
} 

// postorder
void insertFromBstToAVL(TreeNode *rootBst, TreeNode **rootAvl) {
    if (rootBst->right != NULL) {
        insertFromBstToAVL(rootBst->right, rootAvl);
        rootBst->right = NULL;
    }
    if (rootBst->left != NULL) {
        insertFromBstToAVL(rootBst->left, rootAvl);
        rootBst->left = NULL;
    }

    if (rootBst->right == NULL && rootBst->left == NULL) {
        (*rootAvl) = insertAvl(rootAvl, rootBst);
    }
}

void printPostOrder(TreeNode *root, char *outputFilePath) {
    if (root->left) {
        printPostOrder(root->left, outputFilePath);
    }
    if (root->right) {
        printPostOrder(root->right, outputFilePath);
    }
    FILE *outputFile = fopen(outputFilePath, "at");
    fprintf(outputFile, "%f ", root->score);
    fclose(outputFile);
}

void printLevelOrderTraversal(TreeNode *root, int level, char *outputFilePath) {
    if (root->left) {
        printLevelOrderTraversal(root->left, level, outputFilePath);
    }

    if (root->right) {
        printLevelOrderTraversal(root->right, level, outputFilePath);
    }

    if (root != NULL)
        ;
}

void printLevel(TreeNode *root, int level, char *outputFilePath) {
    if (root == NULL)
        return;
    if (level == 1) {
        FILE *outputFile = fopen(outputFilePath, "at");
        fprintf(outputFile, "%s %.2f\n", root->teamName, root->score);
        fclose(outputFile);
    } else if (level > 1) {
        printLevel(root->right, level - 1, outputFilePath);
        printLevel(root->left, level - 1, outputFilePath);
    }
}

void extractFromBstToList(TreeNode *root, TeamNode **listForAvl) {
    if (root->left != NULL) {
        extractFromBstToList(root->left, listForAvl);
        root->left = NULL;
    }
    if (root->right != NULL) {
        extractFromBstToList(root->right, listForAvl);
        root->right = NULL;
    }

    if (root->right == NULL && root->left == NULL) {
        insertInList(listForAvl, &root);
    }
}

void insertInList(TeamNode **root, TreeNode **value) {
    // build a node for list
    TeamNode *node = calloc(1, sizeof(TeamNode));

    if (*root == NULL) {
        node->score = (*value)->score;
        node->teamName = (*value)->teamName;
        free(*value);
        (*root) = node;
        return;
    }

    node->score = (*value)->score;
    node->teamName = (*value)->teamName;
    free(*value);

    TeamNode *aux = *root;
    while (aux->nextTeam != NULL)
        aux = aux->nextTeam;

    aux->nextTeam = node;
}

void insertFromListToAvl(TreeNode **rootAvl, TeamNode **listAvl) {
    // convert a teamnode into an treenode
    TeamNode *aux = NULL;
    while (*listAvl != NULL) {
        TreeNode *node = calloc(1, sizeof(TreeNode));
        node->score = (*listAvl)->score;
        node->teamName = (*listAvl)->teamName;
        aux = *listAvl;
        (*listAvl) = (*listAvl)->nextTeam;
        (*rootAvl) = insertAvl(rootAvl, node);

        free(aux);
    }
}