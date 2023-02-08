from __future__ import annotations
from btree.node import *


class BTree(Generic[T]):

    def __init__(self, t: int, other=None) -> None:
        self.__root: Optional[Node] = None
        self.__t: int = t
        if other is None:
            if t < 2:
                raise ValueError("ValueError: incorrect t parameter(t must be more or equal than 2)")
        elif type(other) is not type(self):
            raise TypeError(f"TypeError: must be {type(self)}, not {type(other)}")
        else:
            if other.__root is not None:
                self.__t = other.__t
                self.__root = Node[T](other.__t)
                self.__copy_node(self.__root, other.__root)

    def __copy_node(self, node: Node, other: Node) -> None:
        node.keys_count = other.keys_count
        node.is_leaf = other.is_leaf
        for i in range(other.keys_count):
            node.keys[i] = other.keys[i]
        if not node.is_leaf:
            i = 0
            while other.children[i] is not None:
                node.children[i] = Node[T](self.__t)
                self.__copy_node(node.children[i], other.children[i])
                i += 1

    def clear(self) -> None:
        self.__root = None

    def find(self, data: T) -> Optional[T]:
        return self.__find_key(self.__root, data)

    def __find_key(self, node: Node, data: T) -> Optional[T]:
        if node is None:
            return None
        i = 0
        for i in range(node.keys_count):
            if data < node.keys[i]:
                break
            if data == node.keys[i]:
                return node.keys[i]
        if node.is_leaf:
            return None
        else:
            return self.__find_key(node.children[i], data)

    @staticmethod
    def __binary_search(node: Node, key: T) -> int:
        low = 0
        high = node.keys_count - 1
        middle = int((high + low) / 2)
        while low <= high and key != node.keys[middle]:
            middle = int((high + low) / 2)
            if key < node.keys[middle]:
                high = middle - 1
            else:
                low = middle + 1

        if node.keys[middle] is not None and key > node.keys[middle]:
            return middle + 1
        return middle

    def insert(self, key: T) -> None:
        if self.__root is None:
            self.__root = Node[T](self.__t)
            self.__root.keys[0] = key
            self.__root.keys_count += 1
            return

        if self.__root.keys_count == 2 * self.__t - 1:
            new_root = Node[T](self.__t)
            self.__root_split(new_root)
            self.__insert(new_root, key)
        else:
            self.__insert(self.__root, key)

    def __insert(self, node: Node, key: T) -> None:
        index = self.__binary_search(node, key)
        if node.is_leaf:
            for i in range(node.keys_count, index, -1):
                node.keys[i] = node.keys[i - 1]
            node.keys[index] = key
            node.keys_count += 1
        else:
            tmp = node.children[index]
            if tmp.keys_count == 2 * self.__t - 1:
                self.__split(node, tmp)
                if key > node.keys[index]:
                    index += 1
            self.__insert(node.children[index], key)

    def __root_split(self, new_root: Node) -> None:
        old_root = self.__root
        self.__root = new_root
        new_root.is_leaf = False
        new_root.children[0] = old_root
        self.__split(new_root, old_root)

    def __split(self, parent: Node, current: Node) -> None:
        new_node = Node[T](self.__t)
        new_node.is_leaf = current.is_leaf
        new_node.keys_count = self.__t - 1
        for i in range(self.__t - 1):
            new_node.keys[i] = current.keys[i + self.__t]

        if not current.is_leaf:
            for i in range(self.__t):
                new_node.children[i] = current.children[i + self.__t]
        current.keys_count = self.__t - 1

        index = self.__binary_search(parent, current.keys[self.__t - 1])
        for i in range(parent.keys_count + 1, index, -1):
            parent.children[i] = parent.children[i - 1]
        parent.children[index + 1] = new_node

        for i in range(parent.keys_count, index, -1):
            parent.keys[i] = parent.keys[i - 1]
        parent.keys[index] = current.keys[self.__t - 1]
        parent.keys_count += 1

    def to_list(self) -> List[T]:
        lst = []
        if self.__root is not None:
            self.__get_node(self.__root, lst)
        return lst

    def __get_node(self, node: Node, lst: List[T]) -> None:
        if node is not None:
            i = 0
            for i in range(node.keys_count):
                if i < len(node.children) and node.children[i] is not None:
                    self.__get_node(node.children[i], lst)
                lst.append(node.keys[i])
                i += 1
            if i < len(node.children) and node.children[node.keys_count] is not None:
                self.__get_node(node.children[node.keys_count], lst)

    def print_tree(self) -> None:
        if self.__root is not None:
            self.__print_node(self.__root)

    def __print_node(self, node: Node) -> None:
        if node is not None:
            print(node)
        if not node.is_leaf:
            for i in range(node.keys_count + 1):
                self.__print_node(node.children[i])
