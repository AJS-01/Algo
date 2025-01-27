#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

// Structure representing a player in the leaderboard
struct Player {
    string name;   // Player's name
    int score;     // Player's score
    Player* next;  // Pointer to the next player in the list

    // Default constructor
    Player() : name(""), score(0), next(nullptr) {}

    // Parameterized constructor
    Player(const string& name, int score) : name(name), score(score), next(nullptr) {}
};

const int TOP_10 = 10;       // Number of top players to display
const int MAX_PLAYERS = 25;  // Maximum number of players allowed

// Class representing the leaderboard system
class Leaderboard {
private:
    Player* head;                // Head of the linked list of players
    Player topPlayers[TOP_10];   // Array to store top 10 players
    int playerCount;             // Current count of players in the leaderboard

    // Sorts the linked list of all players in descending order of scores
    void sortAllPlayers() {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            Player* current = head;
            Player* prev = nullptr;

            while (current && current->next) {
                if (current->score < current->next->score) {

                    if (prev) {
                        prev->next = current->next;
                    }
                    else {
                        head = current->next;
                    }
                    Player* temp = current->next->next;
                    current->next->next = current;
                    current->next = temp;

                    swapped = true;
                }
                prev = current;
                current = current->next;
            }
        } while (swapped);
    }

    // Updates the array of top 10 players based on the current leaderboard
    void updateTopPlayers() {
        sortAllPlayers();  // Ensure the list is sorted
        Player* current = head;
        for (int i = 0; i < TOP_10; i++) {
            if (current) {
                topPlayers[i] = *current;
                current = current->next;
            }
            else {
                topPlayers[i] = Player();
            }
        }
    }

public:
    // Constructor to initialize the leaderboard
    Leaderboard() : head(nullptr), playerCount(0) {
        addExistingPlayers();  // Add some initial players
    }

    ~Leaderboard() {
        while (head) {
            Player* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Adds some predefined players to the leaderboard
    void addExistingPlayers() {
        string names[10] = { "Kurt", "Jeff", "Nahida", "LinkinFork", "Eve", "WalterW", "MrBeast", "Batman", "Nuggies", "KSI" };
        int scores[10] = { 50, 75, 23, 85, 37, 92, 43, 69, 74, 49 };

        for (int i = 0; i < 10; i++) {
            addOrUpdatePlayer(names[i], scores[i]);
        }
    }

    // Adds a new player or updates an existing player's score
    void addOrUpdatePlayer(const string& name, int score) {
        if (score < 0 || score > 100) {
            throw invalid_argument("Score must be between 0 and 100.");
        }

        // Check if the player already exists
        Player* current = head;
        while (current) {
            if (current->name == name) {
                current->score = score; // Update score
                return;
            }
            current = current->next;
        }

        // Add a new player if there's space
        if (playerCount >= MAX_PLAYERS) {
            cout << "Maximum players (" << MAX_PLAYERS << ") reached. Cannot add more players." << endl;
            return;
        }

        Player* newPlayer = new Player(name, score);
        newPlayer->next = head; // Add to the beginning of the list
        head = newPlayer;
        playerCount++;
    }

    // Displays the top 10 players
    void displayTop10() {
        updateTopPlayers(); // Update the top players array
        system("CLS");      // Clear screen
        cout << "\n-----------------------------------------------\n";
        cout << "\n               Top 10 Players:\n";
        cout << "\n-----------------------------------------------\n";
        for (int i = 0; i < TOP_10; i++) {
            if (!topPlayers[i].name.empty()) {
                cout << (i + 1) << ". " << topPlayers[i].name << " - " << topPlayers[i].score << endl;
            }
        }
        saveLeaderboard();
    }

    // Displays all players with their ranks
    void displayAllPlayers() {
        system("CLS");
        sortAllPlayers();
        cout << "\n-----------------------------------------------\n";
        cout << "\n                All Players:\n";
        cout << "\n-----------------------------------------------\n";
        int rank = 1;
        Player* current = head;
        while (current) {
            cout << rank << ". " << current->name << " - " << current->score << endl;
            current = current->next;
            rank++;
        }
        saveLeaderboard();
    }

    // Saves the leaderboard to a file with ranks
    void saveLeaderboard() {
        ofstream file("leaderboard.txt");
        if (file.is_open()) {
            file << "-----------------------------------------------\n";
            file << "\n                  Leaderboard\n";
            file << "\n-----------------------------------------------\n";

            sortAllPlayers(); // Ensure the list is sorted before saving
            Player* current = head;
            int rank = 1;    // Rank counter
            while (current) {
                file << rank << ". " << current->name << " - " << current->score << endl;
                current = current->next;
                rank++;
            }
            file.close();
        }
        else {
            cout << "Error opening file!" << endl;
        }
    }
};

int main() {
    Leaderboard lb;
    string playerName;
    int playerScore, choice;

    cout << "\n-----------------------------------------------\n";
    cout << "\n   Welcome to the Game Leaderboard System!\n";

    do {
        cout << "\n-----------------------------------------------\n";
        cout << "                    Menu\n";
        cout << "-----------------------------------------------\n";
        cout << "[1] Add or Update Player\n[2] Show Top 10 Players\n[3] Show All Players\n[4] Exit\n";
        cout << "-----------------------------------------------\nEnter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid input. Please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }

        cin.ignore();  // Clear the input buffer

        switch (choice) {
        case 1: // Add or update a player
            cout << "\nEnter player name: ";
            getline(cin, playerName);

            do {
                cout << "Enter player score (0-100): ";
                cin >> playerScore;

                if (cin.fail() || playerScore < 0 || playerScore > 100) {
                    cout << "Error: Score must be between 0 and 100.\n";
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                }
            } while (playerScore < 0 || playerScore > 100);

            lb.addOrUpdatePlayer(playerName, playerScore);
            break;

        case 2: // Show top 10 players
            lb.displayTop10();
            break;

        case 3: // Show all players
            lb.displayAllPlayers();
            break;

        case 4: // Exit the program
            cout << "\nSaving leaderboard and exiting program. Goodbye!\n";
            lb.saveLeaderboard();
            return 0;  // Exit the program
        }

        if (choice != 4) {
            cout << "\nPress ENTER to return to the menu...\n";
            cin.ignore(INT_MAX, '\n'); // Wait for ENTER
        }

        system("CLS"); // Clear screen for next menu
    } while (choice != 4);

    return 0;
}