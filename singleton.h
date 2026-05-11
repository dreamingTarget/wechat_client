#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>
#include <iostream>
#include <mutex>

template <class T>
class Singleton {
public:
    static std::shared_ptr<T> getInstance() {
        static std::once_flag flag;
        std::call_once(flag, [&](){
            m_instance = std::shared_ptr<T>(new T);
        });
        return m_instance;
    }

    void printInstance() {
        std::cout << "Singleton instance: " << m_instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "Singleton destructor" << std::endl;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton& ) = delete;
    Singleton& operator=(const Singleton& ) = delete;

    static std::shared_ptr<T> m_instance;

};

template<class T>
std::shared_ptr<T> Singleton<T>::m_instance = nullptr;

#endif // SINGLETON_H
