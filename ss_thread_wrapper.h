#pragma once
#include <thread>
#include <iostream>
#include <concepts>
#include <type_traits>

namespace ss::wrapper{
  // define abstract interface
  struct ss_join {};
  struct ss_detach {};

  // define thread wrapper
  // template <typename T>
  // class ss_thread_wrapper {
  // private:
  //   using ss_flag = std::false_type;
  //
  // public:
  //   ss_thread_wrapper() = delete;
  //   ss_thread_wrapper(std::thread&& m_th) {
  //     static_assert(ss_flag::value == false, "type is wrong");
  //   }
  // };

  // define concepts
  template <typename T>
  concept ss_thread_type = std::is_same_v<ss_join, T> || std::is_same_v<ss_detach, T>;

  // define join thread
  template <ss_thread_type _th>
  class ss_thread_wrapper {
  private:
    using ss_flag = std::true_type;
    std::thread m_th;

  public:
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
        std::cout << "detach finish" << std::endlm_th::get_id
    std::thread::id get_id() { return m_th::get_id(); };
  };    
}
