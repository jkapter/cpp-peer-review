#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Reader {
public:
    void Read(const int id, const int page) {
        if(readers_to_pages_.size() < id + 1) {
            readers_to_pages_.resize(2*(id + 1));    
        }
        if(pages_to_readers_.size() < page + 1) {
            pages_to_readers_.resize(2*(page + 1));
        }
    int old_page = readers_to_pages_[id];
    readers_to_pages_[id] = page;

    if(old_page > 0) {
        --pages_to_readers_[old_page];
    }

    ++pages_to_readers_[page];
    }
    
    double Cheer(const int id) {
        if(readers_to_pages_.size() < (id) || readers_to_pages_[id] == 0) {
            return 0.0;
        }
    
    double all_readers = static_cast<double>(accumulate(pages_to_readers_.begin(), pages_to_readers_.end(), 0));
    double less_readers =  static_cast<double>(accumulate(pages_to_readers_.begin(), pages_to_readers_.begin() + readers_to_pages_[id], 0));

    double ret_val = 0.0;

    if(all_readers > 0.0) {
        ret_val = (all_readers == 1.0) ? 1.0 : less_readers / (all_readers - 1);
    }

    return  ret_val; 
    }

private:
    vector<int> readers_to_pages_;
    vector<int> pages_to_readers_;

};

int main() {
    Reader reader;

    int lines;
    string str;
    cin >> lines;
    cout << setprecision(6);

    for(;lines > 0; --lines) {
        cin >> str;
        if(str == "CHEER") {
            int id;
            cin >> id;
            cout << reader.Cheer(id) << endl;
        } else if(str == "READ") {
            int id, page;
            cin >> id >> page;
            reader.Read(id, page);
        } else {
            cout << "Wrong input!";
            return -1;
        }

    }
}