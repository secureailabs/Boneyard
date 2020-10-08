#ifndef STD_THREAD_SUBSTITUTE_H
#define STD_THREAD_SUBSTITUTE_H
#include <pthread.h>
#include <functional>
//#include <bits/functional_hash.h>

namespace std {

void * start_routine_t(void * arg) ;

class thread {

	public:
		using native_handle_type = pthread_t;
		/* thread::id */
		class id {
			native_handle_type _M_thread;

		public:
			id() noexcept : _M_thread(0) {}

			explicit id (native_handle_type _id) : _M_thread(_id) {}
			inline native_handle_type get_handle(){ return _M_thread;}

		private:
			friend class thread;
			friend class hash<thread::id>;

			friend bool operator==(thread::id __x, thread::id __y) noexcept;

			friend bool operator < (thread::id __x, thread::id __y)  noexcept;
		};
		 
		thread() noexcept = default;
		template<class F, class... Args>
		explicit thread(F&& f, Args&&... args) {
			std::function<void(void)> *func_start = new std::function<void(void)>(std::bind(std::forward<F>(f),std::forward<Args>(args)...));
			if(pthread_create(&_M_id._M_thread, nullptr, start_routine_t, func_start)) {
		                std::terminate();
		        }
		}

		virtual ~thread();
		thread(const thread&) = delete;
		thread(thread&& ) noexcept;
		thread& operator=(const thread&) = delete;
		thread& operator=(thread&&);

		bool joinable() const noexcept;
		void join();
		inline void detach() { 
			if(_M_id._M_thread == 0) return;
			//pthread_detach(_M_id._M_thread); 
			_isJoinable = false;} // no support to detach from sgx's pthread
		inline native_handle_type native_handle() const noexcept{ return _M_id._M_thread; };
		inline thread::id get_id() const noexcept { return _M_id; };
		void swap(thread& other);

	private:
		id _M_id;
		bool _isJoinable = true;
		
};

inline void swap(thread& __x, thread& __y) noexcept {
	__x.swap(__y);
}

inline bool operator == (thread::id __x, thread::id __y) noexcept {
	return __x._M_thread == __y._M_thread;
}

inline bool 
operator != (thread::id __x, thread::id __y) noexcept {
	return !(__x == __y);
}

inline bool operator < (thread::id __x, thread::id __y) noexcept{
	return __x._M_thread < __y._M_thread;
}

inline bool 
operator <= (thread::id __x, thread::id __y) noexcept {
	return !(__y < __x);
}

inline bool operator > (thread::id __x, thread::id __y ) noexcept { 
	return __y < __x;
}

inline bool operator >= (thread::id __x, thread::id __y) noexcept {
 	return !( __x < __y);
}


// std::hash specialization for thread::id
template<>
struct hash<thread::id> {
	size_t operator() (const thread::id& __id) const noexcept {
		return (size_t)(__id._M_thread);
	}
};

namespace this_thread {
	inline thread::id get_id() {
		return thread::id(pthread_self());
	}
}
}

#endif
