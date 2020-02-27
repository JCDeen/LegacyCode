#ifndef _WAFER_CRITICALSECTION_DEFINED
#define _WAFER_CRITICALSECTION_DEFINED

//
// A generic Wafer Critical Section class. Has to be overridden to use it..
//
class WFCriticalSection {
public:
	WFCriticalSection() {}
	virtual ~WFCriticalSection() {}

	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

#endif
