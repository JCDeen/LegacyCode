#ifndef IDGEN_DEFINED
#define IDGEN_DEFINED

//-----------------------------------------------------------------------------
//
// This is the header for a class which generates an unique ID for a given
// object. A simple algorithm is used to find the first available ID in an array
// and return it back to the caller. Methods to Verify or Release an ID are also
// implemented in this class
//
//-----------------------------------------------------------------------------

#define MAX_COUNT	1024
#define	FREE		-1

#ifndef BOOL
typedef int BOOL;

#define FALSE	0
#define TRUE	1
#endif

class IDGenerator {
protected:
	long			*m_pnIDs;
	unsigned long	m_nMin;
	unsigned long	m_nMax;
	unsigned long	m_nCount;
	unsigned long	m_nMaxCount;
public:
	IDGenerator(unsigned long nMin = 0, unsigned long nMax = MAX_COUNT);
	~IDGenerator();

	void Initialize(unsigned long nMin, unsigned long nMax);
	long GetUniqueID();
	BOOL ReleaseID(unsigned long id);
};
#endif
