
#include "Irisha.hpp"
#include "User.hpp"
#include "utils.hpp"
#include "Channel.hpp"
/**
 * @description	Adds local user
 * @param		sock: user socket
 * @param		nick: user nick
 */
void Irisha::add_user(const int sock, const std::string& nick)
{
	User* user = new User(sock, domain_, nick);
	connections_.insert(std::pair<std::string, AConnection*>(nick, user));
}

/**
 * @description	Adds user by other server request
 * @param		sock: user socket
 */
void Irisha::add_user(int source_sock) // [0]<nick> [1]<hopcount> [2]<username> [3]<host> [4]<servertoken> [5]<umode> [6]<realname>
{
	User* user = new User(U_EXTERNAL_CONNECTION, cmd_.arguments_[3],
					   str_to_int(cmd_.arguments_[1]), source_sock,
					   str_to_int(cmd_.arguments_[4]));

	user->set_nick(cmd_.arguments_[0]);
	user->set_username(cmd_.arguments_[2]);
	//mode
	if (cmd_.arguments_.size() >= 6 && !cmd_.arguments_[5].empty() && cmd_.arguments_[5][0] == '+')
	{
		cmd_.arguments_[5].erase(cmd_.arguments_[5].begin());
		user->set_mode_str(cmd_.arguments_[5]);
	}
	user->set_realname(cmd_.arguments_[2]);
	connections_.insert(std::pair<std::string, AConnection*>(cmd_.arguments_[0], user));

	sys_msg(E_ALIEN, "New external user", cmd_.arguments_[0], "registered!");
}

/**
 * @description	Removes user by nick
 * @param		nick
 */
void Irisha::remove_user(const std::string& nick)
{
	User* user = find_user(nick);
	if (user == nullptr)
	{
		std::cout << E_CROSS RED "Can't remove user " + nick + CLR << std::endl;
		return;
	}
	std::string ch_name;
	while (!user->channels().empty())
	{
		ch_name = user->channels().back();
		channels_.find(ch_name)->second->delUser(user);
		channels_.find(ch_name)->second->delOperators(user);
		if (channels_.find(ch_name)->second->getUsers().size() == 0)
		{
			Channel* channel = channels_.find(ch_name)->second;
			channels_.erase(channel->getName());
			delete channel;
		} else
			send_local_channel(channels_.find(ch_name)->second, "PART " + ch_name, user->nick(), user->socket());
		user->channels().pop_back();
    }
	connections_.erase(nick);
	delete user;
}

/**
 * @description	Removes user by pointer
 * @param		user: pointer to User
 */
void Irisha::remove_user(User*& user)
{
    if (user == nullptr)
    {
        std::cout << E_CROSS RED "Can't remove user " CLR << std::endl;
        return;
    }
	std::string ch_name;
	while (!user->channels().empty())
	{
		ch_name = user->channels().back();
		channels_.find(ch_name)->second->delUser(user);
		channels_.find(ch_name)->second->delOperators(user);
		if (channels_.find(ch_name)->second->getUsers().size() == 0)
		{
			Channel* channel = channels_.find(ch_name)->second;
			channels_.erase(channel->getName());
			delete channel;
		} else
			send_local_channel(channels_.find(ch_name)->second, "PART " + ch_name, user->nick(), user->socket());
		user->channels().pop_back();
	}
	connections_.erase(user->nick());
	delete user;
}

/**
 * @description	Removes all users by server name
 * @param		name: name of the server
 */
void Irisha::remove_server_users(const std::string& name)
{
	User* user;
	for (con_it it = connections_.begin(); it != connections_.end();)
	{
		if (it->second->type() == T_CLIENT)
		{
			user = static_cast<User*>(it->second);
			++it;
			if (user->server() == name)
				remove_user(user);
		}
		++it;
	}
}

/**
 * @description	Finds user by nick
 * @param		nick
 * @return		user pointer or nullptr
 */
User* Irisha::find_user(const std::string& nick) const
{
	User*			user;
	con_const_it	it = connections_.begin();
	for (; it != connections_.end(); ++it)
	{
		if (it->second->type() == T_CLIENT)
		{
			user = static_cast<User*>(it->second);
			if (user->nick() == nick)
				return user;
		}
	}
	return nullptr;
}

/**
 * @description	Finds user by socket
 * @param		sock: socket
 * @return		user pointer or nullptr
 */
User* Irisha::find_user(const int sock) const
{
	User*			user;
	con_const_it	it = connections_.begin();
	for (; it != connections_.end(); ++it)
	{
		if (it->second->type() == T_CLIENT)
		{
			user = static_cast<User*>(it->second);
			if (user->socket() == sock)
				return user;
		}
	}
	return nullptr;
}
