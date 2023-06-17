#include <map>
#include <condition_variable>
namespace ss::detail {

class work_factory {
public:
  using ss_worker = ss_thread_wrapper<ss_detach>; 
  using ss_worker_map = std::map<ss_worker::id, worker>;

public:
  size_t ss_decline;
  size_t ss_task_done_workers;
  bool ss_waiting = {false};
  bool ss_descturting = {false};
  std::string name = {"ss_default"};
  
  ss_worker_map ss_workers = {};
  ss_task_queue<std::function<void()>> ss_queue = {};

  std::mutex ss_mtx;
  std::condition_variable ss_thread_cv;
  std::condition_variable ss_task_done_cv;

public:
  // common methods
  void add_ss_worker() {
    std::lock_guard<std::mutex>   ss_lock(ss_mtx);
    std::thread t(&work_factory::ss_mission, this);
    ss_workers.emplace(t.get_id(), std::move(t));
  }

  void del_ss_worker() {
    std::lock_guard<std::mutex>   ss_lock(ss_mtx);
    if (ss_workers.empty()) {
      std::cerr << "ss_workers is empty!!!" << std::endl;
    } else {
      ss_decline++;
    }
  }

private:
  void ss_mission() {
    std::function<void()> task;
    while (true) {
      if (ss_decline <= 0 && ss_queue.try_pop()) {
        task();
      } else if(ss_decline > 0) {
        std::lock_guard<std::mutex> ss_lock(ss_mtx);
        if (ss_decline > 0 && ss_decline--) {
          ss_workers.erase(std::this_thread::get_id());
          if(ss_waiting)
            ss_task_done_cv.notify_one();
          if(ss_descturting)
            ss_thread_cv.notify_one();
          return;
        }
      } else {
        if (ss_waiting) {
          std::unique_lock<std::mutex>  u_ss_lock(ss_mtx);
          ss_task_done_workers++;
          ss_task_done_cv.notify_one();
          ss_thread_cv.wait(u_ss_lock);
        } else {
          // yield cpu time
          std::this_thread::yield();
        }
      }
    }
  }

}


}
