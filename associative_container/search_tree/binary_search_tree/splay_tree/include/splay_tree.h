#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class splay_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:

    class splay
    {

    private:

        splay_tree<tkey, tvalue> *_tree;

    protected:

        explicit splay(
            splay_tree<tkey, tvalue> *tree);

    public:

        virtual ~splay() noexcept = default;

    protected:

        void make(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path);

    };
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method,
        public splay
    {
    
    public:
        
        explicit insertion_template_method(
            splay_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method,
        splay
    {
    
    public:
        
        explicit obtaining_template_method(
            splay_tree<tkey, tvalue> *tree);

    private:

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            splay_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };

public:
    
    explicit splay_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception,
        allocator *allocator = nullptr,
        logger *logger = nullptr);

public:
    
    ~splay_tree() noexcept final;
    
    splay_tree(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree(
        splay_tree<tkey, tvalue> &&other) noexcept;
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> &&other) noexcept;
    
};

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay::splay(
    splay_tree<tkey, tvalue> *tree):
        _tree(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::splay::make(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    if (path.empty())
    {
        // TODO: this should not happen o_O
    }

    typename binary_search_tree<tkey, tvalue>::node **to_splay = path.top();
    path.pop();

    while (!path.empty())
    {
        auto **parent = path.top();
        path.pop();

        if (path.empty())
        {
            (*parent)->left_subtree == *to_splay
                ? _tree->small_right_rotation(*parent)
                : _tree->small_left_rotation(*parent);

            // TODO: this is optional o_O
            to_splay = parent;
        }
        else
        {
            auto **grandparent = path.top();
            path.pop();

            (*grandparent)->left_subtree == parent
                ? ((*parent)->left_subtree == *to_splay
                    ? _tree->double_right_rotation(*grandparent, true)
                    : _tree->big_right_rotation(*grandparent))
                : (*parent->left_subtree == *to_splay
                    ? _tree->big_left_rotation(*grandparent)
                    : _tree->double_left_rotation(*grandparent, true));

            to_splay = grandparent;
        }
    }
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    splay_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    insertion_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree), insertion_strategy),
    splay(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    // this->make(path);
    splay::make(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    splay_tree<tkey, tvalue> *tree):
    obtaining_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree)),
    splay(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::obtaining_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    //this->make(path);
    splay::make(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    splay_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy,
    allocator *allocator,
    logger *logger):
    binary_search_tree<tkey, tvalue>()
{

}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::~splay_tree() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::~splay_tree() noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    splay_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::splay_tree(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(
    splay_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    splay_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::splay_tree(splay_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(
    splay_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(splay_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H