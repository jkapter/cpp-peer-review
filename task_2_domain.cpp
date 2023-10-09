#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <execution>

using namespace std;

class DomainChecker;

class Domain {
public:    
    friend class DomainChecker;

    Domain(const string& domain_name)
    :  name_(domain_name)
    {
        reverse_name_ = '.' + domain_name;
        reverse(reverse_name_.begin(), reverse_name_.end());
    }

    bool operator==(const Domain& other) const {
        return name_ == other.name_;
    }

    bool IsSubdomain(const Domain& domain) const {
        return (domain.reverse_name_.find(reverse_name_) == 0);
    }

private: 
    string name_;
    string reverse_name_;

};

class DomainChecker {
public:

    template <typename InputIt>
    DomainChecker(InputIt begin, InputIt end) {
       
        InputIt it = begin;
        while(it < end) {
            forbidden_domains_.push_back(*it++);
        }

        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs){
            return lexicographical_compare(lhs.reverse_name_.begin(), lhs.reverse_name_.end(), rhs.reverse_name_.begin(), rhs.reverse_name_.end());
        });
        
        auto u_it = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const auto& lhs, const auto& rhs){return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);});
        
        forbidden_domains_.erase(u_it, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs){
            return lexicographical_compare(lhs.reverse_name_.begin(), lhs.reverse_name_.end(), rhs.reverse_name_.begin(), rhs.reverse_name_.end());
        });
        if(it == forbidden_domains_.begin()) {
            return false;
        } else {
            return (it-1)->IsSubdomain(domain);
        }
           
    }

    size_t Count() const {
        return forbidden_domains_.size();
    }

private:
    vector<Domain> forbidden_domains_;
    

};

vector<Domain> ReadDomains(istream& is, const size_t domain_quantity) {
    vector<Domain> ret_vec;
    size_t cnt = domain_quantity;
    while(0 < cnt--) {
        string str;
        getline(is, str);
        ret_vec.push_back(std::move(Domain{str}));
    }    
    return ret_vec;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

//----------------- TESTS SECTION ------------------------
template <typename TestFunction>
void RunTestImpl(TestFunction test_function, const std::string& function_name) {
    test_function();
    std::cerr << function_name << " OK" << std::endl;
}

#define RUN_TEST(func) RunTestImpl(func, #func)  

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
                     const std::string& func, unsigned line, const std::string& hint) {
    using namespace std;
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "(" << line << "): " << func << ": ";
        cerr << "ASSERT_EQUAL(" << t_str << ", " << u_str << ") failed: ";
        cerr << t << " != " << u << ".";
        if (!hint.empty()) {
            cerr << " Hint: " << hint;
        }
        cerr << endl;
        abort();
    }
}

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT(expr) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_HINT(expr, hint) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

void TestCheckSubdomain(){
    Domain dom1("com"s);
    ASSERT(dom1.IsSubdomain(Domain{"a.com"s}));
    ASSERT(!dom1.IsSubdomain(Domain{"compot.a"}));
    ASSERT(!dom1.IsSubdomain(Domain{""}));
    ASSERT(!dom1.IsSubdomain(Domain{" "}));
}

void TestCheckEqualDomain() {
    Domain dom{"a.com"};
    ASSERT(dom == Domain("a.com"s));
    ASSERT(!(dom == Domain(".com"s)));
    ASSERT(!(dom == Domain(""s)));
}

void TestDomainChecker(){
    vector<Domain> dom_vec = {Domain{"gdz.ru"s}, Domain{"maps.me"s}, Domain{"m.gdz.ru"s}, Domain{"com"s}};
    DomainChecker dc(dom_vec.begin(), dom_vec.end());
    ASSERT_EQUAL(dc.Count(), size_t(3)); //m.gdz.ru дублирует gdz.ru
    ASSERT(!dc.IsForbidden("aaaa"s));
    ASSERT(!dc.IsForbidden("."s));
    ASSERT(!dc.IsForbidden(""s));
    ASSERT(!dc.IsForbidden(" "s));
    ASSERT(dc.IsForbidden(".com"s));
    ASSERT(dc.IsForbidden("gdz.com"s));
    ASSERT(!dc.IsForbidden("gdz.it"s));
    ASSERT(dc.IsForbidden("aaaa.ssss.gdz.ru"s));
}

void RunTests() {
    RUN_TEST(TestCheckSubdomain);
    RUN_TEST(TestCheckEqualDomain);
    RUN_TEST(TestDomainChecker);
}
//--------------------END OF TESTS SECTION ---------------------------


int main() {
    RunTests();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}