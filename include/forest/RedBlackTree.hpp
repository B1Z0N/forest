#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <queue>

namespace forest {
template <typename T> class RedBlackTree {
public:
  using Callback = std::function<void(T &)>;

private:
  class RedBlackTreeNode {
    friend class RedBlackTree;

  private:
    RedBlackTreeNode *mLeft{NIL};
    RedBlackTreeNode *mRight{NIL};
    RedBlackTreeNode *mParent{NIL};

  private:
    enum Color { RED, BLACK, DOUBLE_BLACK }; 
    Color mColor{BLACK}; 

  public:
    T mKey;

  public:
    RedBlackTreeNode(Color color = RED) :mColor{color} {}

    RedBlackTreeNode(const T &key, Color color = RED) : mKey(key), mColor{color} {}

    RedBlackTreeNode(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode(RedBlackTreeNode &&) = delete;
    RedBlackTreeNode &operator=(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode &operator=(RedBlackTreeNode &&) = delete;
    ~RedBlackTreeNode() = default;
  
  public:
    inline bool red() const { return mColor == RED; }
    inline bool black() const { return mColor == BLACK; }
    inline bool double_black() const { return mColor == DOUBLE_BLACK; }
    inline void set_red() { mColor = RED; }
    inline void set_black() { mColor = BLACK; }
    inline void set_double_black() { mColor = DOUBLE_BLACK; }
  };

  static RedBlackTreeNode NIL_VAL;
  static RedBlackTreeNode* NIL;

private:
  RedBlackTreeNode *mroot{NIL};

private:
  void pre_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (root == NIL)
      return;
    callback(root->mKey);
    pre_order_traversal(root->mLeft, callback);
    pre_order_traversal(root->mRight, callback);
  }

  void in_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (root == NIL)
      return;
    in_order_traversal(root->mLeft, callback);
    callback(root->mKey);
    in_order_traversal(root->mRight, callback);
  }

  void post_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (root == NIL)
      return;
    post_order_traversal(root->mLeft, callback);
    post_order_traversal(root->mRight, callback);
    callback(root->mKey);
  }

  void breadth_first_traversal(RedBlackTreeNode *root, Callback callback) {
    if (root == NIL)
      return;
    std::queue<RedBlackTreeNode *> queue;
    queue.push(root);
    while (!queue.empty()) {
      RedBlackTreeNode *current{queue.front()};
      callback(current->mKey);
      queue.pop();
      if (current->mLeft != NIL)
        queue.push(current->mLeft);
      if (current->mRight != NIL)
        queue.push(current->mRight);
    }
  }

private:
  RedBlackTreeNode *minimum(RedBlackTreeNode *root) {
    if (root == NIL)
      return NIL;
    while (root->mLeft != NIL)
      root = root->mLeft;
    return root;
  }

  RedBlackTreeNode *maximum(RedBlackTreeNode *root) {
    if (root == NIL) 
      return NIL;
    while (root->mRight != NIL)
      root = root->mRight;
    return root;
  }

private:
  auto height(const RedBlackTreeNode *root) {
    if (root == NIL) return 0;
    return std::max(height(root->mLeft), height(root->mRight)) + 1;
  }

  auto size(const RedBlackTreeNode *root) {
    if (root == NIL)
      return 0;
    return size(root->mLeft) + size(root->mRight) + 1;
  }

private:
  void rotate_left(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *right = pt->mRight;
    pt->mRight = right->mLeft;
    if (pt->mRight != NIL) pt->mRight->mParent = pt;
    right->mParent = pt->mParent;

    if (pt->mParent == NIL) root = right;
    else if (pt == pt->mParent->mLeft) pt->mParent->mLeft = right;
    else pt->mParent->mRight = right;

    right->mLeft = pt;
    pt->mParent = right;
  }

  void rotate_right(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *left = pt->mLeft;
    pt->mLeft = left->mRight;
    if (pt->mLeft != NIL) pt->mLeft->mParent = pt;
    left->mParent = pt->mParent;

    if (pt->mParent == NIL) root = left;
    else if (pt == pt->mParent->mLeft) pt->mParent->mLeft = left;
    else pt->mParent->mRight = left;

    left->mRight = pt;
    pt->mParent = left;
  }

  void transplant(RedBlackTreeNode*& root, RedBlackTreeNode *to, RedBlackTreeNode *from) {
    if (to->mParent == NIL) root = from;
    else if (to == to->mParent->mLeft) to->mParent->mLeft = from;
    else to->mParent->mRight = from;

    from->mParent = to->mParent;
  }

private:
  RedBlackTreeNode *insert(RedBlackTreeNode *root, const T &key) {
    RedBlackTreeNode *parent = NIL, *child = root, *nd = new RedBlackTreeNode{key};

    while (child != NIL) {
      parent = child;
      if (nd->mKey < child->mKey) child = child->mLeft;
      else child = child->mRight;
    }

    if (parent == NIL) { nd->set_black(); return nd; }
    else if (nd->mKey < parent->mKey) parent->mLeft = nd;
    else parent->mRight = nd;
    nd->mParent = parent;

    insert_fix(root, nd);
    return root;
  }

  void insert_fix(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *parent = NIL, *gparent = NIL;
    while ((pt != root) && (pt->red()) && (pt->mParent->red())) {
      parent = pt->mParent;
      gparent = pt->mParent->mParent;
      if (parent == gparent->mLeft) {
        RedBlackTreeNode *uncle = gparent->mRight;
        if (uncle != NIL && uncle->red()) {
          gparent->set_red();
          parent->set_black();
          uncle->set_black();
          pt = gparent;
        } else {
          if (pt == parent->mRight) {
            rotate_left(root, parent);
            pt = parent;
            parent = pt->mParent;
          }
          rotate_right(root, gparent);
          std::swap(parent->mColor, gparent->mColor);
          pt = parent;
        }
      } else {
        RedBlackTreeNode *uncle = gparent->mLeft;
        if ((uncle != NIL) && (uncle->red())) {
          gparent->set_red();
          parent->set_black();
          uncle->set_black();
          pt = gparent;
        } else {
          if (pt == parent->mLeft) {
            rotate_right(root, parent);
            pt = parent;
            parent = pt->mParent;
          }
          rotate_left(root, gparent);
          std::swap(parent->mColor, gparent->mColor);
          pt = parent;
        }
      }
    }

    root->set_black();
  }

private:
  template<typename U> RedBlackTreeNode *remove(RedBlackTreeNode *&root, const U& key) {
    if (root == NIL) return root;

    if (key < root->mKey) return remove(root->mLeft, key);
    else if (!(key == root->mKey)) return remove(root->mRight, key);

    if (root->mLeft == NIL || root->mRight == NIL) return root;

    auto *temp = minimum(root->mRight);
    root->mKey = temp->mKey;
    return remove(root->mRight, temp->mKey);
  }

  void remove_fix(RedBlackTreeNode *&root, RedBlackTreeNode *&node) {
    if (node == NIL) return;

    if (node == root) {
      root = NIL;
      return;
    }

    if (node->red() || node->mLeft->red() || node->mRight->red()) {
      RedBlackTreeNode *child = node->mLeft != NIL ? node->mLeft : node->mRight;

      if (node == node->mParent->mLeft) {
        node->mParent->mLeft = child;
        if (child != NIL) child->mParent = node->mParent;
        child->set_black();
        delete node;
      } else {
        node->mParent->mRight = child;
        if (child != NIL) child->mParent = node->mParent;
        child->set_black();
        delete node;
      }
    } else {
      RedBlackTreeNode *sibling = NIL, *parent = NIL, *ptr = node;
      ptr->set_double_black();
      while (ptr != root && ptr->double_black()) {
        parent = ptr->mParent;
        if (ptr == parent->mLeft) {
          sibling = parent->mRight;
          if (sibling->red()) {
            sibling->set_black();
            parent->set_red();
            rotate_left(root, parent);
          } else {
            if (sibling->mLeft->black() && sibling->mRight->black()) {
              sibling->set_red();
              if (parent->red()) parent->set_black();
              else parent->set_double_black();
              ptr = parent;
            } else {
              if (sibling->mRight->black()) {
                sibling->mLeft->set_black();
                sibling->set_red();
                rotate_right(root, sibling);
                sibling = parent->mRight;
              }
              sibling->mColor = parent->mColor;
              parent->set_black();
              sibling->mRight->set_black();
              rotate_left(root, parent);
              break;
            }
          }
        } else {
          sibling = parent->mLeft;
          if (sibling->red()) {
            sibling->set_black();
            parent->set_red();
            rotate_right(root, parent);
          } else {
            if (sibling->mLeft->black() && sibling->mRight->black()) {
              sibling->set_red();
              if (parent->red()) parent->set_black();
              else parent->set_double_black();
              ptr = parent;
            } else {
              if (sibling->mLeft->black()) {
                sibling->mRight->set_black();
                sibling->set_red();
                rotate_left(root, sibling);
                sibling = parent->mLeft;
              }
              sibling->mColor = parent->mColor;
              parent->set_black();
              sibling->mLeft->set_black();
              rotate_right(root, parent);
              break;
            }
          }
        }
      }
      
      if (node == node->mParent->mLeft) node->mParent->mLeft = NIL;
      else node->mParent->mRight = NIL;
      delete node;
      root->set_black();
    }
  }

private:
  template <typename U> RedBlackTreeNode * search(RedBlackTreeNode *root, const U &key)
  {
    while (root != NIL)
    {
      if (root->mKey < key)
      {
        root = root->mRight;
      }
      else if (key < root->mKey)
      {
        root = root->mLeft;
      }
      else
      {
        return root;
      }
    }
    return NIL;
  }

private:
  void clear(RedBlackTreeNode *root)
  {
    if (root == NIL)
      return;
    if (root->mLeft != NIL)
      clear(root->mLeft);
    if (root->mRight != NIL)
      clear(root->mRight);
    delete root;
    root = NIL;
  }

public:
  RedBlackTree() = default;

  RedBlackTree(const RedBlackTree &) = delete;
  RedBlackTree(RedBlackTree &&) = delete;
  RedBlackTree &operator=(const RedBlackTree &) = delete;
  RedBlackTree &operator=(RedBlackTree &&) = delete;

  ~RedBlackTree() { clear(); }

public:
  void pre_order_traversal(Callback callback) {
    pre_order_traversal(mroot, callback);
  }

  void in_order_traversal(Callback callback) {
    in_order_traversal(mroot, callback);
  }

  void post_order_traversal(Callback callback) {
    post_order_traversal(mroot, callback);
  }

  void breadth_first_traversal(Callback callback) {
    breadth_first_traversal(mroot, callback);
  }

public:
  auto minimum() {
    auto min = minimum(mroot);
    return min != NIL ? std::optional<std::reference_wrapper<T>>{min->mKey}
               : std::nullopt;
  }

  auto maximum() {
    auto max = maximum(mroot);
    return max != NIL ? std::optional<std::reference_wrapper<T>>{max->mKey}
               : std::nullopt;
  }

public:
  auto height() { return height(mroot); }

  auto size() { return size(mroot); }

public:
  void insert(const T &key) { mroot = insert(mroot, key); }

  template <typename U> void remove(const U &key) {
    auto *nd = remove(mroot, key);
    remove_fix(mroot, nd);
  }

  template <typename U> auto search(const U &key) {
    auto res = search(mroot, key);
    return res != NIL ? std::optional<std::reference_wrapper<T>>{res->mKey}
               : std::nullopt;
  }

public:
  void clear() {
    clear(mroot);
    mroot = NIL;
  }
};

template <typename T>
typename RedBlackTree<T>::RedBlackTreeNode RedBlackTree<T>::NIL_VAL = \
  RedBlackTree<T>::RedBlackTreeNode{RedBlackTree<T>::RedBlackTreeNode::BLACK};
template <typename T>
typename RedBlackTree<T>::RedBlackTreeNode *RedBlackTree<T>::NIL = &RedBlackTree<T>::NIL_VAL;

} // namespace forest
