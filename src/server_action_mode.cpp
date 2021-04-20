/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 12:08:34 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_mode.h"

#include "iserver_action.h"
#include "server.h"
#include "string_conversions.h"
#include "construct_reply.h"
#include "logger.h"

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
