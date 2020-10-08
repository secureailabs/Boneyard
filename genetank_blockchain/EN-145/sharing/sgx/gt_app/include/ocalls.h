/*
 * ocalls.h
 *
 *  Created on: Sep 4, 2018
 *      Author: jingwei
 */

#ifndef GT_APP_INCLUDE_OCALLS_H_
#define GT_APP_INCLUDE_OCALLS_H_

sgx_status_t ocall_load_base(const char *name, char *buf, size_t len);

//sgx_status_t ocall_task_table(task_table_struct* task_table);
#endif /* GT_APP_INCLUDE_OCALLS_H_ */
