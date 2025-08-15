#include "Database/Database.h"
#include "CLI/Context.h"
#include "CLI/CommandBus/CommandBus.h"
#include "CLI/CommandFactory.h"

int main() {
    Database db;

    Table users("users");
    users.addColumn("id", "int");
    users.addColumn("name", "string");
    users.addColumn("dob", "date");
    db.addTable(users);

    Context ctx{db, "", std::cin, std::cout, std::cerr};
    CommandBus bus;
    CommandFactory::registerDefaultCommands(bus);

    std::cout << "MiniDB REPL started. Type 'help' or 'exit'.\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) 
            break;
        if (!line.empty()) 
        {
            try 
            { 
                bus.dispatch(ctx, line); 
            }
            catch (const std::exception& e) 
            { 
                std::cout << "Error: " << e.what() << "\n"; 
            }
        }
    }
}
