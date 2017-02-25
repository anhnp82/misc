#include <assert.h>
#include <map>
#include <limits>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

template<class K, class V>
class interval_map {
    friend void IntervalMapTest();

private:
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val) {
        //m_map.insert(m_map.begin(),std::make_pair(std::numeric_limits<K>::lowest(),val));
        m_map.insert(m_map.begin(),std::make_pair(std::numeric_limits<K>::min(),val));
    };

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Do not change values outside this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, const V& val ) {
        if (!( keyBegin < keyEnd )) return;

        typename std::map<K, V>::iterator itLow, itUp, itTemp, itTemp2;
        V preVal;

        itLow = m_map.lower_bound(keyBegin);

        if (itLow == m_map.end()) // first time after init or largest
        {
            itTemp = --itLow;
            if (itTemp->second == val) // check for merging
            {
                // do not insert duplicate entry
            }
            else
            {
                preVal = itTemp->second;
                itTemp2 = m_map.insert(itTemp, std::make_pair(keyBegin, val)); // hint wrong, must check against the value
                m_map.insert(itTemp2, std::make_pair(keyEnd, preVal));
            }
        }
        else if (itLow != m_map.begin()) // avoid the whole range
        {
            // check lower key with keyBegin
            bool bExactLowerFound = false;
            // if (itLow->first == keyBegin) // no operator supported
            if ( !(itLow->first < keyBegin) && !(keyBegin < itLow->first) ) // exact lower bound
            {
                bExactLowerFound = true;
            } // else the lower entry does not exist, itLow points to upper key than keyBegin

            itTemp = itUp = m_map.upper_bound(keyEnd);
            --itTemp;

            bool bExactUpperFound = false;
            if ( !(itTemp->first < keyEnd) && !(keyEnd < itTemp->first) )
            {
                bExactUpperFound = true;
            } // else the upper entry does not exist, itTemp points to lower key than keyEnd, itUp points to upper key than keyEnd

            if ( !bExactLowerFound && !bExactUpperFound )
            {
                itTemp = itLow;
                --itTemp;

                itTemp2 = itUp;
                --itTemp2;
                preVal = itTemp2->second;

                //m_map.insert(itTemp, std::make_pair(keyBegin, val)); // hint

                m_map.erase(itLow, itUp);

                //m_map.insert(itTemp, std::make_pair(keyBegin, val)); // hint
                m_map.insert(std::make_pair(keyBegin, val)); // hint

                //m_map.insert(itTemp2, std::make_pair(keyEnd, preVal)); // hint
                m_map.insert(std::make_pair(keyEnd, preVal)); // hint
            }
            else if (bExactLowerFound && !bExactUpperFound)
            {
                itTemp = itLow;
                ++itTemp;

                itTemp2 = itUp;
                --itTemp2;

                preVal = itTemp2->second;

                itLow->second = val;
                m_map.erase(itTemp, itUp);

                //m_map.insert(itTemp2, std::make_pair(keyEnd, preVal)); // hint
                m_map.insert(std::make_pair(keyEnd, preVal)); // hint
            }
            else if (!bExactLowerFound && bExactUpperFound)
            {
                itTemp2 = itUp;
                --itTemp2; // exclude the upper bound

                itTemp = itLow;
                --itTemp;

                //m_map.insert(itTemp, std::make_pair(keyBegin, val)); // hint
                m_map.insert(std::make_pair(keyBegin, val)); // hint
                //m_map.erase(itLow, itUp);
                m_map.erase(itLow, itTemp2); // exclude the upper bound
            }
            else
            {
                itTemp = itLow;
                --itTemp;

                --itUp; // exclude the upper bound

                m_map.erase(itLow, itUp); // erase before to avoid inserting duplicate key
                //m_map.insert(itTemp, std::make_pair(keyBegin, val)); // hint
                m_map.insert(std::make_pair(keyBegin, val)); // hint
            }
        }
        else // overwrite the old default value
        {
            itUp = m_map.upper_bound(keyEnd);
            itTemp2 = itUp;
            --itTemp2;
            preVal = itTemp2->second;

            itLow->second = val;

            itTemp = itLow;
            ++itTemp;

            bool bInsert = false;

            if ( !(itTemp2->first < keyEnd) && !(keyEnd < itTemp2->first) )
            {
                --itUp; // exclude the upper bound
            }
            else  // insert (keyEnd, preVal)
            {
                bInsert = true;
            }

            m_map.erase(itTemp, itUp);

            //if (bInsert) m_map.insert(itTemp2, std::make_pair(keyEnd, preVal)); // hint
            if (bInsert) m_map.insert(std::make_pair(keyEnd, preVal)); // hint
        }
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        return ( --m_map.upper_bound(key) )->second;
    }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a function IntervalMapTest() here that tests the
// functionality of the interval_map, for example using a map of unsigned int
// intervals to char.

void IntervalMapTest()
{
    interval_map<unsigned int, unsigned char> myMap(9);

    if (false)
    {
        //myMap.m_map.insert(std::make_pair(92, 1));

        myMap.assign(92, 95, 1);

        unsigned char cTest = myMap[95];

        printf("test %d\n", cTest);

        printf("map content\n");
        for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
             it!=myMap.m_map.end(); ++it)
        {
            printf("key %d, value %d\n", it->first, it->second);
        }
    }

    if (true)
    {

        unsigned char acCheck[100];

        for (unsigned int j = 0; j < 100; ++j) // avoid iEnd
        {
            acCheck[j] = 9;
        }

        unsigned int iCount = 1000000;

        unsigned int iBegin, iEnd, iVal;

        unsigned int iQuery = 9;

        bool bOk = true;

        /* initialize random seed: */
        srand (time(NULL));

        for (unsigned int i = 0; i < iCount; ++i)
        {
            iBegin = rand() % 100;
            iEnd = rand() % 100;
            iVal = rand() % 100;

            myMap.assign(iBegin, iEnd, iVal);

            for (unsigned int j = iBegin; j < iEnd; ++j) // avoid iEnd
            {
                acCheck[j] = iVal;
            }

            bOk = true;

            // check against array
            for (unsigned int j = iBegin; j < iEnd; ++j) // avoid iEnd
            {
                if (myMap[j] != acCheck[j])
                {
                    printf("wrong in range at (%d, %d), begin %d end %d value %d - map content\n", j, myMap[j], iBegin, iEnd, iVal);
                    for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
                         it!=myMap.m_map.end(); ++it)
                    {
                        printf("key %d, value %d\n", it->first, it->second);
                    }

                    bOk = false;
                    break;
                }
            }

            if (!bOk) break;

            if (0 < iBegin && (myMap[iBegin - 1] != acCheck[iBegin - 1]))
            {
                printf("wrong at begin - 1, begin %d end %d value %d - map content\n", iBegin, iEnd, iVal);
                for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
                     it!=myMap.m_map.end(); ++it)
                {
                    printf("key %d, value %d\n", it->first, it->second);
                }

                bOk = false;
                break;
            }

            if (iEnd < 100 && (myMap[iEnd] != acCheck[iEnd]))
            {
                printf("wrong at end value %d vs %d, begin %d end %d value %d - map content\n", myMap[iEnd], acCheck[iEnd], iBegin, iEnd, iVal);
                for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
                     it!=myMap.m_map.end(); ++it)
                {
                    printf("key %d, value %d\n", it->first, it->second);
                }

                bOk = false;
                break;
            }

            if (iEnd + 1 < 100 && (myMap[iEnd + 1] != acCheck[iEnd + 1]))
            {
                printf("wrong at end + 1, begin %d end %d value %d - map content\n", iBegin, iEnd, iVal);
                for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
                     it!=myMap.m_map.end(); ++it)
                {
                    printf("key %d, value %d\n", it->first, it->second);
                }

                bOk = false;
                break;
            }
        }


        if (bOk)
        {
            printf("map content\n");
            for (std::map<unsigned int, unsigned char>::iterator it = myMap.m_map.begin();
                 it!=myMap.m_map.end(); ++it)
            {
                printf("key %d, value %d\n", it->first, it->second);
            }
        }

    }
}

int main(int argc, char* argv[]) {
    IntervalMapTest();
    return 0;
}
