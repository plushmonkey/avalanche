# Avalanche  
A simple cli Minecraft server stress test tool.  

## Installation  
Set `connection-throttle: 0` in `bukkit.yml` when using the multiple instances.  
A delay can be added to the login sequence if needed.  
This setting doesn't need to be changed when using `127.0.0.1` to log in. 

## Building
### Ubuntu  
`git submodule update`  
Install the [dependencies](https://github.com/plushmonkey/mclib/blob/master/README.md#building) for mclib.  
`make`

### Windows
`git submodule update`  
Unzip `lib.zip` in the mclib directory.  
Build mclib.  
Open `avalanche.sln` with Visual Studio 2017 and build.

## Behaviors  
### delay  
Pauses the current instance for some time. This is used for adding a delay in a sequence.  
Configuration:  
- `delay`: The number of milliseconds to pause for.  

### message  
Sends a message. This can be used to send commands.  
Configuration:
- `message`: The message to send to the server.  

### sneak
Start or stop sneaking.  
Configuration:  
- `start`: Boolean indicating whether or not the instance should start sneaking.
- `stop`: Boolean indicating whether or not the instance should stop sneaking.

### swing
Swing either main hand or off hand.  
Configuration:  
- `attack`: The instance will ray cast to find an entity to attack if this is true.
- `hand`: Set to `"off"` to swing with off hand.

### sequence  
This is a composite behavior for executing behaviors sequentially.  
Behaviors that are part of the sequence will only be executed once unless repeat is true.  
Configuration:
- `repeat`: Whether or not the sequence should repeat itself once it finishes.  
- `children`: An array of behaviors that the sequence should execute.  

### interact
This will spam the server with block dig packets. This can cause massive lag with WorldEdit permission checks.  
Countering the attack is as easy as putting in a packet limiting plugin, such as NoCheatPlus.  
Configuration:  
- `send-per-tick`: How many dig packets should be sent each tick.  

### bookedit
This sends book data with many pages to the MC|BEdit plugin channel.  
The pages aren't valid JSON, so each page that the server tries to deserialize will throw an exception.  
The deserialization attack only works on Bukkit/Spigot derivative servers. This was fixed in Spigot 1.12.0, but the hidden attack still works.  
One instance is enough to freeze a server up by sending 10000 page book every tick.  
With enough of these placed in a Shulker Box, the player holding it won't be able to login.  
This is all possible in Survival mode.  
Configuration:  
- `send-per-tick`: How many books should be sent each tick.  
- `pages`: How many pages each book should have. The server will kick the instance if this is too high.  
- `hidden`: This will conceal the attack by forcing deserialization to happen on a netty thread. The main thread will still be running at 20 tps, so it will make it seem like it's network lag. The attack will be hidden in the profiler deep in one of the netty threads. This method won't lag the entire server for everyone, but it will freeze up one of the netty threads, causing lag for anyone using that thread.

### recipe
This attack is similar to the bookedit hidden attack, but instead it uses the 1.12 PrepareCraftingGridPacket to send the books.  

Configuration:  
- `send-per-tick`: How many packets should be sent each tick.  
- `pages`: How many pages each book should have. The server will kick the instance if this is too high.   
- `return-count`: How many books are sent in the return entry array.  
- `prepare-count`: How many books are sent in the prepare entry array.  

### creative-world-lag  
This behavior requires creative mode. This can be put in a sequence after a message behavior that sends gamemode to the server.  
  
This behavior uses CreativeInventoryActionPacket to create an item with block entity data in its inventory.  
The server will read the xyz of the item and try to load the block entity in the world at that position.  
There's no range limit on this, so the server can be forced to load/generate chunks that are far away.  
Configuration:
- `send-per-tick`: How many packets are sent each tick.
- `position`:
  - `method`: Which method should be used to select the position. `random` or `increment`.  
  - `initial`: This is only used for `increment` method. 
    - `x`: The starting x position.
    - `y`: The starting y position.
    - `z`: The starting z position.
  - `increment`: This is only used for `increment` method.
    - `x`: How much x should change each packet.
    - `z`: How much z should change each packet.  

## Authentication Generators  
### increment
This will generate usernames with a base name and sequential digits appended.  
Configuration:
- `username`: Sets the base name used for generating usernames.
- `password`: Sets the password used for all generated usernames.
- `start`: Sets the starting index for generation.

### random
This will generate random all-lowercase usernames.  
Configuration:
- `min`: Sets the minimum length of the username to generate.
- `max`: Sets the maximum length of the username to generate.

### multi-user
This will read username:password combinations from a file.  
Configuration:
- `filename`: The file that contains the username:password combinations.

### multi-token
This will read username:accessToken:clientToken:profileId combinations from a file.  
The profileId isn't required, but it needs to hit the API to fetch it. It will get rate limited faster.  
Configuration:
- `filename`: The file that contains the tokens.

## Examples  
```
./avalanche --username bot --password pw --server 127.0.0.1:25565
```
Spawns in a single instance that does nothing.  

```
./avalanche --username bot --password pw --server 127.0.0.1 --count 19
```
Spawns in 19 instances that do nothing.  

```
./avalanche --username bot --password pw --server 127.0.0.1 --count 19 --behavior interact
```
Spawns in 19 instances that will use the interact behavior.

```
./avalanche --json basic.json
```
Reads the config from basic.json file.
