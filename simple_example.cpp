#include <SDL3/SDL.h>
#include <vector>

class History;

class Command
{

public:
    Command() = default;
    virtual ~Command() = default;
    virtual void Execute(History *history) = 0;
    virtual void Undo() = 0;
};

class History
{
public:
    History() = default;
    ~History() = default;
    std::vector<Command *> commands;

    void UndoLast()
    {
        if(commands.empty())
        {
            return;
        }
        Command *command = commands.back();
        command->Undo();
        commands.pop_back();
    }

};

class TestCommand : public Command
{
public:
    void Execute(History *history) override
    {
        SDL_Log("Test successful!");
        history->commands.push_back(this);
    }

protected:
    void Undo() override
    {
        SDL_Log("Undo: Test successful!");
    }
};

class FunCommand : public Command
{
public:
    void Execute(History *history) override
    {
        SDL_Log("Have fun!");
        history->commands.push_back(this);
    }

protected:
    void Undo() override
    {
        SDL_Log("Undo: Have fun!");
    }
};

class BabaCommand : public Command
{
public:
    void Execute(History *history) override
    {
        SDL_Log("BABA!");
        history->commands.push_back(this);
    }

protected:
    void Undo() override
    {
        SDL_Log("Undo: BABA!");
    }
};

int main(int argc, char *argv[])
{
    History *history = new History();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("", 640, 640, 0);
SDL_Renderer *rend=  SDL_CreateRenderer(win, 0);
    SDL_Log("hello, SDL");

    while (1)
    {
        SDL_Event e = {0};
        while (SDL_PollEvent(&e) == true)
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                return 0;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                if (e.key.scancode == SDL_SCANCODE_1)
                {

                    Command *my_command = new TestCommand();
                    my_command->Execute(history);
                }
                if (e.key.scancode == SDL_SCANCODE_2)
                {

                    Command *my_command = new FunCommand();
                    my_command->Execute(history);
                }
                if (e.key.scancode == SDL_SCANCODE_3)
                {

                    Command *my_command = new BabaCommand();
                    my_command->Execute(history);
                }
                if (e.key.scancode == SDL_SCANCODE_BACKSPACE)
                {
                    history->UndoLast();
                }
            }
        }

        SDL_SetRenderDrawColor(rend, 55, 0,0, 255);
        SDL_RenderClear(rend);
        SDL_RenderPresent(rend);
    }

    return 0;
}