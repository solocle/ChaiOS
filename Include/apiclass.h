#ifndef CHAIOS_CXX_API_CLASS_H
#define CHAIOS_CXX_API_CLASS_H

#include "CHAIKRNL.h"

#ifdef __cplusplus

class CChaiOSAPIClass {
protected:
	~CChaiOSAPIClass(){ ; }			//Use destroy
public:
	virtual void CALLING_CONVENTION destroy() = 0;
private:
public:
	inline void operator delete(void* p)
	{
		if (p)
		{
			CChaiOSAPIClass* apiclass = static_cast<CChaiOSAPIClass*>(p);
			apiclass->destroy();
		}
	}
};

#endif	//__cplusplus

#endif	//CHAIOS_CXX_API_CLASS_H
