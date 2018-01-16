/* 
 * File:   main.cpp
 * Author: venera-varbanova
 *
 * Created on January 16, 2018, 10:52 AM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

/*
 * Calculate how many valid numbers can be dialed by a knight or a bishop on 
 * on a keypad (defined via STDIN).
 * 
 * On the board/keypad, each digit appears at most once. 
 */


// Should we be printing trace? 
static const bool g_trace = true;


// Could have been done during input

static map< int, pair<int, int> > MapDigitToPosition(
						     vector< vector<char> >& board,
						     int num_rows, int num_cols) {
  map< int, pair<int, int> > digit_to_position;

  for (int row = 0; row < num_rows; row++) {
    for (int col = 0; col < num_cols; col++) {
      char c = board[row][col];
      if (c >= '0' && c <= '9') {
	// Convert to int

	int char_as_int = c - '0';
	if (digit_to_position.find(char_as_int) != digit_to_position.end()) {
	  cerr << "Error: Duplicate digit found on board: " << c << endl;
	  exit(1);
	}

	if (g_trace) cout << "Found digit " << char_as_int
			  << " at position " << row << "," << col << endl;

	digit_to_position[char_as_int] = make_pair(row, col);
      }
    }
  }
  return digit_to_position;
}

static int CalculateNumValidPhoneNumbers(
					 vector < vector<char> > board,
					 const map< int, vector<int> >& valid_moves_from_digit_to_digits,
					 const vector<int>& valid_starting_digits,
					 int num_board_rows, int num_board_cols, int phone_number_length) {

  // column per digit, 0 to 0
  vector<int> num_phones_ending_in(10, 0);

  // init for size of 1
  for (int d : valid_starting_digits) {
    num_phones_ending_in[d] = 1;
  }

  // gradually build up until we reach size of phone number length
  for (int row_as_num_length = 1; row_as_num_length < phone_number_length; row_as_num_length++) {
    vector<int> one_shorter_phones_ending_in = num_phones_ending_in;

    num_phones_ending_in.assign(10, 0);
    for (auto& digit_to_digits : valid_moves_from_digit_to_digits) {
      int from_digit = digit_to_digits.first;
      const vector<int>& to_digits = digit_to_digits.second;
      for (auto& to_digit : to_digits) {

	num_phones_ending_in[to_digit] += one_shorter_phones_ending_in[from_digit];
      }
    }

  }

  int sum = 0;
  for (auto& n : num_phones_ending_in)
    sum += n;

  return sum;
}

static bool IsValidChessFigure(const string& figure) {
  if (figure != "knight" && figure != "bishop") {
    cerr << "Invalid figure: " << figure
	 << ". Valid figures are: knight, bishop." << endl;
    return false;
  }
  return true;
}

static string ConsumeStringFromStdin(const string& prompt) {

  if (g_trace && !prompt.empty()) cout << prompt << endl;
  string result;
  getline(cin, result);
  if (g_trace) cout << "Input consumed was: " << result << endl;
  return result;
}

static int ConsumeIntFromStdin(const string& prompt) {
  if (g_trace && !prompt.empty()) cout << prompt << endl;

  string temp;
  getline(cin, temp);
  int result = stoi(temp);
  if (g_trace) cout << "Input consumed was: " << result << endl;
  return result;
}

static vector<int> ConsumeIntsFromStdin(const string& prompt) {
  if (g_trace && !prompt.empty()) cout << prompt << endl;

  vector<int> result;
  string tmp;
  getline(cin, tmp);
  stringstream ss(tmp);
  int ti;
  while (ss >> ti)
    result.push_back(ti);
  return result;
}

static vector<char> ConsumeCharsFromStdin(const string& prompt) {

  if (g_trace && !prompt.empty()) cout << prompt << endl;
  vector<char> result;
  string tmp;
  getline(cin, tmp);
  stringstream ss(tmp);
  char tc;
  while (ss >> tc)
    result.push_back(tc);

  return result;
}

static bool IsMoveValidForFigure(map< int, pair<int, int> >& digit_to_position,
				 const string& figure, int from_d, int to_d) {
  // same digit is not a valid move,
  if (from_d == to_d)
    return false;

  // no such digit 
  if (digit_to_position.find(from_d) == digit_to_position.end())
    return false;

  if (digit_to_position.find(to_d) == digit_to_position.end())
    return false;

  int from_d_row = digit_to_position[from_d].first;
  int from_d_col = digit_to_position[from_d].second;
  int to_d_row = digit_to_position[to_d].first;
  int to_d_col = digit_to_position[to_d].second;

  int row_diff = abs(from_d_row - to_d_row);
  int col_diff = abs(from_d_col - to_d_col);

  if (figure == "knight") {
    if (row_diff == 2 && col_diff == 1)
      return true;
    else if (row_diff == 1 && col_diff == 2)
      return true;
    else
      return false;
  } else if (figure == "bishop") {
    return (row_diff > 0 && col_diff > 0 && row_diff == col_diff);

  } else {
    cerr << "Unknown figure: " << figure << endl;
    return false;
  }
}

static map< int, vector<int> > GetValidMoves(
					     vector< vector<char>>&board,
					     const string& figure,
					     int num_board_rows, int num_board_cols) {

  map< int, vector<int> > valid_moves_from_digit_to_digits;

  map< int, pair<int, int> > digit_to_position =
    MapDigitToPosition(board, num_board_rows, num_board_cols);

  vector<int> digits;
  for (auto& kv : digit_to_position)
    digits.push_back(kv.first);

  // calculate valid moves for each digit  
  for (int& from_d : digits) {
    for (int& to_d : digits) {

      // not a valid move from self to self
      if (from_d == to_d) continue;

      // exit early due to symmetry
      if (from_d < to_d) break;

      if (IsMoveValidForFigure(digit_to_position, figure, from_d, to_d)) {
	if (g_trace) cout << "Yes, move from " << from_d << " to "
			  << to_d << " is valid for " << figure << endl;
	valid_moves_from_digit_to_digits[from_d].push_back(to_d);
	// make use of symmetry
	valid_moves_from_digit_to_digits[to_d].push_back(from_d);
      } else {
	if (g_trace) cout << "No, move from " << from_d << " to "
			  << to_d << " is NOT valid for " << figure << endl;
      }
    }

  }

  return valid_moves_from_digit_to_digits;
}

int main(int argc, char** argv) {

  // Consume chess figure. Sample value: knight.
    const string figure =
      ConsumeStringFromStdin("Enter chess figure (knight or bishop): ");
    if (!IsValidChessFigure(figure)) return -1;

    // Consume phone number length. Sample value: 7
    int phone_number_length =
      ConsumeIntFromStdin("Enter length of phone number: ");

    // Consume valid starting digits. Sample value: 2 3 4 5 6 7 8 9.
    vector<int> valid_starting_digits =
      ConsumeIntsFromStdin("Enter valid starting digits str: ");

    // Consume board rows. Sample value: 4.
    int num_board_rows = ConsumeIntFromStdin("Enter num board rows: ");


    // Consume board columns. Sample value: 3.
    int num_board_cols = ConsumeIntFromStdin("Enter num board columns: ");


    if (g_trace) cout << "Now consuming rows... " << endl;

    vector< vector<char> > board;

    for (int r = 0; r < num_board_rows; r++) {

      if (g_trace) cout << "Enter row " << (r + 1) << ":" << endl;
      vector<char> chars = ConsumeCharsFromStdin("");

      // Validate number of chars is correct
      if (chars.size() != num_board_cols) {
	cerr << "Invalid number of columns " << chars.size() << endl;
	exit(1);
      }

      board.push_back(chars);

    }

    // Sample board:
    // 1 2 3
    // 4 5 6
    // 7 8 9
    // * 0 #

    if (g_trace) {
      cout << "Board entered is:" << endl;
      cout << "===================" << endl;
      for (int r = 0; r < num_board_rows; r++) {
	cout << endl;
	for (int c = 0; c < num_board_cols; c++) {
	  cout << board[r][c] << " ";
	}
      }
      cout << endl << "===================" << endl;
    }

    // Build adjacency matrix for the current figure
    // Map of current digit to next possible digits we can step on 
    // with this figure
    map< int, vector<int> > valid_moves_from_digit_to_digits =
      GetValidMoves(board, figure, num_board_rows, num_board_cols);
    if (valid_moves_from_digit_to_digits.empty()) {
      if (g_trace) cout << "no valid moves found" << endl;
      return 0;
    }

    // Actually do the math with dynamic programming
    // building on top of previous calcs to get to the result
    // starting with the smallest calcs (length of 1 number) 
    // and building on calcs on number of phone numbers ending 
    // in a given digit gradually going from length of 1 to n
    int num_valid_phone_numbers = CalculateNumValidPhoneNumbers(
								board,
								valid_moves_from_digit_to_digits,
								valid_starting_digits,
								num_board_rows, num_board_cols, phone_number_length);

    if (g_trace) cout << "RESULT: Num valid phone numbers is: " << endl;

    cout << num_valid_phone_numbers << endl;

    if (g_trace) cout << "Done!" << endl;
    return 0;
}
