#include "declare.hpp"

void stock::updatemap(string line){
    std::vector<double> trade_data;
    trade_data.resize(datasize);
    istringstream readstr(line);
    string stock_name, number;
    getline(readstr, stock_name, ',');
    for(int j = 0; j < 13; ++j){

        getline(readstr, number, ',');
        if(j == 3){
            string trade_price(number);
            trade_data[13] = islastdigitzero(trade_price);
            trade_data[14] = countzeros(trade_price);
            
        }
        if(j == 6){
            string trade_vol(number);
            trade_data[15] = islastdigitzero(trade_vol);
            trade_data[16] = countzeros(trade_vol);
        }
        trade_data[j] = (atof(number.c_str()));

    }
    getline(readstr, trade_con, ',');
    double spread = trade_data[2] - trade_data[1];
    if((trade_con=="" || trade_con=="XT") && spread >= 0){
        mymap::const_iterator citer = stockdata.begin();
        mymap::const_iterator eiter = stockdata.end();
        mymap::iterator iter = stockdata.find(stock_name);
        if(iter == eiter){
            stockdata.insert(make_pair(stock_name,calculate(trade_data)));
            tradebidinfo.insert(make_pair(stock_name, updatetradebid(trade_data)));
            medianmap.insert(updatemedian(stock_name, spread));
        }
        else{
            trademap::iterator trademapiter = tradebidinfo.find(stock_name);
            stock_medianmap::iterator medianiter = medianmap.find(stock_name);
            calculate(iter->second, trade_data, trademapiter->second, medianiter->second);;
        }
    }
}

//for initialisation
std::vector<double> stock::calculate(std::vector<double> inputvec){
    std::vector<double> returnvec(outputsize);
    //output: spread mean
    returnvec[0] = inputvec[2] - inputvec[1]; //initialize mean spread
    returnvec[1] = inputvec[2] - inputvec[1]; //initialize median spread
    returnvec[2] = 0;//longest time between trades
    returnvec[3] = 0;//longest time between tick changes
    returnvec[4] = std::max(inputvec[14],.0); // count zeros ratio of other places (mean) - price
    returnvec[5] = inputvec[13]; //add zeros if in the last place - price
    returnvec[6] = std::max(inputvec[16],.0); //volume zeros ratio
    returnvec[7] = inputvec[15]; //volunme zeros - last digit
    returnvec[8] = 0; //mean trade time
    returnvec[9] = 0; //median trade time
    returnvec[10] = 0; //mean tick change time
    returnvec[11] = 0; //median tick change time
    returnvec[12] = 0; //trade change times
    returnvec[13] = 0; //tick change times
    return returnvec;
}
//for the rest
void stock::calculate(std::vector<double> & existvec, 
std::vector<double> inputvec, std::vector<std::vector<double>> & tradetime,
medianclass & medianvec){
    if(inputvec[7] == 1){
        double timegap = counttime(tradetime[0][0], tradetime[0][1], inputvec[9], inputvec[10]);
        //existvec[0] = existvec[0]*(existvec[12]/(existvec[12]+1)) + (inputvec[1] - inputvec[2])/(existvec[12]+1);
        existvec[2] = std::max(existvec[2],timegap);
        if(inputvec[14] >= 0){
            existvec[4] = existvec[4]*(existvec[12]/(existvec[12]+1)) + inputvec[14]/(existvec[12]+1);
        }
        existvec[5] = existvec[5]*(existvec[12]/(existvec[12]+1)) + inputvec[13]/(existvec[12]+1);
        if(inputvec[16] >= 0){
            existvec[6] = existvec[6]*(existvec[12]/(existvec[12]+1)) + (inputvec[16])/(existvec[12]+1);
        }
        existvec[7] = existvec[7]*(existvec[12]/(existvec[12]+1)) + inputvec[15]/(existvec[12]+1);
        existvec[8] = existvec[8]*(existvec[12]/(existvec[12]+1)) + timegap/(existvec[12]+1);
        //existvec[9] = 0; //median time of trades;
        updatetradebid(tradetime, inputvec);
        updatemedian(medianvec, timegap);
        existvec[12] += 1;
    }
    else if(inputvec[7] == 2 || inputvec[7] == 3){
        double timegap = counttime(tradetime[1][0], tradetime[1][1], inputvec[9], inputvec[10]);
        existvec[0] = existvec[0]*existvec[13]/(existvec[13]+1) + (inputvec[2] - inputvec[1])/(existvec[13]+1);
        //existvec[1] = 0;//median of bid ask spreda
        existvec[3] = std::max(existvec[3],timegap);
        existvec[10] = existvec[10]*(existvec[13]/(existvec[13]+1)) + timegap/(existvec[13]+1);
        //existvec[11] = 0;//median of tick changes;
        updatetradebid(tradetime, inputvec);
        updatemedian(medianvec, inputvec[2] - inputvec[1], timegap);
        existvec[13] += 1;
    }
}

//for initialisation
std::vector<std::vector<double>> stock::updatetradebid(std::vector<double> trade_data){
    std::vector<std::vector<double>> existvec;
    existvec.resize(2);
    existvec[0].resize(2);
    existvec[1].resize(2);
    if(trade_data[7]==0){
    existvec[0][0] = trade_data[9]; //trade
    existvec[0][1] = trade_data[10];
    existvec[1][0] = trade_data[9]; //tick
    existvec[1][1] = trade_data[10];
    }
    else if(trade_data[7] == 1){
        existvec[0][0] = trade_data[9];
        existvec[0][1] = trade_data[10];
    }
    else if(trade_data[7] == 2 || trade_data[7] == 3){
        existvec[1][0] = trade_data[9];
        existvec[1][1] = trade_data[10];
    }
    return existvec;
}
//for the rest
void stock::updatetradebid(std::vector<std::vector<double>> & existvec, std::vector<double> trade_data){
    if(trade_data[7]==0){
    existvec[0][0] = trade_data[9]; //trade
    existvec[0][1] = trade_data[10];
    existvec[1][0] = trade_data[9]; //tick
    existvec[1][1] = trade_data[10];
    }
    else if(trade_data[7] == 1){
        existvec[0][0] = trade_data[9];
        existvec[0][1] = trade_data[10];
    }
    else if(trade_data[7] == 2 || trade_data[7] == 3){
        existvec[1][0] = trade_data[9];
        existvec[1][1] = trade_data[10];
    }
}


//for initialisation
std::pair<string, medianclass> stock::updatemedian(string stock_name, double spread){
    medianclass m_median(spread);
    //medianvec.resize(3);
    //medianvec[1].resize(estsize); //trade time
    //medianvec[2].resize(estsize); //tick time
    return make_pair(stock_name, m_median);
}
//for tick change, condition == 2/3
void stock::updatemedian(medianclass & m_median, double spread, double timegap){
        new_median(m_median.tickmap, timegap);
        m_median.spreadvec.push_back(spread);
        //cout << "loop sequence: " << existvec[13] <<" " << medianvec[2][existvec[13]]<< endl;
}
//for trade, condition == 1
void stock::updatemedian(medianclass & m_median, 
        double timegap){
        new_median(m_median.trademap, timegap);
        //cout << "loop sequence: " << existvec[12] <<" "<< medianvec[1][existvec[12]] << endl;
}


void stock::findmedian(medianclass & medianvec,
    std::vector<double> & existvec){
        std::sort (medianvec.spreadvec.begin(), medianvec.spreadvec.end());
        existvec[1] = medianvec.spreadvec[int((medianvec.spreadvec.size())/2)]; //median spread
        existvec[9] = returnmedian(medianvec.trademap,existvec[12]); //median trade
        existvec[11] = returnmedian(medianvec.tickmap,existvec[13]); //median tick change
}
void stock::printoutput(){
    fstream outfile;
    outfile.open("stock.txt", ios::in | ios::out| ios::trunc);
    mymap::iterator iter = stockdata.begin();
    mymap::const_iterator eiter = stockdata.end();
    outfile <<"stock name\t"<< "mean spd\t" <<"med spd\t" <<"longest time - trades\t" 
    <<"ltime - tick\t" <<"zero rate - price\t" <<"zeros - p\t"
    <<"z rate - vol\t" <<"zeros - vol\t" <<"mean trade time\t" <<"med tr time\t"
    <<"mean tick time\t"<<"med tick time\t"<<"trade times\t" 
    <<"tick update times\t" <<endl;
    for( ;iter!= eiter; ++iter){
        stock_medianmap::iterator miter = medianmap.find(iter->first);
        outfile << iter->first << " ";
        findmedian(miter->second,iter->second);
        for (unsigned int i = 0; i < outputsize;++i){
            outfile << iter->second[i] <<" ";
        }
        outfile << endl;
    }
    outfile.close();
}


double stock::countzeros(string inputstring){
    double zeroamt = 0; //discard the first digit
    unsigned int string_length = inputstring.size();
    if(string_length <= 2){
        return -1;
    }
    else if(string_length == 3 && inputstring[1] == '.'){
        return -1;
    }
    for(int i = 1; i < string_length -2; ++i){
        if(inputstring[i] == '0'){
            zeroamt++;
        }
        if(inputstring[i] == '.'){
            string_length--;
        }
    }
    return (zeroamt/string_length);
}
double stock::islastdigitzero(string inputstring){
    if(inputstring[inputstring.size()-1] == '0'){
        return 1;
    }
    else{
        return 0;
    }
}
double stock::counttime(double prevtime, double prevsecs, double transtime, double transsecs){
    if(prevtime == transtime){
        return (transsecs - prevsecs);
    }
    else{
        return 0;
    }
}
