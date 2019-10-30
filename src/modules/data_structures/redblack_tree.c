#include <stdlib.h>
#include <assert.h>

#include "redblack_tree.h"

typedef enum color_t {
    BLACK, RED
} Color;

typedef struct node_t {
    Color color;
    struct node_t *parent;
    struct node_t *left;
    struct node_t *right;
    Key key;
    Value value;
} *NodeImpl;

typedef struct rbtree_t {
    NodeImpl nil;
    NodeImpl root;
    int (*compare)(Key, Key);
} *RBTreeImpl;

static void _fixUp(RBTreeImpl tree, NodeImpl node);
static void _leftRotate(RBTreeImpl tree, NodeImpl node);
static void _rightRotate(RBTreeImpl tree, NodeImpl node);
static void _finishRemoving(RBTreeImpl tree, NodeImpl node);
static void _destroyNode(RBTreeImpl tree, NodeImpl node, void (*destroy)(Value));

RBTree RBTree_Create(int (*compare)(Key, Key)) {
    RBTreeImpl tree = malloc(sizeof(struct rbtree_t));
    tree->nil = malloc(sizeof(struct node_t));
    tree->nil->color = BLACK;
    tree->nil->key = NULL;
    tree->root = tree->nil;
    tree->compare = compare;
    return tree;
}

Value RBTree_Insert(RBTree treeVoid, Key key, Value value) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    NodeImpl node = malloc(sizeof(struct node_t));
    node->left = tree->nil;
    node->right = tree->nil;
    node->color = RED;
    node->key = key;
    node->value = value;
    // Árvore vazia
    if (tree->root == tree->nil) {
        node->parent = tree->nil;
        tree->root = node;
    } else {
        NodeImpl pastNode;
        NodeImpl currentNode = tree->root;
        int cmpResult;
        while (currentNode != tree->nil) {
            pastNode = currentNode;
            cmpResult = tree->compare(key, currentNode->key);
            if (cmpResult < 0) {
                currentNode = currentNode->left;
            } else if (cmpResult > 0) {
                currentNode = currentNode->right;
            } else {
                Value oldVal = currentNode->value;
                currentNode->key = key;
                currentNode->value = value;
                free(node);
                return oldVal;
            }
        }
        node->parent = pastNode;
        if (cmpResult < 0) {
            pastNode->left = node;
        } else {
            pastNode->right = node;
        }
    }
    _fixUp(tree, node);
    return NULL;
}

Value RBTree_Find(RBTree treeVoid, Key key) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    NodeImpl currentNode = tree->root;
    while (currentNode != tree->nil) {
        int cmpResult = tree->compare(key, currentNode->key);
        if (cmpResult < 0)
            currentNode = currentNode->left;
        else if (cmpResult > 0)
            currentNode = currentNode->right;
        else
            return currentNode->value;
    }
    return NULL;
}

Value RBTree_Remove(RBTree treeVoid, Key key) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    NodeImpl node = tree->root;
    while (node != tree->nil) {
        int cmpResult = tree->compare(key, node->key);
        if (cmpResult < 0)
            node = node->left;
        else if (cmpResult > 0)
            node = node->right;
        else
            break;
    }
    if (node == tree->nil)
        return NULL;

    _finishRemoving(tree, node);

    Value value = node->value;
    free(node);
    return value;
}

// TODO: remover (debug)
static int d = 0;

// TODO: tipo void (retorno int só para debug)
static int _executeNode(RBTreeImpl tree, NodeImpl node, void (*func)(Value, void*), void *param, int depth) {
    if (node == tree->nil) {
        // TODO: remover (debug)
        printf("black height: %d\n", depth + 1);
        if (d != 0) {
            assert(depth + 1 == d);
        } else {
            d = depth + 1;
        }
        return 0;
    } else {
        // TODO: remover (debug)
        int s = 0;
        if (node->color == RED) {
            assert(node->left->color == BLACK);
            assert(node->right->color == BLACK);
        }
        s += _executeNode(tree, node->left, func, param, depth + (node->color == BLACK ? 1 : 0));
        printf("key: %d; left: %s; right: %s; color: %s, depth: %d\n", *((int*) node->key), 
                node->left == tree->nil ? "nil" : "node", 
                node->right == tree->nil ? "nil" : "node",
                node->color == RED ? "red" : "black",
                depth);
        func(node->value, param);
        s++;
        s += _executeNode(tree, node->right, func, param, depth + (node->color == BLACK ? 1 : 0));
        return s;
    }
}

void RBTree_Execute(RBTree treeVoid, void (*func)(Value, void*), void *param) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    int size = _executeNode(tree, tree->root, func, param, 0);
    printf("SIZE: %d\n", size);
}

void RBTree_Destroy(RBTree treeVoid, void (*destroy)(Value)) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    if (tree->root != tree->nil)
        _destroyNode(tree, tree->root, destroy);
    free(tree->nil);
    free(tree);
}

static void _destroyNode(RBTreeImpl tree, NodeImpl node, void (*destroy)(Value)) {
    if (node->left != tree->nil)
        _destroyNode(tree, node->left, destroy);
    if (destroy != NULL)
        destroy(node->value);
    if (node->right != tree->nil)
        _destroyNode(tree, node->right, destroy);
    free(node);
}

Node RBTree_GetRoot(RBTree treeVoid) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    if (tree->root == tree->nil)
        return NULL;
    else
        return tree->root;
}

Node RBTreeN_GetLeftChild(RBTree treeVoid, Node nodeVoid) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    NodeImpl node = (NodeImpl) nodeVoid;
    if (node->left == tree->nil)
        return NULL;
    else
        return node->left;
}

Node RBTreeN_GetRightChild(RBTree treeVoid, Node nodeVoid) {
    RBTreeImpl tree = (RBTreeImpl) treeVoid;
    NodeImpl node = (NodeImpl) nodeVoid;
    if (node->right == tree->nil)
        return NULL;
    else
        return node->right;
}

Key RBTreeN_GetKey(RBTree treeVoid, Node nodeVoid) {
    NodeImpl node = (NodeImpl) nodeVoid;
    return node->key;
}

Value RBTreeN_GetValue(RBTree treeVoid, Node nodeVoid) {
    NodeImpl node = (NodeImpl) nodeVoid;
    return node->value;
}

static void _leftRotate(RBTreeImpl tree, NodeImpl node) {
    NodeImpl y = node->right;

    node->right = y->left;
    if (y->left != tree->nil)
        y->left->parent = node;

    y->parent = node->parent;

    if (node->parent == tree->nil)
        tree->root = y;
    else if (node->parent->left == node)
        node->parent->left = y;
    else
        node->parent->right = y;
    
    y->left = node;
    node->parent = y;
}

static void _rightRotate(RBTreeImpl tree, NodeImpl node) {
    NodeImpl y = node->left;

    node->left = y->right;
    if (y->right != tree->nil)
        y->right->parent = node;

    y->parent = node->parent;

    if (node->parent == tree->nil)
        tree->root = y;
    else if (node->parent->left == node)
        node->parent->left = y;
    else
        node->parent->right = y;
    
    y->right = node;
    node->parent = y;
}

static void _fixUp(RBTreeImpl tree, NodeImpl node) {
    while (node != tree->root && node->parent != tree->root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            NodeImpl y = node->parent->parent->right;
            if (y->color == RED) {
                // Caso 1
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    // Caso 2
                    node = node->parent;
                    _leftRotate(tree, node);
                }
                // Caso 3
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                _rightRotate(tree, node->parent->parent);
            }
        } else {
            NodeImpl y = node->parent->parent->left;
            if (y->color == RED) {
                // Caso 4
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    // Caso 5
                    node = node->parent;
                    _rightRotate(tree, node);
                }
                // Caso 6
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                _leftRotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

static void _finishRemoving(RBTreeImpl tree, NodeImpl node) {
    NodeImpl replacement;
    NodeImpl x;
    NodeImpl w; // x's sibling

    bool xLeftChild;

    // First step
    if (node->left == tree->nil && node->right == tree->nil) {
        // two nil children (caso a)
        replacement = tree->nil;
        x = replacement;
        x->parent = node->parent;
        if (node->parent != tree->nil && node->parent->left == node)
            xLeftChild = true;
        else
            xLeftChild = false;
        
    } else if (node->left == tree->nil && node->right != tree->nil) {
        // one nil child on the left (caso b)
        replacement = node->right;
        x = replacement;
        x->parent = node->parent;
        xLeftChild = false;
    } else if (node->left != tree->nil && node->right == tree->nil) {
        // one nil child on the right (caso c)
        replacement = node->left;
        x = replacement;
        x->parent = node->parent;
        xLeftChild = true;
    } else {
        // zero nil children (caso d)
        replacement = node->right;
        // find successor to node
        while (replacement->left != tree->nil)
            replacement = replacement->left;
        x = replacement->right;
        if (replacement != node->right) {
            //replacement->parent->left = x;
            replacement->left = node->left;
            replacement->right = node->right;
            replacement->left->parent = replacement;
            replacement->right->parent = replacement;
            x->parent = replacement->parent;
            x->parent->left = x;
            xLeftChild = true;
        } else {
            x->parent = replacement;
            replacement->left = node->left;
            node->left->parent = replacement;
            xLeftChild = false;
        }
    }

    // replace removed node with its replacement
    if (node->parent != tree->nil) {
        if (node->parent->left == node)
            node->parent->left = replacement;
        else
            node->parent->right = replacement;
    } else {
        tree->root = replacement;
    }

    if (replacement != tree->nil) {
        replacement->parent = node->parent;
    }


    // Second step
    if (node->color == RED && (replacement->color == RED || replacement == tree->nil))
        return;
    else if (node->color == RED && replacement->color == BLACK) {
        replacement->color = RED;
        // proceed to the appropriate case
    } else if (node->color == BLACK && replacement->color == RED) {
        replacement->color = BLACK;
        return;
    }

    // Third step (cases)
    while (x != tree->root) {
        // find x's sibling
        if (xLeftChild)
            w = x->parent->right;
        else
            w = x->parent->left;
        
        if (x->color == RED) {
            // Case 0
            x->color = BLACK;
            return;
        } else {
            if (w->color == RED) {
                // Case 1
                w->color = BLACK;
                x->parent->color = RED;
                if (xLeftChild) {
                    _leftRotate(tree, x->parent);
                    w = x->parent->right;
                } else {
                    _rightRotate(tree, x->parent);
                    w = x->parent->left;
                }
            }
            if (w->color == BLACK) {
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    // Case 2
                    w->color = RED;
                    x = x->parent;
                    xLeftChild = x == x->parent->left;
                    if (x->color == RED) {
                        x->color = BLACK;
                        return;
                    }
                    continue;
                } else if (xLeftChild && w->left->color == RED && w->right->color == BLACK
                        || !xLeftChild && w->right->color == RED && w->left->color == BLACK) {
                    // Case 3
                    w->color = RED;
                    if (xLeftChild) {
                        w->left->color = BLACK;
                        _rightRotate(tree, w);
                        w = x->parent->right;
                    } else {
                        w->right->color = BLACK;
                        _leftRotate(tree, w);
                        w = x->parent->left;
                    }
                }
                // Case 4
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (x->parent->left == x) {
                    w->right->color = BLACK;
                    _leftRotate(tree, x->parent);
                } else {
                    w->left->color = BLACK;
                    _rightRotate(tree, x->parent);
                }
                return;
            }
        }
    }
}
