#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

struct CardEntry {
    int number;
    bool marked;
};

typedef struct CardEntry* Card;

CardEntry* create_card(const std::vector<int>& card_entry_buffer);
void print_card(const CardEntry* card);
bool is_winning(const Card candidate_card);

void part1(const std::vector<int>& random_inputs, std::vector<Card>& cards);
void part2(const std::vector<int>& random_inputs, std::vector<Card> cards);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " " << "<input-filename>" << std::endl;
        std::exit(-1);
    }

    std::string input_filename = argv[1];
    if (!std::filesystem::exists(input_filename)) {
        std::cout << "error: file " << input_filename << " does not exist" << std::endl;
        std::exit(-1);
    }

    std::ifstream inputfile;
    inputfile.open(input_filename);

    std::string random_inputs_string; // comma seperated random inputs, basically
                                      // the first line of the input.txt file
    std::getline(inputfile, random_inputs_string);

    std::vector<int> random_inputs; // integer random inputs after parsing csv list

    // iterate and fill random_inputs vector
    char delimiter = ',';
    size_t pos = 0; // position of next delimiter, ',' in this case
    while ((pos = random_inputs_string.find(delimiter)) != std::string::npos) {
        std::string token = random_inputs_string.substr(0, pos);
        random_inputs_string.erase(0, pos + 1 /* delimiter length */);
        random_inputs.push_back(std::stoi(token));
    }
    // parse the last integer in random_inputs_string
    if (pos == std::string::npos && random_inputs_string.length() != 0) {
        random_inputs.push_back(std::stoi(random_inputs_string));
    }

    /*
    for (int x : random_inputs) {
        std::cout << x << std::endl;
    }
    */

    std::vector<Card> cards; // vector to hold all the cards (CardEntry*)
    std::vector<int> card_entry_buffer; // vector to buffer 25 integers from inputfile 
                                        // before creating a Card
    
    card_entry_buffer.reserve(25); // preallocate to avoid repeated allocations.
                                   // This buffer will never be longer than 25 int.

    int card_entry;
    // keep reading new entries while there is some entry to read
    while (inputfile >> card_entry) {
        if (card_entry_buffer.size() == 25) { // If buffer is full
            cards.push_back(create_card(card_entry_buffer)); // Create new card
            card_entry_buffer.clear(); // Clear the buffer after creatign new card

            card_entry_buffer.push_back(card_entry); // Push the card_entry just read
        } else {
            card_entry_buffer.push_back(card_entry); // Buffer is not full, keep 
                                                     // pushing
        }
    }

    // Drain the buffer one last time
    if (card_entry_buffer.size() != 0) {
        cards.push_back(create_card(card_entry_buffer));
        card_entry_buffer.clear(); // clear the buffer after creatign each card
    }

    inputfile.close();

    /*
    for (const CardEntry* card : cards) {
        print_card(card);
    }
    */

    part1(random_inputs, cards);
    part2(random_inputs, cards);

}

/*
 * Create a new Card using a vector of 25 ints.
 * Caller has to ensure that the vector is of size 25.
 */
CardEntry* create_card(const std::vector<int>& card_entry_buffer) {
    Card card = (Card) malloc(25 * sizeof(CardEntry));
    for (size_t i = 0, e = card_entry_buffer.size(); i != e; i++) {
        card[i] = CardEntry {
            .number = card_entry_buffer[i],
            .marked = false
        };
    }

    return card;
}

/* Pretty print any Card */
void print_card(const CardEntry* card) {
    for (size_t i = 0; i < 25; i++) {
        std::cout << card[i].number << ((i + 1) % 5 == 0 ? '\n' : ' ');
    }

    std::cout << std::endl;
}

void part1(const std::vector<int>& random_inputs, std::vector<Card>& cards) {
    int winning_number;
    Card winning_card;
    // Iterate over each card's entries for each random number and mark the
    // card entry whenever it is the same as the input number. After marking
    // any card immediately check if its winning, if yes then jump to
    // found_winning_card otherwise keep iterating.
    for (int rnumber : random_inputs) {
        for (std::vector<Card>::const_iterator bingo_card = cards.begin();
                bingo_card != cards.end(); bingo_card++) {
            for (size_t i = 0; i < 25; i++) {
                if ((*bingo_card)[i].number == rnumber) {
                    (*bingo_card)[i].marked = true;
                    break;
                }
            }

            // After marking, check if card is winning
            if (is_winning(*bingo_card)) {
                winning_number = rnumber;
                winning_card = *bingo_card;
                goto found_winning_card;
            }
        }
    }

    std::cout << "Could not found winning card in Part-1" << std::endl;
    return; // Early exit, should never happen, but still...

found_winning_card:
    int unmarked_sum = 0;
    for (size_t i = 0; i < 25; i++) {
        if (winning_card[i].marked == false) {
            unmarked_sum += winning_card[i].number;
        }
    }

    // std::cout << "unmarked sum: " << unmarked_sum << std::endl;
    // std::cout << "winning number: " << winning_number << std::endl;
    std::cout << unmarked_sum * winning_number << std::endl;

}

void part2(const std::vector<int>& random_inputs, std::vector<Card> cards) {
    int last_winning_number;
    Card last_winning_card;

    // Whenever a Card wins, put it this vector and erase once all the
    // cards have been processed for a given input number
    std::vector<Card> to_erase;
    to_erase.reserve(cards.size());

    for (int rnumber : random_inputs) {
        for (std::vector<Card>::iterator bingo_card = cards.begin();
                bingo_card != cards.end(); bingo_card++) {
            for (size_t i = 0; i < 25; i++) {
                if ((*bingo_card)[i].number == rnumber) {
                    (*bingo_card)[i].marked = true;
                    break;
                }
            }

            // after marking, check if card is winning and whether it is 
            // last winning card or not
            if (is_winning(*bingo_card)) {
                if (cards.size() == 1) { 
                    // It is the last winning card
                    last_winning_number = rnumber;
                    last_winning_card = *bingo_card;
                    goto found_last_winning_card;
                }
                // If not last winning card then put it in to_erase
                to_erase.push_back(*bingo_card);
            }
        }

        // Erase all Cards from card that are also present in to_erase.
        for (std::vector<Card>::iterator victor_card = to_erase.begin();
                victor_card != to_erase.end(); victor_card++) {
            for (std::vector<Card>::iterator bingo_card = cards.begin();
                    bingo_card != cards.end(); bingo_card++) {
                if (*bingo_card == *victor_card) {
                    cards.erase(bingo_card);
                    break;
                }
            }
        }
        to_erase.clear();
    }

found_last_winning_card:
    int unmarked_sum = 0;
    for (size_t i = 0; i < 25; i++) {
        if (last_winning_card[i].marked == false) {
            unmarked_sum += last_winning_card[i].number;
        }
    }

    // std::cout << "last card unmarked sum: " << unmarked_sum << std::endl;
    // std::cout << "last card winning number: " << last_winning_number
    // << std::endl;
    std::cout << unmarked_sum * last_winning_number << std::endl;

}

/*
 * Does horizontal and vertical checking to check if a candidate card is winning.
 */
bool is_winning(const Card candidate_card) {
    // horizontal check
    for (size_t i = 0; i < 25; i += 5) {
        bool bingo = true;
        for (size_t j = 0; j < 5; j++) {
            if (candidate_card[i + j].marked != true) {
                bingo = false;
                break;
            }
        }
        if (bingo) {
            return true;
        }
    }

    // vertical check
    for (size_t i = 0; i < 5; i++) {
        bool bingo = true;
        for (size_t j = 0; j < 25; j += 5) {
            if (candidate_card[i + j].marked != true) {
                bingo = false;
                break;
            }
        }
        if (bingo) {
            return true;
        }
    }

    return false;
}
