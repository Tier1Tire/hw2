#include "book.h"
#include "util.h"
#include <sstream>

Book::Book(const std::string category, const std::string name, double price, int qty, const std::string isbn, const std::string author)
    : Product(category, name, price, qty),
      isbn_(isbn), author_(author)
{
}

std::set<std::string> Book::keywords() const
{
    std::set<std::string> result = parseStringToWords(name_);
    std::set<std::string> extra = parseStringToWords(author_);
    for(std::set<std::string>::iterator it = extra.begin();
        it != extra.end(); ++it)
    {
        result.insert(*it);
    }
    result.insert(convToLower(isbn_));
    return result;
}

std::string Book::displayString() const
{
    std::ostringstream output;
    output << name_ << "\n";
    output << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
    output << price_ << " " << qty_ << " left.";
    return output.str();
}

void Book::dump(std::ostream& os) const
{
    Product::dump(os);
    os << isbn_ << "\n" << author_ << "\n";
}
