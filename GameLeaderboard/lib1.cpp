#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

struct Player {
    string name;
    int score;
    Player* next;

    Player() : name(""), score(0), next(nullptr) {}

    Player(const string& name, int score) : name(name), score(score), next(nullptr) {}
};

const int TOP_10 = 10;
const int MAX_PLAYERS = 25;

class Leaderboard {
private:
    Player* head;
    Player topPlayers[TOP_10];
    int playerCount;

public:
    Leaderboard() : head(nullptr), playerCount(0) {
        addExistingPlayers();
    }

    ~Leaderboard() {
        while (head) {
            Player* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addExistingPlayers() {
        string names[10] = { "Kurt", "Jeff", "Nahida", "LinkinFork", "Eve", "WalterW", "MrBeast", "Batman", "Nuggies", "KSI" };
        int scores[10] = { 50, 75, 23, 85, 37, 92, 43, 69, 74, 49 };

        for (int i = 0; i < 10; i++) {
            addOrUpdatePlayer(names[i], scores[i]);
        }
    }

    void addOrUpdatePlayer(const string& name, int score) {
        if (score < 0 || score > 100) {
            throw invalid_argument("Score must be between 0 and 100.");
        }

        Player* current = head;
        while (current) {
            if (current->name == name) {
                current->score = score;
                updateTopPlayers();
                return;
            }
            current = current->next;
        }

        if (playerCount >= MAX_PLAYERS) {
            cout << "Maximum players (" << MAX_PLAYERS << ") reached. Cannot add more players." << endl;
            return;
        }

        Player* newPlayer = new Player(name, score);
        newPlayer->next = head;
        head = newPlayer;
        playerCount++;
        updateTopPlayers();
    }

    void updateTopPlayers() {
        for (int i = 0; i < TOP_10; i++) {
            topPlayers[i].score = -1;
        }

        Player* current = head;
        while (current) {
            int pos = -1;
            for (int j = 0; j < TOP_10; j++) {
                if (topPlayers[j].score == -1 || current->score > topPlayers[j].score) {
                    pos = j;
                    break;
                }
            }
            if (pos != -1) {
                for (int k = TOP_10 - 1; k > pos; k--) {
                    topPlayers[k] = topPlayers[k - 1];
                }
                topPlayers[pos].name = current->name;
                topPlayers[pos].score = current->score;
            }
            current = current->next;
        }
    }

    void displayTop10() {
        cout << "\nTop 10 Players:\n";
        for (int i = 0; i < TOP_10; i++) {
            if (topPlayers[i].score != -1) {
                cout << (i + 1) << ". " << topPlayers[i].name << " - " << topPlayers[i].score << endl;
            }
        }
    }

    void displayAllPlayers() {
        cout << "\nAll Players:\n";
        Player* current = head;
        while (current) {
            cout << current->name << " - " << current->score << endl;
            current = current->next;
        }
    }

    void saveLeaderboard() {
        ofstream file("leaderboard.txt");
        if (file.is_open()) {
            for (int i = 0; i < TOP_10; i++) {
                if (topPlayers[i].score != -1) {
                    file << topPlayers[i].name << " " << topPlayers[i].score << endl;
                }
            }
            file.close();
            cout << "Leaderboard saved to leaderboard.txt." << endl;
        }
        else {
            cout << "Error opening file!" << endl;
        }
    }
    
    
};

int main() {
    Leaderboard lb;
    string playerName;
    int playerScore;
    int choice;

	cout << "\n-----------------------------------------------\n";
    cout <<"\n   Welcome to the Game Leaderboard System!" << endl;

    while (true) {
        cout << "\n-----------------------------------------------\n";
		cout << "                    Menu\n";
		cout << "-----------------------------------------------\n";
        cout << "[1] Add or Update Player\n";
        cout << "[2] Show Top 10 Players\n";
        cout << "[3] Show All Players\n";
        cout << "[4] Exit\n";
        cout << "-----------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\nEnter player name: ";
            cin >> playerName;
            cout << "Enter player score: ";
            cin >> playerScore;
			
			
			//for special chars/words
            if (cin.fail() || playerScore < 0 || playerScore > 100) {
                cout << "Error: Score must be between 0 and 100." << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
            }

            try {
                lb.addOrUpdatePlayer(playerName, playerScore);
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            break;
        case 2:
            lb.displayTop10();
            break;
        case 3:
            lb.displayAllPlayers();
            break;
        case 4:
            cout << "\nSaving leaderboard and exiting program. Goodbye!" << endl;
            lb.saveLeaderboard();
            return 0;
        default:
            cout << "\nInvalid choice, please try again." << endl;
        }
        cout << "\n-----------------------------------------------\n";
        cout << "\nPress ENTER to return to the menu...";
        cin.ignore();
        cin.get();
    }

    return 0;
    
}
