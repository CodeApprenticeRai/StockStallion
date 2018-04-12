#include <iostream>
#include <sstream>

using namespace std;

#include </curl/curl.h>

struct request {
    std::string base = std::string("https://www.alphavantage.co/query?")
    std::string function = std::string("function=TIME_SERIES_INTRADAY");
    std::string symbol = std::string("symbol=");
    std::string interval = std::string("interval=1min");
    std::string api_key = std::string("apikey=CRINRUHDWCNFTVK2")

};
int main(void)
{
    CURL *curl;
    CURLcode res;
    std::string ticker_symbol;

    std::cout << "Enter the Ticker Symbol You Would Like to Add: ";
    std::cin >> ticker_symbol;

    if (ticker_symbol.length() > 5){ exit(0);}

    curl = curl_easy_init();

    request request = request{};

    request.symbol.append(ticker_symbol);

    std::string req = request.base + request.function + "&" + request.symbol + "&" + request.interval + "&" + request.api_key;



    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        //Convert response into a CString to be parsed
        CString str(curl_easy_strerror(res));

        //Finds the current price of the stock
        std::string line;
        std::istringstream stream(str);
        std::string openingPrice;

        while (std::getline(stream, line))
        {
            if (line.find("1. open\": \"") != -1)
            {
                openingPrice = line.substr(24, 7);
                std::cout << openingPrice << endl;
                break;
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}