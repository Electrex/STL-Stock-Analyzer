// Yusuf Mostafa
// Code Blocks on a PC
#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <cstring>
#include <cctype>
#include <ctime>
#include <time.h>
#include <string>
#include <cstdlib>
#include <map>
#include <set>
#include <unordered_set>
#include <cstdlib>

using namespace std;

class MutualFund;

void shuffleNavHistoryFile(string);
void analyze2Funds(MutualFund&, MutualFund&);

class Date
{
private:
    time_t date;
public:
    Date() : date(time(0)){};
    Date(const Date&) = default;
    Date(time_t now) : date(now){};  // Date in time_t format
    Date(const char* str)
    {
        string d = str;
        bool type;
        if (d.find("-") != -1)
        {
            type = true;
        } else
        {
            type = false;
        }
        struct tm temp = {0};
        if (type == true)
        {
            temp.tm_year = stoi(d.substr(0, d.find("-"))) - 1900;
            d = d.substr(d.find("-") + 1);
            temp.tm_mon = stoi(d.substr(0, d.find("-"))) - 1;
            d = d.substr(d.find("-") + 1);
            temp.tm_mday = stoi(d);
        } else
        {
            temp.tm_year = stoi(d.substr(0, d.find("/"))) - 1900;
            d = d.substr(d.find("/") + 1);
            temp.tm_mon = stoi(d.substr(0, d.find("/"))) - 1;
            d = d.substr(d.find("/") + 1);
            temp.tm_mday = stoi(d);
        }
        date = mktime(&temp);
    };
    operator string() const
    {
        int day, month, year;
        string result = "";
        struct tm * timess = gmtime(&date);
        month = timess->tm_mon + 1;
        day = timess->tm_mday;
        year = timess->tm_year + 1900;

        if (month < 10)
            result += "0";
        result += to_string(month);
        result += "/";
        if (day < 10)
            result += "0";
        result += to_string(day);
        result += "/";
        if (year < 10)
            result += "0";
        result += to_string(year);
        return result;
    };
    time_t get_date() const { return date;};
};

bool operator <(const Date& d1, const Date& d2) {
    double seconds = difftime(d1.get_date(), d2.get_date());
    if (seconds > 0) {
        return false;
    } else
    {
        return true;
    }
}

bool operator >(const Date& d1, const Date& d2) {
    double seconds = difftime(d1.get_date(), d2.get_date());
    if (seconds < 0) {
        return false;
    } else
    {
        return true;
    }
}

ostream& operator<<(ostream& out, const Date& obj)
{
    int day, month, year;
    time_t d = obj.get_date();
    struct tm * timess = gmtime(&d);
    month = timess->tm_mon + 1;
    day = timess->tm_mday;
    year = timess->tm_year + 1900;

    if (month < 10)
        out << 0;
    out << month << "/";
    if (day < 10)
        out << 0;
    out << day << "/";
    if (year < 10)
        out << 0;
    out << year;
    return out;
}

class Dividend
{
private:
    Date date;
    float div;
public:
    Dividend(Date& d, float f) : date(d), div(f) {};
    Date getDate() const {return date;};
    float getDiv() const {return div;};
};

bool operator <(const Dividend& d1, const Dividend& d2) {
    if (d1.getDate() < d2.getDate()) {
        return true;
    } else
    {
        return false;
    }
}

bool operator >(const Dividend& d1, const Dividend& d2) {
    if (d1.getDate() > d2.getDate()) {
        return true;
    } else
    {
        return false;
    }
}

class MutualFund
{
private:
    std::string ticker;
    Date oldestDate;
    Date newestDate;
    float newestNav;
    std::map<Date, float> navHistory;
    std::set<Dividend> divHistory;
public:
    MutualFund(string& name) : ticker(name)
    {
        const string navFileName = ticker + "shuffled.csv";
        const string divFileName = ticker + "div.csv";
        const string navOutputFileName = ticker + "nav.txt";
        const string divOutputFileName = ticker + "div.txt";
        cout << "Reading file " << navFileName << " into map<Date,float> ..." << endl;
        ifstream navFile(navFileName.c_str());
        if (!navFile)
        {
            cerr << "Cannot open " << navFileName << endl;
            exit(1);
        }
        ifstream divFile(divFileName.c_str());
        if (!divFile)
        {
            cerr << "Cannot open " << divFileName << endl;
            exit(1);
        }
        ofstream navOutputFile(navOutputFileName.c_str());
        ofstream divOutputFile(divOutputFileName.c_str());
        char buffer[300];
        string buf = "";
        string date;
        float nav, div;

        while (navFile.getline(buffer,sizeof(buffer)))
        {
            buf += buffer;
            date = buf.substr(0, buf.find(","));
            for (int i = 0; i < 4; i++)
            {
                buf = buf.substr(buf.find(",") + 1);
            }
            nav = stof(buf.substr(0, buf.find(",")));
            buf.clear();
            Date temp(date.c_str());
            navHistory.insert(pair<Date, float>(temp, nav));
        }
        navFile.close();
        cout << navHistory.size() << " records read" << endl << endl;

        cout << "Reading file " << divFileName << " into set<Dividend> ..." << endl;
        divFile.getline(buffer,sizeof(buffer));
        while (divFile.getline(buffer,sizeof(buffer)))
        {
            buf += buffer;
            date = buf.substr(0, buf.find(","));
            div = stof(buf.substr(buf.find(",") + 1));
            buf.clear();
            Date temp(date.c_str());
            Dividend tempDiv(temp, div);
            divHistory.insert(tempDiv);
        }
        divFile.close();
        cout << divHistory.size() << " records read" << endl << endl;

        cout << "Writing NAV history data into file " << navOutputFileName << " ... ";
        for (auto it = navHistory.begin(); it != navHistory.end(); ++it)
        {
            navOutputFile << it->first << right << setw(10) << showpoint << fixed << setprecision(2) << it->second << endl;
        }
        navOutputFile.close();
        cout << "done" << endl;

        cout << "Writing dividend history data into file " << divOutputFileName << " ... ";
        for (auto it = divHistory.cbegin(); it != divHistory.cend(); ++it)
        {
            divOutputFile << it->getDate() << right << setw(5) << showpoint << fixed << setprecision(2) << it->getDiv() << endl;
        }
        divOutputFile.close();
        cout << "done" << endl << endl;

    }
    void report()
    {
        cout << "Fund: " << ticker << endl;
        string buf = "";
        string date = "";

        Date analysisDate(navHistory.rbegin()->first.get_date());
        newestDate = analysisDate;
        Date startingDate(analysisDate.get_date() - (60*60*24*365));
        oldestDate = startingDate;
        newestNav = navHistory.rbegin()->second;
        cout << "Analysis Date: " << analysisDate << endl;
        cout << "Latest NAV: $" << fixed << showpoint << setprecision(2) << newestNav << endl;

        float maxNav = navHistory.upper_bound(oldestDate)->second, minNav = navHistory.upper_bound(oldestDate)->second;

        for (auto it = navHistory.upper_bound(oldestDate); it != navHistory.upper_bound(newestDate); ++it)
        {
            if (it->second > maxNav)
                maxNav = it->second;
            if (it->second < minNav)
                minNav = it->second;
        }

        cout << "Minimum for the last year = $" << minNav << endl;
        cout << "Maximum for the last year = $" << maxNav << endl;
        Date fifty(newestDate.get_date() - 24*60*60*50);

        float fiftyDay = 0, twoHundredDay = 0;
        auto it2 = navHistory.end();
        advance(it2, -50);
        while (it2 != navHistory.end())
        {
            fiftyDay += it2->second;
            it2++;
        }
        fiftyDay /= 50;
        cout << "50 day moving average = $" << fiftyDay << endl;
        advance(it2, -200);
        while (it2 != navHistory.end())
        {
            twoHundredDay += it2->second;
            it2++;
        }
        twoHundredDay /= 200;
        cout << "200 day moving average = $" << twoHundredDay << endl;
        float percentGain200 = 0;
        advance(it2, -1);
        percentGain200 = it2->second;
        percentGain200 -= twoHundredDay;

        percentGain200 /= twoHundredDay;
        cout << "% gain over 200 Day Moving Avg: " << percentGain200 * 100 << "%" << endl;

        Date lastThree(analysisDate.get_date() - 24*60*60*1095);
        Date lastThreePlus30(lastThree.get_date() + 24*60*60*30);
        Dividend d(lastThree, 0);
        Dividend d1(lastThreePlus30, 0);
        Dividend d2(analysisDate, 0);

        float currDivedend = 0, currShares = 10000 / navHistory.upper_bound(d.getDate())->second, currDivAmt = 0, currSharePrice = 0, currPurchased = 0, currNewShOwned = 0;
        int counter = 0;
        for (auto it = divHistory.upper_bound(d1); it != divHistory.upper_bound(d2); ++it)
        {

            if (counter == 0)
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                currShares = currNewShOwned;
                counter++;
            } else
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                currShares = currNewShOwned;
            }

        }

        cout << "% return over the last 3 years: " << showpoint << fixed << setprecision(2) << (((currNewShOwned * navHistory.upper_bound(analysisDate)->second) - 10000) / 10000) * 100 << "%" << endl << endl;


    }

    std::map<Date, float> getNavHistory()
    {
        return navHistory;
    }
    std::set<Dividend> getDivHistory()
    {
        return divHistory;
    }

    Date getOldestDate()
    {
        return oldestDate;
    }

    Date getNewestDate()
    {
        return newestDate;
    }

    float getNewestNav()
    {
        return newestNav;
    }
};

int main()
{
    // Part 1
    string ticker1 = "VTSMX", ticker2 = "VFINX";

    shuffleNavHistoryFile(ticker1);
    shuffleNavHistoryFile(ticker2);

    // Part 2
    MutualFund vtsmx(ticker1);
    MutualFund vfinx(ticker2);
    vtsmx.report();
    vfinx.report();

    // Part 3
    analyze2Funds(vtsmx, vfinx);
}

void shuffleNavHistoryFile(string name)
{
    const string inputFileName = name + ".csv";
    cout << "Reading " << inputFileName << " into an unordered_set<string> ... " << endl;
    char buffer[300];
    string buf = "";

    ifstream inputFile(inputFileName.c_str());
    if (!inputFile)
    {
        cerr << "Cannot open " << inputFileName << endl;
        exit(1);
    }
    const string outputFileName = name + "shuffled.csv";
    ofstream outputFile(outputFileName.c_str());

    unordered_set<string> data;

    inputFile.getline(buffer,sizeof(buffer));
    while (inputFile.getline(buffer,sizeof(buffer)))
	{
		buf += buffer;
		data.insert(buf);
		buf.clear();
	}

	cout << "Input data stored in " << data.bucket_count() << " buckets" << endl;

	for (const string& x: data)
    {
        outputFile << x << endl;
	}

	cout << "Writing unordered_set<string> data into file " << outputFileName << endl;
	inputFile.close();
	outputFile.close();

}

void analyze2Funds(MutualFund& f1, MutualFund& f2)
{
    srand(time(0));
    std::map<Date, float> navHistory1 = f1.getNavHistory();
    std::set<Dividend> divHistory1 = f1.getDivHistory();

    std::map<Date, float> navHistory2 = f2.getNavHistory();
    std::set<Dividend> divHistory2 = f2.getDivHistory();

    Date oldestDate(navHistory1.begin()->first);
    Date newestDate("2019/02/15");
    Date start(oldestDate.get_date());
    Date end("2019/02/15");

    Date start1("2016/02/16");

    ofstream outputFile("dividend_detail.txt");

    Dividend test1(start1, 0);
    Date test1plus30(test1.getDate().get_date() + 24*60*60*30);
    Dividend test130(test1plus30, 0);
    Dividend test12(end, 0);

    float currDivedend = 0, currShares = 10000 / navHistory1.upper_bound(test1.getDate())->second, currDivAmt = 0, currSharePrice = 0, currPurchased = 0, currNewShOwned = 0, vtsmxVal, vtsmxGain = 0, vfinxGain = 0, vfinxVal;
    int counter = 0;
    outputFile << "Dividends for VTSMX: " << start1 << "-" << end << endl;
    outputFile << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << endl;
    for (auto it = divHistory1.upper_bound(test130); it != divHistory1.upper_bound(test12); ++it)
    {

        if (counter == 0)
        {
            currDivedend = it->getDiv();
            currDivAmt = currDivedend * currShares;
            currSharePrice = navHistory1.upper_bound(it->getDate())->second;
            currPurchased = currDivAmt / currSharePrice;
            currNewShOwned = currShares + currPurchased;
            outputFile << it->getDate() << "   " << fixed << showpoint << setprecision(3) << currDivedend << "   " << currShares << "     " << setprecision(2) << currDivAmt << "     " << currSharePrice << "    " << setprecision(3) << currPurchased << "     " << currNewShOwned << endl;
            currShares = currNewShOwned;
            counter++;
        } else
        {
            currDivedend = it->getDiv();
            currDivAmt = currDivedend * currShares;
            currSharePrice = navHistory1.upper_bound(it->getDate())->second;
            currPurchased = currDivAmt / currSharePrice;
            currNewShOwned = currShares + currPurchased;
            outputFile << it->getDate() << "   " << fixed << showpoint << setprecision(3) << currDivedend << "   " << currShares << "     " << setprecision(2) << currDivAmt << "     " << currSharePrice << "    " << setprecision(3) << currPurchased << "     " << currNewShOwned << endl;
            currShares = currNewShOwned;
        }

    }
    vtsmxGain = (currNewShOwned * navHistory1.upper_bound(end)->second) - 10000;
    vtsmxVal = 10000 + vtsmxGain;

    outputFile << endl;

    currDivedend = 0, currShares = 10000 / navHistory2.upper_bound(test1.getDate())->second, currDivAmt = 0, currSharePrice = 0, currPurchased = 0, currNewShOwned = 0, vfinxVal = 0, vfinxGain = 0;
    outputFile << "Dividends for VFINX: " << start1 << "-" << end << endl;
    outputFile << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << endl;

    for (auto it = divHistory2.upper_bound(test130); it != divHistory2.upper_bound(test12); ++it)
    {

        if (counter == 0)
        {
            currDivedend = it->getDiv();
            currDivAmt = currDivedend * currShares;
            currSharePrice = navHistory2.upper_bound(it->getDate())->second;
            currPurchased = currDivAmt / currSharePrice;
            currNewShOwned = currShares + currPurchased;
            outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
            currShares = currNewShOwned;
            counter++;
        } else
        {
            currDivedend = it->getDiv();
            currDivAmt = currDivedend * currShares;
            currSharePrice = navHistory2.upper_bound(it->getDate())->second;
            currPurchased = currDivAmt / currSharePrice;
            currNewShOwned = currShares + currPurchased;
            outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
            currShares = currNewShOwned;
        }

    }
    vfinxGain = (currNewShOwned * navHistory2.upper_bound(end)->second) - 10000;
    vfinxVal = 10000 + vfinxGain;
    outputFile << endl;





    cout << "  Buy Date    Sell Date VTSMX Value  VTSMX Gain VFINX Value  VFINX Gain VTSMX-VFINX" << endl;

    float avgVtsmx = 0, avgVfinx = 0;
    for (int i = 0; i < 100; i++)
    {
        Date dif(start.get_date() + 24*60*60*(rand()%((6205) + 1)));
        int diff = difftime(end.get_date(), dif.get_date()) / (60*60*24);
        int randomNum = 24*60*60*(rand()%(diff - 365) + 365);
        Date endif(dif.get_date() + randomNum);
        Dividend test1(dif, 0);
        Date test1plus30(test1.getDate().get_date() + 24*60*60*30);
        Dividend test130(test1plus30, 0);
        Dividend test12(endif, 0);

        float currDivedend = 0, currShares = 10000 / navHistory1.upper_bound(test1.getDate())->second, currDivAmt = 0, currSharePrice = 0, currPurchased = 0, currNewShOwned = 0, vtsmxVal = 0, vtsmxGain = 0, vfinxGain = 0, vfinxVal = 0;
        int counter = 0;
        outputFile << "Dividends for VTSMX: " << dif << "-" << endif << endl;
        outputFile << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << endl;
        for (auto it = divHistory1.upper_bound(test130); it != divHistory1.upper_bound(test12); ++it)
        {

            if (counter == 0)
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory1.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
                currShares = currNewShOwned;
                counter++;
            } else
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory1.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
                currShares = currNewShOwned;
            }

        }
        vtsmxGain = (currNewShOwned * navHistory1.upper_bound(endif)->second) - 10000;
        avgVtsmx += vtsmxGain;
        vtsmxVal = 10000 + vtsmxGain;

        outputFile << endl;

        currDivedend = 0, currShares = 10000 / navHistory2.upper_bound(test1.getDate())->second, currDivAmt = 0, currSharePrice = 0, currPurchased = 0, currNewShOwned = 0, vfinxVal = 0, vfinxGain = 0;
        outputFile << "Dividends for VFINX: " << dif << "-" << endif << endl;
        outputFile << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << endl;

        for (auto it = divHistory2.upper_bound(test130); it != divHistory2.upper_bound(test12); ++it)
        {

            if (counter == 0)
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory2.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
                currShares = currNewShOwned;
                counter++;
            } else
            {
                currDivedend = it->getDiv();
                currDivAmt = currDivedend * currShares;
                currSharePrice = navHistory2.upper_bound(it->getDate())->second;
                currPurchased = currDivAmt / currSharePrice;
                currNewShOwned = currShares + currPurchased;
                outputFile << it->getDate() << setw(8) << right << fixed << showpoint << setprecision(3) << currDivedend << setw(10) << right << currShares << setw(10) << right << setprecision(2) << currDivAmt << setw(10) << currSharePrice << setw(9) << setprecision(3) << currPurchased << setw(12) << currNewShOwned << endl;
                currShares = currNewShOwned;
            }

        }
        vfinxGain = (currNewShOwned * navHistory2.upper_bound(endif)->second) - 10000;
        avgVfinx += vfinxGain;
        vfinxVal = 10000 + vfinxGain;
        cout << dif << "   " << endif << setw(12) << right << vtsmxVal << setw(12) << right << vtsmxGain << setw(12) << right << vfinxVal << setw(12) << right << vfinxGain << setw(12) << right << vtsmxVal - vfinxVal << endl;
        outputFile << endl;
    }

    cout << "Average gain for VTSMX = " << showpoint << fixed << setprecision(2) << (avgVtsmx / 100) << endl;
    cout << "Average gain for VFINX = " << showpoint << fixed << setprecision(2) << (avgVfinx / 100) << endl;


}
