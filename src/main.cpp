// OS Specific headers
#include <cstddef>
#include <fstream>
#include <random>
#include <iostream>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
    #include <limits.h>
#endif

#include "slint_string.h"
#include "mother_window.h"

#include "json.hpp"
using json = nlohmann::json; /* You can't make me not use namespaces :b */

std::mt19937 rngDevice;
json deck;
int deckSize;


json loadDeck(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    json data;

    try {
        data = json::parse(file);
    } catch (json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return nullptr;
    }

    return data;
}

CardData getRandomCard() {
    CardData c;
    
    if (deck.empty()) {
        c.front_text = "nocard";
        c.back_text = "nocard";
        return c;
    }

    std::uniform_int_distribution<size_t> dist(0, deckSize - 1);
    size_t rngIndex = dist(rngDevice);

    json selected = deck[rngIndex];

    std::string front_text = selected.value("kana", "0");
    std::string back_text = selected.value("roumaji", "0") + '\n' +  selected.value("type", "0");

    c.front_text = slint::SharedString(front_text);
    c.back_text = slint::SharedString(back_text);

    std::cout << "Picked Index: " << rngIndex << " / " << deckSize << std::endl;
    
    return c;
}

int main()
{
    std::random_device rd;
    rngDevice.seed(rd());

    deck = loadDeck("data/katakanas.json");
    deckSize = deck.size();

    auto app = MotherWindow::create();

    app->on_fetch_card_from_cpp([app]() {
        CardData newCard = getRandomCard();
        app->set_current_card(newCard);
    });

    app->run();

    return 0;
}
