/*
 * Thread.h
 *
 *  Created on: Mar 24, 2009
 *      Author: vasile
 */

#ifndef THREAD_H_
using namespace std;
#include <signal.h>
#include <spawn.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#define THREAD_H_

class Thread
{
   public:
      Thread();
      int Start(void * arg);
   protected:
      int Run(void * arg);
      static void * EntryPoint(void*);
      virtual void Setup();
      virtual void Execute(void*);
      void * Arg() const {return Arg_;}
      void Arg(void* a){Arg_ = a;}
   private:
      THREADID ThreadId_;
      void * Arg_;

};

#endif /* THREAD_H_ */
