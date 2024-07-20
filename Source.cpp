#include "header.h#include "header.h"

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

string System::get_file_content(const string path)
{

    ifstream file("Map.txt");
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    return content;
}

int System::generate_ramdom_number()
{
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 mt_rand(seed);
    return mt_rand();
}

Timer::Timer()
{
    this->clock = 0;
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

Timer Timer::update()
{
    stop();
    clock = clock + elapsedMilliseconds();
    return *this;
}

Timer Timer::print()
{
    printf("%0.1f\n", clock / 1000.0);
    return *this;
}

void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

COORD getxy(CONSOLE_SCREEN_BUFFER_INFO *csbi)
{
    COORD coord = csbi->dwCursorPosition;
    return coord;
}

char get_cursor_char()
{
    char c = '\0';

    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hcon, &con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hcon, &c, 1, con.dwCursorPosition, &read) || read != 1)
            c = '\0';
    }
    return c;
}

string Map::canvas;
Map::Map() : width(calculate_width()), height(calculate_height()) {}
int Map::calculate_width() { return (canvas.find('\n') + 1); }
int Map::calculate_height() { return (canvas.size() / width) + 1; }
int Map::internal_get_width() { return width; }
int Map::internal_get_height() { return height; }

void Map::internal_print()
{
    gotoxy(0, 0);
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
    cout << Map::canvas << '\n';
    
    //cout<<"\033[3;100;30m"<< Map::canvas<<"\033[0m\n";
    /*
    const int n = Map::canvas.length() + 1;
    char *map = (char *)malloc(n * sizeof(char));
    Map::canvas.copy(map, n);

    gotoxy(0, 0);
    for (int i = 0; i < n; i++)
    {
        if (map[i] == '#')
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
        else if (map[i] == '0' || map[i] == 'o')
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 114);
        else if (map[i] == '*')
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 126 | FOREGROUND_INTENSITY);

        printf("%c", map[i]);
    }
    */
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
    this->fruit_position = find_position();
    this->x = set_x();
    this->y = set_y();
}

int Fruit::find_position()
{
    int fruit_position = Map::canvas.find(FRUIT);

    /*if (Map::canvas.find('*') == string::npos)
    {
        System sys;

        fruit_position = sys.generate_ramdom_number() % Map::canvas.length();

        while (Map::canvas.at(fruit_position) != ' ')
            fruit_position = sys.generate_ramdom_number() % Map::canvas.length();

        Map::canvas.replace(fruit_position, 1, "*"); //Paramters: Position, Size, Content
    }*/

    return fruit_position;
}

int Fruit::get_position()
{
    return fruit_position;
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

void Fruit::generate_position()
{
    System sys;
    int new_fruit_position;
    int fruit_x_position;
    int fruit_y_position;

    do
    {
        new_fruit_position = sys.generate_ramdom_number() % Map::canvas.length();
    } while (Map::canvas.at(new_fruit_position) != ' ');

    this->x = new_fruit_position % Map::get_width();
    this->y = new_fruit_position / Map::get_width();
}

void Fruit::draw()
{
    gotoxy(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 126 | FOREGROUND_INTENSITY);
    cout << FRUIT;
}

int Fruit::set_x()
{
    return fruit_position % Map::get_width();
}

int Fruit::set_y()
{
    //2 : upper and lower wall
    return (fruit_position / Map::get_width());
}

tuple<int, int> Fruit::get_coord()
{
    return tie(x, y);
}

void Fruit::generate()
{
    generate_position();
    draw();
}
