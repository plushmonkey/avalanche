# Avalanche  
A simple cli Minecraft server stress test tool.  

## Installation  
Set `connection-throttle: 0` in `bukkit.yml` when using the multiple instances.  
A delay can be added to the login sequence if needed.  
This setting doesn't need to be changed when using `127.0.0.1` to log in. 

## Attacks  
### interact
This will spam the server with block dig packets. This can cause massive lag with WorldEdit permission checks.  
Countering the attack is as easy as putting in a packet limiting plugin, such as NoCheatPlus.  

### bookedit
This sends book data with many pages to the MC|BEdit plugin channel.  
The pages aren't valid JSON, so each page that the server tries to deserialize will throw an exception.  
One instance is enough to freeze a server up by sending 10000 page book every tick.  
With enough of these placed in a Shulker Box, the player holding it won't be able to login.  
This is all possible in Survival mode.  

### creative-world-lag  
This method requires creative mode.  
  
This attack method uses CreativeInventoryActionPacket to create an item with block entity data in its inventory.  
The server will read the xyz of the item and try to load the block entity in the world at that position.  
There's no range limit on this, so the server can be forced to load/generate chunks that are far away.  

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
./avalanche --username bot --password pw --server 127.0.0.1 --count 19 --attack interact
```
Spawns in 19 instances that will do the interact attack.

```
./avalanche --json basic.json
```
Reads the config from basic.json file.

### JSON Examples  
```
{
  "login": {
    "generator": {
      "method": "increment",
      "username": "user",
      "password": "pw",
      "start": 0
    },
    "server": "127.0.0.1",
    "method": {
      "name": "flood",
      "delay": 0
    }
  },
  "count": 5,
  "attack": {
    "method": "bookedit",
    "send-per-tick": 1,
    "pages": 10000
  }
}
```

```
{
  "login": {
    "generator": {
      "method": "random",
      "min": 3,
      "max": 16
    },
    "server": "127.0.0.1",
    "method": {
      "name": "sequential",
      "delay": 5000
    }
  },
  "count": 5,
  "attack": {
    "method": "creative-world-lag",
    "send-per-tick": 30,
    "send-gamemode": true,
    "position": {
      "method": "random",
      "initial": {
        "x": 0,
        "y": 64,
        "z": 0
      },
      "increment": {
        "x": 10,
        "z": 10
      }
    }
  }
}
```

```
{
  "login": {
    "generator": {
      "method": "multi-user",
      "filename": "users.txt"
    },
    "server": "127.0.0.1",
    "method": {
      "name": "sequential",
      "delay": 0
    }
  },
  "count": 3,
  "attack": {
    "method": "interact",
    "send-per-tick": 75
  }
}
```