from __future__ import annotations
from typing import Any


class Node:

    def __init__(self, data: Any):
        self.left = None
        self.right = None
        self.data = data

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


def print_order(node: Node, order: str):
    print(f'{order}:')
    getattr(Node, order)(node)


root = Node(0)
root.left = Node(1)
root.right = Node(2)
root.left.left = Node(3)
root.left.right = Node(4)
root.right.left = Node(5)
root.right.right = Node(6)

line = "-"*len(str(root))

print(f'tree:\n{root}\n{line}')

print_order(root, 'inorder')
print(f'\n{line}')

print_order(root, 'preorder')
print(f'\n{line}')

print_order(root, 'postorder')
print(f'\n{line}')
