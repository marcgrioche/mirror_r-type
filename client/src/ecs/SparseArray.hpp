#pragma once
#include <optional>
#include <vector>

template <typename Component>
class SparseArray {
public:
    using value_type = std::optional<Component>;
    using reference = value_type&;
    using const_reference = const value_type&;

    reference operator[](std::size_t idx) { return _data[idx]; }
    const_reference operator[](std::size_t idx) const { return _data[idx]; }

    std::size_t size() const { return _data.size(); }

    reference insert_at(std::size_t pos, Component const& comp)
    {
        if (pos >= _data.size())
            _data.resize(pos + 1);
        _data[pos] = comp;
        return _data[pos];
    }

    template <typename... Args>
    reference emplace_at(std::size_t pos, Args&&... args)
    {
        if (pos >= _data.size())
            _data.resize(pos + 1);
        _data[pos].emplace(std::forward<Args>(args)...);
        return _data[pos];
    }

    void erase(std::size_t pos)
    {
        if (pos < _data.size())
            _data[pos].reset();
    }

private:
    std::vector<value_type> _data;
};
