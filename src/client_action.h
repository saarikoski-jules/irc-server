/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:25 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 11:43:26 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTACTION_H_
#define CLIENTACTION_H_

#include <vector>
#include <string>

typedef struct msgFormat {
    std::string type;
    int requiredAmtParams;
    int maxAmtParams;
}               msgFormat_t;

static msgFormat_t const msgFormats[] = {
    {"NO_ACTION", 0, 0},
    {"ACCEPT", 0, 0},
    {""},
    {"NICK", 1, 2},
    {"USER", 4, 4}
};

struct ClientAction {
    enum Type {
        NICK,
        USER
    };
    Type type;
    std::vector<std::string> params;
};

//Turn into an interface, create child class for each different kind of action

#endif  //  CLIENTACTION_H_
