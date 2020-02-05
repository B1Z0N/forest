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
    // 0(false) means red, 1(true) means black
    bool mColor{false}; 

  private:
    int height{1};


  public:
    T mKey;

  public:
    RedBlackTreeNode() = default;

    RedBlackTreeNode(const T &key) : mKey(key) {}

    RedBlackTreeNode(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode(RedBlackTreeNode &&) = delete;
    RedBlackTreeNode &operator=(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode &operator=(RedBlackTreeNode &&) = delete;
    ~RedBlackTreeNode() = default;
  };

  static RedBlackTreeNode NIL_VAL;
  static RedBlackTreeNode* NIL;

private:
  RedBlackTreeNode *mRoot{NIL};

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
    if (root == NIL)
      return 0;
    return root->height;
  }

  auto size(const RedBlackTreeNode *root) {
    if (root == NIL)
      return 0;
    return size(root->mLeft) + size(root->mRight) + 1;
  }

private:
  void rotate_left(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *pt_right = pt->mRight;

    pt->mRight = pt_right->mLeft;

    if (pt->mRight != NIL) {
      pt->mRight->mParent = pt;
    }
    
    pt_right->mParent = pt->mParent;

    if (pt->mParent == NIL) {
      root = pt_right;
    } else if (pt == pt->mParent->mLeft) {
      pt->mParent->mLeft = pt_right;
    } else {
      pt->mParent->mRight = pt_right;
    }
    pt_right->mLeft = pt;
    pt->mParent = pt_right;
  }

  void rotate_right(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *pt_left = pt->mLeft;

    pt->mLeft = pt_left->mRight;

    if (pt->mLeft != NIL) {
      pt->mLeft->mParent = pt;
    }

    pt_left->mParent = pt->mParent;

    if (pt->mParent == NIL) {
      root = pt_left;
    } else if (pt == pt->mParent->mLeft) {
      pt->mParent->mLeft = pt_left;
    } else {
      pt->mParent->mRight = pt_left;
    }
    pt_left->mRight = pt;
    pt->mParent = pt_left;
  }

	void ll_rotate(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
		RedBlackTreeNode *pt_right = pt->mRight;

		pt->mRight = pt_right->mLeft;

		if (pt->mRight != NIL) {
			pt->mRight->mParent = pt;
    }

		pt_right->mParent = pt->mParent;

		if (pt->mParent == NIL) {
			root = pt_right;
    } else if (pt == pt->mParent->mLeft) {
			pt->mParent->mLeft = pt_right;
    } else {
			pt->mParent->mRight = pt_right;
    }

		pt_right->mLeft = pt;
		pt->mParent = pt_right;
	}

  void rr_rotate(RedBlackTreeNode*& root, RedBlackTreeNode*& pt) {
		RedBlackTreeNode *pt_left = pt->mLeft;

		pt->mLeft = pt_left->mRight;

		if (pt->mLeft != NIL) {
			pt->mLeft->mParent = pt;
    }

		pt_left->mParent = pt->mParent;

		if (pt->mParent == NIL) {
			root = pt_left;
    } else if (pt == pt->mParent->mLeft) {
			pt->mParent->mLeft = pt_left;
    } else {
			pt->mParent->mRight = pt_left;
    }

		pt_left->mRight = pt;
		pt->mParent = pt_left;
	}

  void transplant(RedBlackTreeNode*& root, RedBlackTreeNode *to, RedBlackTreeNode *from) {
    if (to->mParent == NIL) {
      root = from;
    } else if (to == to->mParent->mLeft) {
      to->mParent->mLeft = from;
    } else {
      to->mParent->mRight = from;
    }

    from->mParent = to->mParent;
  }

private:
  RedBlackTreeNode *insert(RedBlackTreeNode *root, const T &key) {
    if (root == NIL) {
      return new RedBlackTreeNode{key};
    }
    RedBlackTreeNode *current = root, *previous;
    while (current != NIL) {
      previous = current;
      current = current->mKey > key ? current->mLeft : current->mRight;
    }

    current = new RedBlackTreeNode{key};
    if (previous->mKey > key) {
      previous->mLeft = current;
    } else {
      previous->mRight = current;
    }
    current->mParent = previous;

    insert_fix(root, current);
    return root;
  }

  void insert_fix(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *parent = NIL;
    RedBlackTreeNode *grand_parent = NIL;

    while ((pt != root) && (pt->mColor != true) && (pt->mParent->mColor == false)) {
      parent = pt->mParent;
      grand_parent = pt->mParent->mParent;
      if (parent == grand_parent->mLeft) {
        RedBlackTreeNode *uncle = grand_parent->mRight;
        if (uncle != NIL && uncle->mColor == false) {
          grand_parent->mColor = false;
          parent->mColor = true;
          uncle->mColor = true;
          pt = grand_parent;
        } else {
          if (pt == parent->mRight) {
            rotate_left(root, parent);
            pt = parent;
            parent = pt->mParent;
          }
          rotate_right(root, grand_parent);
          std::swap(parent->mColor, grand_parent->mColor);
          pt = parent;
        }
      } else {
        RedBlackTreeNode *uncle = grand_parent->mLeft;
        if ((uncle != NIL) && (uncle->mColor == false)) {
          grand_parent->mColor = false;
          parent->mColor = true;
          uncle->mColor = true;
          pt = grand_parent;
        } else {
          if (pt == parent->mLeft) {
            rotate_right(root, parent);
            pt = parent;
            parent = pt->mParent;
          }
          rotate_left(root, grand_parent);
          std::swap(parent->mColor, grand_parent->mColor);
          pt = parent;
        }
      }
    }

    root->mColor = true;
  }
private:
  // remove 

private:
  template <typename U> RedBlackTreeNode *search(RedBlackTreeNode *root, const U &key) {
    while (root != NIL) {
      if (root->mKey < key) {
        root = root->mRight;
      } else if (key < root->mKey) {
        root = root->mLeft;
      } else {
        return root;
      }
    }
    return NIL;
  }

private:
  void clear(RedBlackTreeNode *root) {
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
    pre_order_traversal(mRoot, callback);
  }

  void in_order_traversal(Callback callback) {
    in_order_traversal(mRoot, callback);
  }

  void post_order_traversal(Callback callback) {
    post_order_traversal(mRoot, callback);
  }

  void breadth_first_traversal(Callback callback) {
    breadth_first_traversal(mRoot, callback);
  }

public:
  auto minimum() {
    auto min = minimum(mRoot);
    return min != NIL ? std::optional<std::reference_wrapper<T>>{min->mKey}
               : std::nullopt;
  }

  auto maximum() {
    auto max = maximum(mRoot);
    return max != NIL ? std::optional<std::reference_wrapper<T>>{max->mKey}
               : std::nullopt;
  }

public:
  auto height() { return height(mRoot); }

  auto size() { return size(mRoot); }

public:
  void insert(const T &key) { mRoot = insert(mRoot, key); }

  template <typename U> void remove(const U &key) {
    mRoot = remove(mRoot, key);
  }

  template <typename U> auto search(const U &key) {
    auto res = search(mRoot, key);
    return res != NIL ? std::optional<std::reference_wrapper<T>>{res->mKey}
               : std::nullopt;
  }

public:
  void clear() {
    clear(mRoot);
    mRoot = NIL;
  }
};

template <typename T>
typename RedBlackTree<T>::RedBlackTreeNode RedBlackTree<T>::NIL_VAL = RedBlackTree<T>::RedBlackTreeNode{};
template <typename T>
typename RedBlackTree<T>::RedBlackTreeNode* RedBlackTree<T>::NIL = &RedBlackTree<T>::NIL_VAL;

} // namespace forest
