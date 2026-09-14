#include <iostream>
#include <vector>
#include <string>

// Classe de base
class Asset {
public:
    std::string name;
    virtual double get_value() const = 0; // "virtual" car chaque actif calcule son prix différemment
    virtual ~Asset() {}
};

// Classe pour une Action
class Stock : public Asset {
public:
    double price;
    int quantity;
    Stock(std::string n, double p, int q) { name = n; price = p; quantity = q; }
    double get_value() const override { return price * quantity; }
};

int main() {
    // Création d'un portefeuille (un vecteur de pointeurs d'actifs)
    std::vector<Asset*> myPortfolio;

    // Ajout d'actifs
    myPortfolio.push_back(new Stock("Apple", 175.0, 10));
    myPortfolio.push_back(new Stock("Total", 60.0, 50));

    double totalValue = 0;
    std::cout << "--- MON PORTEFEUILLE ---" << std::endl;
    for (const auto& asset : myPortfolio) {
        std::cout << asset->name << " : " << asset->get_value() << " EUR" << std::endl;
        totalValue += asset->get_value();
    }

    std::cout << "\nVALEUR TOTALE : " << totalValue << " EUR" << std::endl;

    // Nettoyage de la mémoire
    for (auto asset : myPortfolio) delete asset;

    return 0;
}