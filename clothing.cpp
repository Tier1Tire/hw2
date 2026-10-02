#include "clothing.h"
#include "util.h"
#include <sstream>

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand)
    : Product(category, name, price, qty),
      size_(size), brand_(brand)
{
}

std::set<std::string> Clothing::keywords() const
{
    std::set<std::string> result = parseStringToWords(name_);
    std::set<std::string> extra = parseStringToWords(brand_);
    for(std::set<std::string>::iterator it = extra.begin();
        it != extra.end(); ++it)
    {
        result.insert(*it);
    }
    return result;
}

std::string Clothing::displayString() const
{
    std::ostringstream output;
    output << name_ << "\n";
    output << "Size: " << size_ << " Brand: " << brand_ << "\n";
    output << price_ << " " << qty_ << " left.";
    return output.str();
}

void Clothing::dump(std::ostream& os) const
{
    Product::dump(os);
    os << size_ << "\n" << brand_ << "\n";
}