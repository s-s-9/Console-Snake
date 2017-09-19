#include<bits/stdc++.h>
#include<windows.h>
#include<cstdlib>
#include<conio.h>

using namespace std;

static int score = 0;
int ease = -1;

class Grid{
public:
    Grid(int r, int c)
    : _rows(r), _cols(c)
    {
        cout<<"Grid constuctor"<<endl;
        this->drawGrid();
    }
    void drawGrid();
    void printGrid();
    friend class Snake;
    friend void generateFood(Grid &g, int fr, int fc);
private:
    const int _rows;
    const int _cols;
    vector<string> visual;
};

void Grid::drawGrid()
{
    //rows 0 and _rows-1 will have -'s in all columns, others will be blank, columns 0 and _cols-1 will have |'s in all rows
    for(int i = 0; i<_rows; i++){
        //this will hold row i
        string rowi;
        for(int j = 0; j<_cols; j++){
            if(i==0 || i==(this->_rows-1)){
                rowi.push_back('*');
            }
            else{
                if(j==0 || j==(this->_cols-1)){
                    rowi.push_back('*');
                }
                else{
                    rowi.push_back(' ');
                }
            }
        }
        this->visual.push_back(rowi);
    }
}

void Grid::printGrid()
{
    for(int i = 0; i<(this->visual.size()); i++){
        cout<<this->visual[i]<<endl;
    }
}

class Snake{
public:
    Snake()
    {
        this->body.push_back(make_pair(1, 1));
        this->_direction = 'r';
    }
    void setDirection(char d);
    char getDirection();
    void printSnake(Grid &g);
    bool moveSnake(Grid &g);
private:
    char _direction;
    vector<pair<int, int> > body;
};

void Snake::setDirection(char d)
{
    if(d=='w'){
        this->_direction = 'u';
    }
    else if(d=='a'){
        this->_direction = 'l';
    }
    if(d=='d'){
        this->_direction = 'r';
    }
    if(d=='s'){
        this->_direction = 'd';
    }
}

char Snake::getDirection()
{
    return this->_direction;
}

void Snake::printSnake(Grid &g)
{
    //position the snake in th grid
    for(int i = 0; i<(this->body.size()); i++){
        pair<int, int> p = this->body[i];
        g.visual[p.first][p.second] = '.';
    }
    g.printGrid();
}

bool Snake::moveSnake(Grid &g)
{
    //move the head of the snake direction wise
    pair<int, int> head = this->body[this->body.size()-1];
    int headrow = head.first, headcol = head.second;
    pair<int, int> newhead;
    if(this->_direction=='r'){
        newhead = make_pair(headrow, headcol+1);
    }
    else if(this->_direction=='l'){
        newhead = make_pair(headrow, headcol-1);
    }
    else if(this->_direction=='d'){
        newhead = make_pair(headrow+1, headcol);
    }
    else if(this->_direction=='u'){
        newhead = make_pair(headrow-1, headcol);
    }
    //if there is food ahead, eat food and grow
    if(g.visual[newhead.first][newhead.second]=='x'){
        //increment score
        score++;
        //insert this as the new head
        this->body.push_back(newhead);
        //generate new food
        bool validPos = false;
        int foodRow, foodCol;
        while(validPos==false){
            foodRow = rand()%(g._rows-2) + 1;
            foodCol = rand()%(g._cols-2) + 1;
            //check if food is on snake's body
            int collisions = 0;
            for(int i = 0; i<(this->body.size()); i++){
                pair<int, int> currentPos = this->body[i];
                int currentRow = currentPos.first;
                int currentCol = currentPos.second;
                if(currentRow==foodRow && currentCol==foodCol){
                    collisions++;
                }
            }
            if(collisions==0){
                validPos = true;
            }
        }
        generateFood(g, foodRow, foodCol);
        return true;
    }
    //if there is obstacle ahead, return false
    if(g.visual[newhead.first][newhead.second]!=' '){
        return false;
    }
    //insert this as the new head
    this->body.push_back(newhead);
    //remove the last element from the body
    pair<int, int> tail = this->body[0];
    int tailrow = tail.first, tailcol = tail.second;
    this->body.erase(this->body.begin());
    //remove the last element's existance from the grid
    g.visual[tailrow][tailcol] = ' ';
    return true;
}

void generateFood(Grid &g, int fr, int fc)
{
    g.visual[fr][fc] = 'x';
}

int main(){
    cout<<"How easy do you want it? (Enter 1-10)"<<endl;
    while(ease<1 || ease>10){
        cin>>ease;
    }
    Grid g(10, 20);
    //g.printGrid();
    Snake s;
    generateFood(g, 3, 3);
    s.printSnake(g);
    while(true){
        system("cls");
        cout.flush();
        if(kbhit()){
            char givendir = getch();
            char currentdir = s.getDirection();
            if(currentdir=='u' || currentdir=='d'){
                if(givendir=='d' || givendir=='a'){
                    s.setDirection(givendir);
                }
            }
            else if(currentdir=='l' || currentdir=='r'){
                if(givendir=='w' || givendir=='s'){
                    s.setDirection(givendir);
                }
            }
        }
        if(s.moveSnake(g)==false){
            cout<<"Game over!"<<endl;
            cout<<"Your score: "<<score<<endl;
            return 0;
        }
        s.printSnake(g);
        Sleep(100*ease);
    }
    return 0;
}
