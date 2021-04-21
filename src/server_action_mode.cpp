/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/21 12:11:47 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_mode.h"

#include "iserver_action.h"
#include "server.h"
#include "string_conversions.h"
#include "construct_reply.h"
#include "logger.h"

ServerActionMode::ServerActionMode(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 2, cli, prefix),
params(params) {}

void ServerActionMode::execute() {
    Logger::log(LogLevelInfo, "Executing server action MODE");
    char sign = '+';
    try {
        chan = server->findChannel(params[0]);
        if (!chan->isOperator(cli)) {
            std::string reply = constructChanoPrivsNeededReply(cli->userName, chan->name);
            server->sendReplyToClient(clientFd, reply);
            return;
        }
        if (params[1][0] == '-') {
            sign = '-';
        }
        execByMode(sign);
    } catch (const std::out_of_range& e) {
        server->sendReplyToClient(clientFd, constructNoSuchChannelReply(cli->nickName, params[0]));
    } catch (const std::exception& e) {
        std::string errorMsg(e.what());
        Logger::log(LogLevelDebug, std::string("Unexpected exception caught in ServerActionMode: " + errorMsg));
    }
}

void ServerActionMode::execByMode(char sign) {
    std::string returnOptions;
    std::vector<std::string> returnParams;
    bool success;

    for (size_t i = 0; i < params[1].length(); i++) {
        switch (params[1][i]) {
        case 'p':
        case 's':
        case 'i':
        case 't':
        case 'n':
        case 'm':
            success = editMode(sign, params[1][i]);
            break;
        case 'o':
            success = modeO(sign, params[i + 1]);
            break;
        case 'l':
            success = setLimit(sign, params[i + 1]);
            break;
        case 'b':
            success = setBanMask(sign, params[i + 1]);
            break;
            // TODO(Jules): v == allow to speak on moderated channel
        case 'k':
            success = setKey(sign, params[i + 1]);
            break;
        default:
            sendUnknownModeReply(params[1][i]);
            break;
        }
        if (success) {
            returnOptions.push_back(params[1][i]);
            if (std::string("molbk").find(params[1][i])) {
                returnParams.push_back(params[i + 1]);
            }
        }
    }
    if (returnParams.size() != 0) {
        returnOptions.insert(returnOptions.begin(), sign);
        sendChannelModeIsReply(returnOptions, chan->name, returnParams);
    }
}

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
    Client* target;
    try {
        target = server->getClientByNick(user);
    } catch (const std::exception& e) {
        std::string reply = constructNoSuchNickReply(cli->nickName, user);
        server->sendReplyToClient(clientFd, reply);
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
        chan->removeMode('l');
    } else {
        unsigned int uintLimit;
        try {
            uintLimit = StringConversion::toUint(limit);
        } catch (const std::exception& e) {
            // TODO(Jules): handle bad limit
            return (false);
        }
        chan->setLimit(uintLimit);
    }
    return (true);
}

bool ServerActionMode::setKey(char sign, const std::string& key) {
    if (sign == '-') {
        chan->removeMode('k');
        chan->changeKey("");
    } else {
        chan->addMode('k');
        chan->changeKey(key);
    }
    return (true);
}

bool ServerActionMode::listBanMasks() const {
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
            return (false);//??
        }
    }
}

void ServerActionMode::sendChannelModeIsReply(const std::string& modes, const std::string& channelName, const std::vector<std::string>& params) const {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cli->nickName);
    replyParams.push_back(channelName);
    replyParams.push_back(modes);
    std::string replyString;
    for (std::vector<std::string>::const_iterator i = params.begin(); i != params.end(); i++) {
        replyString = std::string(replyString + *i + " ");
    }
    replyParams.push_back(replyString);
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
