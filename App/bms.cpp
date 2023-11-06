#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

struct Movie
{
    string title;
    string description;
    string genre;
    int runningTime;
    string mainStar;
    string startDate;
    string endDate;
    int screen;
    vector<string> showTimings;
};

struct Customer
{
    string name;
    string ticketType;
};

void addMovies(vector<Movie> &movieList)
{
    int numMovies;
    cout << "How many movies do you want to add? ";
    cin >> numMovies;
    cin.ignore(); // Clear the newline character from the input buffer

    for (int i = 0; i < numMovies; ++i)
    {
        Movie movie;
        cout << "\nMovie " << i + 1 << ":\n";
        cout << "Title: ";
        getline(cin, movie.title);
        cout << "Description: ";
        getline(cin, movie.description);
        cout << "Genre: ";
        getline(cin, movie.genre);
        cout << "Running Time (minutes): ";
        cin >> movie.runningTime;
        cin.ignore(); // Clear the newline character from the input buffer
        cout << "Main Star: ";
        getline(cin, movie.mainStar);
        cout << "Start Date (e.g., MM/DD/YYYY): ";
        getline(cin, movie.startDate);
        cout << "End Date (e.g., MM/DD/YYYY): ";
        getline(cin, movie.endDate);
        cout << "Screen number (1-4): ";
        cin >> movie.screen;
        cin.ignore(); // Clear the newline character from the input buffer

        // Get show timings
        cout << "Enter the show timings for each day (format: HH:MM, e.g., 10:15).\n";
        cout << "Enter 'q' to stop adding show timings.\n";

        string showTime;
        bool isFirstTime = true;
        int prevHour = 10;
        int prevMinute = 15;
        string prevAmPm = "AM";

        while (true)
        {
            cout << "Show Timing (HH:MM AM/PM): ";
            getline(cin, showTime);

            if (showTime == "q" || showTime == "Q")
            {
                break;
            }

            // Extracting hour, minute, and AM/PM from the input string
            int hour, minute;
            string ampm;
            stringstream ss(showTime);
            ss >> hour;
            ss.ignore(); // Ignore the colon separator
            ss >> minute >> ampm;

            // Convert hour to 24-hour format
            if (ampm == "pm" || ampm == "PM")
            {
                if (hour != 12)
                {
                    hour += 12;
                }
            }
            else if (ampm == "am" || ampm == "AM")
            {
                if (hour == 12)
                {
                    hour = 0;
                }
            }
            else
            {
                cout << "Invalid timing! Please enter a valid time in HH:MM AM/PM format." << endl;
                continue;
            }

            // Calculate the time difference in minutes
            int timeDiff = (hour * 60 + minute) - (prevHour * 60 + prevMinute);

            // Check if there is at least a 25-minute gap after the previous movie
            if (isFirstTime || timeDiff >= 25)
            {
                movie.showTimings.push_back(showTime);
                isFirstTime = false;
                prevHour = hour;
                prevMinute = minute;
                prevAmPm = ampm;
            }
            else
            {
                cout << "Invalid timing! There should be at least a 25-minute gap after the previous movie." << endl;
            }
        }

        movieList.push_back(movie);
    }

    cout << "\nMovies added successfully!\n";
}

void writeMoviesToFile(const vector<Movie> &movieList, const string &filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &movie : movieList)
        {
            file << "Movie: " << movie.title << "\n";
            file << "Description: " << movie.description << "\n";
            file << "Genre: " << movie.genre << "\n";
            file << "Running Time (minutes): " << movie.runningTime << "\n";
            file << "Main Star: " << movie.mainStar << "\n";
            file << "Start Date: " << movie.startDate << "\n";
            file << "End Date: " << movie.endDate << "\n";
            file << "Screen: " << movie.screen << "\n";

            file << "Show Timings:\n";
            for (const auto &showTime : movie.showTimings)
            {
                file << showTime << "\n";
            }

            file << "\n";
        }

        cout << "Movie details written to file successfully.\n";
    }
    else
    {
        cout << "Unable to open the file.\n";
    }
}

void printMovieList()
{
    ifstream file("movie_schedule.txt");
    if (file.is_open())
    {
        string line;
        int movieNumber = 1; // Initialize the movie number

        while (getline(file, line))
        {
            if (line.find("Movie: ") != string::npos)
            {
                cout << movieNumber << ". " << line.substr(7) << endl; // Print the movie number and name
                ++movieNumber;                                         // Increment the movie number
            }
        }

        file.close();
    }
    else
    {
        cout << "Unable to open the file.\n";
    }
}

void buyTickets(const vector<Movie> &movieList)
{
    time_t now = time(0);
    tm *timeInfo = localtime(&now);
    int dayOfWeek = timeInfo->tm_wday; // Sunday: 0, Monday: 1, ..., Friday: 5, Saturday: 6

    if ((dayOfWeek != 7) || movieList.empty() || movieList[0].startDate == "")
    {
        int numTickets;
        cout << "How many tickets do you want to buy? ";
        cin >> numTickets;
        cin.ignore(); // Clear the newline character from the input buffer

        vector<Customer> customers;
        float total_transaction = 0.0;
        for (int i = 0; i < numTickets; ++i)
        {
            Customer customer;
            cout << "\nCustomer " << i + 1 << ":\n";
            cout << "Name: ";
            getline(cin, customer.name);
            cout << "Ticket Type (Adult/Child/Senior Citizen/Student): ";
            getline(cin, customer.ticketType);
            if (customer.ticketType == "adult")
            {
                total_transaction += 9.50;
            }
            else if (customer.ticketType == "child")
            {
                total_transaction += 5.50;
            }
            else if (customer.ticketType == "senior citizen")
            {
                total_transaction += 6.50;
            }
            else if (customer.ticketType == "student")
            {
                total_transaction += 7.00;
            }
            customers.push_back(customer);
        }
        // Print movie list
        cout << "\nMovie List:\n";
        printMovieList();

        // Ask for movie choice
        int movieChoice;
        cout << "Enter the index of the movie you want to watch: ";
        cin >> movieChoice;
        cin.ignore(); // Clear the newline character from the input buffer

        if (movieChoice >= 1 && movieChoice <= movieList.size())
        {
            const Movie &selectedMovie = movieList[movieChoice - 1];

            // Print movie details
            cout << "\nSelected Movie:\n";
            cout << "Title: " << selectedMovie.title << endl;
            cout << "Start Date: " << selectedMovie.startDate << endl;
            cout << "End Date: " << selectedMovie.endDate << endl;

            // Print all dates from present to end date
            std::istringstream startDateStream(selectedMovie.startDate);
            std::istringstream endDateStream(selectedMovie.endDate);
            std::string startDateToken;
            std::string endDateToken;
            std::getline(startDateStream, startDateToken, '/');
            std::getline(endDateStream, endDateToken, '/');

            int startMonth = std::stoi(startDateToken);
            int endMonth = std::stoi(endDateToken);
            std::getline(startDateStream, startDateToken, '/');
            std::getline(endDateStream, endDateToken, '/');
            int startDay = std::stoi(startDateToken);
            int endDay = std::stoi(endDateToken);
            std::getline(startDateStream, startDateToken);
            std::getline(endDateStream, endDateToken);
            int startYear = std::stoi(startDateToken);
            int endYear = std::stoi(endDateToken);

            tm startDateInfo{};
            startDateInfo.tm_mon = startMonth - 1;
            startDateInfo.tm_mday = startDay;
            startDateInfo.tm_year = startYear - 1900;

            tm endDateInfo{};
            endDateInfo.tm_mon = endMonth - 1;
            endDateInfo.tm_mday = endDay;
            endDateInfo.tm_year = endYear - 1900;

            time_t startTimestamp = mktime(&startDateInfo);
            time_t endTimestamp = mktime(&endDateInfo);

            time_t now = time(0);
            tm *presentDate = localtime(&now);

            cout << "\nDates from present to end date:\n";
            int index = 1;
            for (time_t currentDate = startTimestamp; currentDate <= endTimestamp; currentDate += 24 * 60 * 60)
            {
                tm *dateInfo = localtime(&currentDate);
                cout << index << ". ";
                cout << setw(2) << setfill('0') << dateInfo->tm_mon + 1 << "/"
                     << setw(2) << setfill('0') << dateInfo->tm_mday << "/"
                     << setw(4) << setfill('0') << dateInfo->tm_year + 1900 << endl;
                index++;
            }

            // Ask for date choice
            int dateChoice;
            cout << "Enter the index of the date you want to watch the movie: ";
            cin >> dateChoice;
            cin.ignore(); // Clear the newline character from the input buffer

            if (dateChoice >= 1 && dateChoice < index)
            {
                // Convert the selected date to a time_t timestamp
                time_t selectedTimestamp = startTimestamp + ((dateChoice - 1) * 24 * 60 * 60);
                tm *selectedDateInfo = localtime(&selectedTimestamp);

                cout << "\nSelected Date:\n";
                cout << "Date: " << setw(2) << setfill('0') << selectedDateInfo->tm_mon + 1 << "/"
                     << setw(2) << setfill('0') << selectedDateInfo->tm_mday << "/"
                     << setw(4) << setfill('0') << selectedDateInfo->tm_year + 1900 << endl;

                // Print all show timings for the selected date
                int movieIndex = movieChoice - 1;
                const Movie &selectedMovie = movieList[movieIndex];
                const vector<string> &showTimings = selectedMovie.showTimings;

                cout << "\nShow Timings for the selected movie on the selected date:\n";
                for (const auto &showTime : showTimings)
                {
                    cout << showTime << endl;
                }

                // Ask for show timing choice
                int timingChoice;
                cout << "Enter the index of the show timing you want to choose: ";
                cin >> timingChoice;
                cin.ignore(); // Clear the newline character from the input buffer

                if (timingChoice >= 1 && timingChoice <= showTimings.size())
                {
                    const string &selectedTiming = showTimings[timingChoice - 1];

                    // Print the selected show timing
                    cout << "\nSelected Show Timing:\n";
                    cout << "Timing: " << selectedTiming << endl;
                }
                else
                {
                    cout << "Invalid show timing choice.\n";
                }
            }
            else
            {
                cout << "Invalid date choice.\n";
            }

            // total transaction and payment type
            cout << "Total Transaction: " << total_transaction << endl;
            cout << "Payment Type (cash or card): ";
            string payment_type;
            cin >> payment_type;

            if (payment_type == "cash")
            {
                cout << "Type 'c' to confirm the transaction " << total_transaction << ": ";
                char confirm;
                cin >> confirm;

                if (confirm == 'c')
                {
                    // Process cash transaction
                    cout << "Transaction confirmed. Thank you!" << endl;
                }
                else
                {
                    cout << "Transaction canceled." << endl;
                }
            }
            else if (payment_type == "card")
            {
                cout << "Enter card number: ";
                string card_number;
                cin >> card_number;

                cout << "Type 'c' to confirm the total transaction " << total_transaction << ": ";
                char confirm;
                cin >> confirm;

                if (confirm == 'c')
                {
                    // Process card transaction
                    cout << "Transaction confirmed. Thank you!" << endl;
                }
                else
                {
                    cout << "Transaction canceled." << endl;
                }
            }
            else
            {
                cout << "Invalid input. Transaction canceled." << endl;
            }
        }
        else
        {
            cout << "Invalid movie choice.\n";
        }
    }
}
int main()
{
    char userType;
    cout << "Are you a customer (C) or manager (M)? ";
    cin >> userType;

    time_t now = time(0);
    tm *timeInfo = localtime(&now);

    if (userType == 'M' || userType == 'm')
    {
        char addMoviesOption;
        cout << "Do you want to add movies? (Y/N) ";
        cin >> addMoviesOption;

        if (addMoviesOption == 'Y' || addMoviesOption == 'y')
        {
            int dayOfWeek = timeInfo->tm_wday; // Sunday: 0, Monday: 1, ..., Friday: 5, Saturday: 6

            if (dayOfWeek != 3)
            {
                vector<Movie> movieList;
                addMovies(movieList);
                writeMoviesToFile(movieList, "movie_schedule.txt");
            }
            else
            {
                cout << "Movies cannot be added on Wednesdays.\n";
            }
        }
        else
        {
            cout << "No movies added.\n";
        }
    }
    else if (userType == 'C' || userType == 'c')
    {
        vector<Movie> movieList;
        ifstream file("movie_schedule.txt");

        if (file.is_open())
        {
            string line;
            Movie movie;

            while (getline(file, line))
            {
                if (line.find("Movie: ") != string::npos)
                {
                    if (!movie.title.empty())
                    {
                        movieList.push_back(movie);
                        movie = Movie(); // Clear the movie struct
                    }

                    movie.title = line.substr(7); // Extract the movie title
                }
                else if (line.find("Description: ") != string::npos)
                {
                    movie.description = line.substr(13); // Extract the movie description
                }
                else if (line.find("Genre: ") != string::npos)
                {
                    movie.genre = line.substr(7); // Extract the movie genre
                }
                else if (line.find("Running Time (minutes): ") != string::npos)
                {
                    movie.runningTime = stoi(line.substr(23)); // Extract the running time
                }
                else if (line.find("Main Star: ") != string::npos)
                {
                    movie.mainStar = line.substr(11); // Extract the main star
                }
                else if (line.find("Start Date: ") != string::npos)
                {
                    movie.startDate = line.substr(12); // Extract the start date
                }
                else if (line.find("End Date: ") != string::npos)
                {
                    movie.endDate = line.substr(10); // Extract the end date
                }
                else if (line.find("Screen: ") != string::npos)
                {
                    movie.screen = stoi(line.substr(8)); // Extract the screen number
                }
                else if (line == "Show Timings:")
                {
                    while (getline(file, line) && !line.empty())
                    {
                        movie.showTimings.push_back(line); // Extract the show timings
                    }
                }
            }

            if (!movie.title.empty())
            {
                movieList.push_back(movie);
            }

            file.close();
        }
        else
        {
            cout << "Unable to open the file.\n";
        }

        buyTickets(movieList);
    }
    else
    {
        cout << "Invalid user type.\n";
    }

    return 0;
}