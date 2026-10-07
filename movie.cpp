#include "movie.h"
#include "util.h"
#include <sstream>
#include <iomanip>

using namespace std;

Movie::Movie(const std::string name, double price, int qty, const string genre, const string rating) : Product("movie", name, price, qty), genre_(genre), rating_(rating){

}

Movie::~Movie(){

}

set<string> Movie::keywords() const{
    set<string> result = parseStringToWords(name_);
    result.insert(convToLower(genre_));
    return result;
}

string Movie::displayString() const{
    stringstream ss;
    ss << name_ << endl;
    ss<<"Genre: " <<genre_<< " Rating: " << rating_ << endl;
    ss<< fixed << setprecision(2) << price_ <<" "<<qty_<< " left.";
    return ss.str();
}

void Movie::dump(ostream& os) const{
    Product::dump(os);
    os<<genre_<<endl;
    os<<rating_<<endl;
}