#include <iostream>
#include <time.h>
#include <string>
#include <list>
#include <thread>
#include <chrono>
//#include <SFML/Audio.hpp>

using namespace std;

// Takes in the time as a string (eg: 03:24) and outputs it as a different string (eg: three twenty four am)
string outputTimeInWords(string time);
// Converts a two digit number into text
string writeOutNumber(int num);
// Splits a string into a list of smaller strings on spaces
list<string> splitString(string input, char splitter);
// Creates a list of sound filenames
list<string> generateSoundFiles(list<string> input);

int main()
{
    // Get the current system time as a string
    time_t rawtime;
    time(&rawtime);
    string time = ctime(&rawtime);

    // Cut the current system time down to just the hour and the minute
    time = time.substr(11, 5);
    if (time.length() != 5 && time.find(':') != 2) { // Make sure that the time is the right length
        cerr << "Input is not a valid time.  The input should be in the format \"00:00\"" << endl;
        return 0;
    }

    // Convert the hour and minute to a string in words
    string writtenTime = "It's ";
    writtenTime.append(outputTimeInWords(time));
    cout << writtenTime << endl;

 /*   // This part should play the sound files in order, but the terminal I use doesn't have audio, so I can't tell if it works
    list<string> soundFiles = generateSoundFiles(splitString(writtenTime, ' '));
    sf::SoundBuffer buffer;
    sf::Sound sound;
    for (list<string>::iterator itr = soundFiles.begin(); itr != soundFiles.end(); itr++) {
        buffer.loadFromFile(*itr);
        sound.setBuffer(buffer);
        sound.play();
        sound.stop();
    }*/
    return 0;
}

// Takes a string based on 24-hour time and outputs it as a time written out
string outputTimeInWords(string time) {
    size_t colon = time.find(':');
    string hour;
    string minute;
    string output = "";
    if (colon != string::npos && time.length() == 5) { // If there's a colon in the input and the input is long enough to be a time code...
        hour = time.substr(0, colon); // ...split the text on the colon placement
        minute = time.substr(colon + 1);
        if (hour.length() > 2 || minute.length() > 2) { // the colon should have split the time in half
            cerr << "Input is not a valid time.  The input should be in the format \"00:00\"" << endl;
            return "";
        }
        // Convert the strings into ints
        int hourInt = stoi(hour);
        int minInt = stoi(minute);
        bool isAfterNoon = hourInt >= 12;

        // Convert the 24 hour timeframe into a 12-hour timeframe
        if (isAfterNoon && hourInt != 12)
            hourInt -= 12;
        if (hourInt == 0)
            hourInt = 12;
        output.append(writeOutNumber(hourInt));

        // if the first digit of the minute is 0 and it's not, like, 12:00, add an "oh" to the code
        if (minute[0] == '0' && minute[1] != '0')
            output.append("oh ");
        output.append(writeOutNumber(minInt));
        // Adds "am" or "pm" to the end, based on if its before or after noon
        output.append(isAfterNoon ? "pm" : "am");
    } else { // Output an error if the input doesn't make a time
        cerr << "Input is not a valid time.  The input should be in the format \"00:00\"" << endl;
        return "";
    }
    return output;
}

// Converts a two digit number into a textual representation of that number
string writeOutNumber(int num) {
    if (num / 100 != 0) { // Make sure that the number is two digits or less, output an error if it's three digits or more
        cerr << "Input should be two or fewer digits." << endl;
        return "";
    }
    string output = "";
    // Split the two digit number into two different one-digit numbers
    int firstDigit = num / 10;
    int secondDigit = num % 10;
    switch (firstDigit) {
        case 1: // Because the rules for numbers starting with one are weird compared to other numbers, I had to write out each one
            switch (secondDigit){
                case 0:
                    output = "ten ";
                    break;
                case 1:
                    output = "eleven ";
                    break;
                case 2:
                    output = "twelve ";
                    break;
                case 3:
                    output = "thirteen ";
                    break;
                case 4:
                    output = "fourteen ";
                    break;
                case 5:
                    output = "fifteen ";
                    break;
                case 6:
                    output = "sixteen ";
                    break;
                case 7:
                    output = "seventeen ";
                    break;
                case 8:
                    output = "eighteen ";
                    break;
                case 9:
                    output = "nineteen ";
                    break;
            }
            return output;
            break; // Probably not a neccessary line, since the code will never reach here because the previous line is a return, but better safe than sorry
        case 2: // From this point on, language rules for numbers follow a pattern, so I can just have the first digit be the first word in the number
            output.append("twenty ");
            break;
        case 3:
            output.append("thirty ");
            break;
        case 4:
            output.append("forty ");
            break;
        case 5:
            output.append("fifty ");
            break;
        case 6:
            output.append("sixty ");
            break;
        case 7:
            output.append("seventy ");
            break;
        case 8:
            output.append("eighty ");
            break;
        case 9:
            output.append("ninety ");
            break;
    }
    switch (secondDigit) { // And this one runs whether or not theres a first digit, so it's all good
        case 1:
            output.append("one ");
            break;
        case 2:
            output.append("two ");
            break;
        case 3:
            output.append("three ");
            break;
        case 4:
            output.append("four ");
            break;
        case 5:
            output.append("five ");
            break;
        case 6:
            output.append("six ");
            break;
        case 7:
            output.append("seven ");
            break;
        case 8:
            output.append("eight ");
            break;
        case 9:
            output.append("nine ");
            break;
    }
    return output;
}

// Splits a string into a list of smaller strings
list<string> splitString(string input, char splitter) {
    list<string> output;
    size_t pos;
    while (pos != string::npos) {
        pos = input.find(splitter); // Find the space, or whatever your splitting your string on
        string word = input.substr(0, pos); // Isolate a part of the string from the longer string
        output.push_back(word); // Add the part of the string to the list
        input.erase(0, pos + 1); // Get rid of the substring that was already added to the list
    }
    return output;
}

// Makes a list of filenames for audio files
list<string> generateSoundFiles(list<string> input) {
    list<string> output;
    string temp;
    for (list<string>::iterator itr = input.begin(); itr != input.end(); itr++) {
        temp = "Audio/";
        temp.append(*itr);
        temp.append(".wav");
        output.push_back(temp);
    }
    return output;
}
