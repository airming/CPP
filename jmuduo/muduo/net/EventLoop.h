// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/CurrentThread.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>

namespace muduo
{
	namespace net
	{
		class Channel;
		class Poller;
		///
		/// Reactor, at most one per thread.
		///
		/// This is an interface class, so don't expose too much details.
		class EventLoop : boost::noncopyable
		{
		public:
			EventLoop();
			~EventLoop();	// force out-line dtor, for scoped_ptr members.

			///
			/// Loops forever.
			///
			/// Must be called in the same thread as creation of the object.
			///
		  	void loop();

		  	void quit();

			///
			/// Time when poll returns, usually means data arrivial.
			///
			Timestamp pollReturnTime() const { return pollReturnTime_; }

			// internal usage
			void updateChannel(Channel* channel);		// ��Poller����ӻ��߸���ͨ��
			void removeChannel(Channel* channel);		// ��Poller���Ƴ�ͨ��

		  	void assertInLoopThread(){
		  		if (!isInLoopThread())
		  		{
		  			abortNotInLoopThread();
		  		}
		  	}

		  	bool isInLoopThread() const{
		  		return threadId_ == CurrentThread::tid();
		  	}

		  	static EventLoop* getEventLoopOfCurrentThread();

		 private:
		 	void abortNotInLoopThread();

		 	void printActiveChannels() const; // DEBUG

  			typedef std::vector<Channel*> ChannelList;
		 	bool looping_; /* atomic */
			bool quit_; /* atomic */
			bool eventHandling_; /* atomic */
			const pid_t threadId_;		// ��ǰ���������߳�ID
			Timestamp pollReturnTime_;
			boost::scoped_ptr<Poller> poller_;
			ChannelList activeChannels_;		// Poller���صĻͨ��
			Channel* currentActiveChannel_;	// ��ǰ���ڴ���Ļͨ��	
		};
	}
}



#endif  // MUDUO_NET_EVENTLOOP_H