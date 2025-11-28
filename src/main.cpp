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

//FOR ANDROID BUILD ONLY
const char* KATAKANAS_HACKY_EMBEDDED = R"(
[
  {
    "kana": "ア",
    "roumaji": "a",
    "type": "gojuuon"
  },
  {
    "kana": "イ",
    "roumaji": "i",
    "type": "gojuuon"
  },
  {
    "kana": "ウ",
    "roumaji": "u",
    "type": "gojuuon"
  },
  {
    "kana": "エ",
    "roumaji": "e",
    "type": "gojuuon"
  },
  {
    "kana": "オ",
    "roumaji": "o",
    "type": "gojuuon"
  },
  {
    "kana": "カ",
    "roumaji": "ka",
    "type": "gojuuon"
  },
  {
    "kana": "キ",
    "roumaji": "ki",
    "type": "gojuuon"
  },
  {
    "kana": "ク",
    "roumaji": "ku",
    "type": "gojuuon"
  },
  {
    "kana": "ケ",
    "roumaji": "ke",
    "type": "gojuuon"
  },
  {
    "kana": "コ",
    "roumaji": "ko",
    "type": "gojuuon"
  },
  {
    "kana": "サ",
    "roumaji": "sa",
    "type": "gojuuon"
  },
  {
    "kana": "シ",
    "roumaji": "shi",
    "type": "gojuuon"
  },
  {
    "kana": "ス",
    "roumaji": "su",
    "type": "gojuuon"
  },
  {
    "kana": "セ",
    "roumaji": "se",
    "type": "gojuuon"
  },
  {
    "kana": "ソ",
    "roumaji": "so",
    "type": "gojuuon"
  },
  {
    "kana": "タ",
    "roumaji": "ta",
    "type": "gojuuon"
  },
  {
    "kana": "チ",
    "roumaji": "chi",
    "type": "gojuuon"
  },
  {
    "kana": "ツ",
    "roumaji": "tsu",
    "type": "gojuuon"
  },
  {
    "kana": "テ",
    "roumaji": "te",
    "type": "gojuuon"
  },
  {
    "kana": "ト",
    "roumaji": "to",
    "type": "gojuuon"
  },
  {
    "kana": "ナ",
    "roumaji": "na",
    "type": "gojuuon"
  },
  {
    "kana": "ニ",
    "roumaji": "ni",
    "type": "gojuuon"
  },
  {
    "kana": "ヌ",
    "roumaji": "nu",
    "type": "gojuuon"
  },
  {
    "kana": "ネ",
    "roumaji": "ne",
    "type": "gojuuon"
  },
  {
    "kana": "ノ",
    "roumaji": "no",
    "type": "gojuuon"
  },
  {
    "kana": "ハ",
    "roumaji": "ha",
    "type": "gojuuon"
  },
  {
    "kana": "ヒ",
    "roumaji": "hi",
    "type": "gojuuon"
  },
  {
    "kana": "フ",
    "roumaji": "hu",
    "type": "gojuuon"
  },
  {
    "kana": "ヘ",
    "roumaji": "he",
    "type": "gojuuon"
  },
  {
    "kana": "ホ",
    "roumaji": "ho",
    "type": "gojuuon"
  },
  {
    "kana": "マ",
    "roumaji": "ma",
    "type": "gojuuon"
  },
  {
    "kana": "ミ",
    "roumaji": "mi",
    "type": "gojuuon"
  },
  {
    "kana": "ム",
    "roumaji": "mu",
    "type": "gojuuon"
  },
  {
    "kana": "メ",
    "roumaji": "me",
    "type": "gojuuon"
  },
  {
    "kana": "モ",
    "roumaji": "mo",
    "type": "gojuuon"
  },
  {
    "kana": "ヤ",
    "roumaji": "ya",
    "type": "gojuuon"
  },
  {
    "kana": "ユ",
    "roumaji": "yu",
    "type": "gojuuon"
  },
  {
    "kana": "ヨ",
    "roumaji": "yo",
    "type": "gojuuon"
  },
  {
    "kana": "ラ",
    "roumaji": "ra",
    "type": "gojuuon"
  },
  {
    "kana": "リ",
    "roumaji": "ri",
    "type": "gojuuon"
  },
  {
    "kana": "ル",
    "roumaji": "ru",
    "type": "gojuuon"
  },
  {
    "kana": "レ",
    "roumaji": "re",
    "type": "gojuuon"
  },
  {
    "kana": "ロ",
    "roumaji": "ro",
    "type": "gojuuon"
  },
  {
    "kana": "ワ",
    "roumaji": "wa",
    "type": "gojuuon"
  },
  {
    "kana": "ヲ",
    "roumaji": "wo",
    "type": "gojuuon"
  },
  {
    "kana": "ン",
    "roumaji": "n",
    "type": "gojuuon"
  }
])";

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

    // deck = loadDeck("data/katakanas.json");
    deck = json::parse(KATAKANAS_HACKY_EMBEDDED);
    deckSize = deck.size();

    auto app = MotherWindow::create();

    app->on_fetch_card_from_cpp([app]() {
        CardData newCard = getRandomCard();
        app->set_current_card(newCard);
    });

    app->run();

    return 0;
}
