#include <iostream>
#include <string>

template<typename ValType>
class BST {
public:
    BST() : _root(nullptr) {}

    ~BST() { clear(); }

    BST(const BST& rhs) {
        _root = clone(rhs._root);
    }

    BST& operator=(const BST& rhs) {
        if (this != &rhs) {
            clear();
            _root = clone(rhs._root);
        }
        return *this;
    }

    size_t size() const { return size(_root); }

    void clear() { clear(_root); }

    bool contain(const ValType& x) const { return contain(x, _root); }

    void insert(const ValType& x) { insert(x, _root); }

    void remove(const ValType& x) { remove(x, _root); }

    void print(std::ostream& os = std::cout) const { inOrder(_root, os); }

private:
    struct BTnode {
        ValType _elem;
        BTnode *_left;
        BTnode *_right;

        BTnode(const ValType& elem, BTnode *left, BTnode *right)
                : _elem(elem), _left(left), _right(right) {}
    };

    BTnode *_root;

    size_t size(BTnode *pt) const {
        if (!pt)
            return 0;
        else
            return 1 + size(pt->_left) + size(pt->_right);
    }

    void clear(BTnode *& pt) {  // reference to pointer
        if (pt) {
            clear(pt->_left);
            clear(pt->_right);
            delete pt;
        }
        pt = nullptr;
    }

    BTnode *clone(BTnode *pt) const {
        if (!pt)
            return nullptr;
        else
            return new BTnode(pt->_elem, clone(pt->_left), clone(pt->_right));
    }

    BTnode *findMin(BTnode *pt) const {
        if (pt)
            while (pt->_left)
                pt = pt->_left;
        return pt;
    }

    bool contain(const ValType& x, BTnode *pt) const {
        if (!pt)
            return false;
        if (x < pt->_elem)
            return contain(x, pt->_left);
        else if (pt->_elem < x)
            return contain(x, pt->_right);
        else
            return true;
    }

    void insert(const ValType& x, BTnode *& pt) {
        if (!pt)
            pt = new BTnode(x, nullptr, nullptr);
        else if (x < pt->_elem)
            insert(x, pt->_left);
        else if (pt->_elem < x)
            insert(x, pt->_right);
        else;  // duplicate
    }

    void remove(const ValType& x, BTnode *& pt) {
        if (!pt) return;
        if (x < pt->_elem)
            remove(x, pt->_left);
        else if (pt->_elem < x)
            remove(x, pt->_right);
        else if (pt->_left && pt->_right) {
            pt->_elem = findMin(pt->_right)->_elem;
            remove(pt->_elem, pt->_right);
        } else {
            BTnode *tmp = pt;
            pt = pt->_left ? pt->_left : pt->_right;
            delete tmp;
        }
    }

    void inOrder(BTnode *pt, std::ostream& os) const {
        if (pt) {
            inOrder(pt->_left, os);
            os << pt->_elem << '\t';
            inOrder(pt->_right, os);
        }
    }
};

int main() {
    std::string pi = "3.141592653589793";

    BST<char> bt;
    for (auto c: pi)
        bt.insert(c);
    bt.print();
    bt.remove('.');
    std::cout << std::boolalpha << bt.contain('0') << std::endl;

    auto cp = bt;
    std::cout << "number of nodes: " << cp.size() << std::endl;

    return 0;
}