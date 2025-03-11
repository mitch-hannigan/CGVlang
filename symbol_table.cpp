#include <string>
#include <unordered_map>
#include <vector>

enum SymbolType
{
    TYPE_BYTE,
    TYPE_INT,
    TYPE_REAL
};

struct Symbol
{
    std::string id;
    SymbolType symbol_type;
    int scope;
    union 
    {
        char byte_value;
        long long int int_value;
        double real_value;        
    };
};

int scope = 0;
std::unordered_map<std::string, std::vector<Symbol>> symbol_table = std::unordered_map<std::string, std::vector<Symbol>>();

bool insert_symbol(std::string id, SymbolType symbol_type, char byte_value=0, long long int int_value=0, double real_value=0.0)
{
    Symbol symbol = { id, symbol_type, scope };
    switch (symbol.symbol_type)
    {
    case TYPE_BYTE:
        symbol.byte_value = byte_value;
        break;
    case TYPE_INT:
        symbol.int_value = int_value;
        break;
    case TYPE_REAL:
        symbol.real_value = real_value;
    default:
        break;
    }

    symbol_table.insert({ symbol.id, std::vector<Symbol>() });
    for (int i = 0; i < symbol_table[symbol.id].size(); i++)
        if (symbol_table[symbol.id][i].scope == symbol.scope)
            return false;
    symbol_table[symbol.id].push_back(symbol);
    return true;
}

Symbol find_symbol(std::string id)
{
    auto it = symbol_table.find(id);
    if (it == symbol_table.end())
        return { "not found", TYPE_BYTE, -1 };
    
    Symbol symbol;    
    for (int i = 0; i < symbol_table[id].size(); i++)
        if (symbol_table[id][i].scope <= scope)
            symbol = symbol_table[id][i];
    return symbol;
}

bool remove_symbol(std::string id)
{
    auto it = symbol_table.find(id);
    if (it == symbol_table.end())
        return false;

    for (int i = 0; i < symbol_table[id].size(); i++)
        if (symbol_table[id][i].scope >= scope)
            symbol_table[id].erase(symbol_table[id].begin() + i);
    return true;
}