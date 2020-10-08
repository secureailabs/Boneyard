#include "std_thread_substitute.h"
#include <functional>

namespace std {

thread::~thread() {
	if(_M_id._M_thread && joinable())
		std::terminate();
}

void thread::swap(thread& other) {
	std::swap(_M_id, other._M_id);
	std::swap(_isJoinable, other._isJoinable);
}

thread::thread(thread&& t) noexcept {
	//swap(t);
	_M_id = t._M_id;
	_isJoinable = t._isJoinable;
	t._isJoinable = false;
	t._M_id = thread::id();
}

thread& thread::operator=(thread&& __t) {
	if(_M_id._M_thread && joinable())
		std::terminate();
	//swap(__t);
	_M_id = __t._M_id;
        _isJoinable = __t._isJoinable;
        __t._isJoinable = false;
        __t._M_id = thread::id();
	return *this;
}

bool thread::joinable() const noexcept{
	/*TODO: get state of pthread and then return */
	if(_M_id._M_thread == 0)
		return false;
	return _isJoinable;
}

void thread::join() {
	_isJoinable = false;
	pthread_join(_M_id._M_thread, NULL);
}

void * start_routine_t(void * arg) {
        std::function<void(void)> *f = (std::function<void(void)> *) arg;
	if(*f == nullptr)
		return nullptr;
       	(*f)();
        delete f;
	return nullptr;
}


/*
void thread::detach() {
	//pthread_detach(_threadHandle); // no supoorot for detach
}
*/

}
