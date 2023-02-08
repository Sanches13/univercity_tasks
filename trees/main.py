from rbtree.RBTree import RBTree
from btree.BTree import BTree


def test() -> None:
    print("----------------------------------------------------------")
    print("ПРОВЕРКА КОРРЕКТНОСТИ ВСТАВКИ КЛЮЧА В КРАСНО-ЧЕРНОЕ ДЕРЕВО")
    print("----------------------------------------------------------")
    test_1()
    test_3()


def test_1() -> None:
    rb_tree = RBTree[int]()
    values = [7, 11, 10, 14, 4, 19, 16, 20, 8, 15, 13, 1, 9, 18, 5, 12, 2, 17, 6, 3]
    for key in values:
        rb_tree.insert(key)
    rb_tree.print_tree()

    test_2(rb_tree)


def test_2(rb_tree: RBTree) -> None:
    values = [15, 20, 8, 14, 4]
    for key in values:
        rb_tree.remove(key)

    print("-------------------------------------------------------------")
    print("ПРОВЕРКА КОРРЕКТНОСТИ УДАЛЕНИЯ КЛЮЧА ИЗ КРАСНО-ЧЕРНОГО ДЕРЕВА")
    print("-------------------------------------------------------------")
    rb_tree.print_tree()


def test_3() -> None:
    b_tree = BTree[int](3)
    values = [7, 4, 3, 1, 30, 19, 15, 9, 25, 18,
              20, 21, 8, 28, 2, 29, 26, 22, 27, 23,
              17, 10, 13, 14, 24, 16, 11, 5, 6, 12, 31, 32]
    for key in values:
        b_tree.insert(key)

    print("----------------------------------------------")
    print("ПРОВЕРКА КОРРЕКТНОСТИ ВСТАВКИ КЛЮЧА В B-ДЕРЕВО")
    print("----------------------------------------------")
    b_tree.print_tree()


def main():
    test()


if __name__ == '__main__':
    main()
