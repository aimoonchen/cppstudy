#include <coroutine>
#include <iostream>
#include <thread>

namespace Coroutine {
  struct task {
    struct promise_type {
      promise_type() {
        std::cout << "1.create promie object\n";
      }
      task get_return_object() {
        std::cout << "2.create coroutine return object, and the coroutine is created now\n";
        return {std::coroutine_handle<task::promise_type>::from_promise(*this)};
      }
      std::suspend_never initial_suspend() {
        std::cout << "3.do you want to susupend the current coroutine?\n";
        std::cout << "4.don't suspend because return std::suspend_never, so continue to execute coroutine body\n";
        return {};
      }
      std::suspend_never final_suspend() noexcept {
        std::cout << "13.coroutine body finished, do you want to susupend the current coroutine?\n";
        std::cout << "14.don't suspend because return std::suspend_never, and the continue will be automatically destroyed, bye\n";
        return {};
      }
      void return_void() {
        std::cout << "12.coroutine don't return value, so return_void is called\n";
      }
      void unhandled_exception() {}
    };

    std::coroutine_handle<task::promise_type> handle_;
  };

  struct awaiter {
    bool await_ready() {
      std::cout << "6.do you want to suspend current coroutine?\n";
      std::cout << "7.yes, suspend becase awaiter.await_ready() return false\n";
      return false;
    }
    void await_suspend(
      std::coroutine_handle<task::promise_type> handle) {
      std::cout << "8.execute awaiter.await_suspend()\n";
      std::thread([handle]() mutable { handle(); }).detach();
      std::cout << "9.a new thread lauched, and will return back to caller\n";
    }
    void await_resume() {}
  };

  task test() {
    std::cout << "5.begin to execute coroutine body, the thread id=" << std::this_thread::get_id() << "\n";//#1
    co_await awaiter{};
    std::cout << "11.coroutine resumed, continue execcute coroutine body now, the thread id=" << std::this_thread::get_id() << "\n";//#3
  }
}// namespace Coroutine

int main() {
  Coroutine::test();
  std::cout << "10.come back to caller becuase of co_await awaiter\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return 0;
}

/*************************************************************************
* https://www.chiark.greenend.org.uk/~sgtatham/quasiblog/coroutines-c++20/
*************************************************************************/

/*
#include <coroutine>
#include <iostream>

class UserFacing {
public:
	class promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	class promise_type {
	public:
		UserFacing get_return_object() {
			auto handle = handle_type::from_promise(*this);
			return UserFacing{ handle };
		}
		std::suspend_always initial_suspend() { return {}; }
		void return_void() {}
		void unhandled_exception() {}
		std::suspend_always final_suspend() noexcept { return {}; }
	};

private:
	handle_type handle;

	UserFacing(handle_type handle) : handle(handle) {}

	UserFacing(const UserFacing&) = delete;
	UserFacing& operator=(const UserFacing&) = delete;

public:
	bool resume() {
		if (!handle.done())
			handle.resume();
		return !handle.done();
	}

	UserFacing(UserFacing&& rhs) : handle(rhs.handle) {
		rhs.handle = nullptr;
	}
	UserFacing& operator=(UserFacing&& rhs) {
		if (handle)
			handle.destroy();
		handle = rhs.handle;
		rhs.handle = nullptr;
		return *this;
	}
	~UserFacing() {
		handle.destroy();
	}

	friend class SuspendOtherAwaiter;  // so it can get the handle
};

class TrivialAwaiter {
public:
	bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<>) {}
	void await_resume() {}
};

class ReadyTrueAwaiter {
public:
	bool await_ready() { return true; }
	void await_suspend(std::coroutine_handle<>) {}
	void await_resume() {}
};

class SuspendFalseAwaiter {
public:
	bool await_ready() { return false; }
	bool await_suspend(std::coroutine_handle<>) { return false; }
	void await_resume() {}
};

class SuspendTrueAwaiter {
public:
	bool await_ready() { return false; }
	bool await_suspend(std::coroutine_handle<>) { return true; }
	void await_resume() {}
};

class SuspendSelfAwaiter {
public:
	bool await_ready() { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> h) {
		return h;
	}
	void await_resume() {}
};

class SuspendNoopAwaiter {
public:
	bool await_ready() { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<>) {
		return std::noop_coroutine();
	}
	void await_resume() {}
};

class SuspendOtherAwaiter {
	std::coroutine_handle<> handle;
public:
	SuspendOtherAwaiter(UserFacing& uf) : handle(uf.handle) {}
	bool await_ready() { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<>) {
		return handle;
	}
	void await_resume() {}
};

UserFacing demo_coroutine(UserFacing& aux_instance) {
	std::cout << "TrivialAwaiter:" << std::endl;
	co_await TrivialAwaiter{};
	std::cout << "ReadyTrueAwaiter:" << std::endl;
	co_await ReadyTrueAwaiter{};
	std::cout << "SuspendFalseAwaiter:" << std::endl;
	co_await SuspendFalseAwaiter{};
	std::cout << "SuspendTrueAwaiter:" << std::endl;
	co_await SuspendTrueAwaiter{};
	std::cout << "SuspendSelfAwaiter:" << std::endl;
	co_await SuspendSelfAwaiter{};
	std::cout << "SuspendNoopAwaiter:" << std::endl;
	co_await SuspendNoopAwaiter{};
	std::cout << "SuspendOtherAwaiter:" << std::endl;
	co_await SuspendOtherAwaiter{ aux_instance };
	std::cout << "goodbye from coroutine" << std::endl;
}

UserFacing aux_coroutine() {
	while (true) {
		std::cout << "  aux_coroutine was resumed" << std::endl;
		co_await std::suspend_always{};
	}
}

int main() {
	UserFacing aux_instance = aux_coroutine();
	UserFacing demo_instance = demo_coroutine(aux_instance);
	while (demo_instance.resume())
		std::cout << "  suspended and came back to main()" << std::endl;
	std::cout << "and it's goodbye from main()" << std::endl;
}
*/