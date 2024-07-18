#include "head.h"

void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

string get_file_content(const string path)
{
    ifstream file("Map.txt"); 
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    return content;
}

int calculate_map_width(string map)
{
    return (map.find('\n') + 1);
}

int calculate_map_height(string map)
{
    int map_width = calculate_map_width(map);

    return (map.size() / map_width) + 1;
}

int find_head_position(const string map)
{
    return map.find('0');
}

int calculate_head_height(int head_position, int map_width)
{
    return head_position / map_width;
}

int draw_left(string &map, int &spaceship_position, int map_height, int map_width)
{
    if (spaceship_position % map_width > 1)
    {
        map.replace(spaceship_position - 1, 2, "0 "); //Paramters: Position, Size, Content
        spaceship_position--;
    }

    return spaceship_position;
}

int draw_right(string &map, int &spaceship_position, int map_height, int map_width)
{
    if (spaceship_position % map_width < 23)
    {
        map.replace(spaceship_position, 2, " 0"); //Paramters: Position, Size, Content
        spaceship_position++;
    }

    return spaceship_position;
}

int draw_up(string &map, int &spaceship_position, int map_height, int map_width)
{
    if (spaceship_position / map_width > 1)
    {
        map.replace(spaceship_position, 1, " "); //Paramters: Position, Size, Content
        spaceship_position = spaceship_position - map_width;
        map.replace(spaceship_position, 1, "0"); //Paramters: Position, Size, Content
    }
    return spaceship_position;
}
int draw_down(string &map, int &spaceship_position, int map_height, int map_width)
{
    if (spaceship_position < (map_height - 2) * map_width)
    {
        map.replace(spaceship_position, 1, " "); //Paramters: Position, Size, Content
        spaceship_position = spaceship_position + map_width;
        map.replace(spaceship_position, 1, "0"); //Paramters: Position, Size, Content
    }
    return spaceship_position;
}

tuple<int,bool> detect_shock(string map, int head_position, int head_last_position, int &lives, int map_width, char key_pressed)
{
    bool head_hit_wall = false;
    int head_lock_ahead = calculate_next_head_position(head_position, map_width, key_pressed);
    if (map.at(head_lock_ahead) == 'o')
    {
        head_hit_wall = false;
        lives--;
    }
    else if (head_position == head_last_position)
    {
        head_hit_wall = true;
        lives--;
    }

    return make_tuple(lives, head_hit_wall) ;
}

int calculate_next_head_position(int spaceship_position, int map_width, char key_pressed)
{

    int head_next_position;

    switch (key_pressed)
    {
    case 'W':
        head_next_position = spaceship_position - map_width;
        break;
    case 'A':
        head_next_position = spaceship_position - 1;
        break;
    case 'S':
        head_next_position = spaceship_position + map_width;
        break;
    case 'D':
        head_next_position = spaceship_position + 1;
        break;

    default:
        break;
    }

    return head_next_position;
}

int draw_fruit_position(string &map)
{
    srand(time(NULL));
    int fruit_position = rand() % map.length();

    while (map.at(fruit_position) != ' ')
        fruit_position = rand() % map.length();

    map.replace(fruit_position, 1, "*"); //Paramters: Position, Size, Content

    return fruit_position;
}

void Tail_movenent(list<int> &tail_list, int head_position)
{
    if (tail_list.size() == 1)
    {
        tail_list.push_front(head_position);
    }
    else
    {
        tail_list.push_front(head_position);
        tail_list.pop_back();
    }
}

void tail_increase_size(list<int> &tail_list, int head_position)
{
    tail_list.push_front(head_position);
}

void draw_snake_tail(string &map, list<int> tail)
{
    if (tail.size() == 1)
        map.replace(tail.front(), 1, "o");
    else
    {
        for (int node : tail)
        {
            map.replace(node, 1, "o"); //Paramters: Position, Size, Content
        }
        map.replace(tail.back(), 1, " ");
    }
}

void display_list(list<int> l)
{
    for (int x : l)
        cout << x << " ";
    cout << endl;
}

void print(string txt)
{
    gotoxy(0, 0);
    cout << txt << endl;
}
