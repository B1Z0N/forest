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
    RedBlackTreeNode *mLeft{nullptr};
    RedBlackTreeNode *mRight{nullptr};
    RedBlackTreeNode *mParent{nullptr};
    // 0(false) means red, 1(true) means black
    bool mColor{false}; 

  private:
    int height{1};


  public:
    T mKey;

  public:
    static RedBlackTreeNode* NIL;

  public:
    RedBlackTreeNode() = default;

    RedBlackTreeNode(const T &key) : mKey(key) {}

    RedBlackTreeNode(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode(RedBlackTreeNode &&) = delete;
    RedBlackTreeNode &operator=(const RedBlackTreeNode &) = delete;
    RedBlackTreeNode &operator=(RedBlackTreeNode &&) = delete;
    ~RedBlackTreeNode() = default;
  };

private:
  RedBlackTreeNode *mRoot{nullptr};

private:
  void pre_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (!root)
      return;
    callback(root->mKey);
    pre_order_traversal(root->mLeft, callback);
    pre_order_traversal(root->mRight, callback);
  }

  void in_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (!root)
      return;
    in_order_traversal(root->mLeft, callback);
    callback(root->mKey);
    in_order_traversal(root->mRight, callback);
  }

  void post_order_traversal(RedBlackTreeNode *root, Callback callback) {
    if (!root)
      return;
    post_order_traversal(root->mLeft, callback);
    post_order_traversal(root->mRight, callback);
    callback(root->mKey);
  }

  void breadth_first_traversal(RedBlackTreeNode *root, Callback callback) {
    if (!root)
      return;
    std::queue<RedBlackTreeNode *> queue;
    queue.push(root);
    while (!queue.empty()) {
      RedBlackTreeNode *current{queue.front()};
      callback(current->mKey);
      queue.pop();
      if (current->mLeft)
        queue.push(current->mLeft);
      if (current->mRight)
        queue.push(current->mRight);
    }
  }

private:
  RedBlackTreeNode *minimum(RedBlackTreeNode *root) {
    if (!root)
      return nullptr;
    while (root->mLeft)
      root = root->mLeft;
    return root;
  }

  RedBlackTreeNode *maximum(RedBlackTreeNode *root) {
    if (!root)
      return nullptr;
    while (root->mRight)
      root = root->mRight;
    return root;
  }

private:
  auto height(const RedBlackTreeNode *root) {
    if (!root)
      return 0;
    return root->height;
  }

  auto size(const RedBlackTreeNode *root) {
    if (!root)
      return 0;
    return size(root->mLeft) + size(root->mRight) + 1;
  }

private:
  void rotate_left(RedBlackTreeNode *&root, RedBlackTreeNode *&pt) {
    RedBlackTreeNode *pt_right = pt->mRight;

    pt->mRight = pt_right->mLeft;

    if (pt->mRight != RedBlackTreeNode::NIL) {
      pt->mRight->mParent = pt;
    }
    
    pt_right->mParent = pt->mParent;

    if (pt->mParent == RedBlackTreeNode::NIL) {
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

    if (pt->mLeft != RedBlackTreeNode::NIL) {
      pt->mLeft->mParent = pt;
    }

    pt_left->mParent = pt->mParent;

    if (pt->mParent == RedBlackTreeNode::NIL) {
      root = pt_left;
    } else if (pt == pt->mParent->mLeft) {
      pt->mParent->mLeft = pt_left;
    } else {
      pt->mParent->mRight = pt_left;
    }
    pt_left->mRight = pt;
    pt->mParent = pt_left;
  }

	void ll_rotate(RedBlackTreeNode *&pt) {
		RedBlackTreeNode *pt_right = pt->mRight;

		pt->mRight = pt_right->mLeft;

		if (pt->mRight != RedBlackTreeNode::NIL) {
			pt->mRight->mParent = pt;
    }

		pt_right->mParent = pt->mParent;

		if (pt->mParent == RedBlackTreeNode::NIL) {
			root = pt_right;
    } else if (pt == pt->mParent->mLeft) {
			pt->mParent->mLeft = pt_right;
    } else {
			pt->mParent->mRight = pt_right;
    }

		pt_right->mLeft = pt;
		pt->mParent = pt_right;
	}

  void rr_rotate(RedBlackTreeNode*& pt) {
		RedBlackTreeNode *pt_left = pt->mLeft;

		pt->mLeft = pt_left->mRight;

		if (pt->mLeft != RedBlackTreeNode::NIL) {
			pt->mLeft->mParent = pt;
    }

		pt_left->mParent = pt->mParent;

		if (pt->mParent == RedBlackTreeNode::NIL) {
			root = pt_left;
    } else if (pt == pt->mParent->mLeft) {
			pt->mParent->mLeft = pt_left;
    } else {
			pt->mParent->mRight = pt_left;
    }

		pt_left->mRight = pt;
		pt->mParent = pt_left;
	}

private:
  RedBlackTreeNode *insert(RedBlackTreeNode *root, const T &key) {
    if (root == RedBlackTreeNode::NIL)
      return new RedBlackTreeNode{key};

    RedBlackTreeNode *current = root, previous;
    while (current != RedBlackTreeNode::NIL) {
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
    RedBlackTreeNode *parent = RedBlackTreeNode::NIL;
    RedBlackTreeNode *grand_parent = RedBlackTreeNode::NIL;

    while ((pt != root) && (pt->mColor != true) && (pt->mParent->mColor == false)) {
      parent = pt->mParent;
      grand_parent = pt->mParent->mParent;
      if (parent == grand_parent->mLeft) {
        Node *uncle = grand_parent->mRight;
        if (uncle != RedBlackTreeNode::NIL && uncle->mColor == RED) {
          grand_parent->mColor = RED;
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
        Node *uncle = grand_parent->mLeft;
        if ((uncle != RedBlackTreeNode::NIL) && (uncle->mColor == false)) {
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
    while (root) {
      if (root->mKey < key) {
        root = root->mRight;
      } else if (key < root->mKey) {
        root = root->mLeft;
      } else {
        return root;
      }
    }
    return nullptr;
  }

private:
  void clear(RedBlackTreeNode *root) {
    if (!root)
      return;
    if (root->mLeft)
      clear(root->mLeft);
    if (root->mRight)
      clear(root->mRight);
    delete root;
    root = nullptr;
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
    return min ? std::optional<std::reference_wrapper<T>>{min->mKey}
               : std::nullopt;
  }

  auto maximum() {
    auto max = maximum(mRoot);
    return max ? std::optional<std::reference_wrapper<T>>{max->mKey}
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
    return res ? std::optional<std::reference_wrapper<T>>{res->mKey}
               : std::nullopt;
  }

public:
  void clear() {
    clear(mRoot);
    mRoot = nullptr;
  }
};
} // namespace forest
