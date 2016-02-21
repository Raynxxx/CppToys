/*
** Tree.cpp
** Created by Rayn on 2016/02/20
** red black tree implemention
*/

#include "Tree.h"

namespace rayn {

    static __rb_tree_node_base*
    local_rb_tree_increment(__rb_tree_node_base* node) throw () {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0) {
                node = node->left;
            }
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->right) {
                node = pa;
                pa = pa->parent;
            }
            if (node->right != pa) {
                node = pa;
            }
        }
        return node;
    }

    __rb_tree_node_base*
    _rb_tree_increment(__rb_tree_node_base* node) throw ()
    {
        return local_rb_tree_increment(node);
    }

    const __rb_tree_node_base*
    _rb_tree_increment(const __rb_tree_node_base* node) throw ()
    {
        return local_rb_tree_increment(const_cast<__rb_tree_node_base*>(node));
    }

    static __rb_tree_node_base*
    local_rb_tree_decrement(__rb_tree_node_base* node) throw () {
        if (node->color == _s_red &&
            node == node->parent->parent) {
            node = node->right;
        } else if (node->left != 0) {
            __rb_tree_node_base* temp = node->left;
            while (temp->right != 0) {
                temp = temp->right;
            }
            node = temp;
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->left) {
                node = pa;
                pa = pa->parent;
            }
            node = pa;
        }
        return node;
    }

    __rb_tree_node_base*
    _rb_tree_decrement(__rb_tree_node_base* node) throw () {
        return local_rb_tree_decrement(node);
    }

    const __rb_tree_node_base*
    _rb_tree_decrement(const __rb_tree_node_base* node) throw () {
        return local_rb_tree_decrement(const_cast<__rb_tree_node_base*>(node));
    }

    static void
    local_rb_tree_rotate_left(__rb_tree_node_base* const x,
                              __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* const y = x->right;
        x->right = y->left;
        if (y->left != 0) {
            y->left->parent = x;
        }
        y->parent = x->parent;

        if (x == root) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    static void
    local_rb_tree_rotate_right(__rb_tree_node_base* const x,
                               __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* const y = x->left;
        x->left = y->right;
        if (y->right != 0) {
            y->right->parent = x;
        }
        y->parent = x->parent;

        if (x == root) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void
    _rb_tree_insert_and_rebalance(const bool insert_left,
                                  __rb_tree_node_base* x,
                                  __rb_tree_node_base* pa,
                                  __rb_tree_node_base& header)
    {
        __rb_tree_node_base*& root = header.parent;

        // initialize fields in new node to insert
        x->parent = pa;
        x->left = 0;
        x->right = 0;
        x->color = _s_red;

        // insert
        // Make new node child of parent and 
        // maintain root, leftmost and rightmost nodes.
        // First node is always inserted left.
        if (insert_left) {
            // also makes leftmost = x when pa == &header
            pa->left = x;
            if (pa == &header) {
                header.parent = x;
                header.right = x;
            } else if (pa == header.left) {
                // maintain leftmost pointing to min node
                header.left = x;
            }
        } else {
            pa->right = x;
            if (pa == header.right) {
                // maintain rightmost pointing to max node
                header.right = x;
            }
        }

        // Rebalance
        while (x != root && x->parent->color == _s_red) {
            __rb_tree_node_base* const xpp = x->parent->parent;
            if (x->parent == xpp->left) {
                __rb_tree_node_base* const uncle = xpp->right;
                if (uncle && uncle->color == _s_red) {
                    x->parent->color = _s_black;
                    uncle->color = _s_black;
                    xpp->color = _s_red;
                    x = xpp;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        local_rb_tree_rotate_left(x, root);
                    }
                    x->parent->color = _s_black;
                    xpp->color = _s_red;
                    local_rb_tree_rotate_right(xpp, root);
                }
            } else {
                __rb_tree_node_base* const uncle = xpp->left;
                if (uncle && uncle->color == _s_red) {
                    x->parent->color = _s_black;
                    uncle->color = _s_black;
                    xpp->color = _s_red;
                    x = xpp;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        local_rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = _s_black;
                    xpp->color = _s_red;
                    local_rb_tree_rotate_left(xpp, root);
                }
            }
        }
        root->color = _s_black;
    }


    __rb_tree_node_base*
    _rb_tree_rebalance_for_erase(__rb_tree_node_base* const z,
                                 __rb_tree_node_base& header)
    {
        __rb_tree_node_base*& root = header.parent;
        __rb_tree_node_base*& leftmost = header.left;
        __rb_tree_node_base*& rightmost = header.right;
        __rb_tree_node_base* y = z;
        __rb_tree_node_base* x = 0;
        __rb_tree_node_base* x_parent = 0;

        
        if (z->left == 0) { // z has at most one non-null child.
            x = z->right;   // x might be null.
        } else {
            if (z->right == 0) {    // z has exactly one non-null child.
                x = z->left;        // x is not null.
            } else {
                // z has two non-null children.
                // Set y to z's successor.  __x might be null.
                y = z->right;
                while (y->left != 0) {
                    y = y->left;
                }
                x = y->right;
            }
        }
        if (y != z) {
            // relink y in place of z.  y is z's successor
            z->left->parent = y;
            y->left = z->left;
            if (y != z->right) {
                x_parent = y->parent;
                if (x) {
                    x->parent = y->parent;
                }
                // y must be a left child
                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            } else {
                x_parent = y;
            }
            if (root == z) {
                root = y;
            } else if (z->parent->left == z) {
                z->parent->left = y;
            } else {
                z->parent->right = y;
            }
            y->parent = z->parent;
            rayn::swap(y->color, z->color);
            y = z;
            // y now points to node to be actually deleted
        } else {
            // y == z
            x_parent = y->parent;
            if (x) {
                x->parent = y->parent;
            }
            if (z == root) {
                root = x;
            } else if (z->parent->left == z) {
                z->parent->left = x;
            } else {
                z->parent->right = x;
            }
            if (leftmost == z) {
                // z->left must be null also
                if (z->right == 0) {
                    leftmost = z->parent;
                    // makes leftmost == header if z == root
                } else {
                    leftmost = __rb_tree_node_base::minimum(x);
                }
            }
            if (rightmost == z) {
                // z->right must be null also
                if (z->left == 0) {
                    rightmost = z->parent;
                    // makes rightmost == header if z == root
                } else {
                    rightmost = __rb_tree_node_base::maximum(x);
                }
            }
        }
        if (y->color != _s_red) {
            while (x != root && (x == 0 || x->color == _s_black)) {
                if (x == x_parent->left) {
                    __rb_tree_node_base* w = x_parent->right;
                    if (w->color == _s_red) {
                        w->color = _s_black;
                        x_parent->color = _s_red;
                        local_rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }
                    if ((w->left == 0 || w->left->color == _s_black)
                        && (w->right == 0 || w->right->color == _s_black)) {
                        w->color = _s_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else {
                        if (w->right == 0 || w->right->color == _s_black) {
                            w->left->color = _s_black;
                            w->color = _s_red;
                            local_rb_tree_rotate_right(w, root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;
                        x_parent->color = _s_black;
                        if (w->right) {
                            w->right->color = _s_black;
                        }
                        local_rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                } else {
                    __rb_tree_node_base* w = x_parent->left;
                    if (w->color == _s_red) {
                        w->color = _s_black;
                        x_parent->color = _s_red;
                        local_rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }
                    if ((w->right == 0 || w->right->color == _s_black)
                        && (w->left == 0 || w->left->color == _s_black)) {
                        w->color = _s_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else {
                        if (w->left == 0 || w->left->color == _s_black) {
                            w->right->color = _s_black;
                            w->color = _s_red;
                            local_rb_tree_rotate_left(w, root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = _s_black;
                        if (w->left) {
                            w->left->color = _s_black;
                        }
                        local_rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }
            }
            if (x) {
                x->color = _s_black;
            }
        }
        return y;
    }
}