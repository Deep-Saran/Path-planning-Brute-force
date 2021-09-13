compute_route.cpp is the file and ./compute_route_compiled to run the file.
Three json files with three test cases

Read the details from the JSON file.
Store the individual start x,y coordinates of each person as start_x_coord, start_y_coord and end x,y coordinates of each as end_x_coord, end_y_coord. 
Store names in names array. 
Create empty visited list, unvisited list with all start points and end points


Algorithm: 
Check network size
    If:(network size=1) // ( only request for ride)
       Then
move to start point
            Print details
            Move to end point
    Else:
       Start from initial x,y=(0,0)
       Calculate Manhattan Distance from: 
1. Each start point to other start points 
2. Each start point to each end points
       Find the nearest start point in all points using Manhattan Distance from (0,0)
       Move to the nearest point
       Update the Visited coordinates list 
       Update univisited coordinates list
       Print the location, passenger list, pickup list
       Find the index of start point 
       Find if the endpoint of current passenger is near or start point of any other passenger
                If Manhattan distance of end point < manhattan distance of nearest start point:
                            Then 
                                    Move to end point
                        Update the Visited coordinates list 
                               Update univisited coordinates list
                        Update  passenger  list
                              Print the location, passenger list, dropoff list
                Else:
                        Move to nearest start point
                        Update the Visited coordinates list 
                               Update univisited coordinates list
                        Update  passenger  list
                              Print the location, passenger list, pickup list
        






      For k=0; k< size of unvisited list; k++:
        For i=0; i<=size of the network; i++;
                If  unvisited_x[k] is in list of start_x_coord and unvisited_y[k] is in list of start_y_coord: 
                Update the Visited coordinates list 
                       Update univisited coordinates list
                Update  passenger  list
                      Print the location, passenger list, pickup list
                Else:
                      If univisited size is 0:
                        Break;
                      If univisited list size is 1:
                        Update the Visited coordinates list 
                               Update univisited coordinates list
                        Update  passenger  list
                              Print the location, passenger list, dropoff list
                     Else: 
                        If Manhattan distance(last visited node to univisted list[0])< Manhattan distance(last visited node to univisted list[1]):
                                 If unvisited list[0] is in end coordinates list:
                                        Update the Visited coordinates list 
                                               Update univisited coordinates list
                                        Update  passenger  list
                                              Print the location, passenger list, dropoff list
                                Else if unvisited list[0] is in start coordinates list: 
                                                           Update the Visited coordinates list 
                                               Update univisited coordinates list
                                        Update  passenger  list
                                              Print the location, passenger list, pickup list
                        Else:
                                If unvisited list[0] is in end coordinates list:
                                        Update the Visited coordinates list 
                                               Update univisited coordinates list
                                        Update  passenger  list
                                              Print the location, passenger list, dropoff list
                                Else if unvisited list[0] is in start coordinates list: 
                                                           Update the Visited coordinates list 
                                               Update univisited coordinates list
                                        Update  passenger  list
                                              Print the location, passenger list, pickup list