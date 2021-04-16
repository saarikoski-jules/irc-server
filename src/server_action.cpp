/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/16 16:53:27 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include <vector>
#include <iostream>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"
#include "string_conversions.h"

ServerActionNick::ServerActionNick(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    try {
        if (params.size() >= 1) {
            newNickName = &params[0];
            handleNickNameChange();
        } else {
            handleNoNicknameGiven();
        }
    } catch (const std::out_of_range& e) {
        Logger::log(LogLevelError, "No valid fd found in NICK action");
        // TODO(Jelle) Handle non valid client fd
    }
}

void ServerActionNick::handleNickNameChange() const {
    if (server->nicknameExists(*newNickName) == false) {
        cli->nickName = *newNickName;
        if (cli->userName.empty() == false) {
            cli->registered = true;
        }
    } else {
        handleNickNameInUse();
    }
}

void ServerActionNick::handleNickNameInUse() const {
    std::vector<std::string> params;
    params.push_back(cli->nickName);
    params.push_back(*newNickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NICKNAMEINUSE, params));
}

void ServerActionNick::handleNoNicknameGiven() const {
    std::vector<std::string> params;
    params.push_back(cli->nickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NONICKNAMEGIVEN, params));
}

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 4, cli, prefix),
params(params) {}

void ServerActionUser::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    try {
        const std::string& newUserName = params[0];
        const std::string& newHostName = params[1];
        const std::string& newServerName = params[2];
        const std::string& newRealName = params[3];
        if (server->usernameExists(newUserName) == false) {
            cli->userName = newUserName;
            cli->hostName = newHostName;
            cli->serverName = newServerName;
            cli->realName = newRealName;
            if (cli->nickName != "*") {
                cli->registered = true;
            }
            std::cout << *cli << std::endl;
        } else {
            Logger::log(LogLevelError, "Something went wrong while setting user info");
            // server->sendErrorToClient(ERR_NONICKNAMEGIVEN, clientFd);
        }
        // TODO(Jelle) Handle server error's
        // TODO(Jelle) Handle server2server communication
    } catch (const std::out_of_range& e) {
        // TODO(Jelle) Handle non valid client fd
    }
}

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, clientFd);
    }
    chan->addClient(server->getClientByFd(clientFd), key);
    return (chan);
}

void ServerActionJoin::joinServer(const std::string& name, const std::string& key) {
    std::string reply;
    Channel* chan;
    std::vector<std::string> replyParams;

    replyParams.push_back(cli->nickName);
    replyParams.push_back(name);
    try {
        chan = getChannel(name, key);
        if (chan->topicIsSet) {
            replyParams.push_back(chan->topic);
            reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
        } else {
            reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
        }
    } catch (const ChannelException& e) {
        reply = e.what();
    }
    server->sendReplyToClient(clientFd, reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    errParams.push_back(cli->nickName);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(clientFd, errReply);
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
    if (params.size() < 1) {    // TODO(Jules): handle need more params somewhere else
        handleNeedMoreParams();
        return;
    }
    std::vector<std::string> chans = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::vector<std::string> keys;
    if (params.size() > 1) {
        keys = Utils::String::tokenize(params[1], params[1].length(), ",");
    }
    for (size_t i = 0; i < chans.size(); i++) {
        std::string key;
        if (keys.size() > i) {
            key = keys[i];
        } else {
            key = "";
        }
        try {
            joinServer(chans[i], key);
        } catch (const ChannelException& e) {
            server->sendReplyToClient(clientFd, e.what());
        }
    }
}

std::string constructNoSuchChannelReply(const std::string& cliNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
    return (reply);
}

std::string constructNoSuchNickReply(const std::string& cliNick, const std::string& targetNick) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(targetNick);
    reply = ReplyFactory::newReply(ERR_NOSUCHNICK, replyParams);
    return (reply);
}


std::string constructChanoPrivsNeededReply(const std::string& cliNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams);
    return (reply);
}

void ServerActionMode::modeO(char sign, const std::string& user) {
    Client* target;
    try {
        target = server->getClientByNick(user);
    } catch (const std::exception& e) {
        std::string reply = constructNoSuchNickReply(cli->nickName, user);
        server->sendReplyToClient(clientFd, reply);
        return;
    }
    if (sign == '-') {
        chan->removeOperator(target);
    } else {
        chan->addOperator(target);
    }
}

void ServerActionMode::editMode(char sign, char mode) {
    if (sign == '-') {
        chan->removeMode(mode);
    } else {
        chan->addMode(mode);
    }
}

void ServerActionMode::setLimit(char sign, const std::string& limit) {
    if (sign == '-') {
        chan->removeMode('l');
    } else {
        unsigned int uintLimit;
        try {
            uintLimit = StringConversion::toUint(limit);
        } catch (const std::exception& e) {
            // TODO(Jules): handle bad limit
            return;
        }
        chan->setLimit(uintLimit);
    }
}

void ServerActionMode::setKey(char sign, const std::string& key) {
    if (sign == '-') {
        chan->removeMode('k');
        chan->changeKey("");
    } else {
        chan->addMode('k');
        chan->changeKey(key);
    }
}

void ServerActionMode::listBanMasks() const {
    size_t i = 0;
    std::vector<std::string> replyParams;
    replyParams.push_back(cli->nickName);
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
            server->sendReplyToClient(clientFd, reply);
        } catch (const std::exception& e) {
            if (replyParams.size() == 3) {
                replyParams.pop_back();
            }
            std::string reply = ReplyFactory::newReply(RPL_ENDOFBANLIST, replyParams);
            server->sendReplyToClient(clientFd, reply);
            return;
        }
    }
}

void ServerActionMode::setBanMask(char sign, const std::string& mask) {
    if (mask == "" && sign == '+') {
        listBanMasks();
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
}

ServerActionMode::ServerActionMode(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 2, cli, prefix),
params(params) {}

void ServerActionMode::sendChannelModeIsReply() const {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cli->nickName);
    replyParams.insert(replyParams.end(), params.begin(), params.end());
    reply = ReplyFactory::newReply(RPL_CHANNELMODEIS, replyParams);
    server->sendReplyToClient(clientFd, reply);
}

void ServerActionMode::sendUnknownModeReply(char c) const {
    std::string reply;
    std::vector<std::string> params;
    std::string character(1, c);

    params.push_back(cli->nickName);
    params.push_back(character);
    reply = ReplyFactory::newReply(ERR_UNKNOWNMODE, params);
}

void ServerActionMode::execute() {
    Logger::log(LogLevelInfo, "Executing server action MODE");
    try {
        chan = server->findChannel(params[0]);
    } catch (const std::exception& e) {
        server->sendReplyToClient(clientFd, constructNoSuchChannelReply(cli->nickName, params[0]));
        return;
    }
    if (!chan->isOperator(cli)) {
        std::string reply = constructChanoPrivsNeededReply(cli->userName, chan->name);
        server->sendReplyToClient(clientFd, reply);
    }
    char sign = '+';
    if (params[1][0] == '-') {
        sign = '-';
    }
    for (size_t i = 0; i < params[1].length(); i++) {
        switch (params[1][i]) {
        case 'o':
            modeO(sign, params[2]);//sometimes 2, smetimes 3
            break;
        case 'p':
        case 's':
        case 'i':
        case 't':
        case 'n':
        case 'm':
            editMode(sign, params[1][i]);
            break;
        case 'l':
            setLimit(sign, params[2]);
            break;
        case 'b':
            setBanMask(sign, params[2]);//2, 3 or 4
            break;
            // can it only be one? or a list?
            //
            // user in form of nick!user@host, wildcard allowed, look up for each type
            // TODO(Jules): b == ban mask
            // TODO(Jules): v == allow to speak on moderated channel
        case 'k':
            setKey(sign, params[2]);//idk which param
            break;
        default:
            sendUnknownModeReply(params[1][i]);
            break;
        }
    }
    // sendChannelModeIsReply(); //if mode has been edited
}

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionAccept::execute() {
    // Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewClient(clientFd);
}

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionReceive::execute() {
    // Logger::log(LogLevelInfo, "server action receive");
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], clientFd, cli);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    Logger::log(LogLevelInfo, "server action disconnect");
    server->deleteClient(clientFd);
}

// TODO(Jules): construct general channel replies from functions, maybe under reply?
// TODO(Jules): send ERR_NOLOGIN from action creator, as well as need more params?
