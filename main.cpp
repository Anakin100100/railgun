#include <iostream>
#include "uci.hpp"
#include "chess.hpp"

#include <string_view>

class chess_engine
{
private:
    chess::Board *board = nullptr;

public:
    chess_engine();
    ~chess_engine();
    void set_position(const std::string &fen, const std::vector<std::string> &moves);
    void start_search();
    void restart();
};

chess_engine::chess_engine()
{
}

chess_engine::~chess_engine()
{
}

void chess_engine::set_position(const std::string &fen, const std::vector<std::string> &moves)
{
    if (this->board == nullptr)
    {
        std::cout << "Creating new board" << std::endl;
        this->board = new chess::Board(fen);
    }
    else
    {
        this->board->makeMove(chess::uci::uciToMove(*this->board, moves[moves.size() - 1]));
        std::cout << "debugging comment" << std::endl;
    }
}

void chess_engine::start_search()
{
    std::cout << "Starting search" << std::endl;

    chess::Movelist moves;
    chess::movegen::legalmoves(moves, *this->board);

    chess::Move move = moves[std::rand() % moves.size()];

    this->board->makeMove(move);

    std::cout << "bestmove " << chess::uci::moveToUci(move) << std::endl;
}

void chess_engine::restart()
{
    delete this->board;
    this->board = nullptr;
}

int main(int argc, char **argv)
{
    chess_engine engine;
    uci uci;

    // Register callbacks to the messages from the UI and respond appropriately.
    uci.receive_uci.connect([&]()
                            {
    uci.send_id("Engine name", "Author name");
    uci.send_option_uci_limit_strength(false);
    uci.send_uci_ok(); });
    uci.receive_is_ready.connect([&]()
                                 { uci.send_ready_ok(); });
    uci.receive_position.connect([&](const std::string &fen, const std::vector<std::string> &moves)
                                 { engine.set_position(fen, moves); });
    uci.receive_go.connect([&](const std::map<uci::command, std::string> &parameters)
                           { engine.start_search(); });

    uci.receive_uci_new_game.connect([&]()
                                     { engine.restart(); });

    uci.launch();
}