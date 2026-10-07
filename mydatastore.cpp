#include "mydatastore.h"
#include "util.h"
#include <iostream>
using namespace std;

MyDataStore::MyDataStore(){

}

MyDataStore:: ~MyDataStore(){
    for (size_t i=0; i<products_.size(); i++){
        delete products_[i];
    }
    for(size_t i=0; i<users_.size(); i++){
        delete users_[i];
    }
}

void MyDataStore::addProduct(Product* p){
    products_.push_back(p);
    set<string> keys = p->keywords();

    for(set<string>::iterator it=keys.begin(); it!=keys.end(); ++it){
        string key = convToLower(*it);
        keywordMap_[key].insert(p);
    }
}

void MyDataStore::addUser(User* u){
    users_.push_back(u);
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type){
    vector<Product*> result;

    if(terms.size()==0){
        return result;
    }
    string firstTerm = convToLower(terms[0]);
    set<Product*> matches;
    map<string, set<Product*>>::iterator firstIt = keywordMap_.find(firstTerm);

    if(firstIt!=keywordMap_.end()){
        matches = firstIt->second;
    }
    
    for(size_t i=1; i<terms.size(); i++){
        string term = convToLower(terms[i]);
        set<Product*> current;

        map<string, set<Product*>>::iterator it = keywordMap_.find(term);
        if(it!=keywordMap_.end()){
            current=it->second;
        }

        if(type==0){
            matches = setIntersection(matches,current);
        }
        else{
            matches = setUnion(matches,current);
        }
    }

    for(set<Product*>::iterator it=matches.begin(); it!=matches.end(); ++it){
        result.push_back(*it);
    }
    return result;
}

void MyDataStore::dump(ostream& ofile){
    ofile<<"<products>"<<endl;

    for(size_t i=0; i<products_.size(); i++){
        products_[i]->dump(ofile);
    }

    ofile<< "</products>" << endl;
    ofile<< "<users>" << endl;

    for(size_t i=0; i<users_.size(); i++){
        users_[i]->dump(ofile);
    }

    ofile<<"</users>" << endl;
}

bool MyDataStore::addToCart(string username, Product* p){
    username = convToLower(username);
    for (size_t i=0; i<users_.size(); i++){
        if(convToLower(users_[i]->getName())==username){
            carts_[username].push_back(p);
            return true;
        }
    }
    return false;
}

bool MyDataStore::viewCart(string username){
    username = convToLower(username);
    bool foundUser = false;
    
    for(size_t i=0; i<users_.size(); i++){
        if(convToLower(users_[i]->getName()) == username){
            foundUser = true;
            break;
        }
    }

    if(!foundUser){
        return false;
    }

    vector<Product*>& cart = carts_[username];

    for(size_t i=0; i<cart.size(); i++){
        cout<<"Item "<< i+1<<endl;
        cout<<cart[i]->displayString()<<endl;
        cout << endl;
    }
    return true;
}

bool MyDataStore::buyCart(string username){
    username = convToLower(username);
    User* user = NULL;
    
    for(size_t i=0; i<users_.size(); i++){
        if(convToLower(users_[i]->getName()) == username){
            user = users_[i];
            break;
        }
    }

    if(user == NULL){
        return false;
    }

    vector<Product*>& cart = carts_[username];

    for(vector<Product*>::iterator it=cart.begin(); it!=cart.end();){
        Product* p = *it;
        if(p->getQty() > 0 && user->getBalance()>=p->getPrice()){
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
            it=cart.erase(it);
        }
        else{
            ++it;
        }
    }
    return true;
}