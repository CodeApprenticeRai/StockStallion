#include <iostream>
#include <sstream>

using namespace std;

#include <curl/curl.h>

struct request {
    std::string base = std::string("https://www.alphavantage.co/query?");
    std::string function = std::string("function=TIME_SERIES_INTRADAY");
    std::string symbol = std::string("symbol=");
    std::string interval = std::string("interval=1min");
    std::string api_key = std::string("apikey=CRINRUHDWCNFTVK2");

};
int main(void) {
    CURL *curl;
    CURLcode res;
    std::string ticker_symbol;

    std::cout << "Enter the Ticker Symbol You Would Like to Add: ";
    std::cin >> ticker_symbol;

    if (ticker_symbol.length() > 5) { exit(0); }

    curl = curl_easy_init();

    request request;

    request.symbol.append(ticker_symbol);

    std::string req =
            request.base + request.function + "&" + request.symbol + "&" + request.interval + "&" + request.api_key;
    //new
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());

        std::string resultBody{};
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (__stdcall*)(char*, size_t, size_t, void*)>(
                [](char* ptr, size_t size, size_t nmemb, void* resultBody){
                    *(static_cast<std::string*>(resultBody)) += std::string {ptr, size * nmemb};
                    return size * nmemb;
                }
        ));

        CURLcode curlResult = curl_easy_perform(curl);

        if (curlResult != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(curlResult));
        }

        //Finds the current price of the stock
        std::string openingPrice;
        int pos = 0;

        for (int position = 0; position < resultBody.length();) {
            if (resultBody.find("1. open\": \"") != -1) {
                position = resultBody.find("1. open\": \"");

//                openingPrice = openingPrice + resultBody.at(pos) + resultBody.at(pos + 1) + resultBody.at(pos + 2);
//
//                openingPrice = resultBody.at(position + 11);
//                openingPrice = openingPrice + resultBody.at(position + 12) + resultBody.at(position + 13) +
//                               resultBody.at(position + 14) + resultBody.at(position + 15) +
//                               resultBody.at(position + 16)
//                               + resultBody.at(position + 17);


                //better price printer in stockstallion.cpp
                std::cout << "Opening price is: " << openingPrice << endl;
                break;
            }
        }


        /* always cleanup */
        curl_easy_cleanup(curl);



//        /* example.com is redirected, so we tell libcurl to follow redirection */
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//
//
//        /* Perform the request, res will get the return code */




    }
    return 0;
}