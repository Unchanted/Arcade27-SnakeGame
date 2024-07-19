#include "head.h"






System::System(/* args */)
{
}

System::~System()
{
}

void System::show_consol_cursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}




int System::generate_ramdom_number()
{
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 mt_rand(seed);
    return mt_rand();
}

void Timer::start()
{
    m_StartTime = std::chrono::system_clock::now();
    m_bRunning = true;
}

void Timer::stop()
{
    m_EndTime = std::chrono::system_clock::now();
    m_bRunning = false;
}

double Timer::elapsedMilliseconds()
{
    std::chrono::time_point<std::chrono::system_clock> endTime;

    if (m_bRunning)
        endTime = std::chrono::system_clock::now();
    else
        endTime = m_EndTime;

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

string Map::get_file_content(const string path)
{

    ifstream file("Map.txt");
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    return content;
}

int Map::calculate_width()
{
    return (canvas.find('\n') + 1);
}

int Map::calculate_height()
{
    return (canvas.size() / width) + 1;
}

Map::Map()
{
    this->canvas = get_file_content("Map.txt");
    this->width = calculate_width();
    this->height = calculate_height();
}

void Map::print()
{
    gotoxy(0, 0);
    cout << canvas << endl;
}

int Head::find_position()
{
    return this->canvas.find('0');
}
int Head::detect_wall(char key_pressed)
{
    int wall_position = 0;
    int head_lock_ahead = calculate_next_position(key_pressed);
    if (this->canvas.at(head_lock_ahead) == '#')
    {
        wall_position = head_lock_ahead;
    }
    return wall_position;
}

bool Head::detect_shock(char key_pressed)
{
    bool head_hit_wall = false;
    int head_lock_ahead = calculate_next_position(key_pressed);
    if (this->canvas.at(head_lock_ahead) == 'o')
        head_hit_wall = false;
    else if (this->head_position == this->head_last_position)
        head_hit_wall = true;
    if (this->head_position == this->wall_position)
        head_hit_wall = true;

    return head_hit_wall;
}

int Head::calculate_next_position(char key_pressed)
{

    int head_next_position;

    switch (key_pressed)
    {
    case 'W':
        head_next_position = this->head_position - this->width;
        break;
    case 'A':
        head_next_position = this->head_position - 1;
        break;
    case 'S':
        head_next_position = this->head_position + this->width;
        break;
    case 'D':
        head_next_position = this->head_position + 1;
        break;

    default:
        break;
    }

    return head_next_position;
}

Head::Head()
{
    this->head_position = find_position();
    this->wall_shock = false;
    this->head_last_position = this->head_position;
}

void Head::draw_left(string &map)
{
    if (this->head_position % this->width > 1 && this->head_position - 1 != this->wall_position)
    {
        map.replace(this->head_position - 1, 2, "0 "); //Paramters: Position, Size, Content
        this->head_position--;
    }
}

void Head::draw_right(string &map)
{
    if (this->head_position % this->width < this->width - 3 && this->head_position + 1 != this->wall_position) // 3 por causa das duas paredes '#' e do \n
    {
        map.replace(this->head_position, 2, " 0"); //Paramters: Position, Size, Content
        this->head_position++;
    }
}

void Head::draw_up(string &map)
{
    if (this->head_position / this->width > 1 && this->head_position - this->width != this->wall_position)
    {
        map.replace(this->head_position, 1, " "); //Paramters: Position, Size, Content
        this->head_position = this->head_position - this->width;
        map.replace(this->head_position, 1, "0"); //Paramters: Position, Size, Content
    }
}
void Head::draw_down(string &map)
{
    if (this->head_position < (this->height - 2) * this->width && this->head_position + this->width != this->wall_position)
    {
        map.replace(this->head_position, 1, " "); //Paramters: Position, Size, Content
        this->head_position = this->head_position + this->width;
        map.replace(this->head_position, 1, "0"); //Paramters: Position, Size, Content
    }
}

int Head::inform_position()
{
    return this->head_position;
}

void Head::move_up(string &map)
{
    this->wall_position = detect_wall(MOVE_UP);
    draw_up(map);
    this->wall_shock = detect_shock(MOVE_UP);
}

void Head::move_left(string &map)
{
    this->wall_position = detect_wall(MOVE_LEFT);
    draw_left(map);
    this->wall_shock = detect_shock(MOVE_LEFT);
}

void Head::move_right(string &map)
{
    this->wall_position = detect_wall(MOVE_RIGHT);
    draw_right(map);
    this->wall_shock = detect_shock(MOVE_RIGHT);
}

void Head::move_down(string &map)
{
    this->wall_position = detect_wall(MOVE_DOWN);
    draw_down(map);
    this->wall_shock = detect_shock(MOVE_DOWN);
}

void Head::set_last_position()
{
    this->head_last_position = this->head_position;
}

int Head::get_last_position()
{
    return this->head_last_position;
}
int Head::get_position()
{
    return this->head_position;
}

void Tail::Tail_movenent(int head_last_position)
{
    if (this->tail_list.size() == 1)
    {
        this->tail_list.push_front(head_last_position);
    }
    else
    {
        this->tail_list.push_front(head_last_position);
        this->tail_list.pop_back();
    }
}

void Tail::tail_increase_size(int head_last_position)
{
    this->tail_list.push_front(head_last_position);
}

void Tail::draw_snake_tail(string &map)
{
    if (this->tail_list.size() == 1)
        map.replace(this->tail_list.front(), 1, "o");
    else
    {
        for (int node : this->tail_list)
        {
            map.replace(node, 1, "o"); //Paramters: Position, Size, Content
        }
        map.replace(this->tail_list.back(), 1, " ");
    }
}

void Tail::move(string &map)
{
    if (this->tail_list.size() > 0)
    {
        draw_snake_tail(map);
        map.replace(this->tail_list.back(), 1, " ");
    }
}

void print_score(int map_height, int score, int lives)
{
    gotoxy(1, map_height + 2);
    cout << "Score: " << score << endl;
    gotoxy(1, map_height + 3);
    cout << "Lives: " << lives << endl;
}

Fruit::Fruit()
{
    this->fruit_position = find_position(canvas);
}

int Fruit::find_position(string& map)
{
    int fruit_position = this->canvas.find('*');

    if (this->canvas.find('*') == string::npos)
    {
        System sys;

        fruit_position = sys.generate_ramdom_number() % this->canvas.length();

        while (canvas.at(fruit_position) != ' ')
            fruit_position = sys.generate_ramdom_number() % this->canvas.length();

        map.replace(fruit_position, 1, "*"); //Paramters: Position, Size, Content

    }

    return fruit_position;
}
int Fruit::get_position()
{
    return this->fruit_position;
}

void Fruit::draw(string &map)
{
    System sys;

    int fruit_position = sys.generate_ramdom_number() % map.length();

    while (map.at(fruit_position) != ' ')
        fruit_position = sys.generate_ramdom_number() % map.length();

    map.replace(fruit_position, 1, "*"); //Paramters: Position, Size, Content

    this->fruit_position = fruit_position;
}
