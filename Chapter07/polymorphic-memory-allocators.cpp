#if 0
#include <array>
#include <iostream>
#include <memory_resource>
#include <set>

#include <gtest/gtest.h>



// Polymorphic memory allocators is a C++17 feature.
// Clang doesn't support polymorphic memory allocators yet (mid 2020)


// Using pmr containers
TEST(PolymorphicMemoryAllocators, MonotonicBuffer) {
  auto buffer = std::array<std::byte, 512>{};
  auto resource = std::pmr::monotonic_buffer_resource{
      buffer.data(), buffer.size(), std::pmr::new_delete_resource()};

  auto unique_numbers = std::pmr::set<int>{&resource};

  auto n = int{};
  while (std::cin >> n) {
    unique_numbers.insert(n);
  }

  for (const auto& number : unique_numbers) {
    std::cout << number << '\n';
  }
}

// A custom memory resource

class PrintingResource : public std::pmr::memory_resource {
public:
  PrintingResource() : res_{std::pmr::get_default_resource()} {}

private:
  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    std::cout << "allocate: " << bytes << '\n';
    return res_->allocate(bytes, alignment);
  }

  void do_deallocate(void* p, std::size_t bytes,
                     std::size_t alignment) override {
    std::cout << "deallocate: " << bytes << '\n';
    return res_->deallocate(p, bytes, alignment);
  }

  bool
  do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
    return (this == &other);
  }
  std::pmr::memory_resource* res_;
};

TEST(PolymorphicMemoryAllocators, CustomMemoryResource) {
  auto res = PrintingResource{};
  auto vec = std::pmr::vector<int>{&res};

  vec.emplace_back(1);
  vec.emplace_back(2);
}
#endif
