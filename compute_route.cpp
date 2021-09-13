#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp" //https://github.com/nlohmann/json
#include <typeinfo>
using json = nlohmann::json;

class Route{
    public:
        Route();
        std::string FilePath;
        int network_size;
        int size_of_1D_array;
        std::vector<int>  start_x_coord,start_y_coord;
        std::vector<int>  end_x_coord,end_y_coord;
        std::vector<int> intial_dist_array;
        std::vector<int> visited_x, visited_y;
        std::vector<int> unvisited_x, unvisited_y;

        bool isValid();
        int manhattan_distance(int x1, int x2, int y1, int y2);
        int index_Smallest_Element(std::vector<int> array, int size); 
        int index_smallest_excluding_zero(std::vector<int> array, int size);
        void delete_visited_in_unvisted(int v_x, std::vector<int> &array_unx, int v_y, std::vector<int> &array_uny);
        void print_vec(std::vector<int> array);
        void print_coord(int array_x, int array_y);
        void print_string(std::vector<std::string> array);
        void print_all(std::vector<std::string> passenger,std::string pick, std::string drop);
};

Route::Route(){  
    // std::cout << "Please enter a file path: ";
    // std::cin >> FilePath;
    FilePath = "/home/deep/Downloads/Perceptive Automata/route.json";

    std::ifstream file(FilePath.c_str());
    json j = json::parse(file);

    network_size = j["requests"].size();
    isValid();

    std::string names[network_size];
    std::vector<std::string> passenger_list;
    
    for(int i=0; i < network_size; i++){  // assigning and seperating values from json file
        start_x_coord.push_back(j["requests"][i]["start"][0]);
        start_y_coord.push_back(j["requests"][i]["start"][1]);
        end_x_coord.push_back(j["requests"][i]["end"][0]);
        end_y_coord.push_back(j["requests"][i]["end"][1]);
        names[i] = j["requests"][i]["name"];
        unvisited_x.push_back(start_x_coord[i]);
        unvisited_y.push_back(start_y_coord[i]);
    }

    // Merging cordinates to unvisited array x and y
    unvisited_x.insert( unvisited_x.end(), end_x_coord.begin(), end_x_coord.end());
    unvisited_y.insert( unvisited_y.end(), end_y_coord.begin(), end_y_coord.end());
    
    if(network_size == 1){
        //visiting the start cordinate from Zero
        visited_x.push_back(start_x_coord[0]);
        visited_y.push_back(start_y_coord[0]);
        print_coord(visited_x[0],visited_y[0]);
        delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_x.size()-1], unvisited_y);
        passenger_list.push_back(names[0]);
        print_all(passenger_list,names[0],"None");

        //start to end coordinate and updating the visit and unvisted nodes
        visited_x.push_back(end_x_coord[0]);
        visited_y.push_back(end_y_coord[0]);
        print_coord(visited_x[1],visited_y[1]);
        delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_x.size()-1], unvisited_y);
        passenger_list.erase(std::remove(passenger_list.begin(), passenger_list.end(), names[0]), passenger_list.end());
        print_all(passenger_list,"None",names[0]);
    }
    else{
    int x_intial = 0, y_intial = 0; // starting from (0,0)
    for(int i=0; i < network_size; i++){ // Manhattan distance from (0,0) to all start points
        intial_dist_array.push_back(manhattan_distance(x_intial,start_x_coord[i],y_intial,start_y_coord[i]));
        // std::cout << intial_dist_array[i] << std::endl;
    }


    //finding index of smallest emelent from intial distance array
    int small_index_1D = index_Smallest_Element(intial_dist_array,intial_dist_array.size());  
    // std::cout << small_index_1D << std::endl; 
    visited_x.push_back(start_x_coord[small_index_1D]);
    visited_y.push_back(start_y_coord[small_index_1D]);
    // std::cout << visited_x[0]  << std::endl; 
    // std::cout << visited_y[0]  << std::endl;
    visited_x.push_back(visited_x[0]);
    visited_y.push_back(visited_y[0]);

    //delete visited[0] element from unvisited 
    delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_x.size()-1], unvisited_y);

    print_coord(visited_x[0],visited_y[0]);
    passenger_list.push_back(names[small_index_1D]);
    print_all(passenger_list,names[small_index_1D],"None");

    //computing two Manhattan distance matrices -start to start cordinate and start to end
    int start_matrix[network_size][network_size];
    int start_end_matrix[network_size][network_size];
    for(int i=0; i < network_size; i++){
        for(int j=0; j < network_size; j++){
            start_matrix[i][j] = manhattan_distance(start_x_coord[i],start_x_coord[j],start_y_coord[i],start_y_coord[j]);
            start_end_matrix[i][j] = manhattan_distance(start_x_coord[i],end_x_coord[j],start_y_coord[i],end_y_coord[j]);
            // std::cout << start_matrix[i][j] << std::endl;
            // std::cout << start_end_matrix[i][j] << std::endl;
        }
    }
 
    std::vector<int> nearest_start, nearest_end;
    for(int j=0; j < network_size; j++){
        nearest_start.push_back(start_matrix[small_index_1D][j]); //finding nearest start array in start matrix based on small index
        nearest_end.push_back(start_end_matrix[small_index_1D][j]);
    }
    
    int index_current;
    //finding next visited coordinate from unvisited coordinates and deleting it
    if(nearest_start[index_smallest_excluding_zero(nearest_start,nearest_start.size())] < 
                    nearest_end[index_smallest_excluding_zero(nearest_end,nearest_end.size())]){
        visited_x[1] = start_x_coord[index_smallest_excluding_zero(nearest_start,nearest_start.size())];
        visited_y[1] = start_y_coord[index_smallest_excluding_zero(nearest_start,nearest_start.size())];
        index_current = index_smallest_excluding_zero(nearest_start,nearest_start.size());
        }
    else{
        visited_x[1] = end_x_coord[index_smallest_excluding_zero(nearest_end,nearest_end.size())];
        visited_y[1] = end_y_coord[index_smallest_excluding_zero(nearest_end,nearest_end.size())];
        index_current = index_smallest_excluding_zero(nearest_end,nearest_end.size());
    }
    delete_visited_in_unvisted(visited_x[1], unvisited_x, visited_y[1], unvisited_y);
    print_coord(visited_x[1],visited_y[1]);

    //updating and deleting the passenger list based on visited coordinates
    for(int i =0; i<network_size; i++){
        if(visited_x[1]==start_x_coord[i] && visited_y[1]==start_y_coord[i]){
            passenger_list.push_back(names[index_current]);
            print_all(passenger_list,names[index_current],"None");
        }
        else{
            passenger_list.erase(std::remove(passenger_list.begin(), passenger_list.end(), names[index_current]), passenger_list.end());
            print_all(passenger_list,"None",names[index_current]);
            break;
        }
    }

    //finding rest of cordinates after visiting first and second coordinates and updtaing visited and unvisited list
    for(int k=0; k < unvisited_x.size(); k++){
        for(int i=0; i <= network_size; i++){
            if(unvisited_x[k] == start_x_coord[i] && unvisited_y[k] == start_y_coord[i]){
                //checking the coordinates in unvisted array to the coordinates list with Start of (x,y)
                visited_x.push_back(unvisited_x[k]);
                visited_y.push_back(unvisited_y[k]);
                // std::cout << names[index_current] << std::endl;
                print_coord(visited_x[visited_x.size()-1],visited_y[visited_x.size()-1]);
                passenger_list.push_back(names[i]);
                print_all(passenger_list,names[i],"None");
            }            
            else{
                // std::cout << "unvisited size:" << unvisited_x.size() <<std::endl;
                if(unvisited_x.size() == 0 && unvisited_y.size() == 0){
                    break; //breaking the loop when the unvivited list is empty
                }
                if(unvisited_x.size() == 1 && unvisited_y.size() == 1){
                    //Updating adn printing when the unvisited list has only 1 coordinate
                    visited_x.push_back(unvisited_x[0]);
                    visited_y.push_back(unvisited_y[0]);
                    print_coord(visited_x[visited_x.size()-1],visited_y[visited_x.size()-1]);
                    passenger_list.erase(std::remove(passenger_list.begin(), passenger_list.end(), names[i-1]), passenger_list.end());
                    print_all(passenger_list,"None",names[i-1]);
                }
                else{
                    if(manhattan_distance(visited_x[visited_x.size()-1],unvisited_x[0],visited_y[visited_y.size()-1],unvisited_y[0]) 
                        < manhattan_distance(visited_x[visited_x.size()-1],unvisited_x[1],visited_y[visited_y.size()-1],unvisited_y[1])){
                            //checking the manhattan distance between the current point to first two unvisited coordinates
                        visited_x.push_back(unvisited_x[0]);
                        visited_y.push_back(unvisited_y[0]);
                        print_coord(visited_x[visited_x.size()-1],visited_y[visited_x.size()-1]);
                        for(int j=0; j< network_size; j++){
                            if(unvisited_x[0]==end_x_coord[j] && unvisited_y[0]==end_y_coord[j]){
                            /*Checking the unvisited coordinates with end (x,y) to match the index of name, remove that name 
                                from passenger list indicationg drop off point and print*/
                                passenger_list.erase(std::remove(passenger_list.begin(), passenger_list.end(), names[j]), passenger_list.end());
                                print_all(passenger_list,"None",names[j]);
                            }
                            else if(unvisited_x[0]==start_x_coord[j] && unvisited_y[0]==start_y_coord[j]){
                            /*Checking the unvisited coordinates with start (x,y) to match the index of name, add that name 
                                to passenger list indicationg pick up and print*/
                                passenger_list.push_back(names[j]);
                                print_all(passenger_list,names[j],"None");
                            }
                        }
                    }
                    else{
                        visited_x.push_back(unvisited_x[1]);
                        visited_y.push_back(unvisited_y[1]);
                        print_coord(visited_x[visited_x.size()-1],visited_y[visited_x.size()-1]);
                        for(int j=0; j< network_size; j++){
                            if(unvisited_x[1]==end_x_coord[j] && unvisited_y[1]==end_y_coord[j]){
                            /*Checking the unvisited coordinates with end (x,y) to match the index of name, remove that name 
                                from passenger list indicationg drop off point and print*/
                                passenger_list.erase(std::remove(passenger_list.begin(), passenger_list.end(), names[j]), passenger_list.end());
                                print_all(passenger_list,"None",names[j]);
                            }
                            else if(unvisited_x[1]==start_x_coord[j] && unvisited_y[1]==start_y_coord[j]){
                            /*Checking the unvisited coordinates with start (x,y) to match the index of name, add that name 
                                to passenger list indicationg pick up and print*/
                                passenger_list.push_back(names[j]);
                                print_all(passenger_list,names[j],"None");
                            }
                        }
                    }
                    delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_y.size()-1], unvisited_y);
                }
                delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_y.size()-1], unvisited_y);
            }
            delete_visited_in_unvisted(visited_x[visited_x.size()-1], unvisited_x, visited_y[visited_y.size()-1], unvisited_y);
        }
    }
    }
    // // printing visited and unvivted list
    // std::cout << "visited list" << std::endl;
    // print_vec(visited_x);
    // std::cout << "unvisited list" << std::endl;
    // print_vec(unvisited_x);
}

bool Route::isValid(){
    //check if list is empty
    if(network_size == 0){
        std::cout << "No requests" << std::endl;
        return false;
    }
    return true;
}

void Route::print_vec(std::vector<int> array){
    //checking and printing vector
    if(array.size() != 0){
        for(int i=0; i< array.size(); i++){
            std::cout << array[i]  << std::endl;
        }
    }
    else{
        std::cout << "No passanger (Empty List)" << std::endl;
    }
}

void Route::print_string(std::vector<std::string> array){
    if(array.size() != 0){
        for(int i=0; i< array.size(); i++){
            std::cout << array[i]  << std::endl;
        }
    }
    else{
        std::cout << "No passanger (Empty List)" << std::endl;
    }
}

void Route::print_coord(int array_x, int array_y){
    //Printing Coordinates
    if(array_x != 0 && array_y != 0){
        std::cout << "Car is at :(" << array_x << "," << array_y << ")"  << std::endl;
    }
    else{
        std::cout << "No passanger (Empty List)" << std::endl;
    }
}

void Route::print_all(std::vector<std::string> passenger,std::string pick, std::string drop){
    //grouping printing statements
    std::cout<< "List of Passangers in Car:" << std::endl;
    print_string(passenger);
    std::cout<< "Pick up List: "<< pick << std::endl;
    std::cout<< "Drop off List: "<< drop << std::endl;
}


int Route::manhattan_distance(int x1, int x2, int y1, int y2){
    //calaulating manhattan distance
    double distance;
    int x_dif, y_dif;
    x_dif = x2 - x1;
    y_dif = y2 - y1;
    if(x_dif == 0 && y_dif == 0)
      distance = 0;
    else{
    if(x_dif < 0)
      x_dif = -x_dif;
    if(y_dif < 0)
      y_dif = -y_dif;
    distance = x_dif + y_dif;
    }
    // std::cout << "\n\nManhattan Distance between P1("<<x1<<","<<y1<<") and P2("<<x2<<","<<y2<<") : "<< distance << std::endl;
}

int Route::index_Smallest_Element(std::vector<int> array, int size)
{   // Index of smallest element
    int index = 0;
    for(int i = 1; i < size; i++)
    {
        if(array[i] < array[index])
            index = i;              
    }
    return index;
}

int Route::index_smallest_excluding_zero(std::vector<int> array, int size) 
{   // finding index of smallest element excluding Zero
    int index = 0;
    for(int i = 1; i < size; i++)
    {
        if(array[i] < array[index] && array[i] != 0)
            index = i;              
    }
    return index;
}

void Route::delete_visited_in_unvisted(int v_x, std::vector<int> &array_unx, int v_y, std::vector<int> &array_uny){
    //Deleting the visited cordinates in unvisited list
    for (int i = 0; i < array_unx.size(); i++){
        if (array_unx[i] == v_x  && array_uny[i] == v_y){
            array_unx.erase(std::remove(array_unx.begin(), array_unx.end(), v_x), array_unx.end());
            array_uny.erase(std::remove(array_uny.begin(), array_uny.end(), v_y), array_uny.end());
        }
    }
}

int main(int argc, char** argv){
    Route();
    return 0;
}