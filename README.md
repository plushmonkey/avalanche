# Avalanche  
A simple cli Minecraft server stress test tool.  

## Installation  
Set `connection-throttle: 0` in `bukkit.yml` when using multiple instances.  
There's currently no delay in the login sequence, so only one instance will get in unless that setting is changed.  
This setting doesn't need to be changed when using `127.0.0.1` to log in.

## Attacks  
### Interact
This will spam the server with block dig packets. This can cause massive lag with WorldEdit permission checks.  
Countering the attack is as easy as putting in a packet limiting plugin, such as NoCheatPlus.  

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
