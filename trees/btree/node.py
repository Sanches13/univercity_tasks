from __future__ import annotations
from typing import TypeVar, Generic, List, Optional

T = TypeVar('T')


class Node(Generic[T]):

    def __init__(self, t: int) -> None:
        self.keys: List[Optional[T]] = [None] * (2 * t - 1)
        self.children: List[Optional[Node]] = [None] * (2 * t)
        self.keys_count: int = 0
        self.is_leaf: bool = True

    def __str__(self) -> str:
        msg = "keys: ["
        for i in range(self.keys_count - 1):
            msg += str(self.keys[i]) + ", "
        msg += str(self.keys[self.keys_count - 1])
        msg += "]; "

        msg += f"keys_count: {self.keys_count}; "

        i = 0
        msg += "children: ["
        while i <= self.keys_count and self.children[i] is not None:
            msg += " ["
            for j in range(self.children[i].keys_count - 1):
                msg += str(self.children[i].keys[j]) + ", "
            msg += str(self.children[i].keys[self.children[i].keys_count - 1])
            msg += "] "
            i += 1
        msg += "]"
        return msg
