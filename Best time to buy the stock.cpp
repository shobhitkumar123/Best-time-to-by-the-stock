#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<int> loadPricesFromFile(const string& filename) {
    ifstream file(filename);
    vector<int> prices;
    int price;
    while (file >> price) {
        prices.push_back(price);
    }
    return prices;
}

pair<int, int> findBestTimeToBuySell(const vector<int>& prices) {
    if (prices.empty()) return {-1, -1};
    int minPrice = prices[0];
    int maxProfit = 0;
    int buyDay = 0, sellDay = 0, tempBuy = 0;

    for (size_t i = 1; i < prices.size(); ++i) {
        if (prices[i] < minPrice) {
            minPrice = prices[i];
            tempBuy = i;
        }
        int profit = prices[i] - minPrice;
        if (profit > maxProfit) {
            maxProfit = profit;
            buyDay = tempBuy;
            sellDay = i;
        }
    }
    return {buyDay, sellDay};
}

void showMenu() {
    cout << "\n=== Stock Profit Maximizer ===\n";
    cout << "1. Load prices from file\n";
    cout << "2. Enter prices manually\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    vector<int> prices;
    int choice;

    while (true) {
        showMenu();
        cin >> choice;

    
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            prices = loadPricesFromFile(filename);
            if (prices.empty()) {
                cout << "File empty or contains invalid data.\n";
                continue;
            }
            cout << "Prices loaded successfully.\n";

        } else if (choice == 2) {
            int n, price;
            cout << "Enter number of days: ";
            cin >> n;
            if (cin.fail() || n <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid number of days.\n";
                continue;
            }

            prices.clear();
            cout << "Enter prices:\n";
            for (int i = 0; i < n; ++i) {
                cin >> price;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid price input. Restarting manual entry.\n";
                    prices.clear();
                    break;
                }
                prices.push_back(price);
            }
            if (prices.size() != n) {
                continue; 
            }

        } else if (choice == 3) {
            cout << "Exiting program.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        auto start = high_resolution_clock::now();
        pair<int, int> result = findBestTimeToBuySell(prices);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        if (result.first == result.second) {
            cout << "No profitable transaction possible.\n";
        } else {
            cout << "Buy on day " << result.first << " at price " << prices[result.first]
                 << ", sell on day " << result.second << " at price " << prices[result.second]
                 << ".\nMax Profit = " << prices[result.second] - prices[result.first] << "\n";
        }

        cout << "Computed in " << duration.count() << " ms.\n";
    }

    return 0;
}
