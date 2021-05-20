/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.cpp                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/20 11:28:09 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_mode.h"

#include "iserver_action.h"
#include "server.h"
#include "string_conversions.h"
#include "construct_reply.h"
#include "logger.h"

ServerActionMode::ServerActionMode(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 2, prefix),
params(params) {}

void ServerActionMode::execute() {
    Logger::log(LogLevelInfo, "Executing server action MODE");
        char sign = '+';
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType) {
        case Connection::ServerType:
		{
            Connection* tmp = connection->getLeafConnection(prefix);
			clientNick = tmp->client.nickName;
			break;
		}
        case Connection::ClientType:
			clientNick = connection->client.nickName;
            break;
        case Connection::NoType:
            connectionNotRegistered();
            return;
    }
    if (params.size() < requiredParams) {
        sendReplyToLocalClient(constructNeedMoreParamsReply(clientNick, "MODE"));
        return;
    }
    try {
        chan = server->findChannel(params[0]);
        Connection* tmp;
        if (connection->connectionType == Connection::ServerType) {
            tmp = connection->getLeafConnection(prefix);
        } else {
            tmp = connection;
        }
        if (!chan->isOperator(tmp)) {
            std::string reply = constructChanoPrivsNeededReply(connection->client.userName, chan->name);
            sendReplyToLocalClient(reply);
            return;
        }
        if (params[1][0] == '-') {
            sign = '-';
        }
        execByMode(sign);
    } catch (const std::out_of_range& e) {
        sendReplyToLocalClient(constructNoSuchChannelReply(clientNick, params[0]));
    } catch (const std::exception& e) {
        std::string errorMsg(e.what());
        Logger::log(LogLevelDebug, std::string("Unexpected exception caught in ServerActionMode: " + errorMsg));
    }
}

void ServerActionMode::execByMode(char sign) {
    std::string returnOptions;
    std::vector<std::string> returnParams;
    std::vector<std::string>::iterator param = params.begin() + 2;
    std::string::iterator mode = params[1].begin();
// TODO(Jules): MODE &channel, or in general handle bad n. params
    if (*mode == '+' || *mode == '-') {
        mode++;
    }
    for (; mode != params[1].end(); mode++) {
        switch (*mode) {
        case 'p':
        case 's':
        case 'i':
        case 't':
        case 'n':
        // case 'm':
           if (editMode(sign, *mode)) {
                returnOptions.push_back(*mode);
            }
            break;
        case 'o':
            if (modeO(sign, *param)) {
                returnOptions.push_back(*mode);
                if (param != params.end()) {
                    returnParams.push_back(*param);
                }
            }
            param++;
            break;
        case 'l':
            if (setLimit(sign, *param)) {
                returnOptions.push_back(*mode);
                if (param != params.end()) {
                    returnParams.push_back(*param);
                }
            }
            if (sign == '+') {
                param++;
            }
            break;
        case 'b':
            if (setBanMask(sign, *param)) {
                returnOptions.push_back(*mode);
                if (param != params.end()) {
                    returnParams.push_back(*param);
                }
            }
            param++;
            break;
        case 'k':
            if (setKey(sign, *param)) {
                returnOptions.push_back(*mode);
                if (param != params.end()) {
                    returnParams.push_back(*param);
                }
            }
            if (sign == '+') {
                param++;
            }
            break;
            // TODO(Jules): v == allow to speak on moderated channel
        default:
            sendUnknownModeReply(*mode);
            break;
        }
    }
    if (returnOptions.length() > 0) {
        returnOptions.insert(returnOptions.begin(), sign);
        sendChannelModeIsReply(returnOptions, chan->name, returnParams);
    }
}

//TODO(Jules): MODE without params will list modes, except b and o, with params for k and l. If no modes present, print only +

bool ServerActionMode::setBanMask(char sign, const std::string& mask) {
    if (mask == "" && sign == '+') {
        return (listBanMasks());
    } else {
        size_t nickEnd = mask.find('!');
        size_t userEnd = mask.find('@');
        if (userEnd != std::string::npos && userEnd > nickEnd && *(mask.end()) != '@') {
            if (sign == '-') {
                chan->removeBanMask(mask);
            } else {
                chan->addBanMask(mask);
            }
        }
    }
    return (true);
}

bool ServerActionMode::modeO(char sign, const std::string& user) {
    Connection* target;
    try {
        target = server->getClientByNick(user);
    } catch (const std::exception& e) {
        std::string reply = constructNoSuchNickReply(clientNick, user);
        sendReplyToLocalClient(reply);
        return (false);
    }
    if (sign == '-') {
        chan->removeOperator(target);
    } else {
        chan->addOperator(target);
    }
    return (true);
}

bool ServerActionMode::editMode(char sign, char mode) {
    if (sign == '-') {
        chan->removeMode(mode);
    } else {
        chan->addMode(mode);
    }
    return (true);
}

bool ServerActionMode::setLimit(char sign, const std::string& limit) {
    if (sign == '-') {
        chan->setLimit(UINT32_MAX);
        chan->removeMode('l');
    } else {
        unsigned int uintLimit;
        if (limit == "") {
            return (false);
        }
        try {
            uintLimit = StringConversion::toUint(limit);
        } catch (const std::exception& e) {
            // TODO(Jules): handle bad limit
            return (false);
        }
        chan->setLimit(uintLimit);
        chan->addMode('l');
    }
    return (true);
}

bool ServerActionMode::setKey(char sign, const std::string& key) {
    if (sign == '-') {
        chan->removeMode('k');
        chan->changeKey("");
    } else if (key != "") {
        chan->addMode('k');
        chan->changeKey(key);
    } else {
        return (false);
    }
    return (true);
}

bool ServerActionMode::listBanMasks() const {
    size_t i = 0;
    std::vector<std::string> replyParams;
    replyParams.push_back(clientNick);
    replyParams.push_back(chan->name);
    while (true) {
        try {
            std::string m = chan->getBanMask(i);
            i++;
            if (replyParams.size() == 3) {
                replyParams.pop_back();
            }
            replyParams.push_back(m);
            std::string reply = ReplyFactory::newReply(RPL_BANLIST, replyParams);
            sendReplyToLocalClient(reply);
        } catch (const std::exception& e) {
            if (replyParams.size() == 3) {
                replyParams.pop_back();
            }
            std::string reply = ReplyFactory::newReply(RPL_ENDOFBANLIST, replyParams);
            sendReplyToLocalClient(reply);
            return (false);//??
        }
    }
}

void ServerActionMode::sendReplyToLocalClient(const std::string& message, const std::string& prefix) const {
    if (connection->connectionType == Connection::ClientType) {
        server->sendReplyToClient(fd, message, prefix);
    }
}

void ServerActionMode::sendChannelModeIsReply(const std::string& modes, const std::string& channelName, const std::vector<std::string>& params) const {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(clientNick);
    replyParams.push_back(channelName);
    replyParams.push_back(modes);
    std::string replyString;
    for (std::vector<std::string>::const_iterator i = params.begin(); i != params.end(); i++) {
        replyString = std::string(replyString + *i + " ");
    }
    replyParams.push_back(replyString);
    reply = ReplyFactory::newReply(RPL_CHANNELMODEIS, replyParams);
    sendReplyToLocalClient(reply);
//broadcast
	std::vector<Connection*> sendTo = chan->getConnections();
	std::string senderPrefix;
	if (connection->connectionType == Connection::ServerType) {
		senderPrefix = prefix;
	} else if (connection->connectionType == Connection::ClientType) {
		senderPrefix = std::string(clientNick + "!" + connection->client.userName + "@" + connection->client.hostName);
	} else {
        Logger::log(LogLevelDebug, "MODE, connectionType NoType");
        return;
    }
    //TODO: fix prefix
    reply = std::string("MODE " + channelName + " " + modes + " " + replyString);
	for (std::vector<Connection*>::iterator it = sendTo.begin(); it != sendTo.end(); it++) {
		if (server->hasLocalConnection(**it)) {
			server->sendReplyToClient((*it)->fd, reply, senderPrefix);
		}
	}
	if (channelName[0] == '#') {
		if (connection->connectionType == Connection::ServerType) {
			server->sendMessageToAllServersButOne(std::string(":" + senderPrefix + " " + reply), fd);
		} else {
			server->sendMessageToAllServers(std::string(":" + senderPrefix + " " + reply));
		}
	}
}

void ServerActionMode::sendUnknownModeReply(char c) const {
    std::string reply;
    std::vector<std::string> params;
    std::string character(1, c);

    params.push_back(clientNick);
    params.push_back(character);
    reply = ReplyFactory::newReply(ERR_UNKNOWNMODE, params);
    sendReplyToLocalClient(reply);
}

void ServerActionMode::connectionNotRegistered() const {
    std::vector<std::string> params;
    params.push_back(clientNick);
    sendReplyToLocalClient(ReplyFactory::newReply(ERR_NOTREGISTERED, params));
}

IServerAction* ServerActionMode::clone() const {
    return (new ServerActionMode(*this));
}

