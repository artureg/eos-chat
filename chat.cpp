#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("chat")]] chat : public eosio::contract {

public:

	chat(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds):
		contract(receiver, code, ds),
		messages(_self, _self.value) {}

	//@abi action
	[[eosio::action("addmessage")]] 
	void addmessage (eosio::name sender, eosio::name recipient, string sms) {
		require_auth(sender);
		eosio_assert( is_account(recipient), "to account doesn't exist");

		require_recipient(sender);
		require_recipient(recipient);

		messages.emplace(sender, [&]( auto& message) {
			message.id = messages.available_primary_key();
			message.owner = sender;
			message.recipient = recipient;
			message.sms = sms;
			message.time = eosio::time_point(eosio::seconds(now()));
		});
        

        uint64_t count = 0;
        for (auto itr = messages.begin(); itr != messages.end(); ++itr) {
            ++count;
        }

        if (count > 100) {
            messages.erase(messages.begin());
        }

		print("Add message");
	}

	//@abi action
	[[eosio::action("rmmessage")]] 
	void rmmessage (eosio::name sender, uint64_t id) {
		require_auth(sender);

		auto message_iter = messages.find(id);
		eosio_assert(message_iter != messages.end(), "Message ID doesn't exist");

		bool check = ( message_iter->owner == sender ) || ( message_iter->recipient == sender );
		eosio_assert( check, "It isn't your message" );

		messages.erase(message_iter);
	}

private:

	//@abi table message i64
	struct [[eosio::table]] message {
		uint64_t		id;
        eosio::name 	owner;
        eosio::name 	recipient;
		string			sms;
        eosio::time_point_sec time;

        uint64_t primary_key() const {return id;}

		EOSLIB_SERIALIZE( message, (id)(owner)(recipient)(sms)(time) )
	};

	typedef eosio::multi_index<"messages"_n, message> Messages;

	Messages messages;
	

};

EOSIO_DISPATCH(chat, (addmessage)(rmmessage))

