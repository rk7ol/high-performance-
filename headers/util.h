/*content:data structure of heat conduction
**by:dx
**time:2020.11.14
**
**
**
**
*/

//Discrete cube
struct cube
{
    //the coordinate of cube
    int x;
    int y;
    int z;

    //index
    int index;

    //temperature
    double temp;
};

// //Adjacency relationship
// enum direction{UP,DOWN,LEFT,RIGHT,BIHING,AHEAD};

//the element of heat conduction array
struct arr
{
    cube cub;
    //0——up
    //1——down
    //2——left
    //3——right
    //4——behind
    //5——ahead
    int direc[6];
};
