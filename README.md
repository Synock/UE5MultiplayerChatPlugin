# UE5MultiplayerChatPlugin
Multiplayer chat plugin for UE5 with basic ingame logging and command parsing capability

The look'n'feel is entirely based on the chat system from the game Everquest as it was in the 2000'.

It comes with a lot of predefined functions that run out of the box, but still require some implementation on the project side
to handle RPCs notably.

This plugin is going to be maintained and modified as it is part of another bigger personal project.
Functions and interfaces are subject to sudden changes.

## Integration example

To see an integrated example go check out https://github.com/Synock/UE5PluginIntegration

## General consideration

### Predefined types of chat

There are several predefined and ready to use chat types:

* **Say** is a local, around the player chat with a default range of 5m.
* **Tell** is a private messaging based on another player name.
* **Group** is a group based chat.
* **Raid** is raid based chat.
* **Guild** is a guild based chat.
* **Shout** is server based chat intended for in character zone discussions.
* **Out** Of Character is a server based chat intended for out of character zone discussions.
* **Auction** is a server buy/sell chat.

![ChatExample](./Doc/Images/ChatExample.png?raw=true "ChatExample")

Note that the style of the speaker is predefined to be different from the style of the player hearing it.

![ChatExample2](./Doc/Images/ChatExample2.png?raw=true "ChatExample2")

### Logging and Logging replication

An array of ingame logging possibilities are available, allowing for both replicated logging stuff in an area, like spell casting, damage IO, and not replicated logging, such as XP gains.

![LogExample](./Doc/Images/LogExample.png?raw=true "LogExample")


## Interfaces

There are **three** interfaces that you **must** inherit from in order for the plugin to work as expected.

### IHUDChatInterface

This interface is supposed to be implemented in your main HUD, it must be returnable by your PlayerController.
Its function is to be a gateway between player controller and UI.

You only have one single pure virtual function to override which is **_GetChatBoxWidget_**.
This function must return a pointer toward your Chat UI to the ChatBox.

### IGameModeChatInterface

This interface is meant to be implemented within your game mode. It contains the core, server only based functions.

There is a default implementation for all the functions but it may advisable to override them to fit your needs.

The following functions actually does nothing with the given implementation and needs to be overriden to be functional:
* **_GetGroupMembers_** Return the group members of the same group as a player.
* **_GetRaidMembers_** Return the raid members of the same group as a player.
* **_GetGuildMembers_** Return the raid members of the same group as a player.
* **_SendMessageToOtherServer_** Is meant to be a function to send message across different servers (using REST or whatever).

Log oriented functions are also present, these allow to replicate logs based on the world around a particular location.
* **_AddLogInArea_** allow to add a log to **every** player present in an area
* **_AddLogAroundPlayer_** allows to add log to every player around another player, **_excluding_** this last one.

### IPlayerChatInterface

This is the main workhorse of the plugin, this interface contains most of the functions, and also ALL the functions that you must override.

#### Pure virtual functions

There are several pure virtuals functions that you must redefine because they are dependant on you project

1) **_GetChatHUD_** This function must return your main HUD, the one that implement the _HUDChatInterface_ interface.
2) **_GetChatName_** This function must return a name for each of your player to be displayed in chat.

There's also several functions that you must override because they are RPCs (and i don't know any way to define RPC in Interfaces).
Note that in this case, a minimum viable code is provided in the functions documentation, so you should be able to copy/paste it quickly to have a working prototype.

1) **_Client_AddChatData_** Add chat data to the client using a color and a category 
2) **_Client_AddChatDataType_** Add chat data to the client using a predefined message type
3) **_Server_AreaSpeak_** Server side implementation of chat within an area around the player, appears in white
4) **_Server_ShoutSpeak_** Server side implementation of chat within the whole map, appear in red.
5) **_Server_OOCSpeak_** Server side implementation of chat within the whole map, appear in dark green.
6) **_Server_AuctionSpeak_** Server side implementation of chat within the whole map, appear in green.
7) **_Server_GroupSpeak_** Server side implementation of chat in a group, appear in light blue.
8) **_Server_TellSpeak_** Server side implementation of private chat to another player, appear purple.
9) **_Server_RaidSpeak_** Server side implementation of chat in a raid (group of groups), appear pink.
10) **_Server_GuildSpeak_** Server side implementation of chat in guild, appear light green.

#### Functions you might want to override to fit your needs

1) **_IsInGroup_** return true if the character is in a group, default implementation always returns false.
2) **_IsInRaid_** return true if the character is in a raid, default implementation always returns false.
3) **_IsInGuild_** return true if the character is in a guild, default implementation always returns false.
4) **_ProcessCommands_** Process commands entered through the chat window. Default does nothing.

## UI

Plugin UI is minimalist and should give you just something to work with.
There are two main c++ classes and their inherited Blueprint Objects

1) ChatBoxWidget which is chatbox widget with a chat type selection box, a chat input and a chat display.
2) ChatLineWidget which is an individual line of the chat that will appear in ChatBoxWidget. Note this object make use of RichText.

A Blueprint Object of particular interest would be DT_ChatStyle which contains the different styles of text that can be displayed.

Otherwise there are some helper blueprint objects such as SChatLine and OChatLine that are used for internal purpose.

## Usage

Player chat is expected to be self contained within the internal Chatbox.
For logging purpose, you are expected to be able to call Client_AddChatData to log in data for a player with a particular style and category.