#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

class chat : public contract {

	using contract::contract;

public:

	chat(account_name self):
		contract(self),
		messages(_self, _self) {}

	//@abi action
	void addmessage (eosio::name sender, eosio::name recipient, string sms) {
		require_auth(sender);
		eosio_assert( is_account(recipient), "to account doesn't exist");

		require_recipient(sender);
		require_recipient(recipient);

		messages.emplace(_self, [&]( auto& message) {
			message.id = messages.available_primary_key();
			message.owner = sender;
			message.recipient = recipient;
			message.sms = sms;
			message.time = eosio::time_point(eosio::seconds(now()));
		});
		print("Add message");
	}

	//@abi action
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
	struct message {
		uint64_t		id;
        eosio::name 	owner;
        eosio::name 	recipient;
		string			sms;
        eosio::time_point_sec time;

        uint64_t primary_key() const {return id;}

		EOSLIB_SERIALIZE( message, (id)(owner)(recipient)(sms)(time) )
	};

	multi_index<N(message), message> messages;

};

EOSIO_ABI( chat, (addmessage)(rmmessage) )