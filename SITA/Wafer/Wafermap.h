#ifdef _WIN32
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <list>

// ----------------------------------------------------------------------------
// Use this pragma to get rid of the message "Variable name exceeds 255 chars.
// ----------------------------------------------------------------------------
#pragma warning(disable:4786)

// ----------------------------------------------------------------------------
// Using the Standard template library namespace.
// ----------------------------------------------------------------------------
using namespace std ;
using namespace std::rel_ops ;

#else
#include <string>
#include <iostream.h>
#include <fstream.h>
#include <algorithm>
#include <iterator>
#include <map>
#include <list>

typedef int BOOL;
#endif

// ----------------------------------------------------------------------------
// class WaferMap: This class helps in associating an object with a "key".
//  This class takes two templates as its arguments. The first is the Key type
//  and the second is the object type. Wafermap makes use of the Standard
//  Template libraries.
//
// WaferMap has basic functionalities required for maintaing an associative
// array. Operations permitted are, Add(), Remove(), RemoveAtIndex() and the
// overloaded operator [].
//
// Currently a linear search is made to look for objects. This could be changed
// to a more efficient algorithm.
//
// All functions have been made inline to get rid of the compiler errors.
// ----------------------------------------------------------------------------
template<class Key, class T>
class WaferMap {
	map<Key, T, less<Key> >				m_WaferMap;
	map<Key, T, less<Key> >::iterator	m_WaferMapIterator;

	typedef pair<const Key, T> value_type;
public:

	WaferMap() {}
	~WaferMap() {}


	// ----------------------------------------------------------------------------
	// Function name	: Add
	// Description	    : Given a key and a value, the items are added into the list.
	// Return type		: BOOL - TRUE if success, FALSE otherwise.
	// Argument         : const Key k - The key associated with "t"
	// Argument         : T t - The object itself.
	// ----------------------------------------------------------------------------
	BOOL Add(const Key k, T t)
	{
		value_type myNewPair(k, t);
		pair<map <Key, T, less<Key> >::iterator, bool> result;

		result = m_WaferMap.insert (myNewPair);
		
		if (result.second)
			return TRUE;
		else
			return FALSE;
	}

	// ----------------------------------------------------------------------------
	// Function name	: Remove
	// Description	    : Searches for the object with key equal to "k" and removes
	//                    it from the collection.
	// Return type		: BOOL - TRUE if the object is found, FALSE otherwise.
	// Argument         : Key k - The key associated with the object to be removed.
	// ----------------------------------------------------------------------------
	BOOL Remove(Key k)
	{
		m_WaferMapIterator = m_WaferMap.find(k);

		if (m_WaferMapIterator == m_WaferMap.end())
			return FALSE;

		m_WaferMap.erase(m_WaferMapIterator);

		return TRUE;
	}

	// ----------------------------------------------------------------------------
	// Function name	: RemoveAtIndex
	// Description	    : Removes the position given the postion of the object in
	//                    list. This method is generally used, when the objects
	//                    position is already known.
	// Return type		: BOOL 
	// Argument         : long lIndex
	// ----------------------------------------------------------------------------
	BOOL RemoveAtIndex(long lIndex)
	{
		if (lIndex > m_WaferMap.size())
			return FALSE;

		m_WaferMapIterator = m_WaferMap.begin();

		for (long l = 0; l < lIndex; l++) {
			m_WaferMapIterator++;
		}
		m_WaferMap.erase(m_WaferMapIterator);
		
		return TRUE;
	}

	// ----------------------------------------------------------------------------
	// Function name	: [] 
	// Description	    : Overloaded operator []. Given the index, this function
	//                    iterates through the list and finds the object specified
	//                    at that position.
	// Return type		: T - The object
	// Argument         : long lIndex - The index of the object.
	// ----------------------------------------------------------------------------
	T operator [] (long lIndex)
	{
		if (lIndex > m_WaferMap.size())
			return NULL;

		T	x;
		m_WaferMapIterator = m_WaferMap.begin();

		for (long l = 0; l <= lIndex; l++) {
			x = (*m_WaferMapIterator).second;
			m_WaferMapIterator++;
		}
		return x;
	}

	// ----------------------------------------------------------------------------
	// Function name	: Find
	// Description	    : Find the object in the map, given the key.
	// Return type		: T - The object associated with the "key"
	// Argument         : Key k - The "key" to be searched for.
	// ----------------------------------------------------------------------------
	T Find(Key k)
	{
		m_WaferMapIterator = m_WaferMap.find(k);

		if (m_WaferMapIterator == m_WaferMap.end())
			return NULL;
		else
			return (*m_WaferMapIterator).second;
	}
	
	// ----------------------------------------------------------------------------
	// Function name	: Count
	// Description	    : Retrieve the number of items in the associative array.
	// Return type		: long 
	// ----------------------------------------------------------------------------
	long Count()
	{
		return m_WaferMap.size();
	}
};
