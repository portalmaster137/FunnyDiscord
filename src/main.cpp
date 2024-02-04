#include "dotenv.h"
#include <dpp/dpp.h>
#include <csignal>

dpp::cluster* bot_ptr = nullptr; // global pointer to bot

void signalHandler(int signum) {
    if (bot_ptr) {
        std::cout << "[SIGTERM RECIEVED]" << std::endl;
        bot_ptr->shutdown();
    }
    exit(signum);
}

int main() {
    
    dotenv::init();

    std::string BOT_TOKEN = dotenv::getenv("TOKEN");

    dpp::members_container unique_members;

    if (BOT_TOKEN.empty()) {
        std::cerr << "Token is empty" << std::endl;
        return 1;
    }

    dpp::cluster bot(BOT_TOKEN, dpp::i_all_intents); // create bot instance
    bot_ptr = &bot; // assign address of bot to global pointer

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
    });

    bot.on_guild_create([&bot, &unique_members](const dpp::guild_create_t& event) {
        int memberCount = event.created->member_count;
        std::cout << "Guild created: " << event.created->name << " with " << memberCount << " members" << std::endl;

        int uniqueCount = 0;

        for (auto& member : event.created->members) {
            if (unique_members.find(member.first) == unique_members.end()) {
                unique_members[member.first] = member.second;
                uniqueCount++;
            }
        }

        std::cout << "Added " << uniqueCount << " unique members, making " << unique_members.size() << " unique members in total" << std::endl;
    });
 
    std::signal(SIGINT, signalHandler);

    bot.start(dpp::st_wait);

}