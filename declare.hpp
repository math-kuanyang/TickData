#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<map>
#include<vector>
#include<algorithm>
using namespace std;
typedef map< std::string, std::vector<double> > mymap;
typedef map< std::string, std::vector<vector<double>> > trademap;
typedef map< double, unsigned long> medmap;
class medianclass;
typedef map< std::string, medianclass> stock_medianmap;

void new_median(medmap & existmap, double newentry);
double returnmedian(medmap & existmap, double totalamt);

class medianclass{
    public:
    medianclass(double spread); //constructor, initialise with the first spread
    medmap tickmap; //frequency map for tick time
    medmap trademap;//frequency map for trade time
    std::vector<double> spreadvec; //vectors of all spreads
};

class stock{
    public:
    /** update the stock class instance by the read line. It calls the calcuate function
    to update the trade bid info, the vector storing required values for this tests,
    and the stock_medianmap.
    * @param line
    */
    void updatemap(string line);

    /** The function that excutes the computation, through polymorphism, it either intialises
    the member variables, or update the member variables by the two update functions. 
    * @param inputvec lines read and split through updatemap function
    * @param existvec passing by reference, this is the vector storing all the required return value
    * @param tradetime vectors store the last update time of trade, bid and ask
    * @param medianvec maps and vectors for computing median of trade / tick times and spread
    * @return initialised vector to store the required return value
    */
    std::vector<double> calculate(std::vector<double> inputvec);
    void calculate(std::vector<double> & existvec, std::vector<double> inputvec, 
    std::vector<std::vector<double>> & tradetime, medianclass & medianvec);
   
    /** The function that updates the last trade / bid time 
    * @param trade_data lines read and split through updatemap function
    * @param existvec passing by reference, this is the vector storing all the required return value
    * @param tradetime vectors store the last update time of trade, bid and ask
    * @param medianvec maps and vectors for computing median of trade / tick times and spread
    * @return intialised map to store the trade bid time info
    */
    std::vector<std::vector<double>> updatetradebid(std::vector<double> trade_data);
    void updatetradebid(std::vector<std::vector<double>> & existvec, std::vector<double> trade_data);
    
    /** The function that updates the maps and vector to store the essential value to compute median 
    * @param stock_name name of the stock, used as index
    * @param spread the spread in bid-ask
    * @param timegap the time gap between trades / bid-ask
    * @return intialised unit of a median map
    */
    std::pair<string, medianclass> updatemedian(string stock_name, double spread);
    void updatemedian(medianclass & m_median, double spread, double timegap);
    void updatemedian(medianclass & m_median, double timegap);
    

    unsigned int datasize; //colums of the input lines
    unsigned int outputsize; //how many data are required
    
    /** The function that update the median entries in the required return value vector
    * @param medianvec the variable that store all the essential information to compute median
    * @param existvec the vector that stores all the required return value
    */
    void findmedian(medianclass & medianvec, std::vector<double> & existvec);
    /** The function that print the output
    */
    void printoutput();
    
    /** The function that return the ratio of zeros in postions other than the last one
    * @param inputstring the input number - use string to avoid round up
    * @return the ratio of zeros in other positions - if it is less than two digits, return -1
    */
    double countzeros(string inputstring);
    /** The function that judge if the last digit is zero
    * @param inputstring the input number - use string to avoid round up
    * @return 0 - not zero in the last digit, 1 - is 0 in the last digit
    */
    double islastdigitzero(string inputstring);
    /** The function that compute the time gap between two time
    * @param prevtime the day of last update
    * @param prevsecs the seconds of last update
    * @param transtime the day of current update
    * @param transsecs the seconds of current update
    * @return timegap, if prevtime!=transtime - interday transcation, return 0
    */
    double counttime(double prevtime, double prevsecs, double transtime, double transsecs);
    
    private:
    mymap stockdata; //the main map that store all the required return value, index by stock name
    stock_medianmap medianmap; // the map that store all the info for computing median, index by stock name
    trademap tradebidinfo; // the map that store all the trade bid info, index by stock name
    string trade_con; // recored the trade type
};

