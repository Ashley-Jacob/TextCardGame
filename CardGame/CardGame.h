#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Player;

class Card {
public:
    Card(string val, string color);
    virtual ~Card() {};
    string GetValue() {return value;}
    string GetSuit() {return suit;}
    virtual void DisplayCard();
    virtual void PlayEffect(Player* player) {}

protected:
    string value;
    string suit;
};

class WildCard : public Card {
public:
    WildCard(string name);
    virtual void PlayEffect(Player* player);
    virtual void SwitchSuit(string color) {suit = color;}
};

class GiftCard : public Card {
public:
    GiftCard(string color);
    virtual void PlayEffect(Player* player);
    Card* ReceiveGift(Player* player) {return giftedCard;}
    bool isGiftGiven() {return giftGiven;}
private:
    bool giftGiven;
    Card* giftedCard;
};

class SkipCard : public Card {
public:
    SkipCard(string color);
    void Skip() {skipped = true;}
    bool GetSkipped() {return skipped;}
private:
    bool skipped;
};

class VortexCard : public WildCard {
public:
    VortexCard();
    virtual void PlayEffect(Player* player);
    bool GetVortexStatus() {return vortexUsed;}
    void UseVortex() {vortexUsed = true;}
private:
    bool vortexUsed;
};

class DrawCard : public Card {
public:
    DrawCard(string color, int num);
    int GetNumCards() {return cardsToDraw;}
    bool GetCardsDrawn() {return cardsDrawn;}
    void DrawCards() {cardsDrawn = true;}
private:
    int cardsToDraw;
    bool cardsDrawn;
};

class Deck {
public:
    Deck();
    ~Deck() {};
    void Reset();
    void Shuffle();
    Card* Draw();
    int CardsLeft() {return deck.size();}

private:
    vector<Card*> deck;
};

class Hand {
public:
    Hand() {}
    ~Hand() {}
    void Discard(int index);
    Card* GetCard(int index);
    void Draw(Deck deck);
    vector<Card*> GetHand() {return hand;}
    int GetHandSize() {return hand.size();}
    void AddCard(Card* card);

private:
    vector<Card*> hand;
};

class GameHandler;

class Player {
public:
    Player(Deck* deck, string name) : playerHand(new Hand()), gameDeck(deck), name(name) {Draw(5);}
    virtual ~Player() {delete playerHand;}
    virtual Hand* GetHand() {return playerHand;}
    void Draw(int num);
    virtual void TakeTurn(GameHandler &game) = 0;
    virtual string Wild() = 0;
    string GetName() {return name;}
    virtual Card* GetGift() = 0;
protected:
    Hand* playerHand;
    Deck* gameDeck;
    string name;
};

class CPU : public Player {
public:
    CPU(Deck* deck, string name);
    virtual void TakeTurn(GameHandler &game);
    virtual string Wild();
    virtual Card* GetGift();
private:
};

class HumanPlayer : public Player {
public:
    HumanPlayer(Deck* deck, string name);
    virtual void TakeTurn(GameHandler &game);
    virtual string Wild();
    virtual Card* GetGift();
    void PrintHand();
private:
};

class GameHandler {
public:
    GameHandler(int numPlayers);
    ~GameHandler();
    void ShowHandSize();
    void PrintPlayerHand();
    bool PlayCard(Card* card) {
        if(card->GetSuit() == "Black") {activeCard = card; return true;}
        if(card->GetSuit() == activeCard->GetSuit()) {activeCard = card; return true;}
        if(card->GetValue() == activeCard->GetValue()) {activeCard = card; return true;}
        return false;
    }
    Card* GetActiveCard() {return activeCard;}
    bool IsGameOver() {return gameOver;}
    vector<Player*> GetPlayers() {return players;}
    void AdvanceTurn();
    Player* GetCurrentPlayer() {return players[current];}
    void PrintResults();
    void EndGame() {gameOver = true;}
    void EndGameEarly();
    void HandleGift(GiftCard* active);
    void HandleSkip(SkipCard* active);
    void HandleVortex(VortexCard* active);
    void HandleDraw(DrawCard* active);

private:
    int numPlayers;
    vector<Player*> players;
    Deck deck;
    Card* activeCard;
    bool gameOver;
    int current;
    vector<string> rankings;
};