template<class T>
class CallBack0 {
	typedef void (T::*Func)();
public:
	CallBack0(T *_this, Func _func)
		: _this(_this)
		, _func(_func)
	{

	}

	void operator()() {
		(_this->*_func)();
	}

private:
	T *_this;
	Func _func;
};

template<class T, class P1>
class CallBack1 {
	typedef void (T::*Func)(P1);
public:
	CallBack1(T *_this, Func _func)
		: _this(_this)
		, _func(_func)
	{

	}

	void operator()(P1 p1) {
		(_this->*_func)(p1);
	}

private:
	T *_this;
	Func _func;
};

template<class T, class P1, class P2>
class CallBack2 {
	typedef void (T::*Func)(P1, P2);
public:
	CallBack2(T *_this, Func _func)
		: _this(_this)
		, _func(_func)
	{

	}

	void operator()(P1 p1, P2 p2) {
		(_this->*_func)(p1, p2);
	}

private:
	T *_this;
	Func _func;
};