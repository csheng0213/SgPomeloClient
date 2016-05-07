#ifndef _OnlineObserver_
#define _OnlineObserver_

/**
* Abstract Class XOnlineObserver. 
* Class XOnlineObserver is the observer of XOnline component.
* This class designed as an abstract class. So client developer has to re-implement all virtual functions.
*/

class OnlineObserver
{
public:
	/**
	* Virtual Function OnRequestSuccess.
	*/
	virtual void OnRequestSuccess(int msgx ,int msgy , LuaReader &reader) = 0;

	int msgx;

	int msgy;

};

#endif //_OnlineObserver_