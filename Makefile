################################################################################
########################### Makefile Template ##################################
################################################################################

# Compiler settings - Customizable
CC = clang++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++98 -fsanitize=address
LDFLAGS =
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# Makefile settings - Customizable
NAME = ircserv
EXT = .cpp
SRCDIR = src
OBJDIR = obj
DEPDIR = dep
SRCLST = main \
		socket \
		utils \
		start_irc_server \
		connection \
		server_connection \
		server \
		server_action_send \
		client \
		logger \
		reply \
		string_conversions \
		int_conversions \
		message_parser \
		iserver_action \
		server_action_user \
		server_action_nick \
		server_action_join \
		server_action_mode \
		server_action_receive \
		server_action_accept \
		server_action_disconnect \
		server_action_privmsg \
		server_action_ping \
		server_action_pass \
		server_action_server \
		server_action_motd \
		server_action_kill \
		server_action_quit \
		server_action_names \
		server_action_topic \
		server_action_who \
		server_action_part \
		server_action_squit \
		action_factory \
		channel \
		construct_reply \
		welcome_client \

################################################################################
################## Do not change anything from here downwards! #################
################################################################################
# File variables
SRC = $(addprefix $(SRCDIR)/, $(addsuffix $(EXT), $(SRCLST)))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
# UNIX-based OS variables & settings
RM = rm -f
# Basic printing functions
include Makefile.color

########################### Targets beginning here #############################
all: $(NAME)

# Builds the executable
$(NAME): $(OBJ)
	@$(call FNC_PRINT_BUILD,$@,.o files)
	@$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@$(call FNC_PRINT_FINISH,$@)

# Includes all .h file dependencies
-include $(DEP)

# Building rule for .o files and its .cpp in combination with all .h
# Will call rule to create objdir if it does not exist already
.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) $(DEPDIR)/%.d | $$(@D)/.
	@$(call FNC_PRINT_BUILD,$@,$<)
	@$(CC) $(DEPFLAGS) $(CXXFLAGS) -o $@ -c $<

# Empty rule for dependency files
# Will call rule to create depdir if it does not exist already
$(DEPDIR)/%.d: | $$(@D)/. ;

# Creates subdirectories for objdir and depdir
.PRECIOUS: $(DEPDIR)/. $(DEPDIR)%/. $(OBJDIR)/. $(OBJDIR)%/.
$(DEPDIR)/. $(DEPDIR)%/.:
	@$(call FNC_PRINT_CREATE,$@)
	@mkdir -p $@
$(OBJDIR)/. $(OBJDIR)%/.:
	@$(call FNC_PRINT_CREATE,$@)
	@mkdir -p $@

################### Cleaning rules for Unix-based OS ###################
# Cleans all files with the extension .d
.PHONY: dclean
dclean:
	@$(call FNC_PRINT_CLEAN,.d files)
	@$(RM) $(DEP)

# Cleans complete project
.PHONY: clean
clean:
	@$(call FNC_PRINT_CLEAN,.o and .d files)
	@$(RM) $(OBJ) $(DEP)

# Cleans complete project, including the executable
.PHONY: fclean
fclean: clean
	@$(call FNC_PRINT_CLEAN,$(NAME))
	@$(RM) $(NAME)

# Remakes the project
.PHONY: re
re: fclean all
