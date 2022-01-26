#include "declare.hpp"

/** insert a new value ( add the frequency of this given entry) to the frequency map
    * @param existmap frequency map
    * @param newentry new entry to be inserted
    */
void new_median(medmap & existmap, double newentry){
    medmap::iterator iter = existmap.find(newentry);
    medmap::const_iterator eiter = existmap.end();
    if(iter == eiter){
        unsigned long id = 1;
        existmap.insert(make_pair(newentry, id));
    }
    else{
        iter->second++;
    }
}

/** return the median from the frequency map
    * @param existmap frequency map
    * @param totalamt total frequency, to be divided by 2 to get the median
    * @return median (x[i/2]) from the map
    */
double returnmedian(medmap & existmap, double totalamt){
    double medidx = totalamt/2;
    medmap::iterator iter = existmap.begin();
    medmap::iterator eiter = existmap.end();
    double read_amt = 0;
    for( ; iter!=eiter; ++iter){
        //cout << iter->second<< endl;
        read_amt += iter->second;
        if(read_amt > medidx){
            return (iter->first);
            break;
        }
    }
    eiter--;
    return eiter->first;
}

/** constructor, initialise the two frequency maps by inserting 0 to each map, 
and the spread vector by the given spread
    * @param spread stock spread
    */
medianclass::medianclass(double spread){
    spreadvec.push_back(spread);
    tickmap.insert(make_pair(0.0,0));
    trademap.insert(make_pair(0.0,0));
}