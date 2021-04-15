#!/usr/bin/env python3

from __future__ import annotations
from typing import Any
import sys


class Node:

    def __init__(self, data: Any):
        self.left = None
        self.right = None
        self.data = data

    def __str__(self) -> str:
        return f'{self.data} - ({self.left}, {self.right})'


def inorder(node: Node):
    if node:
        inorder(node.left)
        print(node.data, end=' ')
        inorder(node.right)


def preorder(node: Node):
    if node:
        print(node.data, end=' ')
        preorder(node.left)
        preorder(node.right)


def postorder(node: Node):
    if node:
        postorder(node.left)
        postorder(node.right)
        print(node.data, end=' ')


def invert_tree(node: Node):
    if node is None:
        return
    invert_tree(node.left)
    invert_tree(node.right)
    temp = node.left
    node.left = node.right
    node.right = temp


def print_orders(node: Node):
    for o in ('inorder', 'preorder', 'postorder'):
        print(f'{o}:', end=' ')
        getattr(sys.modules[__name__], o)(node)
        print()


root = Node(0)
root.left = Node(1)
root.right = Node(2)
root.left.left = Node(3)
root.left.right = Node(4)

print(f'tree:\n{root}')
print_orders(root)

invert_tree(root)

print(f'inverted tree:\n{root}')
print_orders(root)
