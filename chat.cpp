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
	void addmessage (account_name sender, account_name recipient, string sms) {
		require_auth(sender);
		eosio_assert( is_account(recipient), "to account doesn't exist");

		require_recipient(sender);
		require_recipient(recipient);

		messages.emplace(_self, [&]( auto& message) {
			message.id = messages.available_primary_key();
			message.owner = sender;
			message.recipient = recipient;
			message.sms = sms;
		});
		print("Add message");
	}

	//@abi action
	void rmmessage (name sender, uint64_t id) {
		require_auth(sender);

		auto message_iter = messages.find(id);
		eosio_assert(message_iter != messages.end(), "Message ID doesn't exist");


	}

private:

	//@abi table message i64
	struct message {
		uint64_t	id;
		account_name 		owner;
		account_name 		recipient;
		string		sms;

		uint64_t primary_key() const {return id;}

		EOSLIB_SERIALIZE( message, (id)(owner)(recipient)(sms) )
	};

	multi_index<N(message), message> messages;

	/*struct channel {
		uint64_t	id;
		name 		alice;
		name 		bob;
	};*/
};

EOSIO_ABI( chat, (addmessage) )