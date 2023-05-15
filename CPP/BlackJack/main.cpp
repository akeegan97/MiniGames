#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

struct Player{
    int wager= 0;
    int handValue = 0;
    int winnings = 0;
    vector<string> hand;
    bool bust = false;
    bool continueHit = true;
    bool play = true;
    void deal(const vector<string> &cards){
        hand.push_back(cards[0]);
        hand.push_back(cards[1]);
    };//dealing the first and second card to the player should be random becuase deck will be shuffled at the begining of each round
    int bet(){
        std::cout<<"Enter Bet Amount: "<<endl;
        int wager;
        cin>>wager;
        return wager;
    };
    int calcHandValue(vector<string> & hand){
        int score = 0;
        int aces = 0;
        for(const auto&card:hand){
            char rank = card[0];
            if (rank=='A'){
                score +=11;
                aces++;
            }else if(rank == 'K' || rank == 'Q' || rank == 'J' || rank =='1'){
                score +=10;
            }else{
                score+=(rank-'0');
            }
        }
        while(score > 21 && aces>0){
            score-=10;
            aces--;
        }
        return score;
    };
    void showHand(){
        cout<<"Player Hand";
        for(const auto&card:hand){
            cout<<" "<<card;
        }
        cout<<""<<endl;
    };
    void hit(vector<string> &cards){
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> distribution(0,52);
        int randomIndex = distribution(rng);
        hand.push_back(cards[randomIndex]);
    };
    void reset(){
        for(const auto &card:hand){
            hand.pop_back();
            continueHit = true;
        }
        wager = 0;
    };
    
};

struct Dealer{
    int handValue =0 ;
    vector<string> hand;
    bool bust = false;
    int calcHandValue(vector<string> & hand){
        int score = 0;
        int aces = 0;
        for(const auto&card:hand){
            char rank = card[0];
            if (rank=='A'){
                score +=11;
                aces++;
            }else if(rank == 'K' || rank == 'Q' || rank == 'J' || rank == '1'){
                score +=10;
            }else{
                score+=(rank-'0');
            }
        }
        while(score > 21 && aces>0){
            score-=10;
            aces--;
        }
        return score;
    };
    void showflop(){
        cout<<"Dealer Flop: ";
        cout<<" "<<hand[0]<<endl;
    };
    void showHand(){
        cout<<"Dealer Hand: ";
        for(const auto&card:hand){
            cout<<" "<<card;
        }
        cout<<""<<endl;
    };
    void deal(vector<string> &cards){
        hand.push_back(cards[4]);
        hand.push_back(cards[5]);
    };
    void draw(vector<string> &cards){
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> distribution(0,52);
        int randomIndex = distribution(rng);
        hand.push_back(cards[randomIndex]);
    };
    void reset(){
        for(const auto &card:hand){
            hand.pop_back();
        }
    };

};

struct Deck{
    vector<string> cards;
    void shuffle(vector<string> &cards){
        random_device rd;
        mt19937 rng(rd());
        std::shuffle(cards.begin(),cards.end(),rng);
    }
};
int main(){
    Deck deck;
    Player p1;
    Dealer d;
    string pcards[52] = {
        "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS",
        "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH",
        "AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC",
        "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD"
    };
    for(auto &card:pcards){
        deck.cards.push_back(card);
    }
    while(p1.play == true ){
    int wonThisRound = 0;
    deck.shuffle(deck.cards);
    p1.wager = p1.bet();
    p1.deal(deck.cards);
    d.deal(deck.cards);
    d.showflop();
    d.handValue = d.calcHandValue(d.hand);

    p1.showHand();
    p1.handValue = p1.calcHandValue(p1.hand);
    cout<<p1.handValue<<endl;
    if(p1.handValue == 21){
        cout<<"Player Hits BlackJack!"<<endl;
    }else{
    while(p1.continueHit == true){
    cout << "Enter h to hit or s to stay" << endl;
    char h_s;
    cin >> h_s;

    while(h_s != 'h' && h_s != 's'){
        cout << "Invalid entry...try again" << endl;
        cout << "Enter h to hit or s to stay" << endl;
        cin >> h_s;
    }

    if (h_s == 'h'){
        p1.hit(deck.cards);
        p1.handValue = p1.calcHandValue(p1.hand);
        if(p1.handValue > 21){
            p1.bust = true;
            p1.continueHit = false;
        }    
    } else {
        p1.continueHit = false;
    }
    p1.showHand();
    cout<<"Player Hand value: "<<p1.handValue<<endl;
    };
    };
    p1.showHand();
    p1.handValue = p1.calcHandValue(p1.hand);
    cout<<p1.handValue<<endl;
    d.showHand();
    d.handValue = d.calcHandValue(d.hand);
    cout<<d.handValue<<endl;

    while(d.handValue < 17){
        d.draw(deck.cards);
        d.handValue = d.calcHandValue(d.hand);
        if(d.handValue > 21){
            d.bust = true;
        }
        d.showHand();
        
    };
    
    cout<<d.handValue<<endl;
    if (p1.bust == true || (d.bust == false && p1.handValue < d.handValue)) {
        cout << "Dealer Wins: Player Busted or Dealer Has Higher Score" << endl;
        wonThisRound = -p1.wager;
    } else if (d.bust == true || (p1.bust == false && p1.handValue > d.handValue)) {
        cout << "Player Wins: Dealer Busted or Player Has Higher Score" << endl;
        wonThisRound = p1.wager * 2;
    } else {
        cout << "Push: Game Drawn" << endl;
        wonThisRound = 0;
    };
    std::cout<<"Continue to Play?"<<endl;
    cout<<"Enter y or n"<<endl;
    char keepPlaying;
    cin>>keepPlaying;
    while(keepPlaying != 'y' && keepPlaying != 'n'){
        cout << "Invalid entry...try again" << endl;
        cout << "Enter y to continue or n to stop" << endl;
        cin >> keepPlaying;
    }

    if (keepPlaying == 'n'){
        p1.play = false;
        p1.winnings+=wonThisRound; 
        cout<<"Winnings: "<<p1.winnings<<endl;   
        
    } else {
        p1.play = true;
        p1.reset();
        d.reset();
        p1.winnings+=wonThisRound;
        cout<<"Winnings: "<<p1.winnings<<endl;
    }

    };

    return 0;
}
