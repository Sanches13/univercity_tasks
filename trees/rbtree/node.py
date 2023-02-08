from __future__ import annotations
from enum import Enum
from typing import TypeVar, Generic, Optional

T = TypeVar('T')


class Node(Generic[T]):
    class Color(Enum):
        RED = 1
        BLACK = 2

    def __init__(self, data: T, color: Color) -> None:
        self.data: T = data
        self.color: Node.Color = color
        self.parent: Optional[Node] = None
        self.left_child: Optional[Node] = None
        self.right_child: Optional[Node] = None

    def __eq__(self, other: Node) -> bool:
        return self.data == other.data

    def __lt__(self, other: Node) -> bool:
        return self.data < other.data

    def __le__(self, other: Node) -> bool:
        return self.data <= other.data

    def __gt__(self, other: Node) -> bool:
        return self.data > other.data

    def __ge__(self, other: Node) -> bool:
        return self.data >= other.data

    def __ne__(self, other: Node) -> bool:
        return self.data != other.data

    def __str__(self) -> str:
        msg = "data: " + str(self.data) + "; color: " + str(self.color)
        if self.parent is None:
            msg += "; parent: null"
        else:
            msg += "; parent: " + str(self.parent.data)
        if self.left_child is None:
            msg += "; left child: null"
        else:
            msg += "; left child: " + str(self.left_child.data)
        if self.right_child is None:
            msg += "; right child: null"
        else:
            msg += "; right child: " + str(self.right_child.data)
        return msg
