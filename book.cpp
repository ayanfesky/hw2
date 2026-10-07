#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>

using namespace std;

Book::Book(const string name, double price, int qty, const string isbn, const string author) : Product("book", name, price, qty), isbn_(isbn), author_(author){

}

Book::~Book(){

}

set<string> Book::keywords() const{
    set<string> result = parseStringToWords(name_);
    set<string> authorWords = parseStringToWords(author_);
    result = setUnion(result, authorWords);
    result.insert(isbn_);

    return result;
}

string Book::displayString() const{
    stringstream ss;
    ss<<name_<<endl;
    ss<<"Author: "<<author_<<" ISBN: "<< isbn_ << endl;
    ss<< fixed << setprecision(2) << price_ <<" "<<qty_<< " left.";
    return ss.str();
}

void Book::dump(ostream& os) const{
    Product::dump(os);
    os<<isbn_<<endl;
    os<<author_<<endl;
}
