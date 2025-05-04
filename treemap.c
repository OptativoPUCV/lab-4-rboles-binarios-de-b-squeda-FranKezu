#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap *) malloc(sizeof(TreeMap));

    new->current = NULL;
    new->root = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if((tree == NULL) || (value == NULL) || (key == NULL)) return;

    TreeNode *new = createTreeNode(key, value);

    if(tree->root == NULL){
        tree->root = new;
        tree->current = new;
        return;
    }

    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    while(current != NULL){
        parent = current;

        if(is_equal(tree, key, current->pair->key)){
            tree->current = current;
            return;
        }

        if(tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    new->parent = parent;

    if(tree->lower_than(key, parent->pair->key))
        parent->left = new;
    else
        parent->right = new;

    tree->current = new;
}

TreeNode * minimum(TreeNode * x){

    if(x == NULL) return NULL;

    TreeNode *current = x;

    if(current->left == NULL) return x;
    
    while(current->left != NULL){
        current = current->left;
    }

    return current;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(tree == NULL || node == NULL) return;

    // CASO 1
    if(node->left == NULL && node->right == NULL){
        if(tree->root == node)
            tree->root = NULL;
        else{
        TreeNode *parent = node->parent;
        if(parent->left == node)
            parent->left = NULL;
        else
            parent->right = NULL;
        }
    }

    // CASO 2
    if(node->left != NULL && node->right == NULL){
        if(tree->root == node){
            tree->root = node->left;
            node->left->parent = NULL;
        } else{
            TreeNode *parent = node->parent;
            if(parent->left == node)
                parent->left = node->left;
            else
                parent->right = node->left;
            node->left->parent = parent;
        }
    }
    if(node->left == NULL && node->right != NULL){
        if(tree->root == node){
            tree->root = node->right;
            node->right->parent = NULL;
        } else{
            TreeNode *parent = node->parent;
            if(parent->left == node)
                parent->left = node->right;
            else
                parent->right = node->right;
            node->right->parent = parent;
        }
    }

    //CASO 3;
    if(node->left != NULL && node->right != NULL){
        TreeNode *min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }

    free(node->pair);
    free(node);
    tree->current = NULL;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if((key == NULL) || (tree->root == NULL)) return NULL;

    TreeNode *current = tree->root;

    while(current != NULL){

        if(is_equal(tree, key, current->pair->key)){
            tree->current = current;
            return current->pair;
        }

        if(tree->lower_than(key, current->pair->key)) current = current->left;
        else current = current->right;
    }

    tree->current = NULL;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
