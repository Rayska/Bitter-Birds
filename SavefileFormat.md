# Bitter Birds savefile format

This file explains the structure and formats of saved levels in this project. 

## Bolded characters are headers:

### SST: 
SoundFX vector Start, indicates that starting from the next line until line with SEN are soundFX paths

### SEN: 
SoundFX vector End, indicates end of soundFX paths

### EST: 
Entity vector Start, indicates that starting from the next line until line with EEN are Entities

### EEN: 
Entity vector End, indicates end of Entity vector

### BST: 
Birds vector Start, indicates that starting from the next line until line with BEN are Birds

### BEN:
Birds vector End, indicates end of Birds vector

### NME: 
Level name, Level constructor variable name

### BGR: 
Background image path, Level constructor variable backgroundPath

### STD: 
Soundtrack path, Level constructor variable soundtrackPath

### UNKNOWN:
For when something goes wrong


# File format:
```
BitterBirds v.0

SST
<sound1Path>
<sound2Path>
…
<soundNPath>
SEN

EST
<Entity1>
<Entity2>
…
<EntityN>
EEN

BST
<Bird1Type>
<Bird2Type>
…
<BirdNType>
BEN

NME<String>
BGR<Path>
SDT<Path>

```
Entity and Bird formats will be discussed in this file when they are done. 
Bird format, N refers to birds type, N = 0, 1, 2, ...:

N

BGR and SDT contain paths which will be strings.
