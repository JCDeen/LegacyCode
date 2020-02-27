#include "Idgen.hpp"

IDGenerator::IDGenerator(unsigned long nMin, unsigned long nMax)
{
	Initialize(nMin, nMax);
}

IDGenerator::~IDGenerator()
{
	delete [] m_pnIDs;
}

// ----------------------------------------------------------------------------
// Function name	: IDGenerator::Initialize
// Description	    : Allocate memory for holding IDs between nMin and nMax
//                    and set them to FREE
// Return type		: void 
// Argument         : unsigned long nMin - The starting ID
// Argument         : unsigned long nMax - The ending ID
// ----------------------------------------------------------------------------
void IDGenerator::Initialize(unsigned long nMin, unsigned long nMax)
{
	if (nMin < nMax) {
		m_nMin = nMin;
		m_nMax = nMax;
	}
	else {
		m_nMin = nMax;
		m_nMax = nMin;
	}

	m_nMaxCount = m_nMax - m_nMin + 1;

	m_nCount = 0;

	m_pnIDs = new long [m_nMaxCount];

	for (unsigned long i = 0; i < m_nMaxCount; i++)
		m_pnIDs[i] = FREE;
}


// ----------------------------------------------------------------------------
// Function name	: IDGenerator::GetUniqueID
// Description	    : This function looks for the first available FREE slot in
//                    the array m_pnIDs and returns the sum of the index of the
//                    FREE slot and the starting ID.
// Return type		: long 
// ----------------------------------------------------------------------------
long IDGenerator::GetUniqueID()
{
	unsigned long i;

	for (i = 0; i < m_nMaxCount; i++) {
		if (m_pnIDs[i] == FREE) {
			break;
		}
	}

	if (i == m_nMaxCount)
		return -1;

	m_pnIDs[i] = i+m_nMin;
	m_nCount++;

	return i+m_nMin;
}


// ----------------------------------------------------------------------------
// Function name	: IDGenerator::ReleaseID
// Description	    : Releases the ID and marks the slot as FREE.
// Return type		: BOOL - TRUE if the id is found in the collection
// Argument         : unsigned long id - The id to be released
// ----------------------------------------------------------------------------
BOOL IDGenerator::ReleaseID(unsigned long id)
{
	unsigned long i = id-m_nMin;

	if (m_pnIDs[i] != FREE) {
		m_pnIDs[i] = FREE;
		
		return TRUE;
	}

	return FALSE;
}

