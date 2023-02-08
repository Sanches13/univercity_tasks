from __future__ import annotations
from binary_tree.node import *
from typing import List


class BinaryTree(Generic[T]):
    def __init__(self, other=None) -> None:
        self.__root: Optional[Node] = None
        if other is not None:
            if type(other) is not type(self):
                raise TypeError(f"TypeError: must be {type(self)}, not {type(other)}")
            else:
                self.__root.data = Node[T](other.__root.data)
                self.__copy_node(self.__root, other.__root)

    def __copy_node(self, node: Node, other_node: Node) -> None:
        if other_node.left_child is not None:
            node.left_child = Node[T](other_node.left_child.data)
            node.left_child.parent = node
            self.__copy_node(node.left_child, other_node.left_child)
        if other_node.right_child is not None:
            node.right_child = Node[T](other_node.right_child.data)
            node.right_child.parent = node
            self.__copy_node(node.right_child, other_node.right_child)

    def clear(self) -> None:
        self.__root = None

    def is_empty(self) -> bool:
        return self.__root is None

    def contains(self, data: T) -> bool:
        current = self.__root
        while current is not None:
            if data < current.data:
                current = current.left_child
            elif data > current.data:
                current = current.right_child
            else:
                return True
        return False

    def find(self, data: T) -> Node:
        current = self.__root
        while current is not None:
            if data < current.data:
                current = current.left_child
            elif data > current.data:
                current = current.right_child
            else:
                return current

    def insert(self, data: T) -> None:
        if self.__root is None:
            self.__root = Node[T](data)
        else:
            self.__insert_node(Node[T](data), self.__root)

    def __insert_node(self, insertion_node: Node, current: Node) -> None:
        while True:
            if insertion_node < current:
                if current.left_child is None:
                    insertion_node.parent = current
                    current.left_child = insertion_node
                    break
                else:
                    current = current.left_child
                    continue

            if insertion_node > current:
                if current.right_child is None:
                    insertion_node.parent = current
                    current.right_child = insertion_node
                    break
                else:
                    current = current.right_child
            else:
                return

    def remove(self, data: T) -> None:
        node = self.find(data)
        if node is not None:
            self.__remove(node)
        return

    def __remove(self, current: Node) -> None:
        if current.left_child is not None and current.right_child is not None:
            nearest = self.__nearest(current)
            tmp = nearest.data
            self.__remove(nearest)
            current.data = tmp

        elif current.left_child is not None or current.right_child is not None:
            if current.left_child is not None:
                current.data = current.left_child.data
                current.left_child = None
            else:
                current.data = current.right_child.data
                current.right_child = None
        else:
            self.__remove_node(current)

    def __remove_node(self, node: Node) -> None:
        parent = node.parent
        if node is parent.left_child:
            parent.left_child = None
        else:
            parent.right_child = None

    def __nearest(self, current: Node) -> Node:
        if current.left_child is not None:
            nearest = current.left_child
            while nearest.right_child is not None:
                nearest = nearest.right_child
        elif current.right_child is not None:
            nearest = current.right_child
            while nearest.left_child is not None:
                nearest = nearest.left_child
        else:
            nearest = current
        return nearest

    def to_list(self) -> List[T]:
        lst = []
        if self.__root is not None:
            self.__get_node(self.__root, lst)
        return lst

    def __get_node(self, node: Node, lst: List[T]) -> None:
        if node.left_child is not None:
            self.__get_node(node.left_child, lst)
        lst.append(node.data)
        if node.right_child is not None:
            self.__get_node(node.right_child, lst)

    def print_tree(self) -> None:
        if self.__root is not None:
            self.__print_node(self.__root)

    def __print_node(self, node: Node) -> None:
        if node.left_child is not None:
            self.__print_node(node.left_child)
        print(node)
        if node.right_child is not None:
            self.__print_node(node.right_child)

