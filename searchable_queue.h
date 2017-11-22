#ifndef TT1_WORDNET_SEARCHABLE_QUEUE_H
#define TT1_WORDNET_SEARCHABLE_QUEUE_H

#include <queue>

// taken from https://stackoverflow.com/a/16749994

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class searchable_queue : public std::priority_queue<T, Container, Compare>
{
public:
    explicit
    searchable_queue(const Compare& __x, Container&& __s = Container())
            : std::priority_queue<T, Container, Compare>(__x, __s)
    { }

    typedef typename
        std::priority_queue<
        T,
        Container,
        Compare>::container_type::const_iterator const_iterator;

    const_iterator cend() const
    {
        return this->c.cend();
    }

    const_iterator find(const T&val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first!=last) {
            if (*first==val) return first;
            ++first;
        }
        return last;
    }

    void update()
    {
        std::make_heap(this->c.begin(), this->c.end(), this->comp);
    }
};

#endif //TT1_WORDNET_SEARCHABLE_QUEUE_H
