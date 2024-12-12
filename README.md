# The Hub

In order to make my apps connect with each other, I used named pipes (FIFO). The hub is an app
that creates "rooms" where a "publisher" and a "subscriber" can communicate to each other.

#### The Central Highway for my Applications

The hub creates a folder called 'hub' in the '~' folder. It creates a named pipe called 

```
hubrequests
```

If anyone writes to this pipe with the commands listed below, they can do different things
like create rooms and interact with the hub.

|Command|Description|
|---|---|
|createroom <room name>|creates a room|

Once you get the program running, all you need to do is get your app to interact with the hub,
then make your apps that you want to communicate with each other with the room name you chose.
