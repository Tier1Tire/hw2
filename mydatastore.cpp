#include "mydatastore.h"
#include "util.h"
#include <iostream>

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for(std::size_t i = 0; i < products_.size(); ++i)
    {
        delete products_[i];
    }
    for(std::size_t i = 0; i < users_.size(); ++i)
    {
        delete users_[i];
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);
    std::set<std::string> words = p->keywords();
    for(std::set<std::string>::iterator it = words.begin();
        it != words.end(); ++it)
    {
        keywordIndex_[convToLower(*it)].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    users_.push_back(u);
    std::string username = convToLower(u->getName());
    userIndex_[username] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms,
                                         int type)
{
    std::vector<Product*> hits;
    if(terms.empty() || (type != 0 && type != 1))
    {
        return hits;
    }

    std::set<Product*> matches;
    for(std::size_t i = 0; i < terms.size(); ++i)
    {
        std::string term = convToLower(terms[i]);
        std::map<std::string, std::set<Product*> >::iterator found =
            keywordIndex_.find(term);
        std::set<Product*> current;
        if(found != keywordIndex_.end())
        {
            current = found->second;
        }

        if(i == 0)
        {
            matches = current;
        }
        else if(type == 0)
        {
            matches = setIntersection(matches, current);
        }
        else
        {
            matches = setUnion(matches, current);
        }

        // An AND search cannot regain matches once its intersection is empty.
        if(type == 0 && matches.empty())
        {
            return hits;
        }
    }

    for(std::set<Product*>::iterator it = matches.begin();
        it != matches.end(); ++it)
    {
        hits.push_back(*it);
    }
    return hits;
}

void MyDataStore::dump(std::ostream& ofile)
{
    ofile << "<products>\n";
    for(std::size_t i = 0; i < products_.size(); ++i)
    {
        products_[i]->dump(ofile);
    }
    ofile << "</products>\n<users>\n";
    for(std::size_t i = 0; i < users_.size(); ++i)
    {
        users_[i]->dump(ofile);
    }
    ofile << "</users>\n";
}

bool MyDataStore::addToCart(const std::string& username, const std::vector<Product*>& hits, int hitIndex)
{
    std::string key = convToLower(username);
    if(userIndex_.find(key) == userIndex_.end() ||
       hitIndex < 1 || static_cast<std::size_t>(hitIndex) > hits.size())
    {
        std::cout << "Invalid request" << std::endl;
        return false;
    }

    carts_[key].push_back(hits[hitIndex - 1]);
    return true;
}

bool MyDataStore::viewCart(const std::string& username)
{
    std::string key = convToLower(username);
    if(userIndex_.find(key) == userIndex_.end())
    {
        std::cout << "Invalid username" << std::endl;
        return false;
    }

    const std::vector<Product*>& cart = carts_[key];
    for(std::size_t i = 0; i < cart.size(); ++i)
    {
        std::cout << "Item " << i + 1 << "\n";
        std::cout << cart[i]->displayString() << "\n\n";
    }
    return true;
}

bool MyDataStore::buyCart(const std::string& username)
{
    std::string key = convToLower(username);
    std::map<std::string, User*>::iterator found = userIndex_.find(key);
    if(found == userIndex_.end())
    {
        std::cout << "Invalid username" << std::endl;
        return false;
    }

    User* user = found->second;
    std::vector<Product*>& cart = carts_[key];
    std::vector<Product*> remaining;
    for(std::size_t i = 0; i < cart.size(); ++i)
    {
        Product* product = cart[i];
        if(product->getQty() > 0 && user->getBalance() >= product->getPrice())
        {
            product->subtractQty(1);
            user->deductAmount(product->getPrice());
        }
        else
        {
            remaining.push_back(product);
        }
    }
    // Replace the cart only after iteration; preserve the order of unbought items.
    cart = remaining;
    return true;
}