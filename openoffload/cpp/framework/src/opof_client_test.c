/*
 *  Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**  \ingroup testlibrary
*
*/

#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <time.h>
#include <pthread.h>

#include "opof.h"
#include "opof_test.h"
#include "opof_error.h"
#include "opof_clientlib.h"
#include "opof_test_util.h"


//int get_key(char *location, char *public_key);

int opof_client_speed_test(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int status;
  streamArgs_t args;
  sessionTable_t *handle;
 
  unsigned long closed_sessions=1;
  sessionRequest_t **request;
  addSessionResponse_t addResp;
  args.pageSize = pageSize;
  sessionResponse_t responses[BUFFER_MAX];
  int sessionCount =1;
  int bufferSize;
  int sessionId=0;
  int total_sessions = max_sessions;

  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;
 
  printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  clock_t begin = clock();
  //
   while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      //for (i=0; i < pageSize; i++){
      //  if ((addResp.errorStatus & (error << i)) > 0) {
      //    printf("Session index: %u failed code: %lu", i, (addResp.errorStatus & (error << i)));
      //  }
      return FAILURE;
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }

  
  print_response_header();
  while(closed_sessions > 0){
      closed_sessions = opof_get_closed_sessions(&args,responses);
      printf("Closed sessions: %lu\n", closed_sessions);
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
        print_response( &responses[i]);
       }
      }
   }
  printf("INFO: Closed Sessions %lu\n",closed_sessions);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)total_sessions)/(time_spent));
  return SUCCESS;
}