#include <boost/shared_ptr.hpp>

template<class T>
class Singleton {
public:
	static T *GetInstance() {
		static boost::shared_ptr<T> instance;
		if (instance.get() == NULL) {
			instance.reset(new T);
		}
		return instance.get();
	}
};

#define INSTANCE(T) Singleton<T>::GetInstance()