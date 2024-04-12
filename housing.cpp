//===========================================================================
//
//
//                        housing.cpp version
//                     Author: Walter Wagude
//                          Date: 03/24/2021
// Program to read in and organize data from a database of houses.
// The Program lets the user search through and print information from 
// the database. 
// Essential idea: The program runs an interactive query loop. It retrieves
//                 and modifies housing data from a 2D array based on
//                 user input.
// Example: query-> a 1
//          House 1 is available.
// Notes: To stop the query loop, the user inputs q and that ends the program
//     
//===========================================================================


#include <iostream>
#include <fstream>

using namespace std;

// DO NOT CHANGE THE FOLLOWING CONSTANTS. WHEN YOU SUBMIT,
// THEY MUST HAVE THESE VALUES AND THEY MUST BE USED 
// AS THE DIMENSIONS FOR YOUR all_houses ARRAY.

const int ROWS = 50;
const int COLS = 20;

//===============================================================
//
//                             House
//
//     Each House struct records the information about one
//     house.  Three of the fields are declared for you and
//     you MUST use them as given.  You should declare below
//     them any additional members that you need.
//
//     Note that if the "no_house_here" member field is set
//     to true, then the other fields are not used.  Of course,
//     if no_house_here is false; then all the fields should
//     be set.
//
//===============================================================

struct House {
        // ===================================================
        //    YOU MUST LEAVE THE DECLARATIONS FOR THESE FIELDS
        //    AS THEY ARE AND MUST USE THEM FOR THEIR STATED
        //    PURPOSE
        // ===================================================

        bool   no_house_here;  // true if there is NO house on this location
        int    id;             // id from the data file, converted to int
        string lot_code;
        float price;
        int num_bedrooms;
        string color;
        string availability;      

};


//===================================================================
//                   REQUIRED FUNCTION PROTOTYPES
//
// Declare your other function prototypes here, but DO NOT 
// MODIFY ANY OF THE PROTOTYPES SUPPLIED FOR YOU BELOW!
// Note that these cannot be much higher up in your .cpp file because
// most of them need the declaration of struct House
//===================================================================

// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 
//   Functions operating on a single house
// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 
// read one line of the input_file; return corresponding House
House read_one_house(ifstream& input_file);

//print information about a single house
void print_house(House h);



// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 
//   Functions relating to the array of houses
// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 

// given a lot code, compute the column and row (respectively)
// in the all_houses array
int col_from_lot(string lot);

int row_from_lot(string lot);

//  set the no_house_here marker for every position
//  in the array
void fill_with_no_houses(int rows, int cols, House all_houses[ROWS][COLS]);

//  given a file with a count n at the top
//  read the count and then n lines of house data
//  into appropriate positions in the array
//
//   return true if file was successfully read
//          false if file could not be opened
bool read_all_house_data(string filename, House all_houses[ROWS][COLS]);

// Command functions
string ask_forCommand();
void Command_loop(House all_houses[ROWS][COLS], House results[ROWS * COLS]);

//action_a functions
void print_Havailable(int id, bool available);
bool is_available(int house_id, House all_houses[ROWS][COLS]);
void action_a(House all_houses[ROWS][COLS]);

//action_m functions
int matching_houses(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS]); 
void action_m(House all_houses[ROWS][COLS], House results[ROWS * COLS]);
void print_matchingH(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS]);
                    
//action_n Functions
int num_neighbors(int house_id, House all_houses[ROWS][COLS]);
int numNeighbors(bool h);
void print_numNeighbors(int num_neighbors);
bool neighbor(House h);
void action_n(House all_houses[ROWS][COLS]);

//action_r functions
int rent_house(int house_id, House all_houses[ROWS][COLS]);
void printrent(int house_id, House all_houses[ROWS][COLS]);


// ============================================================
//     INCLUDE COMP11 TESTING CODE
//
//   LEAVE THIS STATEMENT IN PLACE. IT MUST COME 
//   AFTER YOUR PROTOTYPE FUNCTION DECLARATIONS.
// ============================================================

#include "housing_hooks.hpp"

// ===================================================
//     END FIXED STARTER CODE
// ===================================================



//======================================================================
//                              main 
//======================================================================

int main(int argc, char *argv[])
{
        // If the arguments fromthe command line are less than two, the
        // program prompts the user about it and quites.
        if (argc < 2)
        {
            cout << "Not enough arguments" << endl;
            return 1;
        }
            
        // Variables declarations
        string filename = argv[1]; // gets filename from the command line
        House all_houses[ROWS][COLS];
        House results[ROWS * COLS];
        
        
            
        // Initializing each House in the 2D array as being unoccupied
        fill_with_no_houses(ROWS, COLS, all_houses);
        
        // Puts house information in the houses array when file is opened
        if (read_all_house_data(filename, all_houses) == false)
        {
            cerr << "ERROR: Error opening file, please check file name: "
                 << filename << endl;
            exit(EXIT_FAILURE);
            return 1;
        }
        
        //Run the interactive query loop
        Command_loop(all_houses,results);
        
        return 0;
}

//======================================================================
//                       YOUR FUNCTIONS HERE
//
//        We give you empty starter code for some of the required
//        functions, but you should add any you need to make your
//        code well organized (if you feel the need!)
//======================================================================


// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 
//   Functions operating on a single house
// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 

//======================================================================
//   Purpose:   reads one line of the input_file
//   Arg:       an ifstream with file data
//   Returns:   corresponding House
// 
//======================================================================
House read_one_house(ifstream& input_file) 
{
        House h;
        h.no_house_here = false;
        input_file >> h.id; 
        input_file >> h.lot_code;
        input_file >> h.price;
        input_file >> h.num_bedrooms;
        input_file >> h.color;
        input_file >> h.availability;
            
        return h;
        
}

//======================================================================
//   Purpose:   print information about a single house
//   Arg:       a struct of House
//   Returns:   void- does not return anything, just print outs the
//              information about the respective house 
// 
//======================================================================
void print_house(House h)  
{
        if (h.no_house_here == true)
        {
            cout << "No house" << endl;                    
        }    
        else
        {
            if (h.availability == "available")
            {
                h.availability = "Yes";
            }
            else
            {
                h.availability = "No";
            }
            
            cout << "Id: " << h.id << " Lot: " << h.lot_code << " Color: " <<
                 h.color << " Price: " << h.price << " Bedrooms: " << 
                 h.num_bedrooms << " Available: " << h.availability << endl;
            
        }
                
}

// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 
//   Functions relating to the array of houses
// - -  - - - - - - - - - - - - -  - - - - - - - - - - - 

//===========================================================================
//   Purpose:   given a lot code, computes the column in the all_houses array
//              converts the letter value of the lot code into the
//              corresponding column index of the array
//   Arg:       a string of the lot that the column is computed from
//   Returns:   integer of the corresponding column index
//   Example:   A would be converted into 0
// 
//===========================================================================
int col_from_lot(string lot) 
{
        char col = lot[0];
        int n = col - 65;
        
        return n;

}

//===========================================================================
//   Purpose:   given a lot code, computes the row in the all_houses array
//              converts the number value of the lot code into the
//              corresponding row index of the array
//   Arg:       a string of the lot that the row is computed from
//   Returns:   integer of the corresponding row index
//   Notes:     the lot numbers begin at 1, but array indexing begins at 0!
// 
//===========================================================================
int row_from_lot(string lot) 
{
        if (lot.length() == 3)
        {
            char row1 = lot[1];
            char row2 = lot[2];
            
            int r = (((row1 - '0') * 10) + (row2 - '1'));
            
            return r;
        }
        else
        {
            char row1 = lot[1];
            
            int r = row1 - '1';
            
            return r;
        }
        
}

//===========================================================================
//   Purpose:   set the no_house_here marker for every position in the array
//   Arg:       integer of rows, integer of columns and a Struct House of a
//              2D array of all_houses.
//   Returns:   Void- returns nothing,just fills every lot with no house
// 
//===========================================================================
void fill_with_no_houses(int rows, int cols, House all_houses[ROWS][COLS]) 
{
        for (int i = 0;  i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                all_houses[i][j].no_house_here = true;
            }
             
        }

}

//==============================================================
//   Purpose:   given a file with a count n at the top,
//              reads the count and then n lines of house data
//              into appropriate positions in the array
//   Arg:       string of the filename and a Struct House of a
//              2D array of all_houses.
//   Returns:   return true if file was successfully read
//                     false if file could not be opened
// 
//==============================================================
bool read_all_house_data(string filename, House all_houses[ROWS][COLS]) 
{
        ifstream infile;
        
        infile.open(filename);
        
        if (infile.fail())
        {
            return false;
        }    
        else
        {
            int n;
            
            infile >> n;
            
            for (int i = 0; i < n; i++)
            {
                House h = read_one_house(infile);
                int row = row_from_lot(h.lot_code);
                int col = col_from_lot(h.lot_code);
                all_houses[row][col] = h;
            
            }
            
            return true;
        }
            
}

//===========================================================================
//   Purpose:   this is a recursive function that prompts the user and 
//              channels their command to the command_loop function to 
//              the relevant process.
//   Arg:       Takes no arguments
//   Returns:   returns a string of the users command to the command_loop
//   
// 
//===========================================================================
string ask_forCommand()
{
        string command;
        
        cout << "query-> ";
        cin >> command;
        
        return command;
        
        ask_forCommand();

}

//===========================================================================
//   Purpose:   the command_loop loop acts in accordance with the command 
//              from the ask_forCommand function to give the necessary outputs
//              as required by the user.
//   Arg:       a struct of a 1D array of type house and a
//              struct of 2D array type house.
//   Returns:   void- does not return anything to main, just calls necessary
//              functions to output information as determined by the user.
//   
// 
//===========================================================================
void Command_loop(House all_houses[ROWS][COLS], House results[ROWS * COLS])
{
        string action; int house_id;
    
        do{
      
            action = ask_forCommand();
        
            if (action == "a")
                action_a(all_houses);
                    
            else if (action == "m")
            {
                action_m(all_houses, results);
            }

            else if (action == "n")
                action_n(all_houses);
                
            else if (action == "r")
            {
                cin >> house_id; 
                printrent(house_id, all_houses); 
            }
            
            else if (action != "q") 
                cout << "Unknown command " << action << endl;
                
          }  while (action != "q");
                
}

//===========================================================================
//   Purpose:   checks to see if the inputted house_id is available.
//   Arg:       an integer of the house_id and a struct of 2D array 
//              type house.
//   Returns:   It returns true if the house_id is available, and false
//              otherwise.
//   Notes:     if there's no house with a given ID, then that's considered
//              unavailable.  
// 
//===========================================================================
bool is_available(int house_id, House all_houses[ROWS][COLS])
{
        for (int i = 0;  i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (all_houses[i][j].id == house_id)
                {
                    if (all_houses[i][j].availability == "available")
                    {
                        return true;
                    }
                    else
                    {
                         return false;
                    }
                   
                }
                
            }
         
        }
        
        return false;    
}

//===========================================================================
//   Purpose:   goes through the housing array and finds every house that
//              matches the price, color, and bedroom specifications that 
//              are inputted. It copies each matching house into the results
//              array. The first match goes in slot 0, the second into slot 1,
//              etc.
//   Arg:       an integer of desired_bedrooms, a float of max_price, a string
//              of desired_color,a struct of a 1D array of type house and a
//              struct of 2D array type house.
//   Returns:   It returns the number of matching houses that were stored in 
//              the array. (The caller then knows how many slots in the array 
//              contain houses that match the query.)
// 
//===========================================================================
int matching_houses(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS])
{
        int num_mHouses = 0;
    
        for (int i = 0;  i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if ((all_houses[i][j].price <= max_price) and
                   (all_houses[i][j].color == desired_color) and
                   (all_houses[i][j].num_bedrooms >= desired_bedrooms))
                {
                
                    results[num_mHouses] = all_houses[i][j];
                    
                    num_mHouses++;

                }
             
            }
         
        }
    
        return num_mHouses;    
}

//===========================================================================
//   Purpose:   Prints matching houses.
//   Arg:       an integer of desired_bedrooms, a float of max_price, a string
//              of desired_color,a struct of a 1D array of type house and a
//              struct of 2D array type house.
//   Returns:   void- returns nothing, just prints matching houses using a
//              for loop.
// 
//===========================================================================
void print_matchingH(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS])
{
        int m;
    
        m = matching_houses(max_price, desired_color,
                            desired_bedrooms, all_houses, results);
                            
        cout << "Matching Houses: << endl;
        
        for (int i = 0; i < m; i++)
        {
            print_house(results[i]);
        }

}

//===========================================================================
//   Purpose:   this function counts how many neighbors the given house_id
//              has. A neighbor is an occupied house within 1 lot of the given
//              house in the grid, either to the right, left, top, bottom, or
//              diagonally.
//   Arg:       an integer of house_id, and a struct of 2D array type house.
//   Returns:   the number of neighbors
//   Notes:     The most neighbors you can have is therefore 8 if you have
//              rented houses all around your house.
// 
//===========================================================================
int num_neighbors(int house_id, House all_houses[ROWS][COLS])
{
        int sum;
    
        for (int i = 0;  i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (all_houses[i][j].id == house_id)
                {
                   neighbor(all_houses[i][j]);
                   int row = row_from_lot(all_houses[i][j].lot_code);
                   int col = col_from_lot(all_houses[i][j].lot_code);
                    
                   sum = numNeighbors(neighbor(all_houses[row][col-1])) +
                         numNeighbors(neighbor(all_houses[row][col+1])) +
                         numNeighbors(neighbor(all_houses[row+1][col])) +
                         numNeighbors(neighbor(all_houses[row-1][col])) +
                         numNeighbors(neighbor(all_houses[row-1][col-1])) +
                         numNeighbors(neighbor(all_houses[row+1][col+1])) +
                         numNeighbors(neighbor(all_houses[row+1][col-1])) +
                         numNeighbors(neighbor(all_houses[row-1][col+1]));
                
                }
             
            }
         
        }
    
        return sum;
}

//===========================================================================
//   Purpose:   Checks whether the house is booked, hence seen as a neighbor.
//   Arg:       A struct of type House.
//   Returns:   returns true if the house is booked and false if otherwise or
//              if the their no house there.
//   Notes:     This function supports the num_neighbors function to do it's
//              work.
// 
//===========================================================================
bool neighbor(House h)  
{   
        if (h.no_house_here == true)
        {
            return false;            
        }
        if (h.availability == "booked")
        {
            return true;
        }
        else 
        {
            return false;
        }    
                
}

//===========================================================================
//   Purpose:   checks what the bool parameter returns.
//   Arg:       a boolean h.
//   Returns:   returns x where, x is 1 if the boolean returns true and x is 
//              0 if the bool returns false.
//   Notes:     This function supports the num_neighbors function to do it's
//              work.
// 
//===========================================================================
int numNeighbors(bool h)
{
        int x;
    
        if (h == true)
        {
           x = 1;
        }
        else
        {
           x = 0;
        }
        return x;
}

//===========================================================================
//   Purpose:   Prints out information depending on the num_neighbors
//   Arg:       an integer of num_neighbors
//   Returns:   void- returns nothing, just prints out information about 
//              neighbors.
//   Notes:     This function supports the num_neighbors function to do it's
//              work.
// 
//===========================================================================
void print_numNeighbors(int num_neighbors)
{
        if (num_neighbors == 0)
        {
           cout << "You have no neighbors, practice your drums!" << endl;
        }
        else
        {
           cout << "You have " << num_neighbors << " neighbors!" << endl;
        }
}

//===========================================================================
//   Purpose:   Prints out information depending on the availability of 
//              houses.
//   Arg:       an integer of id and a bool of available.
//   Returns:   void- returns nothing, just prints out information about 
//              availability of houses.
//   Notes:     This function works with the is_available function
//              to tell the user whether the house from their inputed house 
//              id is available or not.
// 
//===========================================================================
void print_Havailable(int id, bool available)
{
        if (available == true )
        {
           cout << "House " << id << " is available" << endl;
        }
        else
        {
           cout << "Sorry, House " << id << " is not available" << endl;
        }
    
}

//===========================================================================
//   Purpose:   this function attempts to rent a house.
//   Arg:       an integer of house_id, and a struct of 2D array type house.
//   Returns:   If the given house does not exist, it returns 0. If the given
//              house exists and is available, the function updates the House
//              structure to record that the house is booked and returns 1. If
//              the given house exists but is unavailable, the function
//              returns 2
//   Notes:     This function works with other functions to fulfill it's
//              purpose.
// 
//===========================================================================
int rent_house(int house_id, House all_houses[ROWS][COLS])
{
        if (house_id == 0)
        {
            return 0;
        }
        for (int i = 0;  i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (all_houses[i][j].id == house_id)
                {
                   if(all_houses[i][j].availability == "booked")
                   {
                       return 2;
                   }
                   if (all_houses[i][j].availability == "available")
                   {
                       all_houses[i][j].availability = "booked";
                       
                       return 1;
                   }
                   
                }
            }
         
        }
            
        return 0;
}

//===========================================================================
//   Purpose:   determine whether the specified house is available
//   Arg:       a struct of 2D array type house.
//   Returns:   void- does not return anything, just helps the command loop 
//              to call on other functions.
//   Notes:     This function works with other functions to fulfill it's
//              purpose.
// 
//===========================================================================
void action_a(House all_houses[ROWS][COLS])
{
        int house_id;
    
        cin >> house_id;
        print_Havailable(house_id, is_available(house_id, all_houses));
        
}

//===========================================================================
//   Purpose:   find and print the information for all the houses that match
//              the user request.
//   Arg:       a struct of 2D array  and 1D array of type house.
//   Returns:   void- does not return anything, just helps the command loop 
//              to call on other functions.
//   Notes:     This function works with other functions to fulfill it's
//              purpose.
// 
//===========================================================================
void action_m(House all_houses[ROWS][COLS], House results[ROWS * COLS])
{
        string color; 
        int  bedrooms;
        float price;
    
        cin >> price >> color >> bedrooms;
        print_matchingH(price, color, bedrooms, all_houses, results);
    
}

//===========================================================================
//   Purpose:   find and print the number of neighbors a house has.
//   Arg:       a struct of 2D array  of type house.
//   Returns:   void- does not return anything, just helps the command loop 
//              to call on other functions.
//   Notes:     This function works with other functions to fulfill it's
//              purpose.
// 
//===========================================================================
void action_n(House all_houses[ROWS][COLS])
{
        int house_id;
    
        cin >> house_id;
        print_numNeighbors(num_neighbors(house_id, all_houses));
        
}

//===========================================================================
//   Purpose:   prints out whether the user can rent a house or not.
//   Arg:       an integer of house_id and a struct of 2D array of type house.
//   Returns:   void- returns nothing, just prints out whether the user can
//              rent a house or not.
//   Notes:     This function works with the rent_house function.
// 
//===========================================================================
void printrent(int house_id, House all_houses[ROWS][COLS])
{
        if (rent_house(house_id, all_houses) == 1)
        {
           cout << "Congrats, you rented a house! Hope your"
                   " door knobs don't fall off" << endl;
            
        }
        else if (rent_house(house_id, all_houses) == 2)
        {
           cout << "Too late...i hear the dumpster"
                   " behind hill has some spaces open" << endl;
                
        }
        else
        {
           cout << "You can't rent a house that doesn't exist,"
                   " but good try" << endl;
       }
          
}
