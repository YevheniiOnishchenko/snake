#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <chrono>


using namespace std;

struct coordinates{
    int x;
    int y;
};
bool game=true;
char lim='o';

bool operator == (coordinates c1, coordinates c2)
{
    if(c1.x==c2.x && c1.y==c2.y){
            return true;
    }else{
        return false;
    }
}

class Field{
private:
    int height;
    int width;
    char display[15][30];
    char bord[17][32];
    coordinates food;
public:
    Field():height(15), width(30){
        for(int i=0; i<height; i++){
            memset(display[i],' ', width);
        }
    }

    void show(){
        for(int i=0; i<height; i++){
             for(int j=0; j<width; j++){
                 cout << "\033[32m" << bord[i][j];
                 if(j==width-1)cout << endl;
             }
        }
    }

    void border(){
        height=17;
        width=32;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                if(i==0||i==height-1){
                    memset(bord[i],'=',width);
                    break;
                }else

                if(j==0||j==width-1){
                   bord[i][j]='|';
                }else

               bord[i][j]=display[i-1][j-1];
            }
        }

    }



    void draw_point(coordinates c, char pixel){
        display[c.x][c.y]=pixel;
        if(pixel=='+'){
            food=c;
        }
    }
    void del_point(coordinates c){
         display[c.x][c.y]=' ';
    }
    coordinates get_point_food(){
        return food;
    }

};


class Snake{
    private:
        friend class Field;
        int size;
        char head;
        char tail;


        vector<char> sn;
        vector<coordinates> coord;
        coordinates c = {9, 15};
        Field* f;

    public:
        Snake(Field* f):size(3), head('8'),tail('*'){this->f=f;}
        int get_size(){
            return size;
        }

        coordinates get_coord(int i){
            return coord[i];
        }

        void snake_lim(coordinates c){
            for(int i=0; i<17;i++){
                for(int j=0; j<32; j++){
                    if(i==0||i==15||j==0||j==30){
                        c.x=i;
                        c.y=j;
                        if(get_coord(get_size()-1)==c){
                            game=false;
                        }
                    }
                }
            }

            for(int i=2; i<size-1;i++){
                if(get_coord(get_size()-1)==get_coord(get_size()-i)){
                    game=false;
                }
            }


        }

        void snake_start(){
            for(int i=0; i<size; i++){
                coord.push_back(c);
                if(size-i==size) f->draw_point(coord[i], tail); else
                if(size-i==1) f->draw_point(coord[i], head); else
                f->draw_point(coord[i], '*');
                c.x--;
            }
            c.x++;
        }


        void snake_draw(){

            for(int i=0; i<size; i++){
                if(size-i==size) f->draw_point(coord[i], tail); else
                if(size-i==1) f->draw_point(coord[i], head); else
                f->draw_point(coord[i], '*');

            }
        }


        void snake_move(char way){

            for(int i=0; i<size; i++) f->del_point(coord[i]);
            if(way!=lim){
                switch(way){
                    case 'w': c.x--; lim='s';  break;
                    case 's': c.x++; lim='w';  break;
                    case 'a': c.y--; lim='d';  break;
                    case 'd': c.y++; lim='a';  break;
                    default: c.x--; lim='s'; break;
                }
            }else{
                switch(lim){
                    case 's': c.x--;   break;
                    case 'w': c.x++;   break;
                    case 'd': c.y--;   break;
                    case 'a': c.y++;   break;
                    default: c.x++;  break;
                }
            }

            coord.push_back(c);
            if(coord[size-1]==f->get_point_food()){
                size++;
                f->del_point(f->get_point_food());

            }else{

                for (int i=0; i<size; i++){
                    coord[i]=coord[i+1];
                }
                coord.pop_back();
            }
            snake_lim(c);
        }
};




class Food{

private:
    coordinates food_coord;
    Snake* s;
    Field* f;
    bool not_ex=true;
public:
    Food(Field* f, Snake* s){
        this->f=f;
        this->s=s;
        while (not_ex) {
            food_coord.x=rand()%14;
            food_coord.y=rand()%29;
            not_ex=false;
            for(int i=0; i<s->get_size(); i++){

                if(food_coord==s->get_coord(i)) {
                    not_ex=true;
                }else {
                    not_ex=false;
                    break;
                }
            }
        }
    }

    void food_draw(){
        f->draw_point(food_coord, '+');
    }

};

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(0)));
    HANDLE hConsole;
    Field field;
    Snake snake(&field);
    snake.snake_start();
    Food food(&field, &snake);
    system("cls");
    field.border();
    field.show();

    int snake_size=snake.get_size();
    int speed=150;

    char route='w';
    auto start_time=chrono::high_resolution_clock::now();
    auto end_time=chrono::high_resolution_clock::now();
    auto time=end_time-start_time;
    COORD position;

    while(game){

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода
        position.X = 0;									// Установка координаты X
        position.Y = 0;									// Установка координаты Y
        SetConsoleCursorPosition(hConsole, position);		// Перемещение каретки по заданным координатам


        if(GetKeyState('W') & 0x8000){
            route='w';
        }
        if(GetKeyState('S') & 0x8000){
            route='s';
        }
        if(GetKeyState('A') & 0x8000){
            route='a';
        }
        if(GetKeyState('D') & 0x8000){
            route='d';
        }
        if(GetKeyState('Q') & 0x8000){
            route='q';
        }
        if(route=='q') {
            break;
        }

        end_time=chrono::high_resolution_clock::now();
        time=end_time-start_time;

        if(route=='w'||route=='s'){
            time/=2;
        }
        if(time/chrono::milliseconds(1)>=speed){
        snake.snake_move(route);

        if(snake_size!=snake.get_size()){
            snake_size++;
            field.del_point(field.get_point_food());
            Food food2(&field, &snake);
            food=food2;
            speed-=5;
            if(speed>=100){
                 speed-=5;
            }
        }
        food.food_draw();

        snake.snake_draw();



        field.border();
        field.show();


        start_time=chrono::high_resolution_clock::now();
        }
    }
    system("cls");
    cout << "Game over" << endl;
    FlushConsoleInputBuffer(hConsole);

    system("PAUSE");
    return 0;
}
