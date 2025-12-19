#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <utility>
#include <fstream>

using namespace std;

// ===== Movie data structure =====
struct Movie {
    int id;        // Unique ID
    string title;  // Movie title
    string rating; // Rating, e.g. G / PG-13 / R
    int duration;  // Duration in minutes
};

// Global movie list
vector<Movie> movies;
int nextMovieId = 1; // Auto-increment ID

// ===== Hall data structure =====
struct Hall {
    int id;      // Unique ID
    string name; // Hall name, e.g. "Hall 1", "IMAX"
    int floor;   // Floor number, e.g. 1, 2, 3
    int rows;    // Number of seat rows
    int cols;    // Number of seat columns
    // Total seats = rows * cols
};

// Global hall list
vector<Hall> halls;
int nextHallId = 1; // Auto-increment ID for halls

// ===== Showtime data structure =====
struct Showtime {
    int id;        // Unique ID
    int movieId;   // ID of the movie
    int hallId;    // ID of the hall
    string datetime; // e.g. "2025-01-01 19:30"
    double price;  // Ticket price

    int rows;      // Number of seat rows (copied from hall)
    int cols;      // Number of seat columns (copied from hall)
    // seats[r][c] = 0 => available, 1 => sold
    vector<vector<int>> seats;
};

// Global showtime list
vector<Showtime> showtimes;
int nextShowtimeId = 1; // Auto-increment ID for showtimes

// ===== File names for saving/loading data =====
const string MOVIE_FILE = "movies.txt";
const string HALL_FILE = "halls.txt";
const string SHOWTIME_FILE = "showtimes.txt";

// ===== Function declarations =====
void mainChoice1();
void mainChoice2();

// Movie management functions
void addMovie();
void listAllMovies();
void deleteMovie();
void editMovie();
int findMovieIndexById(int id);

// Hall management functions
void addHall();
void listAllHalls();
void deleteHall();
int findHallIndexById(int id);

// Showtime management functions
void addShowtime();
void listAllShowtimes();
void listShowtimesForMovie();
void deleteShowtime();
int findShowtimeIndexById(int id);

// Customer purchase functions
void startTicketPurchase();
void displaySeatMap(const Showtime& s);

// Statistics / query functions
int countSoldSeats(const Showtime& s);
void viewTicketStatusOfShowtime();
void viewTotalTicketsForMovie();
void viewOverallSalesOverview();

// Persistence functions
void saveDataToFiles();
void loadDataFromFiles();

// Helpers
bool hasShowtimeForMovie(int movieId);
bool hasShowtimeForHall(int hallId);

// ===== Main function =====
int main() {
    int mainChoice = -1;
    loadDataFromFiles();

    while (true) {
        cout << "\n=========== Movie Ticket System ===========" << endl;
        cout << "1. Ticket Office (Admin Mode)" << endl;
        cout << "2. Purchase Tickets (Customer Mode)" << endl;
        cout << "0. Exit System" << endl;
        cout << "==========================================" << endl;
        cout << "Please enter your choice: ";
        cin >> mainChoice;

        if (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number option." << endl;
            continue;
        }

        if (mainChoice == 0) {
            char ans;
            cout << "Do you want to save the current data? (Y/N): ";
            cin >> ans;

            if (ans == 'Y' || ans == 'y') {
                saveDataToFiles();
                cout << "Data saved." << endl;
            } else {
                cout << "Data not saved." << endl;
            }

            cout << "Program terminated. Goodbye!" << endl;
            break;
        } else if (mainChoice == 1) {
            mainChoice1();
        } else if (mainChoice == 2) {
            mainChoice2();
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

// ===== Admin mode (Ticket Office) =====
void mainChoice1() {
    int adminChoice = -1;

    while (true) {
        cout << "\n========== Ticket Office (Admin) ==========" << endl;
        cout << "1. Movie Management" << endl;
        cout << "2. Hall / Floor Management" << endl;
        cout << "3. Showtime Management" << endl;
        cout << "4. Query & Statistics" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "==========================================" << endl;
        cout << "Please enter your choice: ";
        cin >> adminChoice;

        if (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number option." << endl;
            continue;
        }

        if (adminChoice == 0) {
            cout << "Returning to main menu..." << endl;
            break;
        }

        // Movie Management submenu
        if (adminChoice == 1) {
            int movieChoice = -1;
            while (true) {
                cout << "\n---------- Movie Management ----------" << endl;
                cout << "1. Add Movie" << endl;
                cout << "2. Delete Movie" << endl;
                cout << "3. Edit Movie Information" << endl;
                cout << "4. List All Movies" << endl;
                cout << "0. Back" << endl;
                cout << "--------------------------------------" << endl;
                cout << "Please enter your choice: ";
                cin >> movieChoice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number option." << endl;
                    continue;
                }

                if (movieChoice == 0) {
                    cout << "Returning to admin menu..." << endl;
                    break;
                }

                switch (movieChoice) {
                case 1:
                    addMovie();
                    break;
                case 2:
                    deleteMovie();
                    break;
                case 3:
                    editMovie();
                    break;
                case 4:
                    listAllMovies();
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                }
            }
        }
        // Hall / Floor Management
        else if (adminChoice == 2) {
            int hallChoice = -1;
            while (true) {
                cout << "\n------ Hall / Floor Management ------" << endl;
                cout << "1. Add Hall" << endl;
                cout << "2. Delete Hall" << endl;
                cout << "3. View All Halls" << endl;
                cout << "0. Back" << endl;
                cout << "-------------------------------------" << endl;
                cout << "Please enter your choice: ";
                cin >> hallChoice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number option." << endl;
                    continue;
                }

                if (hallChoice == 0) {
                    cout << "Returning to admin menu..." << endl;
                    break;
                }

                switch (hallChoice) {
                case 1:
                    addHall();
                    break;
                case 2:
                    deleteHall();
                    break;
                case 3:
                    listAllHalls();
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                }
            }
        }
        // Showtime Management
        else if (adminChoice == 3) {
            int showtimeChoice = -1;
            while (true) {
                cout << "\n---------- Showtime Management ----------" << endl;
                cout << "1. Add Showtime" << endl;
                cout << "2. Delete Showtime" << endl;
                cout << "3. View Showtimes for a Movie" << endl;
                cout << "4. View All Showtimes" << endl;
                cout << "0. Back" << endl;
                cout << "-----------------------------------------" << endl;
                cout << "Please enter your choice: ";
                cin >> showtimeChoice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number option." << endl;
                    continue;
                }

                if (showtimeChoice == 0) {
                    cout << "Returning to admin menu..." << endl;
                    break;
                }

                switch (showtimeChoice) {
                case 1:
                    addShowtime();
                    break;
                case 2:
                    deleteShowtime();
                    break;
                case 3:
                    listShowtimesForMovie();
                    break;
                case 4:
                    listAllShowtimes();
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                }
            }
        }
        // Query & Statistics
        else if (adminChoice == 4) {
            int statsChoice = -1;
            while (true) {
                cout << "\n---------- Query & Statistics ----------" << endl;
                cout << "1. View ticket status of a showtime" << endl;
                cout << "2. View today's total tickets of a movie" << endl;
                cout << "3. View ticket sales overview" << endl;
                cout << "0. Back" << endl;
                cout << "----------------------------------------" << endl;
                cout << "Please enter your choice: ";
                cin >> statsChoice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number option." << endl;
                    continue;
                }

                if (statsChoice == 0) {
                    cout << "Returning to admin menu..." << endl;
                    break;
                }

                switch (statsChoice) {
                case 1:
                    viewTicketStatusOfShowtime();
                    break;
                case 2:
                    viewTotalTicketsForMovie();
                    break;
                case 3:
                    viewOverallSalesOverview();
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                }
            }
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

// ===== Customer mode (Purchase) =====
void mainChoice2() {
    int userChoice = -1;

    while (true) {
        cout << "\n========== Purchase (Customer) ==========" << endl;
        cout << "1. Start Ticket Purchase" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "=========================================" << endl;
        cout << "Please enter your choice: ";
        cin >> userChoice;

        if (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number option." << endl;
            continue;
        }

        if (userChoice == 0) {
            cout << "Returning to main menu..." << endl;
            break;
        }

        if (userChoice == 1) {
            startTicketPurchase();
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

// ===== Movie management function implementations =====

// Find movie index in the vector by its ID.
// Return index if found, -1 if not found.
int findMovieIndexById(int id) {
    for (size_t i = 0; i < movies.size(); ++i) {
        if (movies[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Add a new movie.
void addMovie() {
    Movie m;
    m.id = nextMovieId++;

    cout << "\n--- Add New Movie ---" << endl;

    // Clear leftover '\n' from previous cin >>
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter movie title: ";
    getline(cin, m.title);

    cout << "Enter rating (e.g. G, PG, PG-13, R): ";
    getline(cin, m.rating);

    cout << "Enter duration (minutes): ";
    while (!(cin >> m.duration) || m.duration <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid duration. Please enter a positive integer: ";
    }

    movies.push_back(m);
    cout << "Movie added successfully! [ID = " << m.id << "]" << endl;
    saveDataToFiles();
}

// List all movies.
void listAllMovies() {
    cout << "\n--- All Movies ---" << endl;
    if (movies.empty()) {
        cout << "No movies found." << endl;
        return;
    }

    for (const auto& m : movies) {
        cout << "ID: " << m.id
             << " | Title: " << m.title
             << " | Rating: " << m.rating
             << " | Duration: " << m.duration << " minutes" << endl;
    }
}

// Delete a movie by ID.
void deleteMovie() {
    cout << "\n--- Delete Movie ---" << endl;

    if (movies.empty()) {
        cout << "No movies to delete." << endl;
        return;
    }

    int id;
    cout << "Enter movie ID to delete: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid movie ID: ";
    }

    int idx = findMovieIndexById(id);
    if (idx == -1) {
        cout << "Movie ID not found." << endl;
        return;
    }
    if (hasShowtimeForMovie(id)) {
        cout << "Cannot delete this movie because it has existing showtimes." << endl;
        cout << "Please delete those showtimes first." << endl;
        return;
    }
    cout << "Movie \"" << movies[idx].title << "\" deleted." << endl;
    movies.erase(movies.begin() + idx);
    saveDataToFiles();
}

// Edit a movie by ID.
void editMovie() {
    cout << "\n--- Edit Movie ---" << endl;

    if (movies.empty()) {
        cout << "No movies to edit." << endl;
        return;
    }

    int id;
    cout << "Enter movie ID to edit: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid movie ID: ";
    }

    int idx = findMovieIndexById(id);
    if (idx == -1) {
        cout << "Movie ID not found." << endl;
        return;
    }

    Movie& m = movies[idx];
    cout << "Editing movie: " << m.title << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    cout << "Enter new title (leave empty to keep \"" << m.title << "\"): ";
    string newTitle;
    getline(cin, newTitle);
    if (!newTitle.empty()) {
        m.title = newTitle;
    }

    cout << "Enter new rating (leave empty to keep \"" << m.rating << "\"): ";
    string newRating;
    getline(cin, newRating);
    if (!newRating.empty()) {
        m.rating = newRating;
    }

    cout << "Enter new duration in minutes (0 to keep " << m.duration << "): ";
    int newDuration;
    while (!(cin >> newDuration)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a non-negative integer: ";
    }
    if (newDuration > 0) {
        m.duration = newDuration;
    }

    cout << "Movie updated successfully." << endl;
    saveDataToFiles();
}

// ===== Hall management function implementations =====

int findHallIndexById(int id) {
    for (size_t i = 0; i < halls.size(); ++i) {
        if (halls[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void addHall() {
    Hall h;
    h.id = nextHallId++;

    cout << "\n--- Add New Hall ---" << endl;

    // Clear leftover '\n' from previous cin >>
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter hall name (e.g. Hall 1, IMAX): ";
    getline(cin, h.name);

    cout << "Enter floor number: ";
    while (!(cin >> h.floor)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer for floor: ";
    }

    cout << "Enter number of seat rows: ";
    while (!(cin >> h.rows) || h.rows <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive integer for rows: ";
    }

    cout << "Enter number of seat columns: ";
    while (!(cin >> h.cols) || h.cols <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive integer for columns: ";
    }

    halls.push_back(h);
    cout << "Hall added successfully! [ID = " << h.id
         << ", total seats = " << h.rows * h.cols << "]" << endl;
    saveDataToFiles();
}

void listAllHalls() {
    cout << "\n--- All Halls ---" << endl;

    if (halls.empty()) {
        cout << "No halls found." << endl;
        return;
    }

    for (const auto& h : halls) {
        cout << "ID: " << h.id
             << " | Name: " << h.name
             << " | Floor: " << h.floor
             << " | Rows: " << h.rows
             << " | Cols: " << h.cols
             << " | Total seats: " << h.rows * h.cols
             << endl;
    }
}

void deleteHall() {
    cout << "\n--- Delete Hall ---" << endl;

    if (halls.empty()) {
        cout << "No halls to delete." << endl;
        return;
    }

    int id;
    cout << "Enter hall ID to delete: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid hall ID: ";
    }

    int idx = findHallIndexById(id);
    if (idx == -1) {
        cout << "Hall ID not found." << endl;
        return;
    }
    if (hasShowtimeForHall(id)) {
        cout << "Cannot delete this hall because it has existing showtimes." << endl;
        cout << "Please delete those showtimes first." << endl;
        return;
    }
    cout << "Hall \"" << halls[idx].name << "\" deleted." << endl;
    halls.erase(halls.begin() + idx);
    saveDataToFiles();
}

// ===== Showtime management function implementations =====

int findShowtimeIndexById(int id) {
    for (size_t i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void addShowtime() {
    cout << "\n--- Add New Showtime ---" << endl;

    if (movies.empty()) {
        cout << "No movies available. Please add movies first." << endl;
        return;
    }
    if (halls.empty()) {
        cout << "No halls available. Please add halls first." << endl;
        return;
    }

    // List movies and halls for the admin to choose from
    cout << "\nAvailable Movies:" << endl;
    listAllMovies();

    int movieId;
    cout << "\nEnter movie ID for this showtime: ";
    while (!(cin >> movieId) || findMovieIndexById(movieId) == -1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid movie ID. Please enter a valid movie ID: ";
    }

    cout << "\nAvailable Halls:" << endl;
    listAllHalls();

    int hallId;
    cout << "\nEnter hall ID for this showtime: ";
    while (!(cin >> hallId) || findHallIndexById(hallId) == -1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid hall ID. Please enter a valid hall ID: ";
    }

    Showtime s;
    s.id = nextShowtimeId++;
    s.movieId = movieId;
    s.hallId = hallId;

    // Initialize seat map based on the hall's rows and cols
    int hIdx = findHallIndexById(hallId);
    Hall& hall = halls[hIdx]; // hIdx is valid because hallId has been validated
    s.rows = hall.rows;
    s.cols = hall.cols;
    s.seats.assign(s.rows, vector<int>(s.cols, 0)); // Set all seats to 0 (available)

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover '\n' from cin >>

    cout << "Enter date and time (e.g. 2025-01-01 19:30): ";
    getline(cin, s.datetime);

    cout << "Enter ticket price: ";
    while (!(cin >> s.price) || s.price <= 0.0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid price. Please enter a positive number: ";
    }

    showtimes.push_back(s);
    cout << "Showtime added successfully! [ID = " << s.id << "]" << endl;
    saveDataToFiles();
}

void listAllShowtimes() {
    cout << "\n--- All Showtimes ---" << endl;

    if (showtimes.empty()) {
        cout << "No showtimes found." << endl;
        return;
    }

    for (const auto& s : showtimes) {
        int mIdx = findMovieIndexById(s.movieId);
        int hIdx = findHallIndexById(s.hallId);

        string movieTitle = (mIdx != -1) ? movies[mIdx].title : "(unknown movie)";
        string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

        cout << "ID: " << s.id
             << " | Movie: " << movieTitle << " (ID " << s.movieId << ")"
             << " | Hall: " << hallName << " (ID " << s.hallId << ")"
             << " | Time: " << s.datetime
             << " | Price: " << s.price
             << endl;
    }
}

void listShowtimesForMovie() {
    cout << "\n--- Showtimes for a Movie ---" << endl;

    if (movies.empty()) {
        cout << "No movies available." << endl;
        return;
    }
    if (showtimes.empty()) {
        cout << "No showtimes available." << endl;
        return;
    }

    cout << "Available Movies:" << endl;
    listAllMovies();

    int movieId;
    cout << "\nEnter movie ID to view its showtimes: ";
    while (!(cin >> movieId) || findMovieIndexById(movieId) == -1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid movie ID. Please enter a valid movie ID: ";
    }

    bool found = false;
    for (const auto& s : showtimes) {
        if (s.movieId == movieId) {
            int hIdx = findHallIndexById(s.hallId);
            string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

            cout << "Showtime ID: " << s.id
                 << " | Hall: " << hallName << " (ID " << s.hallId << ")"
                 << " | Time: " << s.datetime
                 << " | Price: " << s.price
                 << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No showtimes found for this movie." << endl;
    }
}

void deleteShowtime() {
    cout << "\n--- Delete Showtime ---" << endl;

    if (showtimes.empty()) {
        cout << "No showtimes to delete." << endl;
        return;
    }

    // List all showtimes to help selection
    listAllShowtimes();

    int id;
    cout << "\nEnter showtime ID to delete: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid showtime ID: ";
    }

    int idx = findShowtimeIndexById(id);
    if (idx == -1) {
        cout << "Showtime ID not found." << endl;
        return;
    }

    cout << "Showtime ID " << showtimes[idx].id << " deleted." << endl;
    showtimes.erase(showtimes.begin() + idx);
    saveDataToFiles();
}

void displaySeatMap(const Showtime& s) {
    cout << "\n--- Seat Map ---" << endl;
    cout << "O = available, X = sold" << endl;

    // Column header
    cout << "     ";
    for (int c = 0; c < s.cols; ++c) {
        cout << (c + 1) << " ";
    }
    cout << endl;

    for (int r = 0; r < s.rows; ++r) {
        cout << "Row " << (r + 1);
        if (s.rows >= 10 && r + 1 < 10) {
            cout << " "; // Minor alignment for single-digit rows
        }
        cout << "  ";

        for (int c = 0; c < s.cols; ++c) {
            char ch = (s.seats[r][c] == 0) ? 'O' : 'X';
            cout << ch << " ";
        }
        cout << endl;
    }
}

void startTicketPurchase() {
    cout << "\n=== Ticket Purchase ===" << endl;

    if (movies.empty()) {
        cout << "No movies available. Please ask staff to add movies first." << endl;
        return;
    }
    if (halls.empty()) {
        cout << "No halls available. Please ask staff to add halls first." << endl;
        return;
    }
    if (showtimes.empty()) {
        cout << "No showtimes available at the moment." << endl;
        return;
    }

    // 1) Select a movie
    cout << "\nAvailable Movies:" << endl;
    listAllMovies();

    int movieId;
    cout << "\nEnter movie ID to purchase: ";
    while (!(cin >> movieId) || findMovieIndexById(movieId) == -1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid movie ID. Please enter a valid movie ID: ";
    }

    // 2) Show showtimes for the selected movie
    cout << "\nAvailable showtimes for this movie:" << endl;
    bool hasShowtime = false;
    for (const auto& s : showtimes) {
        if (s.movieId == movieId) {
            int hIdx = findHallIndexById(s.hallId);
            string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

            cout << "Showtime ID: " << s.id
                 << " | Hall: " << hallName
                 << " | Time: " << s.datetime
                 << " | Price: " << s.price
                 << endl;
            hasShowtime = true;
        }
    }

    if (!hasShowtime) {
        cout << "No showtimes for this movie. Please choose another movie." << endl;
        return;
    }

    int showtimeId;
    cout << "\nEnter showtime ID to purchase: ";
    while (true) {
        if (!(cin >> showtimeId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid showtime ID: ";
            continue;
        }
        int idx = findShowtimeIndexById(showtimeId);
        if (idx == -1 || showtimes[idx].movieId != movieId) {
            cout << "Showtime ID not valid for the selected movie. Please try again: ";
            continue;
        }
        break;
    }

    int sIdx = findShowtimeIndexById(showtimeId);
    Showtime& s = showtimes[sIdx];

    int mIdx = findMovieIndexById(s.movieId);
    int hIdx = findHallIndexById(s.hallId);

    string movieTitle = (mIdx != -1) ? movies[mIdx].title : "(unknown movie)";
    string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

    cout << "\nYou selected:" << endl;
    cout << "Movie: " << movieTitle << endl;
    cout << "Hall: " << hallName << endl;
    cout << "Time: " << s.datetime << endl;
    cout << "Price (per ticket): " << s.price << endl;

    // 3) Check seat availability
    int totalSeats = s.rows * s.cols;
    int soldSeats = countSoldSeats(s);
    int availableSeats = totalSeats - soldSeats;

    if (availableSeats <= 0) {
        cout << "Sorry, this showtime is sold out." << endl;
        return;
    }

    cout << "\nTotal seats: " << totalSeats
         << " | Sold: " << soldSeats
         << " | Available: " << availableSeats << endl;

    // 4) Display seat map
    displaySeatMap(s);

    // 5) Ask how many tickets
    int ticketCount;
    cout << "\nHow many tickets do you want to buy? ";
    while (!(cin >> ticketCount) || ticketCount <= 0 || ticketCount > availableSeats) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number. Please enter a number between 1 and "
             << availableSeats << ": ";
    }

    vector<pair<int, int>> selectedSeats;
    selectedSeats.reserve(ticketCount);

    // 6) Seat selection for each ticket
    for (int t = 1; t <= ticketCount; ++t) {
        cout << "\nSelect seat for ticket #" << t << endl;

        int row, col;
        while (true) {
            cout << "Enter row number (1-" << s.rows << "): ";
            if (!(cin >> row) || row < 1 || row > s.rows) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid row. Please enter a number between 1 and "
                     << s.rows << "." << endl;
                continue;
            }

            cout << "Enter column number (1-" << s.cols << "): ";
            if (!(cin >> col) || col < 1 || col > s.cols) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid column. Please enter a number between 1 and "
                     << s.cols << "." << endl;
                continue;
            }

            int rIdx = row - 1;
            int cIdx = col - 1;

            if (s.seats[rIdx][cIdx] == 1) {
                cout << "This seat is already taken. Please choose another seat." << endl;
                continue;
            }

            // Check if this seat is duplicated in the current order
            bool duplicate = false;
            for (const auto& p : selectedSeats) {
                if (p.first == row && p.second == col) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                cout << "You already selected this seat in this order. Please choose another seat." << endl;
                continue;
            }

            // Seat is available: record it and mark as sold
            s.seats[rIdx][cIdx] = 1;
            selectedSeats.push_back({ row, col });
            break;
        }

        // If you want to refresh the seat map after each selection,
        // you can call displaySeatMap(s) here.
    }

    // 7) Print ticket summary
    saveDataToFiles();
    cout << "\nTicket(s) booked successfully!" << endl;
    cout << "----- Ticket Summary -----" << endl;
    cout << "Movie : " << movieTitle << endl;
    cout << "Hall  : " << hallName << endl;
    cout << "Time  : " << s.datetime << endl;
    cout << "Seats : ";

    for (size_t i = 0; i < selectedSeats.size(); ++i) {
        cout << "(Row " << selectedSeats[i].first
             << ", Col " << selectedSeats[i].second << ")";
        if (i + 1 < selectedSeats.size()) cout << ", ";
    }
    cout << endl;

    double totalPrice = s.price * ticketCount;
    cout << "Price per ticket : " << fixed << setprecision(2) << s.price << endl;
    cout << "Total price      : " << fixed << setprecision(2) << totalPrice << endl;
    cout << "-------------------------" << endl;
}

int countSoldSeats(const Showtime& s) {
    int count = 0;
    for (int r = 0; r < s.rows; ++r) {
        for (int c = 0; c < s.cols; ++c) {
            if (s.seats[r][c] == 1) {
                ++count;
            }
        }
    }
    return count;
}

void viewTicketStatusOfShowtime() {
    cout << "\n--- Ticket Status of a Showtime ---" << endl;

    if (showtimes.empty()) {
        cout << "No showtimes available." << endl;
        return;
    }

    // List all showtimes for easier selection
    listAllShowtimes();

    int id;
    cout << "\nEnter showtime ID to view its status: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid showtime ID: ";
    }

    int idx = findShowtimeIndexById(id);
    if (idx == -1) {
        cout << "Showtime ID not found." << endl;
        return;
    }

    Showtime& s = showtimes[idx];

    int mIdx = findMovieIndexById(s.movieId);
    int hIdx = findHallIndexById(s.hallId);

    string movieTitle = (mIdx != -1) ? movies[mIdx].title : "(unknown movie)";
    string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

    int sold = countSoldSeats(s);
    int total = s.rows * s.cols;
    int available = total - sold;

    cout << "\nShowtime Info:" << endl;
    cout << "Showtime ID: " << s.id << endl;
    cout << "Movie      : " << movieTitle << endl;
    cout << "Hall       : " << hallName << endl;
    cout << "Time       : " << s.datetime << endl;
    cout << "Price      : " << s.price << endl;
    cout << "Sold       : " << sold << " / " << total << endl;
    cout << "Available  : " << available << endl;

    // Display seat map
    displaySeatMap(s);
}

void viewTotalTicketsForMovie() {
    cout << "\n--- Total Tickets for a Movie (All Showtimes) ---" << endl;

    if (movies.empty()) {
        cout << "No movies available." << endl;
        return;
    }
    if (showtimes.empty()) {
        cout << "No showtimes available." << endl;
        return;
    }

    cout << "Available Movies:" << endl;
    listAllMovies();

    int movieId;
    cout << "\nEnter movie ID: ";
    while (!(cin >> movieId) || findMovieIndexById(movieId) == -1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid movie ID. Please enter a valid movie ID: ";
    }

    int mIdx = findMovieIndexById(movieId);
    string movieTitle = (mIdx != -1) ? movies[mIdx].title : "(unknown movie)";

    int totalSold = 0;
    double totalRevenue = 0.0;
    bool hasShowtime = false;

    cout << "\nShowtimes for \"" << movieTitle << "\":" << endl;

    for (const auto& s : showtimes) {
        if (s.movieId == movieId) {
            hasShowtime = true;
            int sold = countSoldSeats(s);
            int totalSeats = s.rows * s.cols;
            double revenue = sold * s.price;

            int hIdx = findHallIndexById(s.hallId);
            string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

            cout << "Showtime ID: " << s.id
                 << " | Hall: " << hallName
                 << " | Time: " << s.datetime
                 << " | Sold: " << sold << " / " << totalSeats
                 << " | Revenue: " << fixed << setprecision(2) << revenue
                 << endl;

            totalSold += sold;
            totalRevenue += revenue;
        }
    }

    if (!hasShowtime) {
        cout << "No showtimes for this movie." << endl;
        return;
    }

    cout << "\nTotal tickets sold for \"" << movieTitle << "\": " << totalSold << endl;
    cout << "Total revenue: " << fixed << setprecision(2) << totalRevenue << endl;
}

void viewOverallSalesOverview() {
    cout << "\n--- Overall Ticket Sales Overview ---" << endl;

    if (showtimes.empty()) {
        cout << "No showtimes available." << endl;
        return;
    }

    int grandTotalSold = 0;
    double grandTotalRevenue = 0.0;

    for (const auto& s : showtimes) {
        int sold = countSoldSeats(s);
        int totalSeats = s.rows * s.cols;
        double revenue = sold * s.price;

        int mIdx = findMovieIndexById(s.movieId);
        int hIdx = findHallIndexById(s.hallId);

        string movieTitle = (mIdx != -1) ? movies[mIdx].title : "(unknown movie)";
        string hallName = (hIdx != -1) ? halls[hIdx].name : "(unknown hall)";

        cout << "Showtime ID: " << s.id
             << " | Movie: " << movieTitle
             << " | Hall: " << hallName
             << " | Time: " << s.datetime
             << " | Sold: " << sold << " / " << totalSeats
             << " | Revenue: " << fixed << setprecision(2) << revenue
             << endl;

        grandTotalSold += sold;
        grandTotalRevenue += revenue;
    }

    cout << "\nGrand total tickets sold (all showtimes): " << grandTotalSold << endl;
    cout << "Grand total revenue: " << fixed << setprecision(2) << grandTotalRevenue << endl;
}

void loadDataFromFiles() {
    // ----- Load movies -----
    {
        ifstream fin(MOVIE_FILE);
        if (!fin) {
            // No file yet -> start empty
            // cout << "[Info] No movie file found. Starting with empty movies.\n";
        } else {
            int count;
            if (!(fin >> count)) {
                // cout << "[Warning] Failed to read movie count.\n";
            } else {
                fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip rest of line
                movies.clear();
                int maxId = 0;

                for (int i = 0; i < count; ++i) {
                    Movie m;
                    fin >> m.id;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip end of line

                    getline(fin, m.title);
                    getline(fin, m.rating);

                    fin >> m.duration;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip end of line

                    movies.push_back(m);
                    if (m.id > maxId) maxId = m.id;
                }
                nextMovieId = maxId + 1;
            }
        }
    }

    // ----- Load halls -----
    {
        ifstream fin(HALL_FILE);
        if (!fin) {
            // cout << "[Info] No hall file found. Starting with empty halls.\n";
        } else {
            int count;
            if (!(fin >> count)) {
                // cout << "[Warning] Failed to read hall count.\n";
            } else {
                fin.ignore(numeric_limits<streamsize>::max(), '\n');
                halls.clear();
                int maxId = 0;

                for (int i = 0; i < count; ++i) {
                    Hall h;
                    fin >> h.id;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n');

                    getline(fin, h.name);

                    fin >> h.floor;
                    fin >> h.rows;
                    fin >> h.cols;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n');

                    halls.push_back(h);
                    if (h.id > maxId) maxId = h.id;
                }
                nextHallId = maxId + 1;
            }
        }
    }

    // ----- Load showtimes + seats -----
    {
        ifstream fin(SHOWTIME_FILE);
        if (!fin) {
            // cout << "[Info] No showtime file found. Starting with empty showtimes.\n";
        } else {
            int count;
            if (!(fin >> count)) {
                // cout << "[Warning] Failed to read showtime count.\n";
            } else {
                fin.ignore(numeric_limits<streamsize>::max(), '\n');
                showtimes.clear();
                int maxId = 0;

                for (int i = 0; i < count; ++i) {
                    Showtime s;

                    fin >> s.id;
                    fin >> s.movieId;
                    fin >> s.hallId;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip rest of line

                    getline(fin, s.datetime);
                    fin >> s.price;
                    fin >> s.rows >> s.cols;
                    fin.ignore(numeric_limits<streamsize>::max(), '\n');

                    s.seats.assign(s.rows, vector<int>(s.cols, 0));
                    for (int r = 0; r < s.rows; ++r) {
                        for (int c = 0; c < s.cols; ++c) {
                            fin >> s.seats[r][c];
                        }
                    }
                    fin.ignore(numeric_limits<streamsize>::max(), '\n');

                    showtimes.push_back(s);
                    if (s.id > maxId) maxId = s.id;
                }
                nextShowtimeId = maxId + 1;
            }
        }
    }
}

void saveDataToFiles() {
    // ----- Save movies -----
    {
        ofstream fout(MOVIE_FILE);
        if (!fout) {
            cout << "[Error] Failed to open movie file for writing." << endl;
        } else {
            fout << movies.size() << '\n';
            for (const auto& m : movies) {
                fout << m.id << '\n';
                fout << m.title << '\n';
                fout << m.rating << '\n';
                fout << m.duration << '\n';
            }
        }
    }

    // ----- Save halls -----
    {
        ofstream fout(HALL_FILE);
        if (!fout) {
            cout << "[Error] Failed to open hall file for writing." << endl;
        } else {
            fout << halls.size() << '\n';
            for (const auto& h : halls) {
                fout << h.id << '\n';
                fout << h.name << '\n';
                fout << h.floor << ' ' << h.rows << ' ' << h.cols << '\n';
            }
        }
    }

    // ----- Save showtimes + seats -----
    {
        ofstream fout(SHOWTIME_FILE);
        if (!fout) {
            cout << "[Error] Failed to open showtime file for writing." << endl;
        } else {
            fout << showtimes.size() << '\n';
            for (const auto& s : showtimes) {
                fout << s.id << '\n';
                fout << s.movieId << '\n';
                fout << s.hallId << '\n';
                fout << s.datetime << '\n';
                fout << s.price << '\n';
                fout << s.rows << ' ' << s.cols << '\n';

                for (int r = 0; r < s.rows; ++r) {
                    for (int c = 0; c < s.cols; ++c) {
                        fout << s.seats[r][c];
                        if (c + 1 < s.cols) fout << ' ';
                    }
                    fout << '\n';
                }
            }
        }
    }
}

// Helper
bool hasShowtimeForMovie(int movieId) {
    for (const auto& s : showtimes) {
        if (s.movieId == movieId) return true;
    }
    return false;
}

bool hasShowtimeForHall(int hallId) {
    for (const auto& s : showtimes) {
        if (s.hallId == hallId) return true;
    }
    return false;
}