#ifndef JEWELRY_H
#define JEWELRY_H

#include <QString>


class Product {
protected:
    int id;
    QString name;
    double price;

public:
    Product(int id, QString name, double price)
        : id(id), name(name), price(price) {}

    virtual ~Product() {}


    virtual QString getFullType() const = 0;


    int getId() const { return id; }
    QString getName() const { return name; }
    double getPrice() const { return price; }
};


class Jewelry : public Product {
private:
    QString material;
    double weight;
    QString brand;


public:
    Jewelry(int id, QString name, QString material, double weight, double price, QString brand)
        : Product(id, name, price), material(material), weight(weight), brand(brand){}


    QString getFullType() const override {
        return "Прикраса: " + getName() + " (" + material + ")";
    }


    QString getMaterial() const { return material; }
    double getWeight() const { return weight; }
    QString getBrand() const { return brand; }
};

#endif
