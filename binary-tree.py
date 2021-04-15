#!/usr/bin/env python3

from __future__ import annotations
from typing import Any


class Node:

    def __init__(self, data: Any):
        self.left = None
        self.right = None
        self.data = data

    def print_order(self, order: str):
        print(f'{order}:', end=' ')
        getattr(self, order)(self)

    def __str__(self) -> str:
        return f'{self.data} - ({self.left}, {self.right})'

    @staticmethod
    def inorder(node: Node):
        if node:
            node.inorder(node.left)
            print(node.data, end=' ')
            node.inorder(node.right)

    @staticmethod
    def preorder(node: Node):
        if node:
            print(node.data, end=' ')
            node.preorder(node.left)
            node.preorder(node.right)

    @staticmethod
    def postorder(node: Node):
        if node:
            node.postorder(node.left)
            node.postorder(node.right)
            print(node.data, end=' ')

    @staticmethod
    def invert_tree(node: Node):
        if node is None:
            return
        node.invert_tree(node.left)
        node.invert_tree(node.right)
        temp = node.left
        node.left = node.right
        node.right = temp


root = Node(0)
root.left = Node(1)
root.right = Node(2)
root.left.left = Node(3)
root.left.right = Node(4)

print(f'tree:\n{root}')

for o in ('inorder', 'preorder', 'postorder'):
    root.print_order(o)
    print()

root.invert_tree(root)

print(f'inverted tree:\n{root}')
for o in ('inorder', 'preorder', 'postorder'):
    root.print_order(o)
    print()
