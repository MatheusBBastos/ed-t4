#ifndef RBTREE_H
#define RBTREE_H

// TODO: remover (debug)
#include <stdio.h>
#include <stdbool.h>

typedef void *RBTree;
typedef void *Key;
typedef void *Value;
typedef void *Node;

// Cria uma árvore rubro-negra com a função de comparação (ex: strcmp)
RBTree RBTree_Create(int (*compare)(Key, Key));

// Insere um par chave-valor na árvore
Value RBTree_Insert(RBTree tree, Key key, Value value);

// Retorna o valor correspondente à chave (NULL se não existir)
Value RBTree_Find(RBTree tree, Key key);

// Remove o par chave-valor e retorna o valor removido (NULL se não existir)
Value RBTree_Remove(RBTree tree, Key key);

// Executa func em todos os valores, com o valor como primeiro parâmetro e
// param como o segundo -> func(valor, param)
void RBTree_Execute(RBTree tree, void (*func)(Value, void*), void *param);

// Destrói a árvore, executando destroy em todos os valores
void RBTree_Destroy(RBTree tree, void (*destroy)(Value));

// Retorna o nó raiz da árvore (NULL se não existir)
Node RBTree_GetRoot(RBTree tree);

// Retorna o filho da esquerda do nó (NULL se não existir)
Node RBTreeN_GetLeftChild(RBTree tree, Node node);

// Retorna o filho da direita do nó (NULL se não existir)
Node RBTreeN_GetRightChild(RBTree tree, Node node);

// Retorna a chave do nó
Key RBTreeN_GetKey(RBTree tree, Node node);

// Retorna o valor do nó
Value RBTreeN_GetValue(RBTree tree, Node node);

#endif