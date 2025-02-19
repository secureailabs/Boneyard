/*!
 *  Copyright (c) 2015 by Contributors
 * \file threadediter.h
 * \brief thread backed iterator that can be used to implement
 *   general thread-based pipeline such as prefetch and pre-computation
 * To use the functions in this header, C++11 is required
 * \author Tianqi Chen
 */
#ifndef DMLC_THREADEDITER_H_
#define DMLC_THREADEDITER_H_
// defines DMLC_USE_CXX11
#include "./base.h"
// this code depends on c++11
#if DMLC_ENABLE_STD_THREAD
#include <condition_variable>
#include <functional>
#include <exception>
#include <mutex>
#include <queue>
#include <thread>
#include <pthread.h>
#include <utility>
#include <memory>
#include "./data.h"
#include "./logging.h"

namespace dmlc {
/*!
 * \brief a iterator that was backed by a thread
 *  to pull data eagerly from a single producer into a bounded buffer
 *  the consumer can pull the data at its own rate
 *
 * NOTE: thread concurrency cost time, make sure to store big blob of data in DType
 *
 * Usage example:
 * \code
 * ThreadedIter<DType> iter;
 * iter.Init(&producer);
 * // the following code can be in parallel
 * DType *dptr;
 * while (iter.Next(&dptr)) {
 *   // do something on dptr
 *   // recycle the space
 *   iter.Recycle(&dptr);
 * }
 * \endcode
 * \tparam DType the type of data blob we support
 */
template<typename DType>
class ThreadedIter : public DataIter<DType> {
 public:
  /*!
   * \brief producer class interface
   *  that threaditer used as source to
   *  preduce the content
   */

  struct tdata{
  	   std::function<bool(DType **)> tnext;
  	   std::function<void()> tbeforefirst;
  };

  tdata* mytdata;

  class Producer {
   public:
    // virtual destructor
    virtual ~Producer() {}
    /*! \brief reset the producer to beginning */
    virtual void BeforeFirst(void) {
      NotImplemented();
    }
    /*!
     * \brief load the data content into DType,
     * the caller can pass in NULL or an existing address
     * when inout_dptr is NULL:
     *    producer need to allocate a DType and fill the content
     * when inout_dptr is specified
     *    producer takes need to fill the content into address
     *    specified inout_dptr, or delete the one and create a new one
     *
     * \param inout_dptr used to pass in the data holder cell
     *        and return the address of the cell filled
     * \return true if there is next record, false if we reach the end
     */
    virtual bool Next(DType **inout_dptr) = 0;
  };
  /*!
   * \brief constructor
   * \param max_capacity maximum capacity of the queue
   */
  explicit ThreadedIter(size_t max_capacity = 8)
      : producer_owned_(NULL),
        //producer_thread_(NULL),
        max_capacity_(max_capacity),
        nwait_consumer_(0),
        nwait_producer_(0),
        out_data_(NULL) {
	    //pthread_create(&producer_thread_, NULL, NULL, NULL);
  }
  /*! \brief destructor */
  virtual ~ThreadedIter(void) {
    this->Destroy();
  }
  /*!
   * \brief destroy all the related resources
   *  this is equivalent to dest=;ructor, can be used
   *  to destroy the threaditer when user think it is
   *  appropriate, it is safe to call this multiple times
   */
  inline void Destroy(void);
  /*!
   * \brief set maximum capacity of the queue
   * \param max_capacity maximum capacity of the queue
   */
  inline void set_max_capacity(size_t max_capacity) {
    max_capacity_ = max_capacity;
  }
  /*!
   * \brief initialize the producer and start the thread
   *   can only be called once
   * \param producer pointer to the producer
   * \param pass_ownership whether pass the ownership to the iter
   *    if this is true, the threaditer will delete the producer
   *    when destructed
   */
  inline void* producer_fun();
  inline void Init(Producer *producer, bool pass_ownership = false);
  /*!
   * \brief initialize the producer and start the thread
   *  pass in two function(closure) of producer to represent the producer
   *  the beforefirst function is optional, and defaults to not implemented
   *   NOTE: the closure must remain valid until the ThreadedIter destructs
   * \param next the function called to get next element, see Producer.Next
   * \param beforefirst the function to call to reset the producer, see Producer.BeforeFirst
   */
  inline void Init(std::function<bool(DType **)> next,
                   std::function<void()> beforefirst = NotImplemented);
  /*!
   * \brief get the next data, this function is threadsafe
   * \param out_dptr used to hold the pointer to the record
   *  after the function call, the caller takes ownership of the pointer
   *  the caller can call recycle to return ownership back to the threaditer
   *  so that the pointer can be re-used
   * \return true if there is next record, false if we reach the end
   * \sa Recycle
   */
  inline bool Next(DType **out_dptr);
  /*!
   * \brief recycle the data cell, this function is threadsafe
   * the threaditer can reuse the data cell for future data loading
   * \param inout_dptr pointer to the dptr to recycle, after the function call
   *        the content of inout_dptr will be set to NULL
   */
  inline void Recycle(DType **inout_dptr);

  /*!
   * \brief Rethrows exception which is set by the producer
   */
  inline void ThrowExceptionIfSet(void);

  /*!
   * \brief clears exception_ptr, called from Init
   */
  inline void ClearException(void);

  /*!
   * \brief adapt the iterator interface's Next
   *  NOTE: the call to this function is not threadsafe
   *  use the other Next instead
   * \return true if there is next record, false if we reach the end
   */
  virtual bool Next(void) {
    if (out_data_ != NULL) {
      this->Recycle(&out_data_);
    }
    if (Next(&out_data_)) {
      return true;
    } else {
      return false;
    }
  }
  /*!
   * \brief adapt the iterator interface's Value
   *  NOTE: the call to this function is not threadsafe
   *  use the other Next instead
   */
  virtual const DType &Value(void) const {
    CHECK(out_data_ != NULL) << "Calling Value at beginning or end?";
    return *out_data_;
  }
  /*! \brief set the iterator before first location */
  virtual void BeforeFirst(void) {
    ThrowExceptionIfSet();
    //std::unique_lock<std::mutex> lock(mutex_);
    pthread_mutex_lock(&mutex_);
    if (out_data_ != NULL) {
      free_cells_.push(out_data_);
      out_data_ = NULL;
    }
    if (producer_sig_ == kDestroy)  return;

    producer_sig_ = kBeforeFirst;
    CHECK(!producer_sig_processed_);
    if (nwait_producer_ != 0) {
      //producer_cond_.notify_one();
    	pthread_cond_signal(&producer_cond_);
    }
    CHECK(!producer_sig_processed_);
    // wait until the request has been processed
    //consumer_cond_.wait(lock, [this]()
    while(!producer_sig_processed_)
      pthread_cond_wait(&consumer_cond_, &mutex_);
//    {
//        return producer_sig_processed_;
//      });
    producer_sig_processed_ = false;
    bool notify = nwait_producer_ != 0 && !produce_end_;
    //lock.unlock();
    pthread_mutex_unlock(&mutex_);
    // notify producer, in case they are waiting for the condition.
    if (notify) //producer_cond_.notify_one();
    	pthread_cond_signal(&producer_cond_);
    ThrowExceptionIfSet();
  }

 private:
  /*! \brief not support BeforeFirst */
  inline static void NotImplemented(void) {
    LOG(FATAL) << "BeforeFirst is not supported";
  }
  /*! \brief signals send to producer */
  enum Signal {
    kProduce,
    kBeforeFirst,
    kDestroy
  };
  /*! \brief producer class */
  Producer *producer_owned_;
  /*! \brief signal to producer */
  Signal producer_sig_;
  /*! \brief whether the special signal other than kProduce is procssed */
  bool producer_sig_processed_;
  /*! \brief thread that runs the producer */
  //std::thread *producer_thread_;
  pthread_t producer_thread_;
  /*! \brief whether produce ends */
  bool produce_end_;
  /*! \brief maximum queue size */
  size_t max_capacity_;
  /*! \brief internal mutex */
  //std::mutex mutex_;
  pthread_mutex_t mutex_;
  /*! brief internal mutex for exceptions */
  //std::mutex mutex_exception_;
  pthread_mutex_t mutex_exception_;
  /*! \brief number of consumer waiting */
  unsigned nwait_consumer_;
  /*! \brief number of consumer waiting */
  unsigned nwait_producer_;
  /*! \brief conditional variable for producer thread */
  //std::condition_variable producer_cond_;
  pthread_cond_t producer_cond_;
  /*! \brief conditional variable for consumer threads */
  //std::condition_variable consumer_cond_;
  pthread_cond_t consumer_cond_;
  /*! \brief the current output cell */
  DType *out_data_;
  /*! \brief internal queue of producer */
  std::queue<DType*> queue_;
  /*! \brief free cells that can be used */
  std::queue<DType*> free_cells_;
  /*! \brief holds a reference to iterator exception thrown in spawned threads */
  //std::exception_ptr iter_exception_{nullptr};
};

// implementation of functions
template <typename DType> inline void ThreadedIter<DType>::Destroy(void) {
  if (producer_thread_ != NULL) {
    {
      // lock the mutex
      //std::lock_guard<std::mutex> lock(mutex_);
      pthread_mutex_lock(&mutex_);
      // send destroy signal
      producer_sig_ = kDestroy;
      if (nwait_producer_ != 0) {
        //producer_cond_.notify_one();
      	  pthread_cond_signal(&producer_cond_);
      }
    }
    //producer_thread_->join();
    void* retval;
    pthread_join(producer_thread_, &retval);
    delete producer_thread_;
    producer_thread_ = NULL;
  }
  // end of critical region
  // now the slave thread should exit
  while (free_cells_.size() != 0) {
    delete free_cells_.front();
    free_cells_.pop();
  }
  while (queue_.size() != 0) {
    delete queue_.front();
    queue_.pop();
  }
  if (producer_owned_ != NULL) {
    delete producer_owned_;
  }
  if (out_data_ != NULL) {
    delete out_data_;
    out_data_ = NULL;
  }
}

template<typename DType>
inline void ThreadedIter<DType>::
Init(Producer *producer, bool pass_ownership) {
  CHECK(producer_owned_ == NULL) << "can only call Init once";
  if (pass_ownership) producer_owned_ = producer;
  auto next = [producer](DType **dptr) {
      return producer->Next(dptr);
  };
  auto beforefirst = [producer]() {
    producer->BeforeFirst();
  };
  this->Init(next, beforefirst);
}

template<typename DType>
inline void* ThreadedIter<DType>::producer_fun(){
  while (true) {
    try {
      DType *cell = NULL;
      {
        // lockscope
        //std::unique_lock<std::mutex> lock(mutex_);
        pthread_mutex_lock(&this->mutex_);
        ++this->nwait_producer_;
        //producer_cond_.wait(lock, [this]()
        while(producer_sig_ == kProduce?produce_end_ && (queue_.size() < max_capacity_ ||
      		  free_cells_.size() != 0): false)
          pthread_cond_wait(&producer_cond_, &this->mutex_);
//          {
//            if (producer_sig_ == kProduce) {
//              bool ret = !produce_end_ && (queue_.size() < max_capacity_ ||
//                                           free_cells_.size() != 0);
//              return ret;
//            } else {
//              return true;
//            }
//          });
        --this->nwait_producer_;
        if (producer_sig_ == kProduce) {
          if (free_cells_.size() != 0) {
            cell = free_cells_.front();
            free_cells_.pop();
          }
        } else if (producer_sig_ == kBeforeFirst) {
          // reset the producer
          this->mytdata->tbeforefirst();
          // cleanup the queue
          while (queue_.size() != 0) {
            free_cells_.push(queue_.front());
            queue_.pop();
          }
          // reset the state
          produce_end_ = false;
          producer_sig_processed_ = true;
          producer_sig_ = kProduce;
          // notify consumer that all the process as been done.
          //lock.unlock();
          pthread_mutex_unlock(&mutex_);
          //consumer_cond_.notify_all();
          pthread_cond_broadcast(&consumer_cond_);
          continue;
        } else {
          // destroy the thread
          DCHECK(producer_sig_ == kDestroy);
          producer_sig_processed_ = true;
          produce_end_ = true;
          //consumer_cond_.notify_all();
          pthread_cond_broadcast(&consumer_cond_);
          return NULL;
        }
      }  // end of lock scope
      // now without lock
      produce_end_ = !this->mytdata->tnext(&cell);
      DCHECK(cell != NULL || produce_end_);
      bool notify;
      {
        // lockscope
        //std::lock_guard<std::mutex> lock(mutex_);
        pthread_mutex_lock(&this->mutex_);
        if (!produce_end_) {
          queue_.push(cell);
        } else {
          if (cell != NULL)
            free_cells_.push(cell);
        }
        // put things into queue
        notify = nwait_consumer_ != 0;
      }
      if (notify)
        //consumer_cond_.notify_all();
        pthread_cond_signal(&consumer_cond_);
    } catch (dmlc::Error &e) {
      // Shouldn't throw exception in destructor
      DCHECK(producer_sig_ != kDestroy);
      {
        //std::lock_guard<std::mutex> lock(mutex_exception_);
//          pthread_mutex_lock(&mutex_exception);
//          if (!iter_exception_) {
//            iter_exception_ = std::current_exception();
//          }
      }
      bool next_notify = false;
      {
        //std::unique_lock<std::mutex> lock(mutex_);
        pthread_mutex_lock(&mutex_);
        if (producer_sig_ == kBeforeFirst) {
          while (queue_.size() != 0) {
            free_cells_.push(queue_.front());
            queue_.pop();
          }
          produce_end_ = true;
          producer_sig_processed_ = true;
          //lock.unlock();
          pthread_mutex_unlock(&mutex_);
          //consumer_cond_.notify_all();
          pthread_cond_broadcast(&consumer_cond_);
        } else if (producer_sig_ == kProduce) {
          produce_end_ = true;
          next_notify = nwait_consumer_ != 0;
          //lock.unlock();
          pthread_mutex_unlock(&mutex_);
          if (next_notify)
            //consumer_cond_.notify_all();
            pthread_cond_broadcast(&consumer_cond_);
        }
      }
    }
  }
}

template <typename DType>
struct wrapper{
	ThreadedIter<DType> it;

	wrapper(const ThreadedIter<DType>& t): it(t)  {}
};

template <typename DType>
void* func_call(void* f){
	std::auto_ptr<wrapper<DType>> w(static_cast<wrapper<DType>*>(f));
	w->it.producer_fun();
}

template <typename DType>
inline void ThreadedIter<DType>::Init(std::function<bool(DType **)> next,
                                      std::function<void()> beforefirst) {
  producer_sig_ = kProduce;
  producer_sig_processed_ = false;
  produce_end_ = false;
  ClearException();

  this->mytdata->tnext = next;
  this->mytdata->tbeforefirst = beforefirst;
  wrapper<DType>* w = new wrapper<DType>(*this);
  // procedure running in prodcuer
  // run producer thread
  //auto producer_fun = [this, next, beforefirst](){
//  auto producer_fun = [](void* arg){
//	//tdata* targ = (tdata*) arg;
//    while (true) {
//      try {
//        DType *cell = NULL;
//        {
//          // lockscope
//          //std::unique_lock<std::mutex> lock(mutex_);
//          pthread_mutex_lock(&this->mutex_);
//          ++this->nwait_producer_;
//          //producer_cond_.wait(lock, [this]()
//          while(producer_sig_ == kProduce?produce_end_ && (queue_.size() < max_capacity_ ||
//        		  free_cells_.size() != 0): false)
//            pthread_cond_wait(&producer_cond_, &this->mutex_);
////          {
////            if (producer_sig_ == kProduce) {
////              bool ret = !produce_end_ && (queue_.size() < max_capacity_ ||
////                                           free_cells_.size() != 0);
////              return ret;
////            } else {
////              return true;
////            }
////          });
//          --this->nwait_producer_;
//          if (producer_sig_ == kProduce) {
//            if (free_cells_.size() != 0) {
//              cell = free_cells_.front();
//              free_cells_.pop();
//            }
//          } else if (producer_sig_ == kBeforeFirst) {
//            // reset the producer
//            this->tdata->tbeforefirst();
//            // cleanup the queue
//            while (queue_.size() != 0) {
//              free_cells_.push(queue_.front());
//              queue_.pop();
//            }
//            // reset the state
//            produce_end_ = false;
//            producer_sig_processed_ = true;
//            producer_sig_ = kProduce;
//            // notify consumer that all the process as been done.
//            //lock.unlock();
//            pthread_mutex_unlock(&mutex_);
//            //consumer_cond_.notify_all();
//            pthread_cond_broadcast(&consumer_cond_);
//            continue;
//          } else {
//            // destroy the thread
//            DCHECK(producer_sig_ == kDestroy);
//            producer_sig_processed_ = true;
//            produce_end_ = true;
//            //consumer_cond_.notify_all();
//            pthread_cond_broadcast(&consumer_cond_);
//            return;
//          }
//        }  // end of lock scope
//        // now without lock
//        produce_end_ = !this->tdata->tnext(&cell);
//        DCHECK(cell != NULL || produce_end_);
//        bool notify;
//        {
//          // lockscope
//          //std::lock_guard<std::mutex> lock(mutex_);
//          pthread_mutex_lock(&this->mutex_);
//          if (!produce_end_) {
//            queue_.push(cell);
//          } else {
//            if (cell != NULL)
//              free_cells_.push(cell);
//          }
//          // put things into queue
//          notify = nwait_consumer_ != 0;
//        }
//        if (notify)
//          //consumer_cond_.notify_all();
//          pthread_cond_signal(&consumer_cond_);
//      } catch (dmlc::Error &e) {
//        // Shouldn't throw exception in destructor
//        DCHECK(producer_sig_ != kDestroy);
//        {
//          //std::lock_guard<std::mutex> lock(mutex_exception_);
////          pthread_mutex_lock(&mutex_exception);
////          if (!iter_exception_) {
////            iter_exception_ = std::current_exception();
////          }
//        }
//        bool next_notify = false;
//        {
//          //std::unique_lock<std::mutex> lock(mutex_);
//          pthread_mutex_lock(&mutex_);
//          if (producer_sig_ == kBeforeFirst) {
//            while (queue_.size() != 0) {
//              free_cells_.push(queue_.front());
//              queue_.pop();
//            }
//            produce_end_ = true;
//            producer_sig_processed_ = true;
//            //lock.unlock();
//            pthread_mutex_unlock(&mutex_);
//            //consumer_cond_.notify_all();
//            pthread_cond_broadcast(&consumer_cond_);
//          } else if (producer_sig_ == kProduce) {
//            produce_end_ = true;
//            next_notify = nwait_consumer_ != 0;
//            //lock.unlock();
//            pthread_mutex_unlock(&mutex_);
//            if (next_notify)
//              //consumer_cond_.notify_all();
//              pthread_cond_broadcast(&consumer_cond_);
//          }
//        }
//        return;
//      }
//    }
//  };
    pthread_create(&producer_thread_, NULL, func_call<DType>, w);
}

template <typename DType>
inline bool ThreadedIter<DType>::Next(DType **out_dptr) {
  if (producer_sig_ == kDestroy)
    return false;
  ThrowExceptionIfSet();
  //std::unique_lock<std::mutex> lock(mutex_);
  pthread_mutex_lock(&mutex_);
  CHECK(producer_sig_ == kProduce)
      << "Make sure you call BeforeFirst not inconcurrent with Next!";
  ++nwait_consumer_;
  //consumer_cond_.wait(lock,
  //                    [this]() { return queue_.size() != 0 || produce_end_; });
  while(!(queue_.size() != 0 || produce_end_))
    pthread_cond_wait(&consumer_cond_, &mutex_);
  --nwait_consumer_;
  if (queue_.size() != 0) {
    *out_dptr = queue_.front();
    queue_.pop();
    bool notify = nwait_producer_ != 0 && !produce_end_;
    //lock.unlock();
    pthread_mutex_unlock(&mutex_);
    if (notify)
      //producer_cond_.notify_one();
    	pthread_cond_broadcast(&producer_cond_);

    ThrowExceptionIfSet();
    return true;
  } else {
    CHECK(produce_end_);
    //lock.unlock();
    pthread_mutex_unlock(&mutex_);

    ThrowExceptionIfSet();
    return false;
  }
}

template <typename DType>
inline void ThreadedIter<DType>::Recycle(DType **inout_dptr) {
  bool notify;
  ThrowExceptionIfSet();
  {
    //std::lock_guard<std::mutex> lock(mutex_);
    pthread_mutex_lock(&mutex_);
    free_cells_.push(*inout_dptr);
    *inout_dptr = NULL;
    notify = nwait_producer_ != 0 && !produce_end_;
  }
  if (notify)
    //producer_cond_.notify_one();
	pthread_cond_broadcast(&producer_cond_);
  ThrowExceptionIfSet();
}

template <typename DType> inline void ThreadedIter<DType>::ThrowExceptionIfSet(void) {
 // std::exception_ptr tmp_exception{nullptr};
  {
    //std::lock_guard<std::mutex> lock(mutex_exception_);
//	pthread_mutex_lock(&mutex_exception_);
//    if (iter_exception_) {
//      tmp_exception = iter_exception_;
//    }
  }
//  if (tmp_exception)
//    std::rethrow_exception(tmp_exception);
}

template <typename DType> inline void ThreadedIter<DType>::ClearException(void) {
  //std::lock_guard<std::mutex> lock(mutex_exception_);
//  pthread_mutex_lock(&mutex_exception_);
//  iter_exception_ = nullptr;
}

}  // namespace dmlc
#endif  // DMLC_USE_CXX11
#endif  // DMLC_THREADEDITER_H_
