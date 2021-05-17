/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   welcome_client.h                                  :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 14:29:07 by jules        #+#    #+#                  */
/*   Updated: 2021/05/07 14:32:27 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef WELCOME_CLIENT_H_
#define WELCOME_CLIENT_H_

#include "server.h"

void welcomeClient(Server* server, const int& fd, const std::string& prefix);

#endif  // WELCOME_CLIENT_H_

