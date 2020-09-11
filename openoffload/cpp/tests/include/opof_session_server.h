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

#ifndef __OPOF_SESSION_SERVER_H
#define __OPOF_SESSION_SERVER_H


#include "opof_test.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

class SessionTableImpl final : public SessionTable::Service {
public: 
    
    Status addSession(ServerContext* context, const sessionRequest* request, addSessionResponse* response) override;
    Status getSession(ServerContext* context, const sessionId* sid, sessionResponse* response) override;
    Status deleteSession(ServerContext* context, const sessionId* sid, sessionResponse* response) override;
};


#endif