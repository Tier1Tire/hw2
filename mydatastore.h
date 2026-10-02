#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include "datastore.h"

class MyDataStore : public DataStore
{
public:
    MyDataStore();
    virtual ~MyDataStore();

    // The store owns the products and users passed to these functions.
    virtual void addProduct(Product* p);
    virtual void addUser(User* u);
    virtual std::vector<Product*> search(std::vector<std::string>& terms, int type);
    virtual void dump(std::ostream& ofile);

    // hitIndex is the one-based number displayed in the latest search results.
    bool addToCart(const std::string& username,
                   const std::vector<Product*>& hits, int hitIndex);
    bool viewCart(const std::string& username);
    bool buyCart(const std::string& username);

private:
    std::vector<Product*> products_;
    std::vector<User*> users_;
    std::map<std::string, std::set<Product*> > keywordIndex_;
    std::map<std::string, User*> userIndex_;
    std::map<std::string, std::vector<Product*> > carts_;

    // Copying an owning store would cause the same objects to be deleted twice.
    MyDataStore(const MyDataStore& other);
    MyDataStore& operator=(const MyDataStore& other);
};

#endif
