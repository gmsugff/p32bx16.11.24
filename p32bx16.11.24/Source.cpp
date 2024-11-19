#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

class Program {
public:
    void run() {
        int i;
        string base_url;
        string q;
        string au;
        while (true) {
            cout << "1 - По книге, 2 - По автору, 3 - Выход: ";
            cin >> i;

            switch (i) {
            case 1:
                base_url = "https://openlibrary.org/search.json?q=";
                cout << "Введите название книги, заменив пробелы на '+' (например: the+lord+of+the+rings): " << endl;
                cin >> q;

                {
                    string https = base_url + q;
                    cpr::Response r = cpr::Get(cpr::Url{ https });

                    if (r.status_code == 200) {
                        nlohmann::json json_response = nlohmann::json::parse(r.text);

                        if (json_response.contains("docs")) {
                            for (const auto& s : json_response["docs"]) {
                                if (s.contains("author_alternative_name")) {
                                    cout << "author_alternative_name: " << endl;
                                    for (const auto& g : s["author_alternative_name"]) {
                                        cout << "- " << g << endl;
                                    }
                                }

                                if (s.contains("publish_date")) {
                                    cout << "publish_date: " << endl;
                                    for (const auto& h : s["publish_date"]) {
                                        cout << "- " << h << endl;
                                    }
                                }
                                else {
                                    cout << "publish_date: не указана" << endl;
                                }

                                cout << endl;
                            }
                        }
                        else {
                            cout << "No 'docs' found in the response." << endl;
                        }
                    }
                    else {
                        cout << "Error: " << r.error.message << endl;
                    }
                }
                break;

            case 2:
                base_url = "https://openlibrary.org/search.json?author=";
                cout << "Введите автора (например: tolkien): " << endl;
                cin >> au;

                {
                    string https = base_url + au + "&sort=new";
                    cpr::Response r = cpr::Get(cpr::Url{ https });

                    if (r.status_code == 200) {
                        nlohmann::json json_response = nlohmann::json::parse(r.text);

                        if (json_response.contains("docs")) {
                            for (const auto& s : json_response["docs"]) {
                                if (s.contains("author_alternative_name")) {
                                    cout << "author_alternative_name: " << endl;
                                    for (const auto& g : s["author_alternative_name"]) {
                                        cout << "- " << g << endl;
                                    }
                                }

                                if (s.contains("title")) {
                                    cout << "title: " << s["title"] << endl;
                                }
                                else {
                                    cout << "title: не указано" << endl;
                                }

                                cout << endl;
                            }
                        }
                        else {
                            cout << "No 'docs' found in the response." << endl;
                        }
                    }
                    else {
                        cout << "Error: " << r.error.message << endl;
                    }
                }
                break;

            case 3:
                return; 

            default:
                cout << "Неверный ввод. Пожалуйста, выберите 1, 2 или 3." << endl;
                break;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Program p;
    p.run();
}
