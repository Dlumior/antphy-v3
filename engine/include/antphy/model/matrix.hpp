#pragma once

#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

namespace antphy {

/// Matriz cuadrada densa, almacenada fila por fila en un bloque contiguo.
/// Pensada para distancias y tiempos: acceso O(1) y amigable con la caché.
template <typename T>
class SquareMatrix {
public:
    SquareMatrix() = default;
    explicit SquareMatrix(std::size_t n, T fill = T{}) : n_{n}, data_(n * n, fill) {}

    [[nodiscard]] std::size_t size() const noexcept { return n_; }

    [[nodiscard]] T& operator()(std::size_t i, std::size_t j) noexcept {
        assert(i < n_ && j < n_);
        return data_[i * n_ + j];
    }
    [[nodiscard]] const T& operator()(std::size_t i, std::size_t j) const noexcept {
        assert(i < n_ && j < n_);
        return data_[i * n_ + j];
    }

    [[nodiscard]] std::span<const T> row(std::size_t i) const noexcept {
        assert(i < n_);
        return {data_.data() + i * n_, n_};
    }

private:
    std::size_t n_{0};
    std::vector<T> data_;
};

} // namespace antphy
