#include <iostream>
#include <string>
#include <curl/curl.h>

// Обертка над curl для удобного выполнения HTTP запросов
class HttpClient {
public:
    HttpClient() {
        curl = curl_easy_init();
    }

    ~HttpClient() {
        curl_easy_cleanup(curl);
    }

    // Выполнение GET запроса
    std::string get(const std::string& url) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        return buffer;
    }

private:
    CURL* curl;
    CURLcode res;
    std::string buffer;

    // Callback функция для записи ответа в буфер
    static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append(ptr, size * nmemb);
        return size * nmemb;
    }
};

int main() {
    HttpClient http;
    std::string url = "http://localhost:8080/hello";
    std::string response = http.get(url);
    std::cout << "Response: " << response << std::endl;
    return 0;
}
