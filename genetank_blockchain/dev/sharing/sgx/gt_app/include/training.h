/*
 * training.h
 *
 *  Created on: Jul 15, 2018
 *      Author: accuser
 */

#ifndef GT_APP_INCLUDE_TRAINING_H_
#define GT_APP_INCLUDE_TRAINING_H_

#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "msg_def.h"
#include "enclave_control.h"

using namespace std::placeholders;
class Training
{
  std::mutex m_mutex;
  std::condition_variable m_condVar;
  bool m_bTraining;
  training_action_t taAction;
  bool m_bHasAction;
public:
  Training()
  {
    m_bHasAction = false;
    taAction = ACT_STOP_TRAINING;
    m_bTraining = false;
  }
  void action(training_action_t act)
  {
    // Lock The Data structure
    std::lock_guard<std::mutex> guard(m_mutex);
    // Set the flag to true, means data is loaded
    m_bHasAction = true;
    taAction = act;
    // Notify the condition variable
    m_condVar.notify_one();
  }
  bool hasAction()
  {
    return m_bHasAction;
  }
  void mainTask()
  {
    // Acquire the lock
    std::unique_lock<std::mutex> mlock(m_mutex);
    while(true) {
      // Start waiting for the Condition Variable to get signaled
      // Wait() will internally release the lock and make the thread to block
      // As soon as condition variable get signaled, resume the thread and
      // again acquire the lock. Then check if condition is met or not
      // If condition is met then continue else again go in wait.
      if (taAction == ACT_STOP_TRAINING) {
    	  m_condVar.wait(mlock, std::bind(&Training::hasAction, this));
    	  m_bHasAction = false;
      }
      //std::cout<<"Do training action:"<<taAction<<std::endl;
      switch (taAction) {
        case ACT_START_TRAINING:
        	if (SGX_SUCCESS != enclave.ec_train_model())
        		return;
        	break;
        case ACT_STOP_TRAINING:
        	break;
        case ACT_EXIT_TRAINING:
        	return;
        	break;
        default:
        	break;
      }
    }
  }
};

extern Training training;

#endif /* GT_APP_INCLUDE_TRAINING_H_ */
