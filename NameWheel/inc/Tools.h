#ifndef _KODO__TOOLS_H_
#define _KODO__TOOLS_H_
#include <vector>
#include <ostream>
#include <iomanip>
#include <random>
#include <cstdlib>

namespace kodo
{
    using Vector = std::vector<long double>;
    using Matrix = std::vector<Vector>;

    Matrix inline MakeMateix(std::size_t length,
                             std::size_t width,
                             double value = 0.0);
    Matrix MakeNoiseMatrix(std::size_t length,
                           std::size_t width,
                           double max = 1.0,
                           double min = 0.0);
    Vector MakeNoiseVector(std::size_t length,
                           double max = 1.0,
                           double min = 0.0);

    Vector &Multi(Vector *result, const kodo::Matrix &lhs, const kodo::Vector &rhs);

} // namespace kodo

kodo::Matrix operator*(const kodo::Matrix &lhs, const kodo::Matrix &rhs);
kodo::Vector operator*(const kodo::Matrix &lhs, const kodo::Vector &rhs);
kodo::Vector operator*(const kodo::Vector &lhs, const kodo::Vector &rhs);
kodo::Vector operator+(const kodo::Vector &lhs, const kodo::Vector &rhs);
kodo::Vector operator&(const kodo::Vector &lhs, const kodo::Vector &rhs);

kodo::Vector& operator&=(kodo::Vector &lhs, const kodo::Vector &rhs);
kodo::Matrix operator~(const kodo::Matrix& matrix);

std::ostream &operator<<(std::ostream &os, const kodo::Matrix &matrix);
std::ostream &operator<<(std::ostream &os, const kodo::Vector &vector);

namespace kodo
{
    template <unsigned N, class T>
    constexpr static void Spread(T(&hiddens)[N], T arg) {
        hiddens[N - 1] = arg;
    }

    template <unsigned N, class T, class... Args>
    constexpr static void Spread(T(&hiddens)[N], T arg, Args... args) {
        hiddens[N - (sizeof...(args) + 1)] = arg;
        Spread(hiddens, args...);
    }

    template <int... Args>
    class Hidden {
    public:
        template <unsigned InputCount, unsigned OutputCount, class T>
        friend class MlpNetwork;
        using Type = Hidden<Args...>;
        constexpr static const unsigned HIDDENS_QTY = sizeof...(Args);

    private:
        Hidden() {
            Spread(hiddens, Args...);
        }
        int hiddens[HIDDENS_QTY];

    public:
        int operator[](std::size_t index) {
            return hiddens[index];
        }
    };

    template <int Count, int Size>
    class HiddenGroup {
    public:
        template <unsigned InputCount, unsigned OutputCount, class T>
        friend class MlpNetwork;
        using Type = HiddenGroup<Count, Size>;
        constexpr static const unsigned HIDDENS_QTY = Count;

    private:
        HiddenGroup() {}
    public:
        int operator[](std::size_t index) {
            return Size;
        }
    };

    template <>
    class Hidden<> {
    public:
        template <unsigned InputCount, unsigned OutputCount, class T>
        friend class MlpNetwork;
        using Type = Hidden<>;
        constexpr static const unsigned HIDDENS_QTY = 0;

    private:
        Hidden() {}

    public:
        int operator[](std::size_t index) {
            return 0;
        }
    };
} // namespace kodo

#endif 