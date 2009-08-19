#include <pthread.h>

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
		pthread_t ThreadId_;
      void * Arg_;

};

Thread::Thread() {}

int Thread::Start(void * arg)
{
   Arg(arg); // store user data
   int code = pthread_create(&ThreadId_, NULL, Thread::EntryPoint, (void*) arg);
   return code;
}

int Thread::Run(void * arg)
{
   Setup();
   Execute( arg );
}

/*static */
void * Thread::EntryPoint(void * pthis)
{
   Thread * pt = (Thread*)pthis;
   pt->Run(Arg());
}

 void Thread::Setup()
{
        // Do any setup here
}

 void Thread::Execute(void* arg)
{
        // Your code goes here
}
