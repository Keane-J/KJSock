#ifndef __KJSOCK_RWLOCK_H__
#define __KJSOCK_RWLOCK_H__
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <iostream>
namespace KeaneJ {
	enum class RWType {
		RW_READ = 1,
		RW_WRITE = 2
	};
	template <typename BasicLockable>
	class RWBaseMutex {
	public:
		RWBaseMutex() = default;
		void lock(RWType type) {
			//std::cout << "type" << (int)type << std::endl;
			//std::cout << "readcount" << m_readCount << ";" << m_writeCount << std::endl;
			m_mutex.lock();
			if (type == RWType::RW_READ) {
				
				if (m_isWriting) {
					cv.wait(m_mutex, [=]() { return !m_isWriting; });
				}
				++m_readCount;
			} else {
				++m_writeCount;
				if (m_readCount > 0 || m_isWriting) {
					cv.wait(m_mutex, [=]() { return !m_isWriting && m_readCount == 0; });
				}
				m_isWriting = true;
			}
			m_mutex.unlock();
		}
		void unlock(RWType type) {
			if (type == RWType::RW_READ) {
				--m_readCount;
				if (m_writeCount > 0) {
					cv.notify_one();
				}
			} else {
				--m_writeCount;
				m_isWriting = false;
				if (m_writeCount > 0 || m_readCount > 0) {
					cv.notify_all();
				}
			}
		}
	private:
		std::atomic<bool> m_isWriting{ false };
		std::atomic<int> m_readCount{ 0 };
		std::atomic<int> m_writeCount{ 0 };
		std::condition_variable_any cv;
		BasicLockable m_mutex;
	};
	class RWMutex : public RWBaseMutex<std::mutex> { };

	template <typename  BasicLockable>
	class RWBaseLock {
	public:
		RWBaseLock(RWBaseMutex<BasicLockable> &lk, RWType type) : m_lk(lk), m_type(type){
			m_lk.lock(type);
		}

		~RWBaseLock() {
			m_lk.unlock(m_type);
		}
	private:
		RWType m_type;
		RWBaseMutex<BasicLockable> &m_lk;
	};
	class RWLock : public RWBaseLock<std::mutex> {
	public:
		RWLock(RWMutex &lk, RWType type) : RWBaseLock<std::mutex>(lk, type){

		}
	};
}
#endif