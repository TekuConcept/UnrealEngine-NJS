# UnrealEngine-NJS
Built-in NodeJS scripting in Unreal Engine

![Supported Engine Version](https://img.shields.io/badge/Unreal%20Engine-4.27-blue)
![Supported NodeJS Version](https://img.shields.io/badge/NodeJS-17.0-green)

#

NJS is an Unreal Engine plugin that embeds the nodejs runtime into any project. It uses the engine's game-loop to drive the runtime, so all callbacks and driving logic are synchronous with the engine.

Live scripting is done through REPL over TCP. While the editor is running, open a new terminal/console and run `node repl-client.js`. This will connect you to the built-in engine runtime live.


## ToDo List

1. Right now the engine only runs a REPL server. If the engine is running in non-editor/game mode, it should attempt to run a script instead. If no script is provided, NJS should not run at all.

2. Add some native objects to the runtime so meaningful code may be written. Objects such as UObject, AActor, ACharacter, FTickableGameObject, etc.

3. Relating to (2), devise a way to add custom user classes and structs.

4. Tap into blueprints, editor properties, etc.
