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
  auto balance(const RedBlackTreeNode *root) {
    if (!root)
      return 0;
    return height(root->mLeft) - height(root->mRight);
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
  RedBlackTreeNode *rotate_right(RedBlackTreeNode *root) {
    RedBlackTreeNode *pivot{root->mLeft};
    RedBlackTreeNode *orphan{pivot->mRight};

    pivot->mRight = root;
    root->mLeft = orphan;

    root->height = std::max(height(root->mLeft), height(root->mRight)) + 1;
    pivot->height = std::max(height(pivot->mLeft), height(pivot->mRight)) + 1;

    return pivot;
  }

  RedBlackTreeNode *rotate_left(RedBlackTreeNode *root) {
    RedBlackTreeNode *pivot{root->mRight};
    RedBlackTreeNode *orphan{pivot->mLeft};

    pivot->mLeft = root;
    root->mRight = orphan;

    root->height = std::max(height(root->mLeft), height(root->mRight)) + 1;
    pivot->height = std::max(height(pivot->mLeft), height(pivot->mRight)) + 1;

    return pivot;
  }

private:
  RedBlackTreeNode *insert(RedBlackTreeNode *root, const T &key) {
    // TODO
  }

  template <typename U> RedBlackTreeNode *remove(RedBlackTreeNode *root, const U &key) {
    // TODO
  }

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
