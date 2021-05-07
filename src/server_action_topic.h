/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_topic.h                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 15:20:56 by jules        #+#    #+#                  */
/*   Updated: 2021/05/07 15:26:21 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_TOPIC_H_
#define SERVER_ACTION_TOPIC_H_

#include <vector>
#include <string>

class {
 public:
    ServerActionTopic(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionTopic() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_TOPIC_H_
