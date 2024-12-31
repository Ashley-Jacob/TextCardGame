#include "CardGame.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>

using namespace std;

// ----------------- Card

Card :: Card(string val, string color) : value(val), suit(color) {}

void Card :: DisplayCard() {
    cout << GetSuit() << " " << GetValue() << endl;
}

// ----------------- WildCard

WildCard :: WildCard(string name = "Wild") : Card(name,"Black") {}

void WildCard :: PlayEffect(Player* player) {
    string choice = player->Wild();
    SwitchSuit(choice);
}

// ----------------- GiftCard

GiftCard :: GiftCard(string color) : Card("Gift", color), giftGiven(false) {}

void GiftCard :: PlayEffect(Player* player) {
    giftedCard = player->GetGift();
    giftGiven = false;
}

// ----------------- SkipCard

SkipCard :: SkipCard(string color) : Card("Skip", color), skipped(false) {}

// ----------------- VortexCard

VortexCard :: VortexCard() : WildCard("Vortex"), vortexUsed(false) {}

void VortexCard :: PlayEffect(Player* player) {
    WildCard::PlayEffect(player);
}

// ----------------- DrawCard

DrawCard :: DrawCard(string color, int num) : Card("Draw " + to_string(num), color), cardsToDraw(num), cardsDrawn(false) {}

// ----------------- Deck

Deck :: Deck() {Reset();};

void Deck :: Reset() {
    deck.clear();
    vector<string> suits = {"Red", "Blue", "Green", "Yellow"};
    for (auto x : suits) {
        for (int i = 0; i <5; i++) {
            deck.push_back(new Card(to_string(i), x));
        }
        deck.push_back(new GiftCard(x));
        deck.push_back(new GiftCard(x));
        deck.push_back(new SkipCard(x));
        deck.push_back(new DrawCard(x, 3));
    }
    for (int i = 0; i < 2; i++) {
        deck.push_back(new WildCard());
        deck.push_back(new VortexCard());
    }
    Shuffle();

    cout << "Deck Reset" << endl;
}

void Deck :: Shuffle() {
    random_shuffle(deck.begin(), deck.end());
};

Card* Deck :: Draw() {
    if(deck.size() <= 0) Reset();
    Card* drawnCard = deck.back();
    deck.pop_back();
    return drawnCard;
}

// ----------------- Hand

Card* Hand :: GetCard(int index) {
    return hand[index];
}

void Hand :: Discard(int index) {
    hand.erase(hand.begin()+index);    
}

void Hand :: AddCard(Card* newCard) {
    if(newCard) hand.push_back(newCard);
}

// ----------------- Player

void Player :: Draw(int num) {
    for(int i = 0; i < num; i++) {
        playerHand->AddCard(gameDeck->Draw());
    }
}

// ----------------- HumanPlayer

HumanPlayer :: HumanPlayer(Deck* deck, string name) : Player(deck, name) {}

void HumanPlayer :: TakeTurn(GameHandler &game) {
    game.ShowHandSize();
    cout << endl;
    cout << "Active Card is ";
    game.GetActiveCard()->DisplayCard();
    cout << endl;
    game.PrintPlayerHand();

    int choice;
    cin >> choice;

    while(true) {
        while(choice < 0 || choice > GetHand()->GetHandSize()) {
            cout << "Please type a number corresponding to a card: ";
            cin >> choice;
        }

        if (choice == 0) {
            Draw(1);
            cout << GetName() << " drew a card" << endl;
            break;
        }

        Card* cardToPlay = GetHand()->GetCard(choice - 1);

        if(game.PlayCard(cardToPlay)) {
            GetHand()->Discard(choice - 1);
            cout << GetName() << " played ";
            cardToPlay->DisplayCard();
            cardToPlay->PlayEffect(this);
            break;
        }

        cout << "Card is invalid. Pick another one or draw a card. ";
        cin >> choice;
    }

    cout << endl;
}

string HumanPlayer :: Wild() {
    vector<string> colors = {"Red", "Blue", "Green", "Yellow"};

    cout << "Pick a color:" << endl;
    for(int i = 1; i <= 4; i++) cout << "[" << i << "] " << colors[i-1] << endl;

    int choice;
    cin >> choice;

    while(choice < 1 || choice > 4) {
        cout << "Please pick a valid color." << endl;
        cin >> choice;
    }

    cout << GetName() << " switched the color to " << colors[choice - 1] << endl;

    return colors[choice - 1];
}

Card* HumanPlayer :: GetGift() {

    int handSize = GetHand()->GetHandSize();
    if(handSize == 0) return nullptr;

    cout << "Select a card to gift:" << endl;
    PrintHand();

    int choice;
    cin >> choice;
    while(choice < 1 || choice > handSize) {
        cout << "Please select a valid card" << endl;
        cin >> choice;
    }

    Card* gift = GetHand()->GetCard(choice - 1);
    GetHand()->Discard(choice - 1);
    return gift;
}

void HumanPlayer :: PrintHand() {
    int i = 0;
    for (auto card : GetHand()->GetHand()) {
        cout << "[" << i + 1 << "] ";
        card->DisplayCard();
        i++;
    }
}

// ----------------- CPU

CPU :: CPU(Deck* deck, string name) : Player(deck, name) {}

void CPU :: TakeTurn(GameHandler &game) {
    for (int i = 0; i < playerHand->GetHandSize(); i++) {
        Card* cardToPlay = playerHand->GetCard(i);
        if(game.PlayCard(cardToPlay)) {
            playerHand->Discard(i);
            cout << GetName() << " played ";
            cardToPlay->DisplayCard();
            cardToPlay->PlayEffect(this);
            return;
        }
    }

    cout << GetName() << " drew a card " << endl;
    Draw(1);
}

string CPU :: Wild() {
    vector<string> colors = {"Red", "Blue", "Green", "Yellow"};
    string color = colors[rand() % 4];
    cout << GetName() << " switched the color to " << color << "!" << endl;
    return color;
}

Card* CPU :: GetGift() {
    int handSize = GetHand()->GetHandSize();
    if(handSize == 0) return nullptr;
    int index = rand() % handSize;
    Card* gift = GetHand()->GetCard(index);
    GetHand()->Discard(index);
    return gift;
}

// ----------------- GameHandler

GameHandler :: GameHandler(int numPlayers) : numPlayers(numPlayers), gameOver(false), current(0) {
    players.push_back(new HumanPlayer(&deck, "Player"));
    for (int i = 1; i < numPlayers; i++) {
        players.push_back(new CPU(&deck, "CPU " + to_string(i)));
    }

    activeCard = deck.Draw();
    while(activeCard->GetSuit() == "Black") activeCard = deck.Draw();
} 

GameHandler :: ~GameHandler() {
    for (auto player : players) {
        delete player;
    }
}

void GameHandler :: ShowHandSize() {
    cout << endl;
    cout << "Cards per player" << endl;
    for(int i = 0; i < numPlayers; i++) {
        cout << players[i]->GetName() << ": " << players[i]->GetHand()->GetHandSize() << endl;
    }
}

void GameHandler :: PrintPlayerHand() {
    dynamic_cast<HumanPlayer*>(players[0])->PrintHand();
}

void GameHandler :: AdvanceTurn() {
    if(players[current]->GetHand()->GetHandSize() <= 0) {
        rankings.push_back(players[current]->GetName());
        players.erase(players.begin() + current);
        numPlayers--;

        if(current == 0) EndGameEarly();
    }
    else current = (current + 1) % numPlayers;

    if(numPlayers <= 1) gameOver = true;
    if(players[0]->GetHand()->GetHandSize() <= 0) gameOver = true;

    if(activeCard->GetValue() == "Gift") {
        HandleGift(dynamic_cast<GiftCard*>(activeCard));
    }
    if(activeCard->GetValue() == "Skip") {
        HandleSkip(dynamic_cast<SkipCard*>(activeCard));
    }
    if(activeCard->GetValue() == "Vortex") {
        HandleVortex(dynamic_cast<VortexCard*>(activeCard));
    }
    if(activeCard->GetValue().find("Draw") != std::string::npos) {
        HandleDraw(dynamic_cast<DrawCard*>(activeCard));
    }
}

void GameHandler :: PrintResults() {
    for(int i = 0; i < rankings.size(); i++) {
        cout << i + 1 << ". " << rankings[i] << endl;
    }

    if(rankings[0] == "Player") {
        cout << "Congratulations! You won!" << endl;
    }
    else cout << "Better luck next time!" << endl;
}

void GameHandler :: EndGameEarly() {
    map<int, vector<string>> playersLeft;

    for(auto x : players) {
        playersLeft[x->GetHand()->GetHandSize()].push_back(x->GetName());
    }

    for(auto x : playersLeft) {
        string place = "";
        int numAtRank = 0;
        for (string player : get<1>(x)) {
            if(numAtRank > 0) place += ", ";
            place += player;
            numAtRank++;
        }
        rankings.push_back(place + " (" + to_string(get<0>(x)) + " card(s) left)");
    }

    EndGame();
}

void GameHandler :: HandleGift(GiftCard* active) {
    if(!active->isGiftGiven()) {
        players[current]->GetHand()->AddCard(active->ReceiveGift(players[current]));
        cout << players[current]->GetName() << " received a gifted card" << endl;
    }
}

void GameHandler :: HandleSkip(SkipCard* active) {
    if(!active->GetSkipped()) {
        cout << players[current]->GetName() << " was skipped" << endl;
        current = (current + 1) % numPlayers;
        active->Skip();
    }
}

void GameHandler :: HandleVortex(VortexCard* active) {
    if(!active->GetVortexStatus()) {
        int received = 0;
        for(int i = 0; i<players.size(); i++) {
            if(i == current) continue;
            players[current]->GetHand()->AddCard((players[i]->GetGift()));
        }
        active->UseVortex();
        cout << players[current]->GetName() << " received cards from everyone" << endl;
    }
}

void GameHandler :: HandleDraw(DrawCard* active) {
    if(!active->GetCardsDrawn()) {
        players[current]->Draw(active->GetNumCards());
        cout << players[current]->GetName() << " drew " << active->GetNumCards() << " card(s)";
        active->DrawCards();
    }
}