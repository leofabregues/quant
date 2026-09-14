#include <iostream>
#include <map>
#include <vector>
#include <string>

struct Order {
    int id;
    double price;
    int quantity;
    bool isBuy; // true = Achat, false = Vente
};

class OrderBook {
private:
    // On utilise std::map pour trier automatiquement par prix
    // greater pour les acheteurs (plus haut prix d'abord)
    // less pour les vendeurs (plus bas prix d'abord)
    std::map<double, int, std::greater<double>> bids; 
    std::map<double, int> asks;

public:
    void addOrder(double price, int qty, bool isBuy) {
        if (isBuy) {
            bids[price] += qty;
            std::cout << "Ordre d'ACHAT place : " << qty << " @ " << price << std::endl;
        } else {
            asks[price] += qty;
            std::cout << "Ordre de VENTE place : " << qty << " @ " << price << std::endl;
        }
        match(); // On essaie de croiser les ordres
    }

    void match() {
        // Si le meilleur acheteur offre plus que le meilleur vendeur
        while (!bids.empty() && !asks.empty() && bids.begin()->first >= asks.begin()->first) {
            double bidPrice = bids.begin()->first;
            double askPrice = asks.begin()->first;
            int tradeQty = std::min(bids[bidPrice], asks[askPrice]);

            std::cout << ">>> MATCH : " << tradeQty << " titres exchanges a " << askPrice << " EUR" << std::endl;

            bids[bidPrice] -= tradeQty;
            asks[askPrice] -= tradeQty;

            if (bids[bidPrice] == 0) bids.erase(bidPrice);
            if (asks[askPrice] == 0) asks.erase(askPrice);
        }
    }

    void display() {
        std::cout << "\n--- CARNET D'ORDRES ---\n ASKS (Vente):\n";
        for(auto const& [price, qty] : asks) std::cout << "  " << price << " | " << qty << "\n";
        std::cout << " BIDS (Achat):\n";
        for(auto const& [price, qty] : bids) std::cout << "  " << price << " | " << qty << "\n";
        std::cout << "-----------------------\n";
    }
};

int main() {
    OrderBook lob;
    
    lob.addOrder(100.5, 10, true);  // Acheteur
    lob.addOrder(101.0, 5, false); // Vendeur trop cher
    lob.addOrder(100.0, 20, true);  // Acheteur plus bas
    
    lob.display();

    std::cout << "\nUn vendeur arrive au prix du marche...\n";
    lob.addOrder(100.2, 15, false); // Vendeur qui va matcher avec l'acheteur à 100.5

    lob.display();

    return 0;
}