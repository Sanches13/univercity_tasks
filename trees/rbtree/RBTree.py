from __future__ import annotations
from rbtree.node import *
from typing import List


class RBTree(Generic[T]):

    def __init__(self, other=None) -> None:
        self.__root: Optional[Node] = None
        if other is not None:
            if type(other) is not type(self):
                raise TypeError(f"TypeError: must be {type(self)}, not {type(other)}")
            else:
                self.__root = Node[T](other.__root.data, other.__root.color)
                self.__copy_node(self.__root, other.__root)

    def __copy_node(self, node: Node, other_node: Node) -> None:
        if other_node.left_child is not None:
            node.left_child = Node[T](other_node.left_child.data, other_node.left_child.color)
            node.left_child.parent = node
            self.__copy_node(node.left_child, other_node.left_child)
        if other_node.right_child is not None:
            node.right_child = Node[T](other_node.right_child.data, other_node.right_child.color)
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

    def find(self, data: T) -> Optional[T]:
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
            self.__root = Node[T](data, Node.Color.BLACK)
        else:
            self.__insert_node(Node[T](data, Node.Color.RED), self.__root)

    def __insert_node(self, insertion_node: Node, current: Node) -> None:
        while True:
            if insertion_node < current:
                if current.left_child is None:
                    insertion_node.parent = current
                    current.left_child = insertion_node
                    self.__balance(current.left_child)
                    break
                else:
                    current = current.left_child
                    continue

            if insertion_node > current:
                if current.right_child is None:
                    insertion_node.parent = current
                    current.right_child = insertion_node
                    self.__balance(current.right_child)
                    break
                else:
                    current = current.right_child
            else:
                return

    def __balance(self, node: Node) -> None:
        if node is self.__root:
            node.color = Node.Color.BLACK
            return

        parent = node.parent
        if parent.color == Node.Color.BLACK:
            return

        grandparent = self.__grandfather(node)
        if grandparent is None:
            parent.color = Node.Color.BLACK
            return

        uncle = self.__uncle(node)
        if uncle is not None and uncle.color == Node.Color.RED:
            parent.color = Node.Color.BLACK
            grandparent.color = Node.Color.RED
            uncle.color = Node.Color.BLACK
            self.__balance(grandparent)
        elif parent is grandparent.left_child:
            if node is parent.right_child:
                self.__left_rotate(parent)
                parent = node
            self.__right_rotate(grandparent)
            parent.color = Node.Color.BLACK
            grandparent.color = Node.Color.RED
        else:
            if node is parent.left_child:
                self.__right_rotate(parent)
                parent = node
            self.__left_rotate(grandparent)
            parent.color = Node.Color.BLACK
            grandparent.color = Node.Color.RED

    @staticmethod
    def __grandfather(node: Node) -> Optional[Node]:
        if node.parent is None:
            return None
        return node.parent.parent

    def __uncle(self, node: Node) -> Optional[Node]:
        grandparent = self.__grandfather(node)
        if grandparent.left_child is node.parent:
            return grandparent.right_child
        elif grandparent.right_child is node.parent:
            return grandparent.left_child
        else:
            return None

    def __left_rotate(self, node: Node) -> None:
        parent = node.parent
        right_child = node.right_child
        node.right_child = right_child.left_child
        if right_child.left_child is not None:
            right_child.left_child.parent = node
        right_child.left_child = node
        node.parent = right_child

        if parent is None:
            self.__root = right_child
        elif parent.left_child is node:
            parent.left_child = right_child
        elif parent.right_child is node:
            parent.right_child = right_child
        else:
            return

        if right_child is not None:
            right_child.parent = parent

    def __right_rotate(self, node: Node) -> None:
        parent = node.parent
        left_child = node.left_child
        node.left_child = left_child.right_child
        if left_child.right_child is not None:
            left_child.right_child.parent = node
        left_child.right_child = node
        node.parent = left_child

        if parent is None:
            self.__root = left_child
        elif parent.left_child is node:
            parent.left_child = left_child
        elif parent.right_child is node:
            parent.right_child = left_child
        else:
            return

        if left_child is not None:
            left_child.parent = parent

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
            if current.color == Node.Color.BLACK:
                self.__balance_remove(current)
            self.__remove_node(current)

    @staticmethod
    def __remove_node(node: Node) -> None:
        parent = node.parent
        if node is parent.left_child:
            parent.left_child = None
        else:
            parent.right_child = None

    def __balance_remove(self, current: Node) -> None:
        if current is self.__root:
            self.__first_case()
            return

        brother = self.__brother(current)
        if brother.color == Node.Color.RED:
            self.__second_case(current)
        else:
            self.__third_case(current)

    def __first_case(self) -> None:
        self.__root = None

    def __second_case(self, current: Node) -> None:
        parent = current.parent
        brother = self.__brother(current)

        brother.color = Node.Color.BLACK
        parent.color = Node.Color.RED
        if brother is parent.left_child:
            self.__right_rotate(parent)
        else:
            self.__left_rotate(parent)
        self.__fourth_case(current)

    def __third_case(self, current: Node) -> None:
        parent = current.parent
        brother = self.__brother(current)

        if parent.color == Node.Color.BLACK and brother.color == Node.Color.BLACK and \
                (brother.left_child is None or brother.left_child.color == Node.Color.BLACK) and \
                (brother.right_child is None or brother.right_child.color == Node.Color.BLACK):
            brother.color = Node.Color.RED
            self.__balance_remove(parent)
        else:
            self.__fourth_case(current)

    def __fourth_case(self, current: Node) -> None:
        parent = current.parent
        brother = self.__brother(current)

        if brother.color == Node.Color.BLACK and \
                (brother.left_child is None or brother.left_child.color == Node.Color.BLACK) and \
                (brother.right_child is None or brother.right_child.color == Node.Color.BLACK) and \
                parent.color == Node.Color.RED:
            brother.color = Node.Color.RED
            parent.color = Node.Color.BLACK
        else:
            self.__fifth_case(current)

    def __fifth_case(self, current: Node) -> None:
        brother = self.__brother(current)
        internal = self.__internal(brother)
        external = self.__external(brother)

        if brother.color == Node.Color.BLACK and \
                (external is None or external.color == Node.Color.BLACK) and \
                (internal is None or internal.color == Node.Color.RED):
            if internal is brother.left_child:
                self.__right_rotate(brother)
            else:
                self.__left_rotate(brother)
            internal.color = Node.Color.BLACK
            brother.color = Node.Color.RED
        self.__sixth_case(current)

    def __sixth_case(self, current: Node) -> None:
        parent = current.parent
        brother = self.__brother(current)
        external = self.__external(brother)

        if brother.color == Node.Color.BLACK and external.color is Node.Color.RED:
            if brother is parent.left_child:
                self.__right_rotate(parent)
            else:
                self.__left_rotate(parent)
            brother.color = parent.color
            parent.color = Node.Color.BLACK
            external.color = Node.Color.BLACK

    @staticmethod
    def __brother(node: Node) -> Optional[Node]:
        return node.parent.left_child if node.parent.right_child.data == node.data else node.parent.right_child

    @staticmethod
    def __internal(node: Node) -> Optional[Node]:
        return node.right_child if node is node.parent.left_child else node.left_child

    @staticmethod
    def __external(node: Node) -> Optional[Node]:
        return node.left_child if node is node.parent.left_child else node.right_child

    @staticmethod
    def __nearest(current: Node) -> Node:
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
