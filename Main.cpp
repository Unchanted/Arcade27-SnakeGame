#include "head.h"

// g++ -std=c++11 head.h Source.cpp Main.cpp -o m

int main()
{

    Timer timer;
    Map Map;
    Head head;
    Tail tail;

    int lives = 1;
    int fruit_position = Map.map.find('*');
    int score = 0;
    int difficulty = 80;
    char key_pressed = 'W';
    char old_ch;
    float clock = 0;
    int time_position = Map.map.find("%");
    system("cls");
    Map.print();
    head.get_last_position();

    while (!(GetAsyncKeyState('Q')) && lives > 0)
    {
        timer.start();

        if (_kbhit())
            key_pressed = toupper(getch());

        head.get_last_position();

        switch (key_pressed)
        {
        case MOVE_UP:
            head.move_up(Map.map);
            break;
        case MOVE_LEFT:
            head.move_left(Map.map);
            break;
        case MOVE_DOWN:
            head.move_down(Map.map);
            break;
        case MOVE_RIGHT:
            head.move_right(Map.map);
            break;
        default:
            break;
        }

        if (head.wall_shock)
            lives--;
        else
            tail.Tail_movenent( head.head_last_position);

        tail.move(Map.map);

        if (head.head_position == fruit_position)
        {
            tail.tail_increase_size(head.head_last_position);
            tail.move(Map.map);
            fruit_position = draw_fruit_position(Map.map);
            score += 10;
        }
        Map.print();
        Sleep(difficulty);
        timer.stop();
        clock = clock + timer.elapsedMilliseconds();
        printf("Score: %d\n", score);
        printf("%0.1f\n", clock / 1000.0);
    }

    system("cls");

    return 0;
}
