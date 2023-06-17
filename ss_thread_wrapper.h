#pragma once
#include <thread>
#include <iostream>
#include <concepts>
#include <type_traits>

namespace ss::detail{
  // define abstract interface
  struct ss_join {};
  struct ss_detach {};

  // define thread wrapper
  template <typename T>
  concept ss_thread_type = std::is_same_v<ss_join, T> || std::is_same_v<ss_detach, T>;

  // define join thread
  template <ss_thread_type _th>
  class ss_thread_wrapper {
  private:
    std::thread m_th;

  public:
    using id = std::thread::id;
    ss_thread_wrapper(std::thread&& th) : m_th(std::move(th)) {};
    ss_thread_wrapper(ss_thread_wrapper&&) = default;

    ss_thread_wrapper(const ss_thread_wrapper&) = delete;
    ss_thread_wrapper& operator=(const ss_thread_wrapper&) = delete;

    ~ss_thread_wrapper() {
      if constexpr (std::is_same_v<ss_join, _th>) {
        if (m_th.joinable()) 
          m_th.join();
        std::cout << "join finish" << std::endl;
      } else {
        if (m_th.joinable()) 
          m_th.detach();
        std::cout << "detach finish" << std::endl;
      }
    }; 

    std::thread::id get_id() { return m_th::get_id(); };
  };
};